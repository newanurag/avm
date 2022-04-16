/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "bio_engine.h"

extern PBLOCK_DEVICE	DATA_DISK_DEVICE;
extern NAME				DATA_DISK_DEVICE_NAME;
extern PPBA_BLOCK	pba_table;
extern PHASH_BLOCK	hash_table;
extern PLBA_BLOCK	lba_table;
extern CHAR sysfs_avm_get_target_name[DZ_SYSFS_PAGE_SIZE];
extern SIZE				dz_pba_page_mempool_count;
extern U64 				lba_table_size;
extern U64 				pba_table_size;
extern U64 				hash_table_size;
extern BITMAP 			pba_bitmap;
extern PDZ_THREAD_POOL read_thread_pool;
extern PDZ_THREAD_POOL write_thread_pool;

extern SPINLOCK 	lba_table_lock; 
extern SPINLOCK 	hash_table_lock; 
extern SPINLOCK 	pba_table_lock; 

INT dz_double_page = 0;
EXPORT_SYMBOL(dz_double_page);

#ifdef DZ_KERNEL_VERSION_4
RVOID dz_write_biodone_sync(PBIO bio)
{
	PDZ_COMPLETION		comp = (PDZ_COMPLETION)bio->bi_private;
	comp->error = blk_status_to_errno(bio->bi_status);
	LOGD("Completion:error=%d\n", error);
	complete(&comp->io_completion);
	/*
	struct completion		*comp = (struct completion *)bio->bi_private;
	complete(comp);
	return;
	*/
}
#else
RVOID dz_write_biodone_sync(PBIO bio, INT error)
{
	PDZ_COMPLETION		comp = (PDZ_COMPLETION)bio->bi_private;
	comp->error = error;
	LOGD("Completion:error=%d\n", error);
	complete(&comp->io_completion);
	/*
	struct completion		*comp = (struct completion *)bio->bi_private;
	complete(comp);
	return;
	*/
}
#endif
EXPORT_SYMBOL(dz_write_biodone_sync);

//Note : This function takes data as input in the form of single Page
//It frees those pages once WRITE is completed. Since its name ends with
//sync, so it is a blocking one

RINT dz_write_data_to_raw_disk_sync(PBLOCK_DEVICE device, 
		SECTOR sector, PVOID data, UINT bio_size)
{
	PPAGE      page	= NULL;
	PBIO 		bio = NULL;
	PVOID   pagebuf = NULL;
	CHAR	 *pdata	= NULL;
	UINT  pgdatalen	= 0;
	UINT	  pgcnt	= (bio_size + PAGE_SIZE - 1) >> PAGE_SHIFT;
	INT			ret	= SUCCESS;
	INT			  i = 0;
	INT		  error = SUCCESS;
	DZ_COMPLETION		completion;

	//LOG("pgcnt=%d, bio_size=%d, data=%s\n", pgcnt, bio_size, (char *)data);

	if (pgcnt > DZ_NUMBER_OF_WRITE_PAGES_IN_A_BIO) {
		LOGE("Required Pages are more than %d\n",
			DZ_NUMBER_OF_WRITE_PAGES_IN_A_BIO);
		RETURNF;
	}

	if (!device) {
		LOGE("Device is NULL. Can't proceed ahead\n");
		RETURNF;
	}
 	if (!data) {
		LOGE("Data is NULL\n");
		RETURNF;
	}

	MEMSET(&completion);

	bio = dz_bio_alloc(pgcnt);
	if (!bio) {
		LOGE("Unable to get free bio\n");
		RETURNF;
	}
	DZ_BIO_SET_DEV(bio, device);
	DZ_BIO_SET_SECTOR(bio, sector);
	DZ_BIO_SET_END_IO(bio, dz_write_biodone_sync);
	DZ_BIO_SET_ZIDX(bio);
	DZ_BIO_SET_WRITE(bio);
	DZ_BIO_SET_NEXT(bio, NULL);
	bio->bi_private = &(completion);
	//bio->bi_flags   = 1 << BIO_UPTODATE; TODO:

	pdata = (CHAR *)data;
	for (i=0; i < pgcnt; i++) {
		page =  dz_write_page_alloc();
		if (!page)  {
			LOGE("Unable to get free write page\n");
			RETURNF;
		}

		pgdatalen = (bio_size > PAGE_SIZE) ? PAGE_SIZE : bio_size;
		if (!bio_add_page(bio, page, pgdatalen, 0)) {
			LOGE("Unable to add page to bio\n");
			ret = FAILURE;
			goto exit;
		}
		pagebuf = kmap(page);
		memset(pagebuf, 0, pgdatalen);
		memcpy(pagebuf, pdata, pgdatalen);
		kunmap(page);
		pdata += pgdatalen;

		bio_size -= pgdatalen; 
		if (!bio_size) break;
	}
	init_completion(&completion.io_completion);
	DZ_OS_KERNEL_GENERIC_MAKE_REQUEST(bio);
	wait_for_completion(&completion.io_completion);

	//Get the Error returned (if any)
	error = completion.error;
	if (unlikely(error)){
		LOG("Error in writing raw data. Error Code : %d", error);
		ret = error;
		print_biom(bio, "bio with error");
	}
	//Now since at this point bio is done(fail/success) so freeup the pages

exit:
	dz_bio_bvec_write_page_free(bio);
	DZ_OS_KERNEL_BIO_PUT(bio); // It will free the bio as well
	RETURN;
}
EXPORT_SYMBOL(dz_write_data_to_raw_disk_sync);

//Note : This function takes data as input in the form of single page
//Also it assumes that size of Page is PAGE_SIZE and offset is zero
//This function neither allocates any page nor it touches data
//Since its name ends with sync, so it is a blocking one

RINT dz_write_page_to_raw_disk_sync(PBLOCK_DEVICE device, 
		SECTOR sector, UINT bio_size, PPAGE page)
{
	PBIO 		bio = NULL;
	INT			ret	= SUCCESS;
	INT		  error = SUCCESS;
	DZ_COMPLETION		completion;

	MEMSET(&completion);

	bio = dz_bio_alloc(1);
	if (!bio) {
		LOGE("Unable to get free bio\n");
		RETURNF;
	}
	DZ_BIO_SET_DEV(bio, device);
	DZ_BIO_SET_SECTOR(bio, sector);
	DZ_BIO_SET_END_IO(bio, dz_write_biodone_sync);
	DZ_BIO_SET_ZIDX(bio);
	DZ_BIO_SET_WRITE(bio);
	DZ_BIO_SET_NEXT(bio, NULL);
	//bio->bi_private = &(completion); TODO:
	DZ_BIO_SET_PRIVATE(bio, &(completion));
	//bio->bi_flags   = 1 << BIO_UPTODATE;

	if (!bio_add_page(bio, page,
			PAGE_SIZE, 0)) {
		LOGE("Unable to add page to bio\n");
		ret = FAILURE;
		goto exit;
	}

	init_completion(&completion.io_completion);
	DZ_OS_KERNEL_GENERIC_MAKE_REQUEST(bio);
	LOGD("Waiting for IO to complete\n");
	wait_for_completion(&completion.io_completion);

	//Get the Error returned (if any)
	error = completion.error;
	if (unlikely(error)){
		LOG("Error in writing raw data. Error Code : %d", error);
		ret = error;
		print_biom(bio, "bio with error");
	}

exit:
	DZ_OS_KERNEL_BIO_PUT(bio); // It will free the bio as well
	RETURN;
}
EXPORT_SYMBOL(dz_write_page_to_raw_disk_sync);

RVOID dz_child_biodone(PBIO bio, INT error)
{
	PIOREQUEST ciorequest = (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest = NULL;
	//LOG("child_biodone\n");	
	if (ciorequest->ior_parent) {
		piorequest = ciorequest->ior_parent;
		//print_biom(bio, "During Childdone");
		if (atomic_dec_and_test(&piorequest->ior_child_cnt)) {
			// Now complete the Parent / Original bio
			//print_biom(piorequest->bio, "Parent bio when all childs are done");
			//bio_endio(piorequest->ior_bio, error);TODO:
			if (bio->bi_status != BLK_STS_OK) {
				IOREQUEST_DONE_ERROR(piorequest);
			} else {
			}
		}
	}
	//ciorequest->tend = jiffies;
	DZ_OS_KERNEL_BIO_PUT(bio);
	dz_io_free(ciorequest);
	
	return;
}
EXPORT_SYMBOL(dz_child_biodone);

RINT dz_write_data(PIOREQUEST iorequest)

{
	PBLOCK_DEVICE bdev = DATA_DISK_DEVICE;
	INT ret = SUCCESS;
	bool bdevpass = true;

	if (!bdev) {
		if (strlen(DATA_DISK_DEVICE_NAME) == 0) {
			LOG("Please provide a device for issuing writes\n");
			RETURNF;
		} else {
			bdevpass = false;
			bdev = 	dz_bdev_open(DATA_DISK_DEVICE_NAME);
			if (!bdev) {
				LOG("Unable to open device %s\n", DATA_DISK_DEVICE_NAME);
		   	    return FAILURE;
			}
		}
	}
	if (dz_double_page) {
	//Plan to use sync functions
	/*
		ret = dz_write_data_with_double_page_to_raw_disk(bdev, iorequest->ior_sector,
				iorequest->private, iorequest->ior_size);
	*/

	} else {
		ret = dz_write_data_to_raw_disk_sync(bdev, iorequest->ior_sector,
				iorequest->ior_private, iorequest->ior_size);
	}

	if (bdevpass == false) {
		dz_bdev_close(bdev);
	}
	RETURN;

}
EXPORT_SYMBOL(dz_write_data);

RINT dz_write_data_to_block_device(PIOREQUEST iorequest, CHAR *bdev_name)

{
	PBLOCK_DEVICE bdev = DATA_DISK_DEVICE;
	INT ret = SUCCESS;

	bdev = 	dz_bdev_open(bdev_name);
	if (!bdev) {
		LOGE("Unable to open device %s\n", bdev_name);
   	    return FAILURE;
	} else {
		LOGF("Device %s opened successfully\n", bdev_name);
	}

	ret = dz_write_data_to_raw_disk_sync(bdev, iorequest->ior_sector,
				iorequest->ior_private, iorequest->ior_size);

	dz_bdev_close(bdev);
	RETURN;

}
EXPORT_SYMBOL(dz_write_data_to_block_device);

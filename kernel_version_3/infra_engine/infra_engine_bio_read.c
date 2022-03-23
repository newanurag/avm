/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PBLOCK_DEVICE	DATA_DISK_DEVICE;
extern NAME				DATA_DISK_DEVICE_NAME;
extern CHAR sysfs_avm_get_target_name[DZ_SYSFS_PAGE_SIZE];

#ifdef DZ_KERNEL_VERSION_4
RVOID dz_read_biodone_sync(PBIO bio)
{
	PDZ_COMPLETION		ior = (PDZ_COMPLETION)bio->bi_private;

	int error = blk_status_to_errno(bio->bi_status);

	LOGALIGN("Inside Done: bio->bi_size = %d, bio = %p, bio->bi_cnt = %d bio_page = %p, bi_status = %d\n",
	DZ_BIO_GET_SIZE(bio), bio, ATOMIC_READ(DZ_BIO_GET_CNT(bio)),bio_page(bio), bio->bi_status);

	ior->error = error;

	complete(&ior->io_completion);
	return;
}
#else

RVOID dz_read_biodone_sync(PBIO bio, int error)
{
	PDZ_COMPLETION		ior = (PDZ_COMPLETION)bio->bi_private;
	INT ret = 0;


	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}

	LOGALIGN("Inside Done: bio->bi_size = %d, bio = %p, bio->bi_cnt = %d bio_page = %p\n",
	 bio->bi_size, bio, ATOMIC_READ(bio->bi_cnt),bio_page(bio));

	ior->error = error;
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);

	complete(&ior->io_completion);
	return;
}
#endif
EXPORT_SYMBOL(dz_read_biodone_sync);

// This function does the allocation of Pages inside and keeps the
// readback data there. Caller of this functions is supposed to free
// those pages.
// This function frees the allocated pages only in case of failures.

RINT dz_read_data_from_raw_disk_sync(PBLOCK_DEVICE device, 
		SECTOR sector, PDATA_PAGES_T  data_pages, UINT bio_size)
{
	PPAGE      page	= NULL;
	PBIO 		bio = NULL;
	UINT  pgdatalen	= 0;
	UINT	  pgcnt	= (bio_size + PAGE_SIZE - 1) >> PAGE_SHIFT;
	INT			ret	= SUCCESS;
	INT			  i = 0;
	INT		  error = SUCCESS;
	DZ_COMPLETION   io_completion;

	if (pgcnt > DZ_NUMBER_OF_READ_PAGES_IN_A_BIO) {
		LOGE("Required Pages are more than %d\n",
			DZ_NUMBER_OF_READ_PAGES_IN_A_BIO);
		RETURNF;
	}

	if (!device) {
		LOGE("Device is NULL. Can't proceed ahead\n");
		RETURNF;
	}

	// Data Pages will be allocated here
	if (dz_alloc_data_pages(data_pages, pgcnt)) {
		LOGE("Unable to create Data Pages Vector \n");
		RETURNF;
	}

	bio = dz_bio_alloc(pgcnt);
	if (!bio) {
		LOGE("Unable to get free bio\n");
		dz_free_data_pages(data_pages, false);
		RETURNF;
	}

	io_completion.error = 0;
	DZ_BIO_SET_DEV(bio, device);
	DZ_BIO_SET_SECTOR(bio, sector);
	DZ_BIO_SET_END_IO(bio, dz_read_biodone_sync);
	DZ_BIO_SET_ZIDX(bio);
	DZ_BIO_SET_READ(bio);
	DZ_BIO_SET_NEXT(bio, NULL);
	bio->bi_private = (VOID *)&io_completion;
	//bio->bi_flags   = 1 << BIO_UPTODATE; TODO:

	for (i=0; i < pgcnt; i++) {
		page = data_pages->pagevec[i].bv_page;
		pgdatalen = (bio_size > PAGE_SIZE) ? PAGE_SIZE : bio_size;

		if (!bio_add_page(bio, page, pgdatalen, 0)) {
			LOGE("Unable to add page to bio\n");
			ret = FAILURE;
			goto exit_failure;
		}

		//TODO: Optimization required as it involves of derefencing
		data_pages->pagevec[i].bv_offset = 0;
		data_pages->pagevec[i].bv_len = pgdatalen;

		bio_size -= pgdatalen; 
		if (!bio_size) break;
	}
	init_completion(&io_completion.completion);
	//print_biom(bio, "Reading BIO");
	DZ_OS_KERNEL_GENERIC_MAKE_REQUEST(bio);
	wait_for_completion(&io_completion.completion);

	//Get the Error returned (if any)
	//io_completion = bio->bi_private;
	error = io_completion.error;
	if (unlikely(error)){
		LOGE("Error in reading raw data. Error Code : %d", error);
		print_biom(bio, "bio with error");
	}
	DZ_OS_KERNEL_BIO_PUT(bio); // It will free the bio as well
	RETURN;

exit_failure:
	//Pages will be freed only in case of failures
	dz_free_data_pages(data_pages, false);
	DZ_OS_KERNEL_BIO_PUT(bio); // It will free the bio as well
	RETURN;
}

//This function does a single page allocation and attach that page
//in bio. When bio is completed it returns the readback page
//Caller is supposed to free the page
PPAGE dz_read_page_from_raw_disk_sync(PBLOCK_DEVICE device, 
		SECTOR sector, UINT bio_size)
{
	PPAGE      page	= NULL;
	PBIO 		bio = NULL;
	INT error		= 0;
	DZ_COMPLETION   io_completion;

	page =  dz_read_page_alloc();
	if (!page)  {
		LOGE("Unable to get free read page\n");
		RETURNN;
	}

	bio = dz_bio_alloc(1);
	if (!bio) {
		LOGE("Unable to get free bio\n");
		dz_read_page_free(page);
		RETURNN;
	}

	io_completion.error = 0;
	DZ_BIO_SET_DEV(bio, device);
	DZ_BIO_SET_SECTOR(bio, sector);
	DZ_BIO_SET_END_IO(bio, dz_read_biodone_sync);
	DZ_BIO_SET_ZIDX(bio);
	DZ_BIO_SET_READ(bio);
	DZ_BIO_SET_NEXT(bio, NULL);
	bio->bi_private = (VOID *)&io_completion;
	//bio->bi_flags   = 1 << BIO_UPTODATE; TODO:
	//bio->bi_size	= bio_size;

	if (!bio_add_page(bio, page, PAGE_SIZE, 0)) {
			LOGE("Unable to add page to bio\n");
			dz_read_page_free(page);
			DZ_OS_KERNEL_BIO_PUT(bio); // It will free the bio as well
			RETURNN;
		}
	init_completion(&io_completion.completion);
	//print_biom(bio, "Reading BIO Page");
	DZ_OS_KERNEL_GENERIC_MAKE_REQUEST(bio);
	wait_for_completion(&io_completion.completion);

	//Get the Error returned (if any)
	//io_completion = bio->bi_private;
	error = io_completion.error;
	if (unlikely(error)){
		LOGE("Error in reading raw data. Error Code : %d", error);
		print_biom(bio, "bio with error");
		RETURNN;
	}
	DZ_OS_KERNEL_BIO_PUT(bio); // It will free the bio as well
	return page;
}
EXPORT_SYMBOL(dz_read_page_from_raw_disk_sync);

//This function issues a READ request to the Physical disk.
//The disk is already attached in the bio.
//Also the bio is already associated with a PAGE so no
//allocation/freeing of Page or bio is required.
//Once read is successfull then updated PAGE is returned

PPAGE dz_read_page_sync(PBIO bio)
{
	PPAGE      page	= bio_page(bio);
	INT error		= 0;
	DZ_COMPLETION   io_completion;

	io_completion.error = 0;
	DZ_BIO_SET_END_IO(bio, dz_read_biodone_sync);
	DZ_BIO_SET_ZIDX(bio);
	DZ_BIO_SET_READ(bio);
	DZ_BIO_SET_NEXT(bio, NULL);
	bio->bi_private = (VOID *)&io_completion;
	//bio->bi_flags   = 1 << BIO_UPTODATE;TODO:
	//bio->bi_size	= bio_size;
	//DZ_BIO_SET_SIZE(bio, bio_size)

	init_completion(&io_completion.completion);
	//print_biom(bio, "Reading BIO Page");
	DZ_OS_KERNEL_GENERIC_MAKE_REQUEST(bio);
	wait_for_completion(&io_completion.completion);

	//Get the Error returned (if any)
	//io_completion = bio->bi_private;
	error = io_completion.error;
	if (unlikely(error)){
		LOGE("Error in reading raw data. Error Code : %d", error);
		print_biom(bio, "bio with error");
		RETURNN;
	}
	return page;
}
EXPORT_SYMBOL(dz_read_page_sync);

RINT dz_read_data(PIOREQUEST iorequest, PDATA_PAGES data_pages)
{
	PBLOCK_DEVICE bdev = DATA_DISK_DEVICE;
	INT ret = SUCCESS;
	bool bdevpass = true;

	if (!bdev) {
		if (strlen(DATA_DISK_DEVICE_NAME) == 0) {
			LOGI("Please provide a device for issuing writes\n");
			RETURNF;
		} else {
			bdevpass = false;
			bdev = 	dz_bdev_open(DATA_DISK_DEVICE_NAME);
			if (!bdev) {
				LOGE("Unable to open device %s\n", DATA_DISK_DEVICE_NAME);
		   	    return FAILURE;
			}
		}
	}
	ret = dz_read_data_from_raw_disk_sync(bdev, iorequest->ior_sector,
			data_pages, iorequest->ior_size);

	if (bdevpass == false) {
		dz_bdev_close(bdev);
	}
	RETURN;
}
EXPORT_SYMBOL(dz_read_data);

RINT dz_read_data_from_block_device(PIOREQUEST iorequest, CHAR *bdev_name, PDATA_PAGES data_pages)
{
	PBLOCK_DEVICE bdev = NULL;
	INT ret = SUCCESS;

	bdev = 	dz_bdev_open(bdev_name);
	if (!bdev) {
		LOGE("Unable to open device %s\n", bdev_name);
   	    return FAILURE;
	}

	ret = dz_read_data_from_raw_disk_sync(bdev, iorequest->ior_sector,
			data_pages, iorequest->ior_size);

	dz_bdev_close(bdev);
	RETURN;
}
EXPORT_SYMBOL(dz_read_data_from_block_device);

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PBLOCK_DEVICE	DATA_DISK_DEVICE;
extern PAVM_DEVICE 		aisa_device_object;
extern SIZE				dz_pba_page_mempool_count;
extern BITMAP 			pba_bitmap;
extern PPBA_BLOCK		pba_table;
extern PHASH_BLOCK		hash_table;
extern CHAR 			sysfs_avm_get_target_name[DZ_SYSFS_PAGE_SIZE];
extern SIZE				dz_pba_page_mempool_count;
extern U64 				lba_table_size;
extern U64 				pba_table_size;
extern U64 				hash_table_size;
extern BITMAP 			pba_bitmap;
extern PDZ_THREAD_POOL 	read_thread_pool;
extern PDZ_THREAD_POOL 	write_thread_pool;

extern SPINLOCK 		lba_table_lock; 
extern SPINLOCK 		hash_table_lock; 
extern SPINLOCK 		pba_table_lock; 

extern ATOMIC64 dedupe_zeroed_out_page_reads;
extern U64 dz_disk_io_sector;
extern PBLOCK_DEVICE 	avm_metadata_bdev;
extern PMD_LOCK	md_lock_table;	// Array of spinlocks for MetaData Lock

//This function does a direct read from the raw disk.
//i.e. no creation of bio, pages etc.
//This is applicable for internally generated child bios from 
//alignment engine. It is not for application bios
//Note: It does not free iorequest
RVOID dz_read_block_from_raw_disk_sync_internal(PIOREQUEST iorequest, 
		SECTOR sector,
		PLBA_BLOCK lba_entry)
{
	PBIO 		bio = iorequest->ior_bio;
	INT error		= SUCCESS;
	DZ_COMPLETION   io_completion;
	BIODONE			orig_biodone = bio->bi_end_io;
	SECTOR			orig_sector	 = DZ_BIO_GET_SECTOR(bio);
	
	
	io_completion.error = SUCCESS;
	DZ_BIO_SET_DEV(bio, DATA_DISK_DEVICE);
	DZ_BIO_SET_SECTOR(bio, sector);
	DZ_BIO_SET_END_IO(bio, dz_read_biodone_sync);
	DZ_BIO_SET_ZIDX(bio);
	DZ_BIO_SET_READ(bio);
	DZ_BIO_SET_NEXT(bio, NULL);
	bio->bi_private = (VOID *)&io_completion;
	//TODO:bio->bi_flags   = 1 << BIO_UPTODATE;

	init_completion(&io_completion.completion);
	DZ_OS_KERNEL_GENERIC_MAKE_REQUEST(bio);
	wait_for_completion(&io_completion.completion);

	//Get the Error returned (if any)
	//io_completion = bio->bi_private;
	error = io_completion.error;
	if (unlikely(error)){
		LOGE("Error in reading raw data. Error Code : %d", error);
		print_biom(bio, "bio with error");
		goto exit_done;
	}

exit_done:
	bio->bi_private = iorequest;
	DZ_BIO_SET_END_IO(bio, orig_biodone);
	DZ_BIO_SET_SECTOR(bio, orig_sector);
	_RELEASE_LBA_LOCK_READ(lba_entry);
	dz_bec_proceed_pba_page_add_read(iorequest);
	READ_IO_DONE_STATUS(iorequest, error);
	//Just done the child bio.
	//Iorequest will be freed in the child biodone
	//Add pba page to bec cache
	return;
}


//This function does a single page read from the raw disk
//by allocating a new bio. Creation of bio is needed, because
//we cant change the original bio bi_private field.
//Here the page/pages are provided from a parent bio
//IT does not free those page/pages but frees iorequest
RVOID dz_read_block_from_raw_disk_sync(PIOREQUEST iorequest, 
		SECTOR sector,
		PLBA_BLOCK lba_entry)
{
	PBIO 	parent_bio	= iorequest->ior_bio;
	PBIO 		bio = NULL;
	INT error		= SUCCESS;
#ifdef DZ_KERNEL_VERSION_4
	struct bvec_iter iter;
	BIOVEC	bvec;
#else
	INT		count	= 0;
	PBIOVEC	bvec	= NULL;
#endif
	DZ_COMPLETION   io_completion;
	//PPAGE	page 	= NULL;
	
	bio = dz_bio_alloc(parent_bio->bi_vcnt);
	if (!bio) {
		LOGE("Unable to get free bio\n");
		error = IO_ERROR_ENOMEM;
		goto exit_done;
	}

	io_completion.error = SUCCESS;
	DZ_BIO_SET_DEV(bio, DATA_DISK_DEVICE);
	DZ_BIO_SET_SECTOR(bio, sector);
	DZ_BIO_SET_END_IO(bio, dz_read_biodone_sync);
	DZ_BIO_SET_ZIDX(bio);
	DZ_BIO_SET_READ(bio);
	DZ_BIO_SET_NEXT(bio, NULL);
	bio->bi_private = (VOID *)&io_completion;
	//TODO:bio->bi_flags   = 1 << BIO_UPTODATE;
	//bio->bi_size	= bio_size;
	
	/*
	{
		page = dz_read_page_alloc();
		if (!bio_add_page(bio, page, PAGE_SIZE, 0)) {
			LOGE("Unable to add page to bio\n");
			error = IO_ERROR_EIO;
			goto exit_failure;
		}
	}
	*/
	

#ifdef DZ_KERNEL_VERSION_4
	bio_for_each_segment(bvec, parent_bio, iter) {
		if (!bio_add_page(bio, bvec.bv_page, bvec.bv_len, bvec.bv_offset)) {
			LOGE("Unable to add page to bio\n");
			error = IO_ERROR_EIO;
			goto exit_failure;
		}
	}
#else
	bio_for_each_segment(bvec, parent_bio, count) {
		if (!bio_add_page(bio, bvec->bv_page, bvec->bv_len, bvec->bv_offset)) {
			LOGE("Unable to add page to bio\n");
			error = IO_ERROR_EIO;
			goto exit_failure;
		}
	}
#endif
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
		goto exit_failure;
	}

exit_done:
	_RELEASE_LBA_LOCK_READ(lba_entry);
	//Now add this request to BEC page cache
	iorequest->ior_sector = sector;
	//Increment the bio ref count, so that it does not 
	//get freed while bi_endio().
	//bio will be bio_put once page is added into
	//bec page cache
	DZ_OS_KERNEL_BIO_GET(iorequest->ior_bio);
	READ_IO_DONE_STATUS(iorequest, error);
	dz_bec_proceed_pba_page_add_read(iorequest);
	//iorequest_put(iorequest);
	return;

exit_failure:
	_RELEASE_LBA_LOCK_READ(lba_entry);
	READ_IO_DONE_STATUS(iorequest, error);
	iorequest_put(iorequest);
}

RVOID dz_dedupe_fill_init_data(PBIO bio, LBA lba)
{
	CHAR *ptr = NULL;
	INT cnt = 0;
	INT idx = 0;
	CHAR *buffer_4k_lba = vmalloc(PAGE_SIZE+1);
	CHAR bufline[65];
	if (!buffer_4k_lba) {
		BUG_ON(1);
	}

	ptr = buffer_4k_lba;
	memset(ptr, 0, PAGE_SIZE+1);
	memset(bufline, 0, 65);


	for (idx = 0; idx < 64; idx++) {
		cnt = snprintf(bufline, 65,
			"INIT_LBA_%8d_LINE_%2d_NOTHING_SUCCEED_LIKE_SUCCESS_LINE_%2d_\n",
			(INT)lba, idx+1, idx+1);

		memcpy(ptr, bufline, 64);

		//LOG("idx=%d, cnt=%d, %s", idx, cnt, ptr);
		ptr = ptr + cnt;
		memset(bufline, 0, 65);
	}

	//TODO:Check that if a single 4K data is spread across
	//multiple bvec pages.
	COPY_TO_BIO(bio, buffer_4k_lba);


	//print_bio_page_lines(bio);
	//print_bio_page(bio);

	vfree(buffer_4k_lba);
}

RVOID dz_dedupe_read_io(PIOREQUEST iorequest)
{
	PBIO	bio 				= iorequest->ior_bio; // Parent or Original bio
	SECTOR	sector 				= 0;
	LBA		lba					= iorequest->ior_lba;

	PLBA_BLOCK	lba_entry		= NULL;

	lba_entry = GET_LBA_ENTRY(lba);

	_ACQUIRE_LBA_LOCK_READ(lba_entry);

	if (lba_entry->hash_index == 0) {
		//LOG("AVERMA:Page is zeroed out\n");

		ATOMIC64_INC(dedupe_zeroed_out_page_reads);
		goto zero_out_bio_page;
	} else {
		BUG_ON(1);
		sector = dz_convert_lba_to_sector(lba_entry->pba_index);
	}

	if(dz_bec_pba_page_lookup(sector, bio)) {
		goto exit_done;
	}
	
//disk_read:
	if (iorequest->ior_parent) {
		//bio allocation NOT required
		LOG("Disk Read @ sector %lli for internal bio \n", (U64)sector);
		dz_read_block_from_raw_disk_sync_internal(iorequest, sector, lba_entry);
	} else {
		//bio allocation required
		//LOG("Disk Read @ sector %lli for external bio \n", (U64)sector);
		//LOG("Cache donning REF cnt = %d, iorequest=%p\n",ATOMIC_READ(iorequest->ref_cnt), iorequest);
		dz_read_block_from_raw_disk_sync(iorequest, sector, lba_entry);
	}
	return;

zero_out_bio_page:
	DZ_ZERO_PAGE(bio);
	dz_dedupe_fill_init_data(bio, lba);

exit_done:
	_RELEASE_LBA_LOCK_READ(lba_entry);
	if (iorequest->ior_parent) {
		LOGD("Parent Found for iorequest %p. ior_type = %u\n", iorequest, iorequest->ior_type);
		IO_DONE(iorequest);
		//Just done the child bio.
		//Iorequest will be freed in the child biodone
		return;
	}
	//LOG("Simply donning REF cnt = %d, iorequest=%p\n",ATOMIC_READ(iorequest->ref_cnt), iorequest);
	LOGD("Parent itself Found for iorequest %p. ior_type = %u\n", iorequest, iorequest->ior_type);
	IO_DONE(iorequest);
	dz_io_free(iorequest);
}
EXPORT_SYMBOL(dz_dedupe_read_io);

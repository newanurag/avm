/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PDZ_THREAD_POOL 	align_read_thread_pool;
extern PDZ_THREAD_POOL 	dedupe_thread_pool;
extern INT dz_switch_dedupe_engine;
extern INT dz_switch_bec_engine;
extern INT dz_switch_fec_engine;

extern SPINLOCK			print_iorequest_lock;
extern PBLOCK_DEVICE DATA_DISK_DEVICE;

extern ATOMIC64 align_iocount_disk_read;
extern ATOMIC64 align_iocount_disk_read_internal;
extern ATOMIC64 align_iocount_reads_single_page;
extern ATOMIC64 align_iocount_reads_single_aligned_page;
extern ATOMIC64 align_iocount_reads_single_unaligned_page;


//This function is used to copy the partial tail data from
//child bio to Parent's page
//Here argument bio is a child bio

static RVOID dz_align_read_for_single_block_biodone_tail(PBIO bio)
{
	PIOREQUEST ciorequest 		= (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest 		= ciorequest->ior_parent;
	PVOID   	cpagebuf		= NULL;
	PVOID   	ppagebuf		= NULL;
	BIOVEC		cbvec;
	BIOVEC		pbvec;
	PBIO		pbio			= piorequest->ior_bio; //Parent BIO
	//INT 		ret 			= SUCCESS;
	SECTOR		sector			= DZ_BIO_GET_SECTOR(pbio);
	UINT	sector_pos_in_lba	= 0;
	LBA		lba					= piorequest->ior_lba;
	//TODO:SIZE	size				= pbio->bi_size;
	SIZE	size				= DZ_BIO_GET_SIZE(pbio);
	PBIOVEC		last_bvec  		= NULL;
	INT			bi_vcnt			= 0;
	INT			bi_vcnt_max		= 0;
	INT			bv_len			= 0;
	INT			bv_offset		= 0;
	INT partial_bytes_tail;
	INT partial_bytes_head;
	INT partial_bytes;
	//LOG("child_biodone\n");	
	/*TODO:
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) { error = -EIO; }
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	if (error) {
		IO_DONE_STATUS(piorequest, error);
		goto exit;
	}
	*/
	IF_BIO_ERROR(bio) {
		IOREQUEST_DONE_ERROR(piorequest);
		goto exit;
	}
	sector_pos_in_lba 	= (sector * SECTOR_SIZE)  - (lba * LBA_BLOCK_SIZE);
	partial_bytes_head 	= LBA_BLOCK_SIZE - sector_pos_in_lba;
	partial_bytes_tail 	= size - partial_bytes_head;
	partial_bytes		= partial_bytes_tail;

	//From child bio. Set the pointer where the actual data has to be read
	cbvec	 = bio_iovec(bio);
	cpagebuf = kmap(cbvec.bv_page);
	cpagebuf += sector_pos_in_lba;

	//From Parent bio, Go to the location in parent's page where we have to copy
	//the data from the child bio
	bi_vcnt_max = pbio->bi_vcnt;
	pbvec 	= bio_iovec(pbio);
	bi_vcnt = pbio->bi_vcnt - 1;

	while(partial_bytes > 0) {
		while(bi_vcnt >= 0) {
			last_bvec = &pbio->bi_io_vec[bi_vcnt];
			bv_len = last_bvec->bv_len;
			partial_bytes -= bv_len ;
			if (partial_bytes <= 0) {
				//bv_len 	+= partial_bytes;
				bv_offset 	= last_bvec->bv_offset - partial_bytes;
				ppagebuf 	= kmap(last_bvec->bv_page);
				ppagebuf	+= bv_offset;
				PMEMCPY(ppagebuf, cpagebuf, partial_bytes_tail);
				kunmap(last_bvec->bv_page);
				break;
			} else {
				bv_offset 	= last_bvec->bv_offset;
				ppagebuf 	= kmap(last_bvec->bv_page);
				ppagebuf	+= bv_offset;
				PMEMCPY(ppagebuf, cpagebuf, bv_len);
				kunmap(last_bvec->bv_page);
				cpagebuf	+= bv_len;
				bi_vcnt--;
			}
		}
	}

	kunmap(cbvec.bv_page);
	if (atomic_dec_and_test(&piorequest->ior_child_cnt)) {
		// Now complete the Parent / Original bio
		//IO_DONE_STATUS(piorequest, error);
		IOREQUEST_DONE(piorequest);
		dz_read_page_free(cbvec.bv_page);
		DZ_OS_KERNEL_BIO_PUT(bio);
		iorequest_put(ciorequest);
		iorequest_put(piorequest);

		return;
	}

exit:
	dz_read_page_free(cbvec.bv_page);
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	
	return;
}

//This function is used to copy the partial head data from
//child bio to Parent's page
//Here argument bio is a child bio

static RVOID dz_align_read_for_single_block_biodone_head(PBIO bio)
{
	PIOREQUEST ciorequest 		= (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest 		= ciorequest->ior_parent;
	PVOID   	cpagebuf		= NULL;
	PVOID   	ppagebuf		= NULL;
	BIOVEC		cbvec;
	BIOVEC		pbvec;
	PBIO		pbio			= piorequest->ior_bio; //Parent BIO
	//INT 		ret 			= SUCCESS;
	SECTOR		sector			= DZ_BIO_GET_SECTOR(pbio);
	UINT	sector_pos_in_lba	= 0;
	LBA		lba					= piorequest->ior_lba;
	INT			bv_len			= 0;
	INT partial_bytes_head;
	ITERATOR iter;

	/* TODO:
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	if (error) {
		//TODO:
		IO_DONE_STATUS(piorequest, error);
		goto exit;
	}
	*/
	IF_BIO_ERROR(bio) {
		IOREQUEST_DONE_ERROR(piorequest);
		goto exit;
	}
	sector_pos_in_lba = (sector * SECTOR_SIZE)  - (lba * LBA_BLOCK_SIZE);
	partial_bytes_head = LBA_BLOCK_SIZE - sector_pos_in_lba;

	//From child bio. Set the pointer where the actual data has to be read
	cbvec	 = bio_iovec(bio);
	cpagebuf = kmap(cbvec.bv_page);
	cpagebuf += sector_pos_in_lba;

	//From Parent bio, Go to the location in parent's page where we have to copy
	//the data from the child bio
	bio_for_each_segment(pbvec, pbio, iter) {
		ppagebuf = kmap(pbvec.bv_page);
		ppagebuf += pbvec.bv_offset;
		bv_len	 = pbvec.bv_len;	
		if (partial_bytes_head > bv_len) {
			//Very rare but possible. i.e. Partial tobe read data
			//is spread across multiple pages
			//PRINT_POINTER(pbvec.bv_page);
			//PRINT_INT(pbvec.bv_len);
			//PRINT_INT(pbvec.bv_offset);
			//PRINT_INT(partial_bytes_head);
			//PRINT_UINT(sector_pos_in_lba);
			//print_iorequest(ciorequest, "Unsupported");

			PMEMCPY(ppagebuf, cpagebuf, bv_len);
			cpagebuf += bv_len;
			partial_bytes_head -= bv_len;
			kunmap(pbvec.bv_page);
			continue;
		} else {
			//Actually copy the partial data contents from child bio's page
			//to Parent's bio page
			PMEMCPY(ppagebuf, cpagebuf, partial_bytes_head);
			kunmap(pbvec.bv_page);
			break;
		}
	}
	kunmap(cbvec.bv_page);

	if (atomic_dec_and_test(&piorequest->ior_child_cnt)) {
		// Now complete the Parent / Original bio
		//IO_DONE_STATUS(piorequest, error);
		IOREQUEST_DONE(piorequest);
		dz_read_page_free(cbvec.bv_page);
		DZ_OS_KERNEL_BIO_PUT(bio);
		iorequest_put(ciorequest);
		iorequest_put(piorequest);
		return;
	}

exit:
	dz_read_page_free(cbvec.bv_page);
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	
	return;
}

// This function will create and issue two bios; one for head
// and other for tail. These two bios will be served by read_thread_pool
// We could have used loop to run twice, but that will have additional overhead
// for failure scenarios
static RVOID dz_align_read_double_page_async(PIOREQUEST parent_iorequest,
		BIODONE done_head, BIODONE done_tail)
{
	INT			ret 					= SUCCESS;
	PIOREQUEST	child_iorequest_head	= NULL;
	PIOREQUEST	child_iorequest_tail 	= NULL;
	PBIO		child_bio_head 			= NULL;
	PBIO		child_bio_tail 			= NULL;
	PPAGE		page_head				= NULL;
	PPAGE		page_tail				= NULL;
	PBIO		bio 					= parent_iorequest->ior_bio; // Parent or Original bio
	LBA			lba						= parent_iorequest->ior_lba;
	SECTOR		sector					= dz_convert_lba_to_sector(lba);

	//Do the resource allocation for head part
	//Allocate head child iorequest
	child_iorequest_head = dz_io_alloc(); 
	if (!child_iorequest_head) {
		LOGFECE("Unable to get free iorequest\n");
		goto exit_failure;
	}
	MEMSET(child_iorequest_head);

	//Allocate head child bio
	child_bio_head = dz_bio_alloc(1);
	if (!child_bio_head) {
		LOGFECE("Unable to get free child bio\n");
		goto exit_failure;
	}
	
	//Allocate head child bio read page
	page_head =  dz_read_page_alloc();
	if (!page_head)  {
		LOGFECE("Unable to get free read page\n");
		goto exit_failure;
	}
	dz_init_child_bio(bio, child_bio_head, sector, done_head, child_iorequest_head);

	//Associate head read page to head read bio
	ret = bio_add_page(child_bio_head, page_head, PAGE_SIZE, 0);
	if (!ret) {
			//IO_DONE_ERROR(parent_iorequest, ret);
			IOREQUEST_DONE_ERROR(parent_iorequest);
			LOGFECE("Unable to add Page to child bio head\n");
			goto exit_alloc;
			return;
	}
	atomic_set(&child_iorequest_head->ior_child_cnt, 0);
	child_iorequest_head->ior_thread_pool	= align_read_thread_pool;
	child_iorequest_head->ior_bio 			= child_bio_head;
//	child_iorequest_head->target 		= parent_iorequest->target;
	child_iorequest_head->ior_lba 			= lba;
	child_iorequest_head->ior_sector 		= sector;
	child_iorequest_head->ior_parent 		= parent_iorequest;
	child_iorequest_head->ior_type 		= DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ;

	//Increment the child_cnt in parent iorequest for head iorequest
	atomic_inc(&parent_iorequest->ior_child_cnt);
	
	//Now do the resource allocation for tail part
	//First increment the lba because it will be next lba
	lba++;
	sector								= dz_convert_lba_to_sector(lba);
	
	//Allocate tail child iorequest
	child_iorequest_tail = dz_io_alloc(); 
	if (!child_iorequest_tail) {
		LOGFECE("Unable to get free iorequest tail\n");
		goto exit_failure;
	}
	MEMSET(child_iorequest_tail);
	
	//Allocate tail child bio
	child_bio_tail = dz_bio_alloc(1);
	if (!child_bio_tail) {
		LOGFECE("Unable to get free child bio tail\n");
		goto exit_failure;
	}
	
	//Allocate tail child read page
	page_tail =  dz_read_page_alloc();
	if (!page_tail)  {
		LOGFECE("Unable to get free read page tail\n");
		goto exit_failure;
	}
	dz_init_child_bio(bio, child_bio_tail, sector, done_tail, child_iorequest_tail);

	ret = bio_add_page(child_bio_tail, page_tail, PAGE_SIZE, 0);
	if (!ret) {
			//IO_DONE_ERROR(parent_iorequest, ret);
			IOREQUEST_DONE_ERROR(parent_iorequest);
			LOGFECE("Unable to add Page to child bio tail\n");
			goto exit_alloc;
	}
	atomic_set(&child_iorequest_tail->ior_child_cnt, 0);
	child_iorequest_tail->ior_thread_pool	= align_read_thread_pool;
	child_iorequest_tail->ior_bio 		= child_bio_tail;
//	child_iorequest_tail->target 		= parent_iorequest->target;
	child_iorequest_tail->ior_lba 			= lba;
	child_iorequest_head->ior_sector 		= sector;
	child_iorequest_tail->ior_parent 		= parent_iorequest;
	child_iorequest_tail->ior_type 		= DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ;
	
	//Increment the child_cnt in parent iorequest for tail iorequest also
	atomic_inc(&parent_iorequest->ior_child_cnt);

	//Set siblings. TODO:Need to see impact in kernel
	child_bio_head->bi_next = child_bio_tail;
	child_bio_tail->bi_next = child_bio_head;

	//Now queue these head and tail child iorequests to their
	//corresponding thread pool
	dz_q_iorequest_thread_pool(child_iorequest_head);
	dz_q_iorequest_thread_pool(child_iorequest_tail);
	return;

exit_failure:
//	IO_DONE_ERROR(parent_iorequest, IO_ERROR_ENOMEM);
	IOREQUEST_DONE_ERROR(parent_iorequest);

exit_alloc:
	if (page_head) {
		dz_read_page_free(page_head);
	}
	if (page_tail) {
		dz_read_page_free(page_tail);
	}
	if (child_bio_head) {
		DZ_OS_KERNEL_BIO_PUT(child_bio_head); 
	}
	if (child_bio_tail) {
		DZ_OS_KERNEL_BIO_PUT(child_bio_tail); 
	}
	if (child_iorequest_head) {
		dz_io_free(child_iorequest_head);
	}
	if (child_iorequest_tail) {
		dz_io_free(child_iorequest_tail);
	}
	iorequest_put(parent_iorequest);
}



//This function does a single page read from the raw disk
//by allocating a new bio. Creation of bio is needed, because
//we cant change the original bio bi_private field.
//Here the page/pages are provided from a parent bio
RINT dz_align_read_single_block_from_raw_disk_sync(PIOREQUEST iorequest, SECTOR sector)
{
	PBIO 	parent_bio	= iorequest->ior_bio;
	PBIO 		bio = NULL;
	INT error		= SUCCESS;
	BIOVEC	bvec;
	DZ_COMPLETION   io_completion;
	//PPAGE	page 	= NULL;
	ITERATOR iter;
	
	bio = dz_bio_alloc(parent_bio->bi_vcnt);
	if (!bio) {
		LOGE("Unable to get free bio\n");
		error = IO_ERROR_ENOMEM;
		goto exit_done;
	}

	io_completion.error = SUCCESS;
	//bio->bi_bdev 	= DATA_DISK_DEVICE;
	DZ_BIO_SET_BDEV(bio, DATA_DISK_DEVICE);
	DZ_BIO_SET_SECTOR(bio, sector);
	bio->bi_end_io 	= dz_read_biodone_sync;
	DZ_BIO_SET_ZIDX(bio);
	//bio->bi_rw 		= READ;
	DZ_BIO_SET_READ(bio);
	bio->bi_next 	= NULL;
	bio->bi_private = (VOID *)&io_completion;
	//bio->bi_flags   = 1 << BIO_UPTODATE;
	
	bio_for_each_segment(bvec, parent_bio, iter) {
		if (!bio_add_page(bio, bvec.bv_page, bvec.bv_len, bvec.bv_offset)) {
			LOGE("Unable to add page to bio\n");
			error = IO_ERROR_EIO;
			goto exit_failure;
		}
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
		goto exit_failure;
	}

exit_done:
	//READ_IO_DONE_STATUS(iorequest, error);
	//TODO:We need to free the bio
	
	return error;

exit_failure:
	//READ_IO_DONE_STATUS(iorequest, error);
	return error;
}

EXPORT_SYMBOL(dz_align_read_single_block_from_raw_disk_sync);

//This function does a direct read from the raw disk.
//i.e. no creation of bio, pages etc.
//This is applicable for internally generated child bios from 
//alignment engine. It is not for application bios
//Note: It does not free iorequest
RINT dz_align_read_single_block_from_raw_disk_sync_internal(PIOREQUEST iorequest, SECTOR sector )
{
	PBIO 		bio = iorequest->ior_bio;
	INT error		= SUCCESS;
	DZ_COMPLETION   io_completion;
	BIODONE			orig_biodone = bio->bi_end_io;
	SECTOR			orig_sector	 = DZ_BIO_GET_SECTOR(bio);
	PVOID			orig_private = bio->bi_private;
	
	//LOGALIGN("Inside Before: bio->bi_size = %d, bio = %p, bio->bi_cnt = %d bio_page = %p\n",
	 //DZ_BIO_GET_SIZE(bio), bio, ATOMIC_READ(bio->bi_cnt),bio_page(bio));
	LOGALIGN("Inside Before: bio->bi_size = %d, bio = %p, bio_page = %p\n",
	DZ_BIO_GET_SIZE(bio), bio, bio_page(bio));
	
	io_completion.error = SUCCESS;
	//bio->bi_bdev 	= DATA_DISK_DEVICE;
	DZ_BIO_SET_BDEV(bio, DATA_DISK_DEVICE);
	DZ_BIO_SET_SECTOR(bio, sector);
	//bio->bi_end_io 	= dz_read_biodone_sync;
	DZ_BIO_SET_BIODONE(bio, dz_read_biodone_sync);
	DZ_BIO_SET_ZIDX(bio);
	//bio->bi_rw 		= READ;
	DZ_BIO_SET_READ(bio);
	bio->bi_next 	= NULL;
	bio->bi_private = (VOID *)&io_completion;
	//bio->bi_flags   = 1 << BIO_UPTODATE;

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
	bio->bi_private = orig_private;
	bio->bi_end_io = orig_biodone;
	DZ_BIO_SET_SECTOR(bio, orig_sector);
	//READ_IO_DONE_STATUS(iorequest, error);
	//Just done the child bio.
	//Iorequest will be freed in the child biodone
	LOGALIGN("Inside After: bio->bi_size = %d, bio = %p, bio->bi_cnt = %d\n", DZ_BIO_GET_SIZE(bio), bio, DZ_BIO_GET_PIN_COUNT(bio));
	return error;
}
EXPORT_SYMBOL(dz_align_read_single_block_from_raw_disk_sync_internal);

//Handle scenario when fec is disabled, dedupe is disabled and bec is also disabled
RVOID static dz_align_becd(PIOREQUEST piorequest)
{
	PBIO	bio 				= piorequest->ior_bio; // Parent or Original bio
	SECTOR	sector 				= DZ_BIO_GET_SECTOR(bio);
	INT		error = 0;
	if (piorequest->ior_parent) {
		INC_COUNTER(align_iocount_disk_read_internal);
		error = dz_align_read_single_block_from_raw_disk_sync_internal(piorequest, sector );
		READ_IO_DONE_STATUS(piorequest, error);
		iorequest_put(piorequest);
	} else {
		INC_COUNTER(align_iocount_disk_read);
		error = dz_align_read_single_block_from_raw_disk_sync(piorequest, sector );
		READ_IO_DONE_STATUS(piorequest, error);
		iorequest_put(piorequest);
	}
}

//Handle scenario when fec is disabled, dedupe is disabled and bec is enabled
RVOID static dz_align_fecd_and_bece(PIOREQUEST piorequest)
{
	PBIO	bio 				= piorequest->ior_bio; // Parent or Original bio
	SECTOR	sector 				= DZ_BIO_GET_SECTOR(bio);
	BOOL	status				= FALSE;
	PPAGE	cache_page			= NULL;
	INT		error = 0;
	//Look up from BEC Cache
	status = dz_bec_pba_page_lookup(sector, bio);
	if (status == FALSE) {
		cache_page = dz_bec_page_alloc();
	}
			
	//For Indirect or Non Application IOs
	DZ_OS_KERNEL_BIO_GET(bio);
	if (piorequest->ior_parent) {
		INC_COUNTER(align_iocount_disk_read_internal);
		LOGALIGN("AVERMA:Before:bi_vcnt = %d , error = %d, ior_type = %d, bi_size = %d, bio = %p, ior_bio = %p, ior = %p\n", 
		bio->bi_vcnt, error, piorequest->ior_type, DZ_BIO_GET_SIZE(bio), bio, piorequest->ior_bio, piorequest);
		error = dz_align_read_single_block_from_raw_disk_sync_internal(piorequest, sector );
		LOGALIGN("AVERMA:After:bi_vcnt = %d , error = %d, ior_type = %d, bi_size = %d, bio = %p, ior_bio = %p, ior = %p\n", 
		bio->bi_vcnt, error, piorequest->ior_type, DZ_BIO_GET_SIZE(bio), bio, piorequest->ior_bio, piorequest);

	} else { //For Direct or Application IOs
		INC_COUNTER(align_iocount_disk_read);
		error = dz_align_read_single_block_from_raw_disk_sync(piorequest, sector );
		LOGALIGN("AVERMA:App:bio->bi_vcnt = %d , error = %d, ior_type = %d\n", bio->bi_vcnt, error, piorequest->ior_type);
	}

	if (!error) { //Copy the read bio page data into cache_page
		if (status == FALSE && cache_page) {
			if (bio->bi_vcnt == 1) {
				//COPY_SINGLE_PAGE_FROM_BIO(cache_page, bio);
				{												
					PVOID   	dpagebuf= NULL;					
					PVOID   	spagebuf= NULL;    				
					CHAR		lbuf[32];
					BIOVEC		 bvec	= bio_iovec((bio));		
					dpagebuf = kmap((cache_page));					
					spagebuf = kmap(bvec.bv_page);				
					spagebuf += bvec.bv_offset;				
					PMEMCPY(lbuf, spagebuf, 32);		
					if (bvec.bv_len == 0) {
						LOGALIGN("AVERMA:here4:spagebuf = %p, dpagebuf = %p, bio = %p, iorquest = %p, bv_page =%p\n", spagebuf, dpagebuf, bio, piorequest, bvec.bv_page);
						LOGALIGN("AVERMA:here41:bvec.bvec_len = %d, bvec.bv_offset = %d \n", bvec.bv_len, bvec.bv_offset);
						SL1;
					}

					PMEMCPY(dpagebuf, spagebuf, bvec.bv_len);		
					//LOGALIGN("AVERMA:here5\n");
					kunmap(bvec.bv_page);						
					kunmap(cache_page);							
				}
				
			} else {
				COPY_PAGE_FROM_BIO(cache_page, bio);
			}
		}
	}

	//Now done with the Application IO or Indirect IO
	READ_IO_DONE_STATUS(piorequest, error);

	//Put this Page into BEC (BackeEnd Cache)
	if (cache_page) {
		piorequest->ior_cache_page = cache_page;
		//LOGALIGN("Cache Page is being added to BEC\n");
		dz_bec_page_free(cache_page);
		//dz_bec_proceed_pba_page_add_read(piorequest);
		iorequest_put(piorequest);
	} else {
		iorequest_put(piorequest);
	}
}


// This function handles two different cases:
// 1: Page Aligned. In that case only 1 bio is required.
// 2: Unaligned. Two child bios will be spawned. Sharing the same original bio Page
// Here the sector + bv_len will spill over to next lba.
RVOID dz_align_read_for_single_block(PIOREQUEST piorequest)
{
	PBIO	bio 				= piorequest->ior_bio; // Parent or Original bio
	SECTOR	sector 				= DZ_BIO_GET_SECTOR(bio);


	if (((sector * SECTOR_SIZE) % LBA_BLOCK_SIZE) == 0 ) { // Block Page Aligned. Case 1

		LOGD("Case1: Aligned Single Page\n");
		INC_COUNTER(align_iocount_reads_single_aligned_page);
		//dz_fec_align_read_io_direct(piorequest);

		if (dz_switch_dedupe_engine == DZ_ENABLED) {
			piorequest->ior_thread_pool = dedupe_thread_pool;
			dz_q_iorequest(dedupe_thread_pool, piorequest);
			return;
		}

		if (dz_switch_bec_engine == DZ_ENABLED && dz_switch_fec_engine == DZ_DISABLED) {
			dz_align_fecd_and_bece(piorequest);

		} else { //If BEC is disabled
			dz_align_becd(piorequest);


		}
		return;

	} else {		// Case 2

		// IO is going to spill over to next lba. Here
		// two child requests will be created. One for current Block
		// and other for spilled over block
		LOGFEC("Case2: UnAligned Single Page\n");
		INC_COUNTER(align_iocount_reads_single_unaligned_page);
		dz_align_read_double_page_async(piorequest,
			dz_align_read_for_single_block_biodone_head,
			dz_align_read_for_single_block_biodone_tail);
		return;
	}
}



/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "fec_engine.h"

extern PDZ_THREAD_POOL 	dedupe_thread_pool;
extern PDZ_THREAD_POOL 	read_thread_pool;

//For operational purpose
extern PFEC_TABLE 		fec_tables_active;
extern PFEC_TABLE 		fec_tables_flush;
extern PLBA_BLOCK		lba_table;
//static U64			IO_COUNT = 0;
extern SPINLOCK			print_iorequest_lock;

extern ATOMIC64 fec_iocount_reads_single_page;
extern ATOMIC64 fec_iocount_reads_single_aligned_page;
extern ATOMIC64 fec_iocount_reads_single_unaligned_page;


//This function is used to copy the partial tail data from
//child bio to Parent's page
//Here argument bio is a child bio

RVOID dz_far_for_single_block_biodone_tail(PBIO bio)
{
	PIOREQUEST ciorequest 		= (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest 		= ciorequest->ior_parent;
	PVOID   	cpagebuf		= NULL;
	PVOID   	ppagebuf		= NULL;
	BIOVEC		cbvec;
	BIOVEC		pbvec;
	PBIO		pbio			= piorequest->ior_bio; //Parent BIO
	//TODO:INT 		ret 			= SUCCESS;
	SECTOR		sector			= DZ_BIO_GET_SECTOR(pbio);
	UINT	sector_pos_in_lba	= 0;
	LBA		lba					= piorequest->ior_lba;
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
	//TODO:
	/*
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	*/
	//TODO:ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	if (bio->bi_status != BLK_STS_OK) {
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
		IOREQUEST_DONE_STATUS(piorequest);
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

RVOID dz_far_for_single_block_biodone_head(PBIO bio)
{
	PIOREQUEST ciorequest 		= (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest 		= ciorequest->ior_parent;
	PVOID   	cpagebuf		= NULL;
	PVOID   	ppagebuf		= NULL;
	BIOVEC		cbvec;
	BIOVEC		pbvec;
	PBIO		pbio			= piorequest->ior_bio; //Parent BIO
	//TODO: INT 		ret 			= SUCCESS;
	SECTOR		sector			= DZ_BIO_GET_SECTOR(pbio);
	UINT	sector_pos_in_lba	= 0;
	LBA		lba					= piorequest->ior_lba;
	INT			bv_len			= 0;
	struct bvec_iter iter;
	INT partial_bytes_head;

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

	if (bio->bi_status != BLK_STS_OK) {
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
		IOREQUEST_DONE_STATUS(piorequest);
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
RVOID dz_fec_align_read_double_page_async(PIOREQUEST parent_iorequest,
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
			IOREQUEST_DONE_ERROR(parent_iorequest);
			LOGFECE("Unable to add Page to child bio head\n");
			goto exit_alloc;
			return;
	}
	atomic_set(&child_iorequest_head->ior_child_cnt, 0);
	child_iorequest_head->ior_thread_pool	= read_thread_pool;
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
			IOREQUEST_DONE_ERROR(parent_iorequest);
			LOGFECE("Unable to add Page to child bio tail\n");
			goto exit_alloc;
	}
	atomic_set(&child_iorequest_tail->ior_child_cnt, 0);
	child_iorequest_tail->ior_thread_pool	= read_thread_pool;
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
	//IOREQUEST_DONE_ERROR(parent_iorequest, IO_ERROR_ENOMEM);
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

// This function handles two different cases:
// 1: Page Aligned. In that case only 1 bio is required.
// 2: Unaligned. Two child bios will be spawned. Sharing the same original bio Page
// Here the sector + bv_len will spill over to next lba.
RVOID dz_fec_align_read_for_single_block(PIOREQUEST piorequest)
{
	PBIO	bio 				= piorequest->ior_bio; // Parent or Original bio
	SECTOR	sector 				= DZ_BIO_GET_SECTOR(bio);

	if (((sector * SECTOR_SIZE) % LBA_BLOCK_SIZE) == 0 ) { // Block Page Aligned. Case 1

		LOGD("Case1: Aligned Single Page\n");
		INC_COUNTER(fec_iocount_reads_single_aligned_page);
		dz_fec_align_read_io_direct(piorequest);
		return;

	} else {		// Case 2

		// IO is going to spill over to next lba. Here
		// two child requests will be created. One for current Block
		// and other for spilled over block
		LOGFEC("Case2: UnAligned Single Page\n");
		INC_COUNTER(fec_iocount_reads_single_unaligned_page);
		dz_fec_align_read_double_page_async(piorequest,
			dz_far_for_single_block_biodone_head,
			dz_far_for_single_block_biodone_tail);
		return;
	}
}


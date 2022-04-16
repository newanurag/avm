/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "fec_engine.h"
extern U64 				fec_table_size;
extern U64				fec_active_count;

extern PDZ_THREAD_POOL 	dedupe_thread_pool;

extern SPINLOCK			fec_active_lock;
extern SPINLOCK			fec_flush_lock;

//For operational purpose
extern PFEC_TABLE 		fec_tables_active;
extern PFEC_TABLE 		fec_tables_flush;
extern FEC_WRITE		fec_table_active;
extern PIOREQUEST		fec_flush_iorequest;

extern PDZ_THREAD_POOL read_thread_pool;
extern PLBA_BLOCK		lba_table;
extern ATOMIC64 fec_iocount_writes_partial_page;
extern ATOMIC64 fec_iocount_writes_single_page;
extern ATOMIC64 fec_iocount_writes_single_aligned_page;
extern ATOMIC64 fec_iocount_writes_single_unaligned_page;
extern ATOMIC64 fec_iocount_writes_multi_page;

RVOID dz_faw_partial_block_parent_biodone(PBIO bio)
{
	PIOREQUEST ciorequest = (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest = ciorequest->ior_parent;
	BIOVEC		pbvec;
	piorequest = ciorequest->ior_parent;
	PRINT_ATOMIC(piorequest->ior_child_cnt);
	if (atomic_dec_and_test(&piorequest->ior_child_cnt)) {
		// Now complete the Parent / Original bio
		//print_biom(piorequest->ior_bio, "Parent bio when all childs are done");
		LOGFEC("Parent IO Done\n");
		//TODO:IO_DONE_STATUS(piorequest, error);
		IOREQUEST_DONE_STATUS(piorequest); //How to pass correct error to upper layer
		pbvec = bio_iovec(bio);
		dz_read_page_free(pbvec.bv_page);
		DZ_OS_KERNEL_BIO_PUT(bio);
		iorequest_put(ciorequest);

		bio_data_dir(piorequest->ior_bio) == WRITE ? 
		iorequest_put(piorequest) : 
		iorequest_put(piorequest);
		return;
	}
	pbvec = bio_iovec(bio);
	dz_read_page_free(pbvec.bv_page);
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	
	return;
}

//RVOID dz_faw_partial_block_read_page_sync_biodone(PBIO bio, INT error)
RVOID dz_faw_partial_block_read_page_sync_biodone(PBIO bio)
{
	PDZ_COMPLETION		ior = (PDZ_COMPLETION)bio->bi_private;

	/* TODO
	INT ret = 0;
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	ior->error = error;

	*/
	IF_BIO_ERROR_SET_IO_ERROR(bio, ior->error);


	complete(&ior->io_completion);
}

//TODO:RVOID dz_faw_partial_block_double_page_async_biodone_head(PBIO bio, INT error)
RVOID dz_faw_partial_block_double_page_async_biodone_head(PBIO bio)
{
	PIOREQUEST	ciorequest 	= (PIOREQUEST)(bio->bi_private);
	PIOREQUEST	piorequest 	= ciorequest->ior_parent;
	PBIO		parent_bio	= piorequest->ior_bio;
	PFEC_WRITE	fecw 		= (PFEC_WRITE)(ciorequest->ior_private);
	PVOID   	pagebuf		= NULL;
	BIOVEC		pbvec;
	//INT 		ret 		= SUCCESS;
	INT			bv_offset	= ciorequest->ior_bv_offset;
	INT 		error 		= 0;

	/*
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	*/
	if (bio->bi_status != BLK_STS_OK)  {
		error = BLK_STS_IOERR;
	}

	
	LOGFEC("Inside biodone Head\n");
	PRINT_POINTER(fecw);
	//ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	//print_fecws(fecw, "Head First");

	//Now copy the original head part from child bio
	pbvec = bio_iovec(bio);
	pagebuf = kmap(pbvec.bv_page);
	pagebuf += pbvec.bv_offset;
	PMEMCPY(fecw->bv_page, pagebuf, bv_offset);
	kunmap(pbvec.bv_page);

	//Now merge the application data from parent bio
	pbvec = bio_iovec(parent_bio);
	pagebuf = kmap(pbvec.bv_page);
	pagebuf += bv_offset;
	PMEMCPY(fecw->bv_page + bv_offset, pagebuf, PAGE_SIZE - bv_offset );
	kunmap(pbvec.bv_page);

	SET_FEC_BUFFER_STATE(fecw, FECBUF_STATE_MEMORY_WRITE_COMPLETED);

	//TODO:dz_faw_partial_block_parent_biodone(bio, error);
	dz_faw_partial_block_parent_biodone(bio);

	FEC_ADD_WRITE_BUFFER_TO_LBA_TABLE(fecw, ciorequest->ior_lba);

	//print_fecws(fecw, "Head Last");
}

//TODO:RVOID dz_faw_partial_block_double_page_async_biodone_tail(PBIO bio, INT error)
RVOID dz_faw_partial_block_double_page_async_biodone_tail(PBIO bio)
{
	PIOREQUEST	ciorequest 	= (PIOREQUEST)(bio->bi_private);
	PIOREQUEST	piorequest 	= ciorequest->ior_parent;
	PBIO		parent_bio	= piorequest->ior_bio;
	PFEC_WRITE	fecw 	= (PFEC_WRITE)(ciorequest->ior_private);
	PVOID   	pagebuf	= NULL;
	BIOVEC		pbvec;	
	//INT 		ret 	= SUCCESS;
	INT			bv_len	= ciorequest->ior_bv_len;
	INT 		error 		= 0;

	/*
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	*/
	if (bio->bi_status != BLK_STS_OK)  {
		error = BLK_STS_IOERR;
	}

	LOGFEC("Inside biodone Tail\n");
	PRINT_POINTER(fecw);
	//ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	//print_fecws(fecw, "Tail First");

	pbvec = bio_iovec(parent_bio);
	//Copy unique data from parent bio
	//Note: We are assuming that the tail data
	//and the complete bio data 
	//will always be in one page only
	pagebuf = kmap(pbvec.bv_page);
	pagebuf += pbvec.bv_offset;
	pagebuf += pbvec.bv_len - bv_len;
	PMEMCPY(fecw->bv_page, pagebuf, bv_len);
	kunmap(pbvec.bv_page);
	
	
	//Now copy the remaining part from child bio tail
	pbvec = bio_iovec(bio);
	pagebuf = kmap(pbvec.bv_page);
	pagebuf += bv_len;
	PMEMCPY((fecw->bv_page  + bv_len), pagebuf, PAGE_SIZE - bv_len);
	kunmap(pbvec.bv_page);

	SET_FEC_BUFFER_STATE(fecw, FECBUF_STATE_MEMORY_WRITE_COMPLETED);

	//TODO:dz_faw_partial_block_parent_biodone(bio, error);
	dz_faw_partial_block_parent_biodone(bio);

	FEC_ADD_WRITE_BUFFER_TO_LBA_TABLE(fecw, ciorequest->ior_lba);

	print_fecws(fecw, "Tail Last");
}

//This function allocates a single single Page and issues a READ bio to 
//lower layer and get the READ data into that PAGE. Once done, it returns
//the PAGE to the caller. Caller is supposed to freeup that PAGE
//This operation is done synchronously. 

PPAGE dz_faw_partial_block_read_page_sync(PIOREQUEST iorequest, PBIO parent_bio, SECTOR sector)
{
	PPAGE      page	= NULL;
	PBIO 		bio = NULL;
	INT error		= 0;
	PVOID  	pagebuf	= NULL;
	DZ_COMPLETION   io_completion;

	page =  dz_read_page_alloc();
	if (!page)  {
		LOGFECE("Unable to get free read page\n");
		RETURNN;
	}

	bio = dz_bio_alloc(1);
	if (!bio) {
		LOGFECE("Unable to get free bio\n");
		dz_read_page_free(page);
		RETURNN;
	}

	pagebuf = kmap(page);
	memset(pagebuf, 0, PAGE_SIZE);
	kunmap(page);

	io_completion.error = 0;
	//bio->bi_bdev 	= parent_bio->bi_bdev;
	DZ_BIO_SET_DEV(bio, DZ_BIO_GET_DEV(parent_bio));
	DZ_BIO_SET_SECTOR(bio, sector);
	bio->bi_end_io 	= dz_faw_partial_block_read_page_sync_biodone;
	DZ_BIO_SET_ZIDX(bio);
	DZ_BIO_SET_READ(bio);
	bio->bi_next 	= NULL;
	bio->bi_private = (VOID *)&io_completion;
	//TODO:bio->bi_flags   = 1 << BIO_UPTODATE;
	//bio->bi_size	= bio_size;

	if (!bio_add_page(bio, page, PAGE_SIZE, 0)) {
			LOGFECE("Unable to add page to bio\n");
			dz_read_page_free(page);
			DZ_OS_KERNEL_BIO_PUT(bio); // It will free the bio as well
			RETURNN;
		}
	init_completion(&io_completion.completion);

	//Note: We are using the parent iorequest and assignin the child bio here
	iorequest->ior_bio = bio;
	//print_biom(bio, "Reading BIO Page");
	dz_fec_read_io(iorequest);
	wait_for_completion(&io_completion.completion);

	//Get the Error returned (if any)
	//io_completion = bio->bi_private;
	error = io_completion.error;
	if (unlikely(error)){
		LOGFECE("Error in reading raw data. Error Code : %d", error);
		print_biom(bio, "bio with error");
		DZ_OS_KERNEL_BIO_PUT(bio); // It will free the bio as well
		RETURNN;
	}
	DZ_OS_KERNEL_BIO_PUT(bio); // It will free the bio as well
	return page;

}

// We could have used loop to run twice, but that will have additional overhead
// for failure scenarios
RVOID dz_faw_partial_block_double_page_async(PIOREQUEST parent_iorequest,
		SECTOR sector_pos_in_lba,
		BIODONE done_head,
		BIODONE done_tail)
{
	INT			ret 					= SUCCESS;
	PFEC_WRITE	fecw_head				= NULL;
	PFEC_WRITE	fecw_tail				= NULL;
	PIOREQUEST	child_iorequest_head	= NULL;
	PIOREQUEST	child_iorequest_tail 	= NULL;
	PBIO		child_bio_head 			= NULL;
	PBIO		child_bio_tail 			= NULL;
	PPAGE		page_head				= NULL;
	PPAGE		page_tail				= NULL;
	PBIO		bio 					= parent_iorequest->ior_bio; // Parent or Original bio
	LBA			lba						= parent_iorequest->ior_lba;
	SECTOR		sector					= dz_convert_lba_to_sector(lba);
	INT			partial_bytes_head		= 0;
	INT			partial_bytes_tail		= 0;

	//Allocate fecw for head and tail in advance
	if (!(fecw_head = GET_FEC_SINGLE_FREE_BUFFER())) {
		DELAY_MICRO_SECONDS(1);
		IO_DONE_BUSY(parent_iorequest);
		return;
	}

	if (!(fecw_tail = GET_FEC_SINGLE_FREE_BUFFER())) {
		DELAY_MICRO_SECONDS(1);
		IO_DONE_BUSY(parent_iorequest);
		return;
	}
	//Do the resource allocation for head part
	child_iorequest_head = dz_io_alloc(); 
	if (!child_iorequest_head) {
		LOGFECE("Unable to get free iorequest\n");
		goto exit_failure;
	}
	child_bio_head = dz_bio_alloc(1);
	if (!child_bio_head) {
		LOGFECE("Unable to get free child bio\n");
		goto exit_failure;
	}
	page_head =  dz_read_page_alloc();
	if (!page_head)  {
		LOGFECE("Unable to get free read page\n");
		goto exit_failure;
	}
	dz_init_child_bio(bio, child_bio_head, sector, done_head, child_iorequest_head);
	DZ_BIO_SET_READ(child_bio_head);

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
	child_iorequest_head->ior_private 		= fecw_head;
	atomic_inc(&parent_iorequest->ior_child_cnt);
	
	//Now do the resource allocation for tail part
	lba++;
	sector								= dz_convert_lba_to_sector(lba);
	child_iorequest_tail = dz_io_alloc(); 
	if (!child_iorequest_tail) {
		LOGFECE("Unable to get free iorequest tail\n");
		goto exit_failure;
	}
	child_bio_tail = dz_bio_alloc(1);
	if (!child_bio_tail) {
		LOGFECE("Unable to get free child bio tail\n");
		goto exit_failure;
	}
	page_tail =  dz_read_page_alloc();
	if (!page_tail)  {
		LOGFECE("Unable to get free read page tail\n");
		goto exit_failure;
	}
	dz_init_child_bio(bio, child_bio_tail, sector, done_tail, child_iorequest_tail);
	DZ_BIO_SET_READ(child_bio_tail);

	ret = bio_add_page(child_bio_tail, page_tail, PAGE_SIZE, 0);
	if (!ret) {
			IOREQUEST_DONE_ERROR(parent_iorequest);
			LOGFECE("Unable to add Page to child bio tail\n");
			goto exit_alloc;
	}
	atomic_set(&child_iorequest_tail->ior_child_cnt, 0);
	child_iorequest_tail->ior_thread_pool	= read_thread_pool;
	child_iorequest_tail->ior_bio 			= child_bio_tail;
//	child_iorequest_tail->target 		= parent_iorequest->target;
	child_iorequest_tail->ior_lba 			= lba;
	child_iorequest_head->ior_sector 		= sector;
	child_iorequest_tail->ior_parent 		= parent_iorequest;
	child_iorequest_tail->ior_private 		= fecw_tail;
	atomic_inc(&parent_iorequest->ior_child_cnt);


	partial_bytes_head = LBA_BLOCK_SIZE - sector_pos_in_lba;
	partial_bytes_tail = DZ_BIO_GET_SIZE(bio) - partial_bytes_head;

	child_iorequest_head->ior_bv_len 	= partial_bytes_head;
	child_iorequest_head->ior_bv_offset = sector_pos_in_lba;

	child_iorequest_tail->ior_bv_len 	= partial_bytes_tail;
	child_iorequest_tail->ior_bv_offset = 0;
	//Set siblings
	child_bio_head->bi_next = child_bio_tail;
	child_bio_tail->bi_next = child_bio_head;
	dz_q_iorequest_thread_pool(child_iorequest_head);
	dz_q_iorequest_thread_pool(child_iorequest_tail);
	return;

exit_failure:
	//TODO:IO_DONE_ERROR(parent_iorequest, IO_ERROR_ENOMEM);
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


RVOID dz_fec_align_write_for_partial_block(PIOREQUEST parent_iorequest)
{
	PBIO	parent_bio 				= parent_iorequest->ior_bio; // Parent or Original bio
	SECTOR	sector 				= DZ_BIO_GET_SECTOR(parent_bio);
	LBA		lba					= parent_iorequest->ior_lba;
	UINT	sector_pos_in_lba	= 0;
	INT		size				= DZ_BIO_GET_SIZE(parent_bio);
	PFEC_WRITE	fecw			= NULL;
	PVOID   			pagebuf	= NULL;
	BIOVEC			pbvec;	
	PPAGE			readbackpage		= NULL;
	struct bvec_iter iter;

	if (((sector * SECTOR_SIZE) % LBA_BLOCK_SIZE) == 0 ) {
		// Start Sector is Page Aligned. One bio will be created
		LOGFEC("Case1: Partial but Start Sector Aligned\n");

do_rmw: //Do Read Modify Write i.e. Read a Page and merge the desired changes

		if (!(fecw = GET_FEC_SINGLE_FREE_BUFFER())) {
			DELAY_MICRO_SECONDS(1);
			IO_DONE_BUSY(parent_iorequest);
			return;
		}
		//sector_pos_in_lba = (sector * SECTOR_SIZE)  - (lba * LBA_BLOCK_SIZE);

		fecw->lba 	= lba;
		//Protect state so that during flush it will be consistent.

		if (parent_bio->bi_vcnt > 1) {
			//TODO:Since it is a partial bio, so its vcnt should be 1
			//We need to handle the case where it is more than 1
			BUG_ON(1);
		}

		readbackpage = dz_faw_partial_block_read_page_sync(parent_iorequest, parent_bio, sector);
		parent_iorequest->ior_bio = parent_bio;
		if (!readbackpage) {
			//TODO:IO_DONE_ERROR(parent_iorequest, IO_ERROR_ENOMEM);
			IOREQUEST_DONE_ERROR(parent_iorequest); //How to pass coorect nomem error
			return;
		}

		//First copy the full contents.
		//TODO:We are un-necessarily copying some unwanted
		//data
		pagebuf = kmap(readbackpage);
		PMEMCPY(fecw->bv_page, pagebuf, PAGE_SIZE);
		kunmap(readbackpage);

		//Now merge the changes to the original bio
		bio_for_each_segment(pbvec, (parent_bio), iter) {
			pagebuf = kmap(pbvec.bv_page);
			pagebuf += pbvec.bv_offset;
			PMEMCPY(fecw->bv_page + sector_pos_in_lba, pagebuf, pbvec.bv_len);	
			kunmap(pbvec.bv_page);
		}

		SET_FEC_BUFFER_STATE(fecw, FECBUF_STATE_MEMORY_WRITE_COMPLETED);

		WRITE_IO_DONE(parent_iorequest);
		//print_data(fecw->bv_page);

		FEC_ADD_WRITE_BUFFER_TO_LBA_TABLE(fecw, lba);

		iorequest_put(parent_iorequest);
		dz_read_page_free(readbackpage);

		return;

	} else {
		// Here sector + bv_len can be inside the same lba or spill over to next lba
		sector_pos_in_lba = (sector * SECTOR_SIZE)  - (lba * LBA_BLOCK_SIZE);

		if ((sector_pos_in_lba + size) <= LBA_BLOCK_SIZE) { 
			// Falls into Singe Page boundary
			sector_pos_in_lba = (sector * SECTOR_SIZE)  - (lba * LBA_BLOCK_SIZE);

			LOGFEC("Case2: Falls into Single Page Boundary but Unaligned\n");

			//Note: Here sector is NOT Page Aligned so while doing a single
			//page readback we need to see if we are reading correctly because
			//bio uses sector for readback. Ideally we should do a readback for
			//the entire lba Page in which this sector lies.
			goto do_rmw;
			
		} else {
			// Spill over to next lba/block
			// Here both pages i.e. head and tail will involve
			// Read Modify Write operations

			dz_faw_partial_block_double_page_async(parent_iorequest,
			sector_pos_in_lba,
			dz_faw_partial_block_double_page_async_biodone_head,
			dz_faw_partial_block_double_page_async_biodone_tail);
		}
	}
	return;
}

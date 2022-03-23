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

//RVOID dz_faw_single_block_parent_biodone(PBIO bio, INT error)
RVOID dz_faw_single_block_parent_biodone(PBIO bio)
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
		IOREQUEST_DONE_STATUS(piorequest); //WE need to pass correcto error/status to upper layer
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

//RVOID dz_faw_single_block_double_page_async_biodone_head(PBIO bio, INT error)
RVOID dz_faw_single_block_double_page_async_biodone_head(PBIO bio)
{
	PIOREQUEST	ciorequest 	= (PIOREQUEST)(bio->bi_private);
	PIOREQUEST	piorequest 	= ciorequest->ior_parent;
	PBIO		parent_bio	= piorequest->ior_bio;
	PFEC_WRITE	fecw 		= (PFEC_WRITE)(ciorequest->ior_private);
	PVOID   	pagebuf		= NULL;
	BIOVEC		pbvec;
	//INT 		ret 		= SUCCESS;
	INT			bv_offset	= ciorequest->ior_bv_offset;

	/* TODO:
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	*/

	
	LOGFEC("Inside biodone Head\n");
	PRINT_POINTER(fecw);
	//TODO:ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
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

	//TODO:dz_faw_single_block_parent_biodone(bio, error);
	dz_faw_single_block_parent_biodone(bio); //TODO:Check how the correct can be passed to upper layer

	FEC_ADD_WRITE_BUFFER_TO_LBA_TABLE(fecw, ciorequest->ior_lba);

	//print_fecws(fecw, "Head Last");
}

//RVOID dz_faw_single_block_double_page_async_biodone_tail(PBIO bio, INT error)
RVOID dz_faw_single_block_double_page_async_biodone_tail(PBIO bio)
{
	PIOREQUEST	ciorequest 	= (PIOREQUEST)(bio->bi_private);
	PIOREQUEST	piorequest 	= ciorequest->ior_parent;
	PBIO		parent_bio	= piorequest->ior_bio;
	PFEC_WRITE	fecw 	= (PFEC_WRITE)(ciorequest->ior_private);
	PVOID   	pagebuf	= NULL;
	BIOVEC		pbvec;
	//INT 		ret 	= SUCCESS;
	INT			bv_len	= ciorequest->ior_bv_len;

	/* TODO:
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	*/

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

	//dz_faw_single_block_parent_biodone(bio, error);
	dz_faw_single_block_parent_biodone(bio);//TODO:How to pass correct error

	FEC_ADD_WRITE_BUFFER_TO_LBA_TABLE(fecw, ciorequest->ior_lba);

	print_fecws(fecw, "Tail Last");
}

// We could have used loop to run twice, but that will have additional overhead
// for failure scenarios
// This function will readback two pages one for current lba and another
// for next lba
RVOID dz_faw_single_block_double_page_async(PIOREQUEST parent_iorequest,
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


RVOID dz_faw_single_block_iorequest_done(PVOID iorequest , INT error)
{
	PIOREQUEST		ior 	= (PIOREQUEST)iorequest;
	PFEC_WRITE		fecw 	= (PFEC_WRITE)ior->ior_private;

	FEC_ENTRY_WRITE_LOCK(fecw);
	if (error) {
		fecw->state 	= FECBUF_STATE_DEDUPE_WRITE_ERROR;
		fecw->io_completion.error = error;
	} else {
		fecw->state 	= FECBUF_STATE_DEDUPE_WRITE_COMPLETED;
	}
	FEC_ENTRY_WRITE_UNLOCK(fecw);
	complete(&fecw->io_completion.completion);

	LOGD("Disk IO Done for LBA %lli with iorequest %p\n", ior->lba, ior);
	iorequest_put(ior);
	return;
}

//This function allows writing a single PAGE_SIZE block
//It handles two cases:
//1. Write is PAGE_SIZE aligned
//2. Write is PAGE_SIZE but unaligned and hence ends up
//in issuing two partial writes spreading across two LBAs
//i.e. starting with the current LBA and ending into 
//(spilled over to) next LBA

RVOID dz_fec_align_write_for_single_block(PIOREQUEST iorequest)
{
	PBIO		bio 			= iorequest->ior_bio; // Parent or Original bio
	SECTOR		sector 			= DZ_BIO_GET_SECTOR(bio);
	LBA			lba				= iorequest->ior_lba;
	PFEC_WRITE	fecw			= NULL;
	UINT	sector_pos_in_lba	= 0;
	U64			sector_bytes	= sector * SECTOR_SIZE;
	UINT		retry_cnt		= 0;

	//If unaligned then handled the same way as it was handled for
	//spillover  in partial block
	if (((sector_bytes) % LBA_BLOCK_SIZE) != 0 ) { 
		LOGFEC("Case1: Single block unaligned Sector = %lli\n", (U64)sector);
		sector_pos_in_lba = (sector_bytes)  - (lba * LBA_BLOCK_SIZE);

		dz_faw_single_block_double_page_async(iorequest,
		sector_pos_in_lba,
		dz_faw_single_block_double_page_async_biodone_head,
		dz_faw_single_block_double_page_async_biodone_tail);
		return;
	}

	//Page Aligned IO 
start_processing:
	LOGFEC("Case2: Single block Aligned Sector = %lli\n", (U64)sector);
	if (!(fecw = GET_FEC_SINGLE_FREE_BUFFER())) {
		if (retry_cnt++ >= 64) {
			LOGFEC("Returning BUSY for lba %lli\n", lba);
			IO_DONE_BUSY(iorequest);
			return;
		} else {
			DELAY_MILLI_SECONDS(10);
			goto start_processing;
		}
	}

	// Now is the time to put this entry to read_table so that 
	// if READ comes in, it should be served fastly
	// Idea is to serve the Read IOs as fast as it can. I.e.
	// if the IO which is to be read is already present in fec cache
	// then serve from here.
	// This buffer will be removed from LBA table when the flushing
	// gets completed
	FEC_ADD_WRITE_BUFFER_TO_LBA_TABLE(fecw, lba);

	//print_fecw(fecw);
	//SET_FEC_BUFFER_STATE(fecw, FECBUF_STATE_MEMORY_WRITE_IN_PROGRESS);
	
	fecw->lba = lba;

	//Copy the bio page to fecw so that it can be used by dedupe engine
	// and/or any other purpose if required.
	//Set state so that during flush it will be consistent.
	COPY_FROM_BIO(fecw->bv_page, bio);
	SET_FEC_BUFFER_STATE(fecw, FECBUF_STATE_MEMORY_WRITE_COMPLETED);

	//At this point the actual data from application bio is stored in
	// front end cache write buffer (fecw->bv_page) . Now is the time
	// to complete this bio to top level application

	if (iorequest->ior_parent) { //For internally generated bios
		WRITE_IO_DONE(iorequest);
		return;
	} 

	// Since Application IO does not have parent so IO is completed to application
	// Note that here the context may get changed because the done function
	// of parent bio gets called which may delay the below mentioned processing
	WRITE_IO_DONE(iorequest);

	LOGFEC("Application IO Completed\n");
	
	//Now Free up the io request because this is direct application IO
	//iorequest_put(iorequest);
	
	// New feature, where IOs are happening to backend after completion of IO
	// to application. 
	// Now the next stage is to pass this IO to dedupe engine.
	// Since the IO is completed to Application so there is no bio existence.
	// But we have already copied the data page from the bio to fecw->bv_page.
	// This page data will be used by dedupe engine to check if it is
	// duplicate or not.
	// We are not going to create a new iorequest structure. Instead we are 
	// gonna reuse the existing one but with modified entries.
	
	// Setting the fecw state. This will be cleared inside dedupe engine 
	SET_FEC_BUFFER_STATE(fecw, FECBUF_STATE_DEDUPE_WRITE_IN_PROGRESS);

	fecw->io_completion.error = SUCCESS;
	init_completion(&fecw->io_completion.completion);

	// fecw->bv_page contains the application IO data.
	iorequest->ior_bv_buffer	= fecw->bv_page;
	iorequest->ior_donefun = dz_faw_single_block_iorequest_done;

	//Note : Since Application IO is done so here setting the parent bio as NULL. 
	//This is to ensure that this is internally generated IO Request
	iorequest->ior_bio 			= NULL;

	// fecw is stored as private element in iorequest. This will be used in
	// flush process
	iorequest->ior_private		= fecw;

	//Increment the ref_cnt of iorequest
	iorequest_get(iorequest);

	// There are two entitities here: iorequest and fecw.
	// iorequest: will go to dedupe engine and later to disk if required
	// or ignored if duplicate.
	// fecw: which is a frontend cache write buffer will be queued into
	// the flush queue. Here the actual meaning of flushing means, 
	// flushing of fec buffers to backend cache engine. This will save 
	// time to fill the backend cache.

	// Note: fecw will not be stored in backend cache until the dedupe
	// engine process the iorequeust.
	// E.g.
	// The current context is frontend cache engine. From here the IO
	// data processing is splitted into dedupe engine and bec engine.
	// But bec engine will not get the IO data until dedupe engine 
	// finishes that IO, because the significance of bec engine comes
	// when the IO is issued/forwarded from/through dedupe engine
	

	// Add the fecw buffer to flush queue of fec_tables_flush
	// This is to ensure that how many fecw buffers needs to be flushed
	dz_fec_add_write_buffer_to_flushq(fecw);

	LOGFEC("Dedupe Write IO Start for LBA %lli with iorequest %p\n", lba, iorequest);

	// queue this iorequest for dedupe_thread_pool. It will eventually call dz_dedupe_io()
	dz_q_iorequest(dedupe_thread_pool, iorequest); //Invokes dz_dedupe_io()

	return;
}

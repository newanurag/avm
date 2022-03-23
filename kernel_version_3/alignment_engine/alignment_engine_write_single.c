/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PDZ_THREAD_POOL  dedupe_thread_pool;
extern PDZ_THREAD_POOL align_read_thread_pool;
extern PLBA_BLOCK		lba_table;
extern ATOMIC64 align_iocount_writes_partial_page;
extern ATOMIC64 align_iocount_writes_single_page;
extern ATOMIC64 align_iocount_writes_single_aligned_page;
extern ATOMIC64 align_iocount_writes_single_unaligned_page;
extern ATOMIC64 align_iocount_writes_multi_page;

static RVOID dz_align_write_single_block_parent_biodone(PBIO bio, INT error)
{
	PIOREQUEST ciorequest = (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest = ciorequest->ior_parent;
	PBIOVEC		pbvec		= NULL;	
	piorequest = ciorequest->ior_parent;
	PRINT_ATOMIC(piorequest->ior_child_cnt);
	if (atomic_dec_and_test(&piorequest->ior_child_cnt)) {
		// Now complete the Parent / Original bio
		//print_biom(piorequest->ior_bio, "Parent bio when all childs are done");
		LOGALIGN("Parent IO Done\n");
		IO_DONE_STATUS(piorequest, error);
		pbvec = bio_iovec(bio);
		dz_read_page_free(pbvec->bv_page);
		DZ_OS_KERNEL_BIO_PUT(bio);
		iorequest_put(ciorequest);

		bio_data_dir(piorequest->ior_bio) == WRITE ? 
		iorequest_put(piorequest) : 
		iorequest_put(piorequest);
		return;
	}
	pbvec = bio_iovec(bio);
	dz_read_page_free(pbvec->bv_page);
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	
	return;
}

static RVOID dz_align_write_single_block_double_page_async_biodone_head(PBIO bio, INT error)
{
	PIOREQUEST	ciorequest 	= (PIOREQUEST)(bio->bi_private);
	PIOREQUEST	piorequest 	= ciorequest->ior_parent;
	PBIO		parent_bio	= piorequest->ior_bio;
	PFEC_WRITE	fecw 		= (PFEC_WRITE)(ciorequest->ior_private);
	PVOID   	pagebuf		= NULL;
	PBIOVEC		pbvec		= NULL;	
	INT 		ret 		= SUCCESS;
	INT			bv_offset	= ciorequest->ior_bv_offset;
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}

	
	LOGALIGN("Inside biodone Head\n");
	PRINT_POINTER(fecw);
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	//print_fecws(fecw, "Head First");

	//Now copy the original head part from child bio
	pbvec = bio_iovec(bio);
	pagebuf = kmap(pbvec->bv_page);
	pagebuf += pbvec->bv_offset;
	PMEMCPY(fecw->bv_page, pagebuf, bv_offset);
	kunmap(pbvec->bv_page);

	//Now merge the application data from parent bio
	pbvec = bio_iovec(parent_bio);
	pagebuf = kmap(pbvec->bv_page);
	pagebuf += bv_offset;
	PMEMCPY(fecw->bv_page + bv_offset, pagebuf, PAGE_SIZE - bv_offset );
	kunmap(pbvec->bv_page);

	SET_FEC_BUFFER_STATE(fecw, FECBUF_STATE_MEMORY_WRITE_COMPLETED);

	dz_align_write_single_block_parent_biodone(bio, error);

	FEC_ADD_WRITE_BUFFER_TO_LBA_TABLE(fecw, ciorequest->ior_lba);

	//print_fecws(fecw, "Head Last");
}

static RVOID dz_align_write_single_block_double_page_async_biodone_tail(PBIO bio, INT error)
{
	PIOREQUEST	ciorequest 	= (PIOREQUEST)(bio->bi_private);
	PIOREQUEST	piorequest 	= ciorequest->ior_parent;
	PBIO		parent_bio	= piorequest->ior_bio;
	PFEC_WRITE	fecw 	= (PFEC_WRITE)(ciorequest->ior_private);
	PVOID   	pagebuf	= NULL;
	PBIOVEC		pbvec	= NULL;	
	INT 		ret 	= SUCCESS;
	INT			bv_len	= ciorequest->ior_bv_len;
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}

	LOGALIGN("Inside biodone Tail\n");
	PRINT_POINTER(fecw);
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	//print_fecws(fecw, "Tail First");

	pbvec = bio_iovec(parent_bio);
	//Copy unique data from parent bio
	//Note: We are assuming that the tail data
	//and the complete bio data 
	//will always be in one page only
	pagebuf = kmap(pbvec->bv_page);
	pagebuf += pbvec->bv_offset;
	pagebuf += pbvec->bv_len - bv_len;
	PMEMCPY(fecw->bv_page, pagebuf, bv_len);
	kunmap(pbvec->bv_page);
	
	
	//Now copy the remaining part from child bio tail
	pbvec = bio_iovec(bio);
	pagebuf = kmap(pbvec->bv_page);
	pagebuf += bv_len;
	PMEMCPY((fecw->bv_page  + bv_len), pagebuf, PAGE_SIZE - bv_len);
	kunmap(pbvec->bv_page);

	SET_FEC_BUFFER_STATE(fecw, FECBUF_STATE_MEMORY_WRITE_COMPLETED);

	dz_align_write_single_block_parent_biodone(bio, error);

	FEC_ADD_WRITE_BUFFER_TO_LBA_TABLE(fecw, ciorequest->ior_lba);

}

// We could have used loop to run twice, but that will have additional overhead
// for failure scenarios
// This function will readback two pages; one for current lba and another
// for next lba
static RVOID dz_align_write_single_block_double_page_async(PIOREQUEST parent_iorequest,
		SECTOR sector_pos_in_lba,
		BIODONE done_head,
		BIODONE done_tail)
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
	INT			partial_bytes_head		= 0;
	INT			partial_bytes_tail		= 0;

	//Do the resource allocation for head part
	child_iorequest_head = dz_io_alloc(); 
	if (!child_iorequest_head) {
		LOGALIGNE("Unable to get free iorequest\n");
		goto exit_failure;
	}
	child_bio_head = dz_bio_alloc(1);
	if (!child_bio_head) {
		LOGALIGNE("Unable to get free child bio\n");
		goto exit_failure;
	}
	page_head =  dz_read_page_alloc();
	if (!page_head)  {
		LOGALIGNE("Unable to get free read page\n");
		goto exit_failure;
	}
	dz_init_child_bio(bio, child_bio_head, sector, done_head, child_iorequest_head);
	child_bio_head->bi_rw 		= READ;

	ret = bio_add_page(child_bio_head, page_head, PAGE_SIZE, 0);
	if (!ret) {
			IO_DONE_ERROR(parent_iorequest, ret);
			LOGALIGNE("Unable to add Page to child bio head\n");
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
	//child_iorequest_head->ior_private 		= fecw_head;
	atomic_inc(&parent_iorequest->ior_child_cnt);
	
	//Now do the resource allocation for tail part
	lba++;
	sector								= dz_convert_lba_to_sector(lba);
	child_iorequest_tail = dz_io_alloc(); 
	if (!child_iorequest_tail) {
		LOGALIGNE("Unable to get free iorequest tail\n");
		goto exit_failure;
	}
	child_bio_tail = dz_bio_alloc(1);
	if (!child_bio_tail) {
		LOGALIGNE("Unable to get free child bio tail\n");
		goto exit_failure;
	}
	page_tail =  dz_read_page_alloc();
	if (!page_tail)  {
		LOGALIGNE("Unable to get free read page tail\n");
		goto exit_failure;
	}
	dz_init_child_bio(bio, child_bio_tail, sector, done_tail, child_iorequest_tail);
	child_bio_tail->bi_rw 		= READ;

	ret = bio_add_page(child_bio_tail, page_tail, PAGE_SIZE, 0);
	if (!ret) {
			IO_DONE_ERROR(parent_iorequest, ret);
			LOGALIGNE("Unable to add Page to child bio tail\n");
			goto exit_alloc;
	}
	atomic_set(&child_iorequest_tail->ior_child_cnt, 0);
	child_iorequest_tail->ior_thread_pool	= align_read_thread_pool;
	child_iorequest_tail->ior_bio 			= child_bio_tail;
//	child_iorequest_tail->target 		= parent_iorequest->target;
	child_iorequest_tail->ior_lba 			= lba;
	child_iorequest_head->ior_sector 		= sector;
	child_iorequest_tail->ior_parent 		= parent_iorequest;
	//child_iorequest_tail->ior_private 		= fecw_tail;
	atomic_inc(&parent_iorequest->ior_child_cnt);


	partial_bytes_head = LBA_BLOCK_SIZE - sector_pos_in_lba;
	partial_bytes_tail = bio->bi_size - partial_bytes_head;

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
	IO_DONE_ERROR(parent_iorequest, IO_ERROR_ENOMEM);

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


static RVOID dz_align_write_single_block_iorequest_done(PVOID iorequest , INT error)
{
	PIOREQUEST		ior 	= (PIOREQUEST)iorequest;

	//FEC_ENTRY_WRITE_LOCK(fecw);
	if (error) {
		if (ior->ior_parent) { //For internally generated bios
			DZ_ALIGN_IOREQUEST_DONE_ERROR(ior, error);
		} else {
			WRITE_IO_DONE_STATUS(ior, error);
		}

	} else {
		if (ior->ior_parent) { //For internally generated bios
			DZ_ALIGN_IOREQUEST_DONE(ior);
		} else {
			//Completing direct application IO
			WRITE_IO_DONE(ior);
		} 
	}
	//FEC_ENTRY_WRITE_UNLOCK(fecw);
	//complete(&fecw->io_completion.completion);

	LOGD("Disk IO Done for LBA %lli with iorequest %p\n", ior->lba, ior);
	return;
}

//This function allows writing a single PAGE_SIZE block
//It handles two cases:
//1. Write is PAGE_SIZE aligned
//2. Write is PAGE_SIZE but unaligned and hence ends up
//in issuing two partial writes spreading across two LBAs
//i.e. starting with the current LBA and ending into 
//(spilled over to) next LBA

RVOID dz_align_write_for_single_block(PIOREQUEST iorequest)
{
	PBIO		bio 			= iorequest->ior_bio; // Parent or Original bio
	SECTOR		sector 			= DZ_BIO_GET_SECTOR(bio);
	LBA			lba				= iorequest->ior_lba;
	UINT	sector_pos_in_lba	= 0;
	U64			sector_bytes	= sector * SECTOR_SIZE;

	//If unaligned then handled the same way as it was handled for
	//spillover  in partial block
	if (((sector_bytes) % LBA_BLOCK_SIZE) != 0 ) { 
		LOGALIGN("Case1: Single block unaligned Sector = %lli\n", (U64)sector);
		sector_pos_in_lba = (sector_bytes)  - (lba * LBA_BLOCK_SIZE);

		dz_align_write_single_block_double_page_async(iorequest,
		sector_pos_in_lba,
		dz_align_write_single_block_double_page_async_biodone_head,
		dz_align_write_single_block_double_page_async_biodone_tail);
		return;
	}

	//Page Aligned IO 
	LOGALIGN("Case2: Single block Aligned Sector = %lli\n", (U64)sector);

	COPY_FROM_BIO(iorequest->ior_bv_page, bio);
	iorequest->ior_donefun = dz_align_write_single_block_iorequest_done;

	//Increment the ref_cnt of iorequest
	iorequest_get(iorequest);

	LOGALIGN("Dedupe Write IO Start for LBA %lli with iorequest %p\n", lba, iorequest);

	// queue this iorequest for dedupe_thread_pool. It will eventually call dz_dedupe_io()
	dz_q_iorequest(dedupe_thread_pool, iorequest); //Invokes dz_dedupe_io()

	return;
}

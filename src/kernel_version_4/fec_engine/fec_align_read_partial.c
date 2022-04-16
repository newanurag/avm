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

extern U64 				fec_table_size;
extern SPINLOCK			fec_active_lock;

//For operational purpose
extern PFEC_TABLE 		fec_tables_active;
extern PFEC_TABLE 		fec_tables_flush;
extern PLBA_BLOCK		lba_table;
//static U64			IO_COUNT = 0;
extern SPINLOCK			print_iorequest_lock;

extern ATOMIC64 fec_iocount_reads_partial_page;

//Head Aligned i.e. Page is less than 4K but starts with 4K multiple
extern ATOMIC64 fec_iocount_reads_partial_haligned_page; 

//Head Aligned i.e. Page is less than 4K but ends with 4K multiple
extern ATOMIC64 fec_iocount_reads_partial_taligned_page;

//Neither Head Aligned Nor Tail aligned and spills over to next lba 
//i.e. Page is less than 4K but spilled over to next Page boundary
extern ATOMIC64 fec_iocount_reads_partial_spilled_page;

//Neither Head Aligned Nor Tail aligned and  not even spilled over to next lba 
//i.e. Page is less than 4K but falls in the same lba region
//This is an absolute partial page
extern ATOMIC64 fec_iocount_reads_partial_absolute_page;

extern ATOMIC64 fec_iocount_reads_single_page;
extern ATOMIC64 fec_iocount_reads_single_aligned_page;
extern ATOMIC64 fec_iocount_reads_single_unaligned_page;
extern ATOMIC64 fec_iocount_reads_multi_page;
extern ATOMIC64 fec_iocount_reads_multi_aligned_page;
extern ATOMIC64 fec_iocount_reads_multi_unaligned_page;

//TODO:RVOID dz_far_for_partial_block_biodone_tail(PBIO bio, INT error)
RVOID dz_far_for_partial_block_biodone_tail(PBIO bio)
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

	/* TODO:
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	if (error) {
		IO_DONE_STATUS(piorequest, error);
		goto exit;
	}
	*/
	if (bio->bi_status != BLK_STS_OK) {
		IOREQUEST_DONE_ERROR(piorequest);
		goto exit;
	}

	sector_pos_in_lba 	= (sector * SECTOR_SIZE)  - (lba * LBA_BLOCK_SIZE);
	partial_bytes_head 	= LBA_BLOCK_SIZE - sector_pos_in_lba;
	partial_bytes_tail 	= size - partial_bytes_head;
	partial_bytes		= partial_bytes_tail;

	//From Child bio
	cbvec	 = bio_iovec(bio);
	cpagebuf = kmap(cbvec.bv_page);
	cpagebuf += sector_pos_in_lba;

	//From Parent bio
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

		bio_data_dir(piorequest->ior_bio) == WRITE ? 
		iorequest_put(piorequest) : 
		iorequest_put(piorequest);
		return;
	}

exit:
	dz_read_page_free(cbvec.bv_page);
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	//iorequest_put(piorequest);
	
	return;
}

//TODO:RVOID dz_far_for_partial_block_biodone_head(PBIO bio, INT error)
RVOID dz_far_for_partial_block_biodone_head(PBIO bio)
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

		bio_data_dir(piorequest->ior_bio) == WRITE ? 
		iorequest_put(piorequest) : 
		iorequest_put(piorequest);
		return;
	}

exit:
	dz_read_page_free(cbvec.bv_page);
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	//iorequest_put(piorequest);
	
	return;
}

//RVOID dz_far_for_partial_block_biodone(PBIO bio, INT error)
RVOID dz_far_for_partial_block_biodone(PBIO bio)
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
	SIZE	size				= DZ_BIO_GET_SIZE(pbio);
	INT			bv_len			= 0;
	struct bvec_iter iter;
	//LOG("child_biodone\n");	
	/*
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	if (error) {
		IO_DONE_STATUS(piorequest, error);
		goto exit;
	}
	*/
	if (bio->bi_status != BLK_STS_OK) {
		IOREQUEST_DONE_ERROR(piorequest);
		goto exit;

	}
	//Inside Child bio / Readback bio. Set the location where data has to be copied from
	sector_pos_in_lba = (sector * SECTOR_SIZE)  - (lba * LBA_BLOCK_SIZE);
	cbvec	 = bio_iovec(bio);
	cpagebuf = kmap(cbvec.bv_page);
	cpagebuf += sector_pos_in_lba;

	//Inside Parent bio
	bio_for_each_segment(pbvec, pbio, iter) {
	
		//PRINT_INT(pbvec->bv_len);
		//PRINT_INT(pbvec->bv_offset);
		//PRINT_SIZE(size);
		ppagebuf = kmap(pbvec.bv_page);
		ppagebuf += pbvec.bv_offset;
		bv_len	 = pbvec.bv_len;	
		if (size > bv_len) {
			PMEMCPY(ppagebuf, cpagebuf, bv_len);
			cpagebuf += bv_len;
			size	 -= bv_len;
			kunmap(pbvec.bv_page);
			continue;
		} else {
			PMEMCPY(ppagebuf, cpagebuf, size);
			kunmap(pbvec.bv_page);
			break;
		}
	}
	kunmap(cbvec.bv_page);
	READ_IO_DONE(piorequest);

exit:
	dz_read_page_free(cbvec.bv_page);
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	iorequest_put(piorequest);
	
	return;
}

RVOID dz_fec_align_read_single_page_async(PIOREQUEST parent_iorequest, BIODONE done_func)
{
	INT					ret 	= SUCCESS;
	PIOREQUEST	child_iorequest = NULL;
	PBIO	child_bio 			= NULL;
	PBIO	bio 				= parent_iorequest->ior_bio; // Parent or Original bio
	PPAGE				page	= NULL;
	SECTOR				sector	= dz_convert_lba_to_sector(parent_iorequest->ior_lba);

	child_iorequest = dz_io_alloc(); 
	if (!child_iorequest) {
		LOGFECE("Unable to get free iorequest\n");
		goto exit_failure;
	}
	MEMSET(child_iorequest);
	child_bio = dz_bio_alloc(1);
	if (!child_bio) {
		LOGFECE("Unable to get free child bio\n");
		goto exit_failure;
	}
	page =  dz_read_page_alloc();
	if (!page)  {
		LOGFECE("Unable to get free read page\n");
		goto exit_failure;
	}
	dz_init_child_bio(bio, child_bio, sector, done_func, child_iorequest);

	ret = bio_add_page(child_bio, page, PAGE_SIZE, 0);
	if (!ret) {
			IOREQUEST_DONE_ERROR(parent_iorequest);
			LOGFECE("Unable to add Page to child bio\n");
			dz_read_page_free(page);
			DZ_OS_KERNEL_BIO_PUT(child_bio);
			dz_io_free(child_iorequest);
			return;
	}
	atomic_set(&child_iorequest->ior_child_cnt, 0);
	child_iorequest->ior_thread_pool= read_thread_pool;
	child_iorequest->ior_bio 		= child_bio;
//	child_iorequest->target 	= parent_iorequest->target;
	child_iorequest->ior_lba	 	= parent_iorequest->ior_lba;
	child_iorequest->ior_parent 	= parent_iorequest;
	child_iorequest->ior_type   = DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ;
	atomic_inc(&parent_iorequest->ior_child_cnt);
	//print_biom(child_bio, "Partial child bio");
	//dz_q_iorequest_thread_pool(child_iorequest);
	//Instead of queing to thread pool, calling directly the read function
	dz_fec_align_read_io_direct(child_iorequest);
	return;

exit_failure:
	//IO_DONE_ERROR(parent_iorequest, IO_ERROR_ENOMEM);
	IOREQUEST_DONE_ERROR(parent_iorequest);
	if (child_iorequest) {
		dz_io_free(child_iorequest);
	}
	if (page) {
		dz_read_page_free(page);
	}
	if (child_bio) {
		DZ_OS_KERNEL_BIO_PUT(child_bio); 
	}
}

//This function handles these cases:
//Case1 : If the IO falls into the same block
//Case2 : If the IO spill over to next block.
//Note : Here the io size will always be less than LBA_BLOCK_SIZE
RVOID dz_fec_align_read_for_partial_block(PIOREQUEST piorequest)
{
	PBIO	bio 				= piorequest->ior_bio; // Parent or Original bio
	SECTOR	sector 				= DZ_BIO_GET_SECTOR(bio);
	INT		size				= DZ_BIO_GET_SIZE(bio);
	UINT	sector_pos_in_lba	= 0;
	BOOL	align_flag = true;

	if (((sector * SECTOR_SIZE) % LBA_BLOCK_SIZE) == 0 ) { // Start Sector is Page Aligned. One bio will be created

partial_io:
		if (align_flag == true) {
			LOGD("Case1: Partial Block IO: Start Block Aligned. Only one Child will be created\n");
		}
		//print_biom(bio, "Partial parent bio");
		//TODO:We should try to reuse the Parent page instead of creating our own
		dz_fec_align_read_single_page_async(piorequest, dz_far_for_partial_block_biodone);

	} else {
	// Here sector + bv_len can be inside the same lba or spill over to next lba
		sector_pos_in_lba = (sector * SECTOR_SIZE)  - (piorequest->ior_lba * LBA_BLOCK_SIZE);

		if ((sector_pos_in_lba + size) <= LBA_BLOCK_SIZE) { 
			// Falls into Singe Page boundary
			// One one bio is required
			LOGD("Case2: Partial Block IO: NO Start Block Aligned. Falls into Single Block but no Spillover.\n");
			align_flag = false;
			goto partial_io;

		} else { 
			LOGD("Case3: Partial Block IO: Spill over to next block. Two children will be created\n");
			// This condition is same as we got in Singe Block IO Handling

			dz_fec_align_read_double_page_async(piorequest,
				dz_far_for_partial_block_biodone_head,
				dz_far_for_partial_block_biodone_tail);

		}
	}
}

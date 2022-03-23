/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PDZ_THREAD_POOL 	align_read_thread_pool;

extern SPINLOCK			print_iorequest_lock;

extern ATOMIC64 align_iocount_reads_partial_page;

//Head Aligned i.e. Page is less than 4K but starts with 4K multiple
extern ATOMIC64 align_iocount_reads_partial_haligned_page; 

//Head Aligned i.e. Page is less than 4K but ends with 4K multiple
extern ATOMIC64 align_iocount_reads_partial_taligned_page;

//Neither Head Aligned Nor Tail aligned and spills over to next lba 
//i.e. Page is less than 4K but spilled over to next Page boundary
extern ATOMIC64 align_iocount_reads_partial_spilled_page;

//Neither Head Aligned Nor Tail aligned and  not even spilled over to next lba 
//i.e. Page is less than 4K but falls in the same lba region
//This is an absolute partial page
extern ATOMIC64 align_iocount_reads_partial_absolute_page;

extern ATOMIC64 align_iocount_reads_single_page;
extern ATOMIC64 align_iocount_reads_single_aligned_page;
extern ATOMIC64 align_iocount_reads_single_unaligned_page;
extern ATOMIC64 align_iocount_reads_multi_page;
extern ATOMIC64 align_iocount_reads_multi_aligned_page;
extern ATOMIC64 align_iocount_reads_multi_unaligned_page;
extern ATOMIC64 align_iocount_reads_multi_tail_unaligned_page;
extern ATOMIC64 align_iocount_reads_multi_head_unaligned_page;
extern ATOMIC64 align_iocount_reads_multi_both_unaligned_page;

static RVOID dz_align_read_parent_biodone(PBIO bio, INT error)
{
	PIOREQUEST ciorequest = (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest = ciorequest->ior_parent;
	piorequest = ciorequest->ior_parent;
	//PRINT_ATOMIC(piorequest->child_cnt);
	if (atomic_dec_and_test(&piorequest->ior_child_cnt)) {
		// Now complete the Parent / Original bio
		//print_biom(piorequest->ior_bio, "Parent bio when all childs are done");
		//LOGALIGN("Parent IO Done\n");
		IO_DONE_STATUS(piorequest, error);
		DZ_OS_KERNEL_BIO_PUT(bio);
		iorequest_put(ciorequest);

		bio_data_dir(piorequest->ior_bio) == WRITE ? 
		iorequest_put(piorequest) : 
		iorequest_put(piorequest);
		return;
	}
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	
	return;
}

// Contents of this function is same as dz_far_for_partial_block_biodone_head()
// For simplification purpose we have copied it here. Will dedupe it later
static RVOID dz_align_read_multi_block_head_unaligned_biodone(PBIO bio, INT error)
{
	//dz_far_for_partial_block_biodone_head(bio, error);

	PIOREQUEST ciorequest 		= (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest 		= ciorequest->ior_parent;
	PVOID   	cpagebuf		= NULL;
	PVOID   	ppagebuf		= NULL;
	PBIOVEC		cbvec			= NULL;	
	PBIOVEC		pbvec			= NULL;	
	PBIO		pbio			= piorequest->ior_bio; //Parent BIO
	INT 		ret 			= SUCCESS;
	SECTOR		sector			= pbio->bi_sector;
	UINT	sector_pos_in_lba	= 0;
	LBA		lba					= piorequest->ior_lba;
	INT			bv_len			= 0;
	INT j;
	INT partial_bytes_head;
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	if (error) {
		//TODO:
		IO_DONE_STATUS(piorequest, error);
		goto exit;
	}
	sector_pos_in_lba = (sector * SECTOR_SIZE)  - (lba * LBA_BLOCK_SIZE);
	partial_bytes_head = LBA_BLOCK_SIZE - sector_pos_in_lba;

	//From child bio. Set the pointer where the actual data has to be read
	cbvec	 = bio_iovec(bio);
	cpagebuf = kmap(cbvec->bv_page);
	cpagebuf += sector_pos_in_lba;

	//From Parent bio, Go to the location in parent's page where we have to copy
	//the data from the child bio
	bio_for_each_segment(pbvec, pbio, j) {
		ppagebuf = kmap(pbvec->bv_page);
		ppagebuf += pbvec->bv_offset;
		bv_len	 = pbvec->bv_len;	
		if (partial_bytes_head > bv_len) {
			//Very rare but possible. i.e. Partial tobe read data
			//is spread across multiple pages
			//PRINT_POINTER(pbvec->bv_page);
			//PRINT_INT(pbvec->bv_len);
			//PRINT_INT(pbvec->bv_offset);
			//PRINT_INT(partial_bytes_head);
			//PRINT_UINT(sector_pos_in_lba);
			//print_iorequest(ciorequest, "Unsupported");

			PMEMCPY(ppagebuf, cpagebuf, bv_len);
			cpagebuf += bv_len;
			partial_bytes_head -= bv_len;
			kunmap(pbvec->bv_page);
			continue;
		} else {
			//Actually copy the partial data contents from child bio's page
			//to Parent's bio page
			PMEMCPY(ppagebuf, cpagebuf, partial_bytes_head);
			kunmap(pbvec->bv_page);
			break;
		}
	}
	kunmap(cbvec->bv_page);

	if (atomic_dec_and_test(&piorequest->ior_child_cnt)) {
		// Now complete the Parent / Original bio
		IO_DONE_STATUS(piorequest, error);
		dz_read_page_free(cbvec->bv_page);
		DZ_OS_KERNEL_BIO_PUT(bio);
		iorequest_put(ciorequest);
		iorequest_put(piorequest);
		return;
	}

exit:
	dz_read_page_free(cbvec->bv_page);
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	//iorequest_put(piorequest);
	
	return;
}

// This function will copy the partial/tail data contents from
// child bio to Parent's bio page

static RVOID dz_align_read_multi_block_tail_unaligned_biodone(PBIO bio, INT error)
{
	//dz_far_for_partial_block_biodone_tail(bio, error);
	PIOREQUEST ciorequest 		= (PIOREQUEST) bio->bi_private;
	PIOREQUEST piorequest 		= ciorequest->ior_parent;
	PVOID   	cpagebuf		= NULL;
	PVOID   	ppagebuf		= NULL;
	PBIOVEC		cbvec			= NULL;	
	PBIOVEC		pbvec			= NULL;	
	PBIO		pbio			= piorequest->ior_bio; //Parent BIO
	INT 		ret 			= SUCCESS;
	SECTOR		sector			= pbio->bi_sector;
	UINT	sector_pos_in_lba	= 0;
	LBA		lba					= piorequest->ior_lba;
	SIZE	size				= pbio->bi_size;
	PBIOVEC		last_bvec  		= NULL;
	INT			bi_vcnt			= 0;
	INT			bi_vcnt_max		= 0;
	INT			bv_len			= 0;
	INT			bv_offset		= 0;
	INT partial_bytes_tail;
	INT partial_bytes_head;
	INT partial_bytes;
	//LOGALIGN("child_biodone\n");	
	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	if (error) {
		IO_DONE_STATUS(piorequest, error);
		goto exit;
	}
	sector_pos_in_lba 	= (sector * SECTOR_SIZE)  - (lba * LBA_BLOCK_SIZE);
	partial_bytes_head 	= LBA_BLOCK_SIZE - sector_pos_in_lba;
	partial_bytes_tail 	= size - partial_bytes_head;
	partial_bytes		= partial_bytes_tail;

	//From Child bio
	cbvec	 = bio_iovec(bio);
	cpagebuf = kmap(cbvec->bv_page);
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

	kunmap(cbvec->bv_page);
	if (atomic_dec_and_test(&piorequest->ior_child_cnt)) {
		// Now complete the Parent / Original bio
		IO_DONE_STATUS(piorequest, error);
		dz_read_page_free(cbvec->bv_page);
		DZ_OS_KERNEL_BIO_PUT(bio);
		iorequest_put(ciorequest);

		iorequest_put(piorequest);
		return;
	}

exit:
	dz_read_page_free(cbvec->bv_page);
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(ciorequest);
	//iorequest_put(piorequest);
	
	return;
}

static RVOID dz_align_read_multi_block_both_aligned_biodone(PBIO bio, INT error)
{
	INT 		ret 			= SUCCESS;

	if (unlikely(!bio_flagged(bio, BIO_UPTODATE) && !error)) {
		error = -EIO;
	}
	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	if (error) {
		//TODO:IO_DONE_STATUS(piorequest, error);
		//How to return error when one child is passed and another failed in
		//parent child relationship
	}
	dz_align_read_parent_biodone(bio, error);
}

//  ________________________
// | HEAD | PAGE    | TAIL |
// |  <   | ALIGNED |  <   |
// |_PAGE_|_________|_PAGE_|
//
// HEAD and TAIL are  always less than PAGE
//
static RVOID dz_align_read_multi_block_both_unaligned (PIOREQUEST piorequest)
{
	PBIO		bio 				= piorequest->ior_bio; // Parent or Original bio
	SIZE  		io_size				= bio->bi_size; // Parent or Original bio size
	SECTOR		sector 				= DZ_BIO_GET_SECTOR(bio);
	LBA			lba					= piorequest->ior_lba;
	PBIOVEC		cur_bvec  			= NULL;
	INT			bi_vcnt				= 0;
	INT			bi_vcnt_max			= 0;
	INT			bv_len				= 0;
	INT			remaining_bv_len	= 0;
	UINT		sector_pos_in_lba	= 0;
	SIZE		sector_bytes		= 0;
	U64			tot_bytes			= 0;
	INT			data_bytes			= 0;
	INT 		aligned_bytes 		= 0;
	INT			partial_bytes_head 	= 0;
	INT			partial_bytes_tail 	= 0;
	INT			total_blocks_needed	= 0;
	LIST_HEAD	free_list_head		;
	PIOREQUEST	child_iorequest 	= NULL;
	PIOREQUEST	child_iorequest_head= NULL;
	PIOREQUEST	child_iorequest_tail= NULL;
	PBIO		child_bio 			= NULL;
	PPAGE		page_head			= NULL;
	PPAGE		page_tail			= NULL;
	INT 		ret 				= SUCCESS;
	INT			count;
	INT			nr_iovecs			= 0;


	//TODO: Optimization required. Try to use bit shifting to simplify this
	sector_bytes 		= sector * SECTOR_SIZE;
	sector_pos_in_lba 	= sector_bytes   - (lba * LBA_BLOCK_SIZE);
	tot_bytes 			= sector_bytes + io_size;
	partial_bytes_head 	= LBA_BLOCK_SIZE - sector_pos_in_lba;
	partial_bytes_tail 	= tot_bytes % LBA_BLOCK_SIZE;
	aligned_bytes 		= io_size - partial_bytes_head - partial_bytes_tail ; //Always in multiples of LBA_BLOCK_SIZE

	//Calculate total blocks: one for head, one for tail and ...
	total_blocks_needed = 1 + 1 + (aligned_bytes / LBA_BLOCK_SIZE);

	//It was observed that there were more bio pages attached
	//in the parent and since child bios are using those pages
	//so nr_bvecs should be more than 1
	nr_iovecs = (bio->bi_max_vecs / total_blocks_needed) + 1;

	INIT_LIST_HEAD(&free_list_head);
	atomic_set(&piorequest->ior_child_cnt, 0);
	for (count = 0; count < total_blocks_needed; count++) {

		child_iorequest = dz_io_alloc(); 
		if (!child_iorequest) {
			LOGALIGNE("Unable to get free iorequest\n");
			total_blocks_needed = count + 1; //Used for freeing blocks
			goto exit_failure;
		}
		child_bio = dz_bio_alloc(nr_iovecs);
		if (!child_bio) {
			LOGALIGNE("Unable to get free child bio\n");
			total_blocks_needed = count + 1;
			goto exit_failure;
		}

		dz_init_child_bio(bio, child_bio, sector, 
			dz_align_read_multi_block_both_aligned_biodone, child_iorequest);
		atomic_set(&child_iorequest->ior_child_cnt, 0);
		child_iorequest->ior_thread_pool= align_read_thread_pool;
		child_iorequest->ior_bio 		= child_bio;
//		child_iorequest->target 	= piorequest->target;
		child_iorequest->ior_parent 	= piorequest;
		child_iorequest->ior_lba	 	= lba;
		child_iorequest->ior_type   = DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ;

		list_add_tail(&(child_iorequest->ior_ioq), &free_list_head);
		sector += SECTORS_PER_PAGE;
		atomic_inc(&piorequest->ior_child_cnt);
		lba++;

		if (count == 0) {
			child_iorequest_head = child_iorequest;
		}
	}

	//Need to allocate a seperate page for head, because it will be unaligned
	page_head = dz_read_page_alloc();
	if (!page_head) {
		goto exit_failure;
	}

	ret = bio_add_page(child_iorequest_head->ior_bio, page_head, PAGE_SIZE, 0);
	if (!ret) {
		IO_DONE_ERROR(piorequest, ret);
		LOGALIGNE("Unable to add Page to child bio head\n");
		goto exit_page_add;
	}

	//Set the done function as well. It will be different than the one added above
	child_iorequest_head->ior_bio->bi_end_io = dz_align_read_multi_block_head_unaligned_biodone;

	//Need to allocate a seperate page for tail, because it will be unaligned
	child_iorequest_tail = child_iorequest;
	page_tail = dz_read_page_alloc();
	if (!page_tail) {
		goto exit_failure;
	}

	ret = bio_add_page(child_iorequest_tail->ior_bio, page_tail, PAGE_SIZE, 0);
	if (!ret) {
		IO_DONE_ERROR(piorequest, ret);
		LOGALIGNE("Unable to add Page to child bio tail\n");
		goto exit_page_add;
	}

	//Set the done function as well. It will be different than the one added above
	child_iorequest->ior_bio->bi_end_io = dz_align_read_multi_block_tail_unaligned_biodone;

	//By now we are done with resource allocation part.
	//Now we need to attach the parent bio pages in their
	//corresponding child bios except head and tail
 
	//print_bio(bio);

	//Extract the first entry of fecw from free list
	child_iorequest_head = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
	list_del(&child_iorequest_head->ior_ioq);
	dz_q_child_iorequest_to_parent_q(piorequest, child_iorequest_head);
	bi_vcnt_max = bio->bi_vcnt;
	data_bytes = partial_bytes_head;
	LOGALIGN("Printing Head part\n");

	// Skip the head part of unaligned data. This will be less than a block size
	// and hence will be handled inside its done function
	// Begin from zeroth index of bio vec array
	while(bi_vcnt < bi_vcnt_max) {
		cur_bvec = &bio->bi_io_vec[bi_vcnt];
		bv_len = cur_bvec->bv_len;

		if (bv_len >= data_bytes) {
			if (bv_len == data_bytes) {
				bi_vcnt++;
			} else {
				remaining_bv_len = bv_len - data_bytes;
			}
			break;
		} else {
			data_bytes -= bv_len;
			bi_vcnt++;
		}
	} // End of While loop

	sector += (partial_bytes_head  / SECTOR_SIZE);
	lba++;

	LOGALIGN("Printing Intermediate Aligned part\n");

	//Note continuing with previous bi_vcnt and cur_bvec
	
	while(aligned_bytes) {

		child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
		list_del(&child_iorequest->ior_ioq);
		dz_q_child_iorequest_to_parent_q(piorequest, child_iorequest);
		child_bio = child_iorequest->ior_bio;
		child_iorequest->ior_sector	 	= sector;
		child_iorequest->ior_lba	 	= lba;
		PRINT_POINTER(child_iorequest);

		data_bytes 		= PAGE_SIZE;

		if (remaining_bv_len) {
				data_bytes 		-= remaining_bv_len;
				ret = bio_add_page(child_bio, cur_bvec->bv_page, remaining_bv_len,
				cur_bvec->bv_offset + (cur_bvec->bv_len - remaining_bv_len));
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}
				bi_vcnt++;
				remaining_bv_len = 0;

		}

		while(bi_vcnt < bi_vcnt_max) {
			cur_bvec 	= &bio->bi_io_vec[bi_vcnt];
			bv_len 		= cur_bvec->bv_len;

			if (bv_len >= data_bytes) {
				ret = bio_add_page(child_bio, cur_bvec->bv_page, data_bytes, cur_bvec->bv_offset);
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}

				if (bv_len == data_bytes) {
					bi_vcnt++;
				} else {
					remaining_bv_len = bv_len - data_bytes;
				}
				aligned_bytes -= PAGE_SIZE;
				break;
			} else {

				ret = bio_add_page(child_bio, cur_bvec->bv_page, bv_len, cur_bvec->bv_offset);
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}
				data_bytes -= bv_len;
				bi_vcnt++;
			}
		} // End of Inner While loop
		sector += SECTORS_PER_PAGE;
		lba++;
	} //End of Outer While loop

	//Now copy the unaligned tail part. Aligned tail part is already
	//included in the above loop

	LOGALIGN("Printing Tail part\n");

	if (list_empty(&free_list_head)) {
		BUG_ON(1);
	}
	//Add Tail child iorequest to parent's queue
	child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
	list_del(&child_iorequest->ior_ioq);
	dz_q_child_iorequest_to_parent_q(piorequest, child_iorequest);
	
	dz_merge_parent_local_q_to_threadpool_q(piorequest->ior_thread_pool, piorequest);
	return;

exit_failure:
	IO_DONE_ERROR(piorequest, IO_ERROR_ENOMEM);
	iorequest_put(piorequest);
	if (page_head) {
		dz_read_page_free(page_head);
	}
	if (page_tail) {
		dz_read_page_free(page_tail);
	}
	if (child_iorequest) {
		dz_io_free(child_iorequest);
	}
	if (child_bio) {
		DZ_OS_KERNEL_BIO_PUT(child_bio); 
	}
exit_page_add:
	for (bv_len = 0; bv_len < total_blocks_needed; bv_len++) {

		child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
		if (child_iorequest) {
			DZ_OS_KERNEL_BIO_PUT(child_iorequest->ior_bio); 
			dz_io_free(child_iorequest);
			list_del(&child_iorequest->ior_ioq);
		}
	}
}

// ___________________________
// | HEAD | PAGE    | PAGE    |
// |  <   | ALIGNED | ALIGNED |
// |_PAGE_|_________|_________|
// 
// HEAD is always less than PAGE size

static RVOID dz_align_read_multi_block_head_unaligned (PIOREQUEST piorequest)
{
	PBIO		bio 				= piorequest->ior_bio; // Parent or Original bio
	SIZE  		io_size				= bio->bi_size; // Parent or Original bio size
	SECTOR		sector 				= DZ_BIO_GET_SECTOR(bio);
	LBA			lba					= piorequest->ior_lba;
	PBIOVEC		cur_bvec  			= NULL;
	INT			bi_vcnt				= 0;
	INT			bi_vcnt_max			= 0;
	INT			bv_len				= 0;
	INT			remaining_bv_len	= 0;
	UINT		sector_pos_in_lba	= 0;
	SIZE		sector_bytes		= 0;
	U64			tot_bytes			= 0;
	INT			data_bytes			= 0;
	INT 		aligned_bytes 		= 0;
	INT			partial_bytes_head 	= 0;
	INT			total_blocks_needed	= 0;
	PIOREQUEST	child_iorequest 	= NULL;
	PIOREQUEST	child_iorequest_head= NULL;
	PBIO		child_bio 			= NULL;
	PPAGE		page				= NULL;
	INT 		ret 				= SUCCESS;
	LIST_HEAD	free_list_head		;
	INT			count;
	INT			nr_iovecs			= 0;


	//TODO: Optimization required. Try to use bit shifting to simplify this
	sector_bytes 		= sector * SECTOR_SIZE;
	sector_pos_in_lba 	= sector_bytes   - (lba * LBA_BLOCK_SIZE);
	tot_bytes 			= sector_bytes + io_size;
	partial_bytes_head 	= LBA_BLOCK_SIZE - sector_pos_in_lba;
	aligned_bytes 		= io_size - partial_bytes_head ; //Always in multiples of LBA_BLOCK_SIZE

	//Calculate total blocks: one for head, and ...
	total_blocks_needed = 1 + (aligned_bytes / LBA_BLOCK_SIZE);

	//It was observed that there were more bio pages attached
	//in the parent and since child bios are using those pages
	//so nr_bvecs should be more than 1
	nr_iovecs = (bio->bi_max_vecs / total_blocks_needed) + 1;

	INIT_LIST_HEAD(&free_list_head);

	atomic_set(&piorequest->ior_child_cnt, 0);
	for (count = 0; count < total_blocks_needed; count++) {

		child_iorequest = dz_io_alloc(); 
		if (!child_iorequest) {
			LOGALIGNE("Unable to get free iorequest\n");
			total_blocks_needed = count + 1; //Used for freeing blocks
			goto exit_failure;
		}
		child_bio = dz_bio_alloc(nr_iovecs);
		if (!child_bio) {
			LOGALIGNE("Unable to get free child bio\n");
			total_blocks_needed = count + 1;
			goto exit_failure;
		}

		dz_init_child_bio(bio, child_bio, sector, 
			dz_align_read_multi_block_both_aligned_biodone, child_iorequest);

		atomic_set(&child_iorequest->ior_child_cnt, 0);

		child_iorequest->ior_thread_pool= align_read_thread_pool;
		child_iorequest->ior_bio 	= child_bio;
//		child_iorequest->target 	= piorequest->target;
		child_iorequest->ior_parent 	= piorequest;
		child_iorequest->ior_lba	 	= lba;
		child_iorequest->ior_type   = DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ;

		list_add_tail(&(child_iorequest->ior_ioq), &free_list_head);
		atomic_inc(&piorequest->ior_child_cnt);

		if (count == 0) {
			child_iorequest_head = child_iorequest;
		}
	}

	//Need to allocate a seperate page for head, because it will be unaligned
	page = dz_read_page_alloc();
	if (!page) {
		goto exit_failure;
	}

	ret = bio_add_page(child_iorequest_head->ior_bio, page, PAGE_SIZE, 0);
	if (!ret) {
		IO_DONE_ERROR(piorequest, ret);
		LOGALIGNE("Unable to add Page to child bio\n");
		goto exit_page_add;
	}

	//Set the done function as well. It will be different than the one added above
	child_iorequest_head->ior_bio->bi_end_io = dz_align_read_multi_block_head_unaligned_biodone;

	//By now we are done with resource allocation part.
	//Now we need to attach the parent bio pages in their
	//corresponding child bios except head
 
	//print_bio(bio);

	//Extract the first entry i.e. child_iorequest_head from free list
	child_iorequest_head = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
	list_del(&child_iorequest_head->ior_ioq);
	dz_q_child_iorequest_to_parent_q(piorequest, child_iorequest_head);
	child_iorequest_head->ior_lba = lba;
	child_iorequest_head->ior_sector = sector;

	bi_vcnt_max = bio->bi_vcnt;
	data_bytes = partial_bytes_head;
	LOGALIGN("Printing Head part\n");

	// Skip the head part of unaligned data. This will be less than a block size
	// and hence will be handled inside the done function of head unaligned bio
	// Begin from zeroth index of bio vec array
	while(bi_vcnt < bi_vcnt_max) {
		cur_bvec = &bio->bi_io_vec[bi_vcnt];
		bv_len = cur_bvec->bv_len;

		if (bv_len >= data_bytes) {
			if (bv_len == data_bytes) {
				bi_vcnt++;
			} else {
				remaining_bv_len = bv_len - data_bytes;
			}
			break;
		} else {
			data_bytes -= bv_len;
			bi_vcnt++;
		}
	} // End of While loop

	sector += (partial_bytes_head  / SECTOR_SIZE);
	lba++;


	LOGALIGN("Printing Tail Aligned part\n");

	//Note continuing with previous bi_vcnt and cur_bvec
	
	while(aligned_bytes) {

		child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
		list_del(&child_iorequest->ior_ioq);
		dz_q_child_iorequest_to_parent_q(piorequest, child_iorequest);
		child_bio = child_iorequest->ior_bio;
		PRINT_POINTER(child_iorequest);
		child_iorequest->ior_lba = lba;
		child_iorequest->ior_sector = sector;

		data_bytes 		= PAGE_SIZE;

		if (remaining_bv_len) {
				data_bytes 		-= remaining_bv_len;
				ret = bio_add_page(child_bio, cur_bvec->bv_page, remaining_bv_len,
				cur_bvec->bv_offset + (cur_bvec->bv_len - remaining_bv_len));
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}
				bi_vcnt++;
				remaining_bv_len = 0;

		}

		while(bi_vcnt < bi_vcnt_max) {
			cur_bvec 	= &bio->bi_io_vec[bi_vcnt];
			bv_len 		= cur_bvec->bv_len;

			if (bv_len >= data_bytes) {
				ret = bio_add_page(child_bio, cur_bvec->bv_page, data_bytes, cur_bvec->bv_offset);
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}

				if (bv_len == data_bytes) {
					bi_vcnt++;
				} else {
					remaining_bv_len = bv_len - data_bytes;
				}
				aligned_bytes -= PAGE_SIZE;
				break;
			} else {

				ret = bio_add_page(child_bio, cur_bvec->bv_page, bv_len, cur_bvec->bv_offset);
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}
				data_bytes -= bv_len;
				bi_vcnt++;
			}
		} // End of Inner While loop
		sector += SECTORS_PER_PAGE;
		lba++;
	} //End of Outer While loop

	if (!list_empty(&free_list_head)) {
		BUG_ON(1);
	}
	//Enqueue the head child bio
	//child_iorequest_head = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
	//list_del(&child_iorequest_head->ior_ioq);
	//dz_q_child_iorequest_to_parent_q(piorequest, child_iorequest_head);
	dz_merge_parent_local_q_to_threadpool_q(piorequest->ior_thread_pool, piorequest);
	return;
exit_failure:
	IO_DONE_ERROR(piorequest, IO_ERROR_ENOMEM);
	iorequest_put(piorequest);
	if (page) {
		dz_read_page_free(page);
	}
	if (child_iorequest) {
		dz_io_free(child_iorequest);
	}
	if (child_bio) {
		DZ_OS_KERNEL_BIO_PUT(child_bio); 
	}
exit_page_add:
	for (bv_len = 0; bv_len < total_blocks_needed; bv_len++) {

		child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
		if (child_iorequest) {
			DZ_OS_KERNEL_BIO_PUT(child_iorequest->ior_bio); 
			dz_io_free(child_iorequest);
			list_del(&child_iorequest->ior_ioq);
		}
	}
}
// This function deals with multi page IOs who are
// aligned with starting of Page but dont end with Page aligned
// boundary.
// This function will allocate a new page for tail bio
// Other bios will attach Parent's page
// Except tail bio all the other bios will be page aligned
// Tail's done function will release that page
// ____________________________
// | PAGE    | PAGE    | TAIL |
// | ALIGNED | ALIGNED |      |
// |_________|_________|______|
//
// TAIL is  always less than PAGE
//
static RVOID dz_align_read_multi_block_tail_unaligned(PIOREQUEST piorequest)
{
	PBIO		bio 				= piorequest->ior_bio; // Parent or Original bio
	SIZE  		io_size				= bio->bi_size; // Parent or Original bio size
	SECTOR		sector 				= DZ_BIO_GET_SECTOR(bio);
	LBA			lba					= piorequest->ior_lba;
	PBIOVEC		cur_bvec  			= NULL;
	INT			bi_vcnt				= 0;
	INT			bi_vcnt_max			= 0;
	INT			bv_len				= 0;
	INT			remaining_bv_len	= 0;
	UINT		sector_pos_in_lba	= 0;
	SIZE		sector_bytes		= 0;
	U64			tot_bytes			= 0;
	INT			data_bytes			= 0;
	INT 		aligned_bytes 		= 0;
	INT			partial_bytes_tail 	= 0;
	INT			total_blocks_needed	= 0;
	PIOREQUEST	child_iorequest 	= NULL;
	PIOREQUEST	child_iorequest_tail= NULL;
	PBIO		child_bio 			= NULL;
	PPAGE		page				= NULL;
	LIST_HEAD	free_list_head;
	INT			count;
	INT			ret;
	PIOREQUEST  prev_child_iorequest= NULL;
	INT			nr_iovecs			= 0;

	//TODO: Optimization required. Try to use bit shifting to simplify this
	sector_bytes 		= sector * SECTOR_SIZE;
	sector_pos_in_lba 	= sector_bytes   - (lba * LBA_BLOCK_SIZE);
	tot_bytes 			= sector_bytes + io_size;
	partial_bytes_tail 	= tot_bytes % LBA_BLOCK_SIZE;
	aligned_bytes 		= io_size - partial_bytes_tail ; //Always in multiples of LBA_BLOCK_SIZE

	//Calculate total blocks: one for tail and ...
	//This is needed to create exact no of child bios
	total_blocks_needed = 1 + (aligned_bytes / LBA_BLOCK_SIZE);

	//It was observed that there were more bio pages attached
	//in the parent and since child bios are using those pages
	//so nr_bvecs should be more than 1
	nr_iovecs = (bio->bi_max_vecs / total_blocks_needed) + 1;

	INIT_LIST_HEAD(&free_list_head);

	atomic_set(&piorequest->ior_child_cnt, 0);

	//Note that inside this loop we are not allocating any page
	for (count = 0; count < total_blocks_needed; count++) {

		child_iorequest = dz_io_alloc(); 
		if (!child_iorequest) {
			LOGALIGNE("Unable to get free iorequest\n");
			total_blocks_needed = count + 1; //Used for freeing blocks
			goto exit_failure;
		}
		MEMSET(child_iorequest);
		if (child_iorequest == prev_child_iorequest) {
			LOGALIGN("First Alloc child_iorequest = %p\n", child_iorequest);
			LOGALIGN("Prev Alloc child_iorequest = %p\n", prev_child_iorequest);
			LOGALIGN("Both are same\n");
			SL1;
			BUG_ON(1);
		}


		child_bio = dz_bio_alloc(nr_iovecs);
		if (!child_bio) {
			LOGALIGNE("Unable to get free child bio\n");
			total_blocks_needed = count + 1;
			goto exit_failure;
		}

		//Setting the both aligned done function for all except
		//tail
		dz_init_child_bio(bio, child_bio, sector, 
			dz_align_read_multi_block_both_aligned_biodone, child_iorequest);

		atomic_set(&child_iorequest->ior_child_cnt, 0);
		child_iorequest->ior_thread_pool = align_read_thread_pool;
		child_iorequest->ior_bio 	 = child_bio;
//		child_iorequest->target 	 = piorequest->target;
		child_iorequest->ior_parent 	 = piorequest;
		child_iorequest->ior_lba	 	 = lba;
		child_iorequest->ior_sector	 	 = sector;
		child_iorequest->ior_type    = DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ;

		//free_list_head is a linked list which is storing the child iorequest
		// in a list. This is going to be used while assigning parent pages to
		//child iorequest

		list_add_tail(&(child_iorequest->ior_ioq), &free_list_head);

		sector += SECTORS_PER_PAGE;

		//Increment the no. of childs in parent iorequest
		atomic_inc(&piorequest->ior_child_cnt);

		//Note: lba needs to be increased because child request is created
		//for each lba
		lba++;

		prev_child_iorequest = child_iorequest;
	}

	//Need to allocate a seperate page for tail, because it will be unaligned
	//So a new page will be attached to tail bio which will do a full
	//PAGE_SIZE readback. The biodone of this tail bio will
	//copy the relevant data to original bio (Parent bio)
	//It will be used to read partial bytes i.e. partial_bytes_tail
	//TODO:We are not trying to reuse the Parent's Page but can think of in future
	child_iorequest_tail = child_iorequest; //Last child iorequest will act as tail
	page = dz_read_page_alloc();
	if (!page) {
		goto exit_failure;
	}

	//Here although full page will be attached but the io size will be partial_bytes_tail
	ret = bio_add_page(child_iorequest_tail->ior_bio, page, partial_bytes_tail, 0);
	if (!ret) {
		IO_DONE_ERROR(piorequest, ret);
		LOGALIGNE("Unable to add Page to child bio\n");
		goto exit_page_add;
	}

	//Set the done function as well. It will be different than the one added above
	//Above child bios have done function dz_align_read_multi_block_both_aligned_biodone()
	child_iorequest_tail->ior_bio->bi_end_io = dz_align_read_multi_block_tail_unaligned_biodone;
	//We need to coordinate this tail child bio and above child bios

	//By now we are done with resource allocation part.
	//Now we need to attach the parent bio pages in their
	//corresponding child bios except tail
	
	bi_vcnt_max = bio->bi_vcnt;
	
	while(aligned_bytes) {

		//Fetch the child_iorequest from the free_list_head (stored above)
		child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);

		//Delete that entry from free_list_head
		list_del(&child_iorequest->ior_ioq);
		dz_q_child_iorequest_to_parent_q(piorequest, child_iorequest);
		child_bio = child_iorequest->ior_bio;
		PRINT_POINTER(child_iorequest);

		data_bytes 		= PAGE_SIZE;

		if (remaining_bv_len) {
				data_bytes 		-= remaining_bv_len;
				ret = bio_add_page(child_bio, cur_bvec->bv_page, remaining_bv_len,
				cur_bvec->bv_offset + (cur_bvec->bv_len - remaining_bv_len));
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}
				bi_vcnt++;
				remaining_bv_len = 0;
		}

		while(bi_vcnt < bi_vcnt_max) {
			cur_bvec 	= &bio->bi_io_vec[bi_vcnt];
			bv_len 		= cur_bvec->bv_len;

			if (bv_len >= data_bytes) {

				ret = bio_add_page(child_bio, cur_bvec->bv_page, data_bytes, cur_bvec->bv_offset);
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}

				if (bv_len == data_bytes) {
					bi_vcnt++;
				} else {
					remaining_bv_len = bv_len - data_bytes;
				}
				aligned_bytes -= PAGE_SIZE;
				break;
			} else {

				ret = bio_add_page(child_bio, cur_bvec->bv_page, bv_len, cur_bvec->bv_offset);
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}
				data_bytes -= bv_len;
				bi_vcnt++;
			}
		} // End of Inner While loop

		//At this point the child bio is attached with Parent pages
		//so it ready to be processed
		dz_q_iorequest(align_read_thread_pool, child_iorequest);
	} //End of Outer While loop

	LOGALIGN("Printing Tail part\n");

	//By now list must have one and only one single entry for tail child iorequest.
	//Also quite possible that all the above child bios might have completed
	if (list_empty(&free_list_head)) {
		BUG_ON(1);
	}
	child_iorequest_tail = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
	list_del(&child_iorequest_tail->ior_ioq);
	
	//This child_iorequest_tail will be served just like a partial iorequest
	dz_q_iorequest(align_read_thread_pool, child_iorequest_tail);
	//dz_q_child_iorequest_to_parent_q(piorequest, child_iorequest_tail);

	//dz_merge_parent_local_q_to_threadpool_q(piorequest->ior_thread_pool, piorequest);
	return;

exit_failure:
	IO_DONE_ERROR(piorequest, IO_ERROR_ENOMEM);
	iorequest_put(piorequest);
	if (page) {
		dz_read_page_free(page);
	}
	if (child_iorequest) {
		dz_io_free(child_iorequest);
	}
	if (child_bio) {
		DZ_OS_KERNEL_BIO_PUT(child_bio); 
	}
exit_page_add:
	for (bv_len = 0; bv_len < total_blocks_needed; bv_len++) {

		child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
		if (child_iorequest) {
			DZ_OS_KERNEL_BIO_PUT(child_iorequest->ior_bio); 
			dz_io_free(child_iorequest);
			list_del(&child_iorequest->ior_ioq);
		}
	}
}
//This function does not allocate any new pages.
//Instead it uses the parent pages and attach them
//to child bios.
//It is used to process fec engine aligned read pages
//
//  _____________________________
// | PAGE    | PAGE    | PAGE    |
// | ALIGNED | ALIGNED | ALIGNED |
// |_________|_________|_________|
//
static RVOID dz_align_read_multi_block_both_aligned(PIOREQUEST piorequest)
{
	//static int averma_cnt				= 0;
	PBIO		bio 				= piorequest->ior_bio; // Parent or Original bio
	SIZE  		io_size				= bio->bi_size; // Parent or Original bio size
	SECTOR		sector 				= DZ_BIO_GET_SECTOR(bio);
	LBA			lba					= piorequest->ior_lba;
	PBIOVEC		cur_bvec  			= NULL;
	INT			bi_vcnt				= 0;
	INT			bi_vcnt_max			= 0;
	INT			bv_len				= 0;
	INT			remaining_bv_len	= 0;
	UINT		sector_pos_in_lba	= 0;
	SIZE		sector_bytes		= 0;
	U64			tot_bytes			= 0;
	INT			data_bytes			= 0;
	INT 		aligned_bytes 		= 0;
	INT			total_blocks_needed	= 0;
	PIOREQUEST	child_iorequest 	= NULL;
	PBIO		child_bio 			= NULL;
	LIST_HEAD	free_list_head;
	INT			count;
	INT			ret;
	PIOREQUEST	prev_child_iorequest= NULL;
	INT			nr_iovecs			= 0;

	BOOL		bvec_align			= FALSE;


	//TODO: Optimization required. Try to use bit shifting to simplify this
	sector_bytes 		= sector * SECTOR_SIZE;
	sector_pos_in_lba 	= sector_bytes   - (lba * LBA_BLOCK_SIZE);
	tot_bytes 			= sector_bytes + io_size;
	aligned_bytes 		= io_size ; //Always in multiples of LBA_BLOCK_SIZE

	//Calculate total blocks: one for head, one for tail and ...
	//Since IO is both head and tail aligned so dividing by LBA_BLOCK_SIZE
	//will give no. of child Ios required to process this big parent IO
	total_blocks_needed = (aligned_bytes / LBA_BLOCK_SIZE);

	//It was observed that there were more bio pages attached
	//in the parent and since child bios are using those pages
	//so nr_bvecs should be more than 1
	//print_biom(bio, "Aligned bio");
	//PRINT_INT(total_blocks_needed);
	//nr_iovecs = (bio->bi_max_vecs / total_blocks_needed) + 1;

	//Check if total_blocks_needed are equal to bio->bi_vcnt
	if (total_blocks_needed == bio->bi_vcnt) {
		//bio vecs are also page aligned
		nr_iovecs = 1;
		bvec_align = TRUE;
	} else {
		nr_iovecs = (bio->bi_vcnt / total_blocks_needed) + 1;
		//TODO: Handle this with care, as it may lead to wrong data read
	}

	INIT_LIST_HEAD(&free_list_head);

	LOGALIGND("Total blocks needed = %d, nr_iovecs = %d, bio->bi_max_vecs = %d, bio->bi_vcnt = %d\n",
		total_blocks_needed, nr_iovecs, bio->bi_max_vecs, bio->bi_vcnt);

	//Initialize child count in parent iorequest
	atomic_set(&piorequest->ior_child_cnt, 0);


	if (bvec_align == TRUE) {

		for (count = 0; count < total_blocks_needed; count++) {

			child_iorequest = dz_io_alloc(); 
			LOGALIGN("[count = %d] child_iorequest = %p\n", count, child_iorequest);
			if (!child_iorequest) {
				LOGALIGNE("Unable to get free iorequest\n");
				total_blocks_needed = count + 1; //Used for freeing blocks
				goto exit_failure;
			}
			MEMSET(child_iorequest);

			if (child_iorequest == prev_child_iorequest) {
				LOGALIGNE("#################################################################\n");
				LOGALIGNE("First Alloc child_iorequest = %p\n", child_iorequest);
				LOGALIGNE("Prev Alloc child_iorequest = %p\n", prev_child_iorequest);
				LOGALIGNE("Both are same @ count = %d\n", count);
				LOGALIGNE("Total blocks needed = %d, nr_iovecs = %d\n", total_blocks_needed, nr_iovecs);
				LOGALIGNE("bio->bi_size = %lli, bio->bi_sector = %lli\n", (U64)bio->bi_size, (U64)bio->bi_sector);
				LOGALIGNE("bio->bi_max_vecs = %d, bio->bi_vcnt = %d\n", bio->bi_max_vecs, bio->bi_vcnt);
				LOGALIGNE("sector_bytes = %lli , sector_pos_in_lba = %lli \n", (U64)sector_bytes, (U64)sector_pos_in_lba);
				LOGALIGNE("tot_bytes = %lli , aligned_bytes = %lli \n", (U64)tot_bytes, (U64)aligned_bytes);
				LOGALIGNE("#################################################################\n");
				SL1;
				BUG_ON(1);
			}
			child_bio = dz_bio_alloc(nr_iovecs);
			if (!child_bio) {
				LOGALIGNE("Unable to get free child bio\n");
				total_blocks_needed = count + 1;
				goto exit_failure;
			}


			//Note bio is a parent bio. sector is also getting incremented for each child bio
			dz_init_child_bio(bio, child_bio, sector, 
				dz_align_read_multi_block_both_aligned_biodone, child_iorequest);

			//Setting the size of child bio as page size
			//child_bio->bi_size = PAGE_SIZE;

			atomic_set(&child_iorequest->ior_child_cnt, 0);
			child_iorequest->ior_thread_pool= align_read_thread_pool;
			child_iorequest->ior_bio	= child_bio;
	//		child_iorequest->target 	= piorequest->target;
			child_iorequest->ior_parent 	= piorequest;
			child_iorequest->ior_lba	 	= lba;
			child_iorequest->ior_type	= DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ_SHARE_PAGE;

			//free_list_head is a linked list which is storing the child iorequest
			// in a list. This is going to be used while assigning parent pages to
			//child iorequest

			list_add_tail(&(child_iorequest->ior_ioq), &free_list_head);

			sector += SECTORS_PER_PAGE;

			//Increment the no. of childs in parent iorequest
			atomic_inc(&piorequest->ior_child_cnt);

			//Note: lba needs to be increased because child request is created
			//for each lba
			lba++;
			prev_child_iorequest = child_iorequest;

			cur_bvec 	= &bio->bi_io_vec[count];
			//PRINT_POINTER(cur_bvec->bv_page);
			//PRINT_INT(cur_bvec->bv_len);
			//PRINT_INT(cur_bvec->bv_offset);
			//PRINT_SIZE(io_size);
			ret = bio_add_page(child_iorequest->ior_bio, cur_bvec->bv_page, cur_bvec->bv_len, cur_bvec->bv_offset);
			if (!ret) {
				IO_DONE_ERROR(piorequest, ret);
				LOGALIGNE("Unable to add Page to child bio instance\n");
				goto exit_page_add;
			}

LOGALIGN("AVERMA:BA:bi_vcnt = %d , child_iorequest = %p child_iorequest->ior_bio = %p , ior_type = %d, bi_size = %d, bio_page = %p\n",
bio->bi_vcnt, child_iorequest, child_iorequest->ior_bio,  child_iorequest->ior_type, child_iorequest->ior_bio->bi_size, bio_page(child_iorequest->ior_bio));
			//PRINT_INT(child_bio->bi_size);

		} //Done with iorequest and child bio allocation

		for (count = 0; count < total_blocks_needed; count++) {
			child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
			list_del(&(child_iorequest->ior_ioq));

			//Extra check 
			if (child_iorequest->ior_bio->bi_size > PAGE_SIZE ) {

			}
			dz_q_iorequest(align_read_thread_pool, child_iorequest);
		}

		return;
	} //End of if bvec_align


	//TODO: Currently crashing when bvec is not aligned
	LOGALIGNE("#################################################################\n");
	LOGALIGNE("bio bvec is not aligned\n");
	LOGALIGNE("Total blocks needed = %d, nr_iovecs = %d\n", total_blocks_needed, nr_iovecs);
	LOGALIGNE("bio->bi_size = %lli, bio->bi_sector = %lli\n", (U64)bio->bi_size, (U64)bio->bi_sector);
	LOGALIGNE("bio->bi_max_vecs = %d, bio->bi_vcnt = %d\n", bio->bi_max_vecs, bio->bi_vcnt);
	LOGALIGNE("sector_bytes = %lli , sector_pos_in_lba = %lli \n", (U64)sector_bytes, (U64)sector_pos_in_lba);
	LOGALIGNE("tot_bytes = %lli , aligned_bytes = %lli \n", (U64)tot_bytes, (U64)aligned_bytes);
	print_biom(bio, "bio bvec is not aligned error");
	LOGALIGNE("#################################################################\n");
	SL2;
	BUG_ON(1);


	//When bio is not bvec_align 
	//Need to allocate total_blocks_needed no. of child bios
	for (count = 0; count < total_blocks_needed; count++) {

		child_iorequest = dz_io_alloc(); 
		if (!child_iorequest) {
			LOGALIGNE("Unable to get free iorequest\n");
			total_blocks_needed = count + 1; //Used for freeing blocks
			goto exit_failure;
		}
		MEMSET(child_iorequest);

		if (child_iorequest == prev_child_iorequest) {
			LOGALIGN("First Alloc child_iorequest = %p\n", child_iorequest);
			LOGALIGN("Prev Alloc child_iorequest = %p\n", prev_child_iorequest);
			LOGALIGN("Both are same\n");
			SL1;
			BUG_ON(1);
		}
		child_bio = dz_bio_alloc(nr_iovecs);
		if (!child_bio) {
			LOGALIGNE("Unable to get free child bio\n");
			total_blocks_needed = count + 1;
			goto exit_failure;
		}


		//Note bio is a parent bio. sector is also getting incremented for each child bio
		dz_init_child_bio(bio, child_bio, sector, 
			dz_align_read_multi_block_both_aligned_biodone, child_iorequest);

		atomic_set(&child_iorequest->ior_child_cnt, 0);
		child_iorequest->ior_thread_pool= align_read_thread_pool;
		child_iorequest->ior_bio	= child_bio;
//		child_iorequest->target 	= piorequest->target;
		child_iorequest->ior_parent 	= piorequest;
		child_iorequest->ior_lba	 	= lba;
		child_iorequest->ior_type	= DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ_SHARE_PAGE;

		//free_list_head is a linked list which is storing the child iorequest
		// in a list. This is going to be used while assigning parent pages to
		//child iorequest

		list_add_tail(&(child_iorequest->ior_ioq), &free_list_head);

		sector += SECTORS_PER_PAGE;

		//Increment the no. of childs in parent iorequest
		atomic_inc(&piorequest->ior_child_cnt);

		//Note: lba needs to be increased because child request is created
		//for each lba
		lba++;
		prev_child_iorequest = child_iorequest;

		//LOGALIGN("Child Created %d\n", count);
	}

	//By now we are done with resource allocation part i.e.
	// creation of child iorequests and assigning of lbas.
	//Now we need to attach the parent bio pages in their
	//corresponding child bios including head and tail
	//because here head and tail both are aligned to Pages.
	//Head means the head part of multi block (page) IO
	//Tail means the tail part of multi block (page) IO

	//TODO:Check carefully
	
	bi_vcnt_max = bio->bi_vcnt;
	
	while(aligned_bytes) {

		//Fetch the child_iorequest from the free_list_head (stored above)
		child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);

		//PRINT_POINTER(child_iorequest);

		//Delete that entry from free_list_head
		list_del(&(child_iorequest->ior_ioq));

		//dz_q_child_iorequest_to_parent_q(piorequest, child_iorequest);

		child_bio = child_iorequest->ior_bio;
		//print_biom(child_bio, "Child bio");

		data_bytes 		= PAGE_SIZE;

		if (remaining_bv_len) {
				//PRINT_INT(remaining_bv_len);
				//PRINT_INT(cur_bvec->bv_offset + (cur_bvec->bv_len - remaining_bv_len));
				ret = bio_add_page(child_bio, cur_bvec->bv_page, remaining_bv_len,
				cur_bvec->bv_offset + (cur_bvec->bv_len - remaining_bv_len));
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio\n");
					goto exit_page_add;
				}
				data_bytes 		-= remaining_bv_len;
				bi_vcnt++;
				remaining_bv_len = 0;
		}

		while(bi_vcnt < bi_vcnt_max) {
			cur_bvec 	= &bio->bi_io_vec[bi_vcnt];
			bv_len 		= cur_bvec->bv_len;
			//PRINT_INT(data_bytes);
			//PRINT_INT(bv_len);
			//PRINT_INT(cur_bvec->bv_offset);
			//PRINT_POINTER(cur_bvec->bv_page);
			//PRINT_POINTER(child_bio);

			if (bv_len >= data_bytes) {

				ret = bio_add_page(child_bio, cur_bvec->bv_page, data_bytes, cur_bvec->bv_offset);
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio instance\n");
					goto exit_page_add;
				}

				if (bv_len == data_bytes) {
					bi_vcnt++;
				} else {
					remaining_bv_len = bv_len - data_bytes;
				}
				aligned_bytes -= PAGE_SIZE;
				break;
			} else {

				ret = bio_add_page(child_bio, cur_bvec->bv_page, bv_len, cur_bvec->bv_offset);
				if (!ret) {
					IO_DONE_ERROR(piorequest, ret);
					LOGALIGNE("Unable to add Page to child bio extended\n");
					goto exit_page_add;
				}
				data_bytes -= bv_len;
				bi_vcnt++;
			}
		} // End of Inner While loop


		//At this point the child bio is attached with Parent pages 
		//so it ready to be processed

		//print_iorequest_lite(child_iorequest, "AVERMA:Child Iorequest queued");
		dz_q_iorequest(align_read_thread_pool, child_iorequest);

	} //End of Outer While loop

	LOGD("AVERMA:SET COMPLETED %d \n", averma_cnt++);

	//PRINT_POINTER(piorequest);
	//PRINT_POINTER(piorequest->ior_thread_pool);
	//SL1;
	//dz_merge_parent_local_q_to_threadpool_q(piorequest->ior_thread_pool, piorequest);
	return;

exit_failure:
	IO_DONE_ERROR(piorequest, IO_ERROR_ENOMEM);
	iorequest_put(piorequest);
	if (child_iorequest) {
		dz_io_free(child_iorequest);
	}
	if (child_bio) {
		DZ_OS_KERNEL_BIO_PUT(child_bio); 
	}
exit_page_add:
	for (count = 0; count < total_blocks_needed; count++) {

		child_iorequest = list_first_entry(&free_list_head, struct dz_iorequest_s, ior_ioq);
		if (child_iorequest) {
			DZ_OS_KERNEL_BIO_PUT(child_iorequest->ior_bio); 

			//TODO:Check of double free of child_iorequest, because we have freed one above
			dz_io_free(child_iorequest);
			list_del(&child_iorequest->ior_ioq);
		}
	}
}

//This function handles four cases for multi block (page) READ IO:
// 1. When bio is properly aligned with all pages. It means Head and tail of bio both are Page aligned
// 2. When head of bio is Page Aligned but not tail.
// 3. When head is not Page aligned but tail is aligned
// 4. When head and tail both are not Page aligned

RINT dz_align_read_for_multi_block(PIOREQUEST piorequest)
{
	PBIO	bio 					= piorequest->ior_bio; // Parent or Original bio
	SECTOR	sector 					= DZ_BIO_GET_SECTOR(bio);
	LBA		lba						= 0;
	SIZE  		io_size				= bio->bi_size; // Parent or Original bio size

	SIZE		sector_bytes		= 0;
	U64			tot_bytes			= 0;
	static		INT ba = 0;
	static		INT bu = 0;
	static		INT hu = 0;
	static		INT tu = 0;

	//INC_COUNTER(fec_iocount_reads_multi_page);
	lba = dz_convert_sector_to_lba(sector);

	sector_bytes = sector * SECTOR_SIZE;

	// Four cases can exists here:
	// 1. When bio is properly aligned with all pages. It means Head and tail of bio both are Page aligned
	// 2. When head of bio is Page Aligned but not tail.
	// 3. When head is not Page aligned but tail is aligned
	// 4. When head and tail both are not Page aligned
	//
	// In terms of handling: Case 1 and 2 can be handled together.
	// Case 3 and 4 can also be handled together.

	if ((sector_bytes  % LBA_BLOCK_SIZE) == 0 ) { 
		// Start Sector is Page Aligned. Check if tail is also aligned
		  
		if (((sector_bytes + io_size)  % LBA_BLOCK_SIZE) == 0 ) { 
			// Complete bio is page aligned. So child bios will be 1:1 mapping of pages
			
			//LOGALIGN("Case1: BIO is multiple blocks aligned i.e. Head and Tail Aligned\n");
			// Case 1
			
			//LOGALIGN("Case1:BIO is Head Aligned and Tail Aligned. io_size = %zu \n", io_size);

			if (ba++ == 0) {
				LOGD("Case1:READ BIO is Head Aligned and Tail Aligned. io_size = %zu bytes\n", io_size);
			}
			LOGALIGND("Case1:READ BIO is Head Aligned and Tail Aligned. io_size = %zu bytes\n", io_size);
			INC_COUNTER(align_iocount_reads_multi_aligned_page);
			dz_align_read_multi_block_both_aligned(piorequest);
			RETURNS;

		} else {
			// Tail of the bio is NOT page aligned. 
			// Case 2
			// Here last bv_page will partial page data. 
			//LOGALIGN("Case2:BIO is Head Aligned and Tail UnAligned\n");
			if (tu++ == 0) {
				LOGD("Case2:Read BIO is Head Aligned and Tail UnAligned\n");
			}
			LOGALIGN("%s", "Case2:Read BIO is Head Aligned and Tail UnAligned\n");
			INC_COUNTER(align_iocount_reads_multi_unaligned_page);
			INC_COUNTER(align_iocount_reads_multi_tail_unaligned_page);
			dz_align_read_multi_block_tail_unaligned(piorequest);
			RETURNS;
		}

	} else if (((tot_bytes = (sector_bytes + io_size))  % LBA_BLOCK_SIZE) == 0 ) { 
		// Head is not Page Aligned, but Tail is Page Aligned
		// Case 3
		// Here First page will be a partial one, rest will be complete ones.
		// Here no. of Pages will be same to child bios

		//LOGALIGN("Case3: BIO is Tail Aligned but Not Head Aligned\n");
		
		// Unaligned part of total bytes. It will reside in the Head of bio because tail is always aligned
		//LOGALIGN("Case3:BIO is Head UnAligned and Tail Aligned\n");
		if (hu++ == 0) {
			LOGD("Case3:Read BIO is Head UnAligned and Tail Aligned\n");
		}
		LOGALIGN("%s", "Case3:Read BIO is Head UnAligned and Tail Aligned\n");
		INC_COUNTER(align_iocount_reads_multi_unaligned_page);
		INC_COUNTER(align_iocount_reads_multi_head_unaligned_page);
		dz_align_read_multi_block_head_unaligned(piorequest);
		RETURNS;
	} else  {
		// Head and Tail both are not Page Aligned
		// Case 4
		// Here First page will be a partial one.
		// Last Page will also be partial one
		// Here is the processing part
		// Process the unaligned head part
		// Process the unaligned tail part
		// Process the aligned part from tail to head 
		//LOGALIGN("Case4:BIO is Neither Head Aligned Nor Tail Aligned\n");
		if (bu++ == 0) {
			LOGD("Case4:Read BIO is Neither Head Aligned Nor Tail Aligned\n");
		}
		LOGALIGN("%s", "Case4:Read BIO is Neither Head Aligned Nor Tail Aligned\n");
		INC_COUNTER(align_iocount_reads_multi_unaligned_page);
		INC_COUNTER(align_iocount_reads_multi_both_unaligned_page);
		dz_align_read_multi_block_both_unaligned(piorequest);
		RETURNS;
	}
}

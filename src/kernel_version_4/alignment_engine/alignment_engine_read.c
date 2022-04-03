/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern INT dz_switch_alignment_engine_multi_block_read;
extern INT dz_switch_alignment_engine_partial_block_read;
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

/*
RVOID dz_align_io(PIOREQUEST iorequest)
{
	LOGD("Read Request received at lba %lli\n", lba);
	dz_align_read_io(iorequest);
	return;
}
*/

RVOID dz_align_read_io(PIOREQUEST piorequest)
{
	//PBIO		bio 		= piorequest->ior_bio; // Parent or Original bio
	//TODO:SIZE  io_size	= piorequest->ior_bio->bi_size; // Parent or Original bio size
	SIZE  io_size	= DZ_BIO_GET_SIZE(piorequest->ior_bio); // Parent or Original bio size

	// Parent may/will have multiple childs, hence initializing some counters.
	//PRINT_POINTERS(piorequest->ior_thread_pool,"AVERMA");
	
	INIT_LIST_HEAD(&piorequest->ior_ioq_head_local);
	atomic_set(&piorequest->ior_child_cnt, 0);
	//TODO: Optimization can be done here. 
	if (io_size < LBA_BLOCK_SIZE) {
		

		if (dz_switch_alignment_engine_partial_block_read == DZ_DISABLED) {
			LOGALIGN("Disabled Read Processing io_size %lli for partial block at sector %lli for ior_type = %d\n",
			(U64)io_size, (U64)DZ_BIO_GET_SECTOR(piorequest->ior_bio), piorequest->ior_type);

			FAKE_IO_DONE(piorequest);
			iorequest_put(piorequest);
		} else {
			LOGALIGN("READ_PARTIAL: io_size = %lli sector = %lli ior_type = %d, iorequest = %p\n",
			(U64)io_size, (U64)DZ_BIO_GET_SECTOR(piorequest->ior_bio), piorequest->ior_type, piorequest);
			INC_COUNTER(align_iocount_reads_partial_page);
			dz_align_read_for_partial_block(piorequest);
		}

	} else if (io_size == LBA_BLOCK_SIZE) {

		LOGALIGN("READ_SINGLE: io_size = %lli sector = %lli ior_type = %d, iorequest = %p\n",
		(U64)io_size, (U64)DZ_BIO_GET_SECTOR(piorequest->ior_bio), piorequest->ior_type, piorequest);

		INC_COUNTER(align_iocount_reads_single_page);

		if (piorequest->ior_type == 1) {
			FAKE_IO_DONE(piorequest);
			iorequest_put(piorequest);
		} else {
			dz_align_read_for_single_block(piorequest);
		}

	} else if (io_size > LBA_BLOCK_SIZE) {

		if (dz_switch_alignment_engine_multi_block_read == DZ_DISABLED) {
			LOGALIGN("DISABLED:READ_MULTI: io_size = %lli sector = %lli ior_type = %d, iorequest = %p\n",
			(U64)io_size, (U64)DZ_BIO_GET_SECTOR(piorequest->ior_bio), piorequest->ior_type, piorequest);
			FAKE_IO_DONE(piorequest);
			iorequest_put(piorequest);

		} else {
			LOGALIGN("READ_MULTI: io_size = %lli sector = %lli ior_type = %d, iorequest = %p\n",
			(U64)io_size, (U64)DZ_BIO_GET_SECTOR(piorequest->ior_bio), piorequest->ior_type, piorequest);
			INC_COUNTER(align_iocount_reads_multi_page);
			dz_align_read_for_multi_block(piorequest);
		}

	} else {
		//Should not come here
		BUG_ON(1);
	}
	//return DM_MAPIO_SUBMITTED;
	return;
}

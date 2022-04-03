/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

//For operational purpose
extern ATOMIC64 align_iocount_writes_partial_page;
extern ATOMIC64 align_iocount_writes_single_page;
extern ATOMIC64 align_iocount_writes_single_aligned_page;
extern ATOMIC64 align_iocount_writes_single_unaligned_page;
extern ATOMIC64 align_iocount_writes_multi_page;

RVOID dz_align_write_io(PIOREQUEST iorequest)
{
	PBIO		bio 		= iorequest->ior_bio; // Parent or Original bio
	//TODO:SIZE  		io_size		= bio->bi_size; // Parent or Original bio size
	SIZE  		io_size		= DZ_BIO_GET_SIZE(bio); // Parent or Original bio size


	if (io_size < LBA_BLOCK_SIZE) {
		LOGALIGN("Case1: Write Processing io_size for partial block at lba %lli\n",
		dz_convert_sector_to_lba(DZ_BIO_GET_SECTOR(bio)));

		INC_COUNTER(align_iocount_writes_partial_page);
		dz_align_write_for_partial_block(iorequest);
		return;

	} else if (io_size == LBA_BLOCK_SIZE) {

		LOGALIGN("Case2: Write Processing io_size for single block at lba %lli\n",
		dz_convert_sector_to_lba(DZ_BIO_GET_SECTOR(bio)));

		INC_COUNTER(align_iocount_writes_single_page);

		dz_align_write_for_single_block(iorequest);
		return;

	} else if (io_size > LBA_BLOCK_SIZE) {
		LOGALIGN("Case3: Write Processing io_size for multiple blocks at lba %lli\n",
		dz_convert_sector_to_lba(DZ_BIO_GET_SECTOR(bio)));

		INC_COUNTER(align_iocount_writes_multi_page);
		dz_align_write_for_multi_block(iorequest);
		return;

	} else {
		//Should not come here
		BUG_ON(1);
	}
	return;
}

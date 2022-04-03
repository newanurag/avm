#ifndef _ALIGNMENT_ENGINE_H_
#define _ALIGNMENT_ENGINE_H_

#include "types.h"
#include "macros.h"
#include "metadata_engine.h"
#include "iorequest.h"

// Alignment Functions
RINT dz_process_unaligned_data_from_bio_head(PIOREQUEST piorequest,
		PBVEC_HOLDER bvec_holder, UINT partial_data_head, UINT aligned_data,
		UINT partial_data_tail, SECTOR sector);

RINT dz_process_partial_unaligned_data_from_bio_head(PIOREQUEST piorequest,
		 PBVEC_HOLDER bvec_holder, UINT partial_data, SECTOR sector);

RINT dz_process_partial_unaligned_data_from_bio_tail(PIOREQUEST piorequest,
		PBVEC_HOLDER bvec_holder, INT partial_data, SECTOR sector);

RINT dz_process_aligned_data_from_bio_tail(PIOREQUEST piorequest,
		 PBVEC_HOLDER bvec_holder, UINT aligned_data_bytes, SECTOR sector);

RINT dz_process_alignment_for_multiple_blocks_io(PIOREQUEST piorequest);
RINT dz_process_alignment_for_partial_block_io(PIOREQUEST piorequest);
RINT dz_process_alignment_for_single_block_io(PIOREQUEST piorequest);
RINT dz_write_split_bio_with_alignment_processing(PIOREQUEST piorequest);

RVOID dz_align_read_io(PIOREQUEST piorequest);
RVOID dz_align_write_io(PIOREQUEST piorequest);

RVOID dz_align_read_for_partial_block(PIOREQUEST piorequest);
RVOID dz_align_read_for_single_block(PIOREQUEST piorequest);
RINT dz_align_read_for_multi_block(PIOREQUEST piorequest);

RINT dz_align_read_single_block_from_raw_disk_sync_internal(PIOREQUEST iorequest, SECTOR sector );
RINT dz_align_read_single_block_from_raw_disk_sync(PIOREQUEST iorequest, SECTOR sector);

RVOID dz_align_write_for_partial_block(PIOREQUEST parent_iorequest);
RVOID dz_align_write_for_single_block(PIOREQUEST iorequest);
RVOID dz_align_write_for_multi_block(PIOREQUEST piorequest);

INT dz_align_engine_init(VOID);
RVOID dz_align_engine_exit(VOID);


#endif // _ALIGNMENT_ENGINE_H_

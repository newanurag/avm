#ifndef AISA_COUNTERS_H
#define AISA_COUNTERS_H

#include "types.h"
#include "hash_engine_murmur3.h"
#include "macros.h"

typedef struct dz_base_engine_counters_s {
	INT 		dz_switch_passthrough_mode;
	INT 		dz_switch_passthrough_read;
	INT 		dz_switch_passthrough_write;
	ATOMIC64 	dz_iorequest_alloc_cnt;
	ATOMIC64 	dz_iorequest_free_cnt;
	ATOMIC64 	dz_page_alloc_cnt;
	ATOMIC64 	dz_page_free_cnt;
	U64 		fec_engine_memory_usage;
	U64 		bec_engine_memory_usage;
	U64 		dedupe_engine_memory_usage;
	U64 		infra_engine_memory_usage;
	U64 		md_engine_memory_usage;
	ATOMIC64 	dz_total_memory_consumed_by_all_pools;
	U64 		lba_table_size;
	U64 		pba_table_size;
	U64 		hash_table_size;

	ATOMIC64 	tot_application_iocount;
	ATOMIC64 	tot_application_iocount_reads;
	ATOMIC64 	tot_application_iocount_reads_partial_page;
	ATOMIC64 	tot_application_iocount_reads_single_page;
	ATOMIC64 	tot_application_iocount_reads_multi_page;
	ATOMIC64 	tot_application_iocount_writes;
	ATOMIC64 	tot_application_iocount_writes_partial_page;
	ATOMIC64 	tot_application_iocount_writes_single_page;
	ATOMIC64 	tot_application_iocount_writes_multi_page;

/* Below macros are built for above data types. If a variable with new data type is added then
 * its corresponding macros needs to be created below (if not created below).
 */
#define	BASE_COUNTER_ATOMIC64_SET(var, val) 	ATOMIC64_SET(pdz_basic_counters->var, val)
#define	BASE_COUNTER_ATOMIC64_ADD(var, val) 	ATOMIC64_ADD(pdz_basic_counters->var, val)
#define	BASE_COUNTER_ATOMIC64_SUB(var, val) 	ATOMIC64_SUB(pdz_basic_counters->var, val)

#define	BASE_COUNTER_ATOMIC_INC(var) 			ATOMIC_INC(pdz_basic_counters->var)
#define	BASE_COUNTER_ATOMIC64_INC(var) 			ATOMIC64_INC(pdz_basic_counters->var)

#define	BASE_COUNTER_ATOMIC_DEC(var) 			ATOMIC_DEC(pdz_basic_counters->var)
#define	BASE_COUNTER_ATOMIC64_DEC(var) 			ATOMIC64_DEC(pdz_basic_counters->var)

#define	BASE_COUNTER_ATOMIC64_READ(var) 		ATOMIC64_READ(pdz_basic_counters->var)
#define	BASE_COUNTER_ATOMIC64_GET(var) 			(&(pdz_basic_counters->var))

#define	BASE_COUNTER_U64_GET(var) 				(pdz_basic_counters->var)
#define	BASE_COUNTER_U64_SET(var, val) 			(pdz_basic_counters->var) = (val)

#define	BASE_COUNTER_INT_GET(var) 				(pdz_basic_counters->var)
#define	BASE_COUNTER_INT_SET(var, val) 			(pdz_basic_counters->var) = (val)

}__attribute__((__packed__)) BASIC_COUNTERS, *PBASIC_COUNTERS;

extern BASIC_COUNTERS  dz_basic_counters;
extern PBASIC_COUNTERS pdz_basic_counters;


// Alignment Engine Counters
typedef struct dz_alignment_engine_counters_s {
	U64 		align_engine_memory_usage;
	ATOMIC64 	align_iocount_reads_partial_page;
	ATOMIC64 	align_iocount_reads_single_page;
	ATOMIC64 	align_iocount_reads_single_aligned_page;
	ATOMIC64 	align_iocount_reads_single_unaligned_page;
	ATOMIC64 	align_iocount_reads_multi_page;
	ATOMIC64 	align_iocount_reads_multi_aligned_page;
	ATOMIC64 	align_iocount_reads_multi_unaligned_page;
	ATOMIC64 	align_iocount_writes_partial_page;
	ATOMIC64 	align_iocount_writes_single_page;
	ATOMIC64 	align_iocount_writes_single_aligned_page;
	ATOMIC64 	align_iocount_writes_single_unaligned_page;
	ATOMIC64 	align_iocount_writes_multi_page;

/* Below macros are built for above data types. If a variable with new data type is added then
 * its corresponding macros needs to be created below (if not created below).
 */
#define	ALIGN_COUNTER_ATOMIC64_SET(var, val) 	ATOMIC64_SET(pdz_align_counters->var, val)
#define	ALIGN_COUNTER_ATOMIC64_ADD(var, val) 	ATOMIC64_ADD(pdz_align_counters->var, val)
#define	ALIGN_COUNTER_ATOMIC64_SUB(var, val) 	ATOMIC64_SUB(pdz_align_counters->var, val)

#define	ALIGN_COUNTER_ATOMIC_INC(var) 			ATOMIC_INC(pdz_align_counters->var)
#define	ALIGN_COUNTER_ATOMIC64_INC(var) 		ATOMIC64_INC(pdz_align_counters->var)

#define	ALIGN_COUNTER_ATOMIC_DEC(var) 			ATOMIC_DEC(pdz_align_counters->var)
#define	ALIGN_COUNTER_ATOMIC64_DEC(var) 		ATOMIC64_DEC(pdz_align_counters->var)

#define	ALIGN_COUNTER_ATOMIC64_READ(var) 		ATOMIC64_READ(pdz_align_counters->var)

#define	ALIGN_COUNTER_U64_GET(var) 				(pdz_align_counters->var)
#define	ALIGN_COUNTER_U64_SET(var, val) 		(pdz_align_counters->var) = (val)

#define	ALIGN_COUNTER_INT_GET(var) 				(pdz_align_counters->var)
#define	ALIGN_COUNTER_INT_SET(var, val) 		(pdz_align_counters->var) = (val)

}__attribute__((__packed__)) ALIGN_COUNTERS, *PALIGN_COUNTERS;

extern ALIGN_COUNTERS  dz_align_counters;
extern PALIGN_COUNTERS pdz_align_counters;

// Front End Cach Engine Counters
typedef struct dz_fec_engine_counters_s {
	U64 		fec_engine_memory_usage;
	ATOMIC64 	fec_active_count_sequence;
	ATOMIC64 	fec_free_count_sequence;
	ATOMIC64 	fec_cur_flush_count;
	ATOMIC64 	fec_flush_ops_count;
	ATOMIC64 	fec_cur_flush_ops_count;
	ATOMIC64 	fec_memory_reads_count;
	ATOMIC64 	fec_forwarded_reads_count;
	U64      	fec_active_count;
	ATOMIC64 	fec_iocount_reads_partial_page;
	ATOMIC64 	fec_iocount_reads_single_page;
	ATOMIC64 	fec_iocount_reads_single_aligned_page;
	ATOMIC64 	fec_iocount_reads_single_unaligned_page;
	ATOMIC64 	fec_iocount_reads_multi_page;
	ATOMIC64 	fec_iocount_reads_multi_aligned_page;
	ATOMIC64 	fec_iocount_reads_multi_unaligned_page;
	ATOMIC64 	fec_iocount_writes_partial_page;
	ATOMIC64 	fec_iocount_writes_single_page;
	ATOMIC64 	fec_iocount_writes_single_aligned_page;
	ATOMIC64 	fec_iocount_writes_single_unaligned_page;
	ATOMIC64 	fec_iocount_writes_multi_page;

/* Below macros are built for above data types. If a variable with new data type is added then
 * its corresponding macros needs to be created below (if not created below).
 */
#define	FEC_COUNTER_ATOMIC64_SET(var, val) 		ATOMIC64_SET(pdz_fec_counters->var, val)
#define	FEC_COUNTER_ATOMIC64_ADD(var, val) 		ATOMIC64_ADD(pdz_fec_counters->var, val)
#define	FEC_COUNTER_ATOMIC64_SUB(var, val) 		ATOMIC64_SUB(pdz_fec_counters->var, val)

#define	FEC_COUNTER_ATOMIC_INC(var) 			ATOMIC_INC(pdz_fec_counters->var)
#define	FEC_COUNTER_ATOMIC64_INC(var) 			ATOMIC64_INC(pdz_fec_counters->var)

#define	FEC_COUNTER_ATOMIC_DEC(var) 			ATOMIC_DEC(pdz_fec_counters->var)
#define	FEC_COUNTER_ATOMIC64_DEC(var) 			ATOMIC64_DEC(pdz_fec_counters->var)

#define	FEC_COUNTER_ATOMIC64_READ(var) 			ATOMIC64_READ(pdz_fec_counters->var)

#define	FEC_COUNTER_U64_GET(var) 				(pdz_fec_counters->var)
#define	FEC_COUNTER_U64_SET(var, val) 			(pdz_fec_counters->var) = (val)

#define	FEC_COUNTER_INT_GET(var) 				(pdz_fec_counters->var)
#define	FEC_COUNTER_INT_SET(var, val) 			(pdz_fec_counters->var) = (val)

}__attribute__((__packed__)) FEC_COUNTERS, *PFEC_COUNTERS;

extern FEC_COUNTERS  dz_fec_counters;
extern PFEC_COUNTERS pdz_fec_counters;

//DeDupe Engine Counters
typedef struct dz_dedupe_engine_counters_s {
	U64 		dedupe_engine_memory_usage;
	U64 		lba_table_size;
	U64 		pba_table_size;
	U64 		hash_table_size;
	ATOMIC64    dedupe_disk_reads;
	ATOMIC64    dedupe_disk_writes;
	ATOMIC64    dedupe_cache_hits;
	ATOMIC64    dedupe_deleted_entries;
	U64         dedupe_tot_logical_space;
	U64         dedupe_tot_physical_space;
	ATOMIC64    dedupe_cur_logical_space;
	ATOMIC64    dedupe_cur_physical_space;
	ATOMIC64    dedupe_cur_duplicate_space;
	ATOMIC64    dedupe_zeroed_out_page_reads;
	USHORT   dedupe_num_threads;

/* Below macros are built for above data types. If a variable with new data type is added then
 * its corresponding macros needs to be created below (if not created below).
 */
#define	DDE_COUNTER_ATOMIC64_SET(var, val) 		ATOMIC64_SET(pdz_dde_counters->var, val)
#define	DDE_COUNTER_ATOMIC64_ADD(var, val) 		ATOMIC64_ADD(pdz_dde_counters->var, val)
#define	DDE_COUNTER_ATOMIC64_SUB(var, val) 		ATOMIC64_SUB(pdz_dde_counters->var, val)

#define	DDE_COUNTER_ATOMIC_INC(var) 			ATOMIC_INC(pdz_dde_counters->var)
#define	DDE_COUNTER_ATOMIC64_INC(var) 			ATOMIC64_INC(pdz_dde_counters->var)

#define	DDE_COUNTER_ATOMIC_DEC(var) 			ATOMIC_DEC(pdz_dde_counters->var)
#define	DDE_COUNTER_ATOMIC64_DEC(var) 			ATOMIC64_DEC(pdz_dde_counters->var)

#define	DDE_COUNTER_ATOMIC64_READ(var) 			ATOMIC64_READ(pdz_dde_counters->var)

#define	DDE_COUNTER_U64_GET(var) 				(pdz_dde_counters->var)
#define	DDE_COUNTER_U64_SET(var, val) 			(pdz_dde_counters->var) = (val)

#define	DDE_COUNTER_INT_GET(var) 				(pdz_dde_counters->var)
#define	DDE_COUNTER_INT_SET(var, val) 			(pdz_dde_counters->var) = (val)

}__attribute__((__packed__)) DDE_COUNTERS, *PDDE_COUNTERS;

extern DDE_COUNTERS  dz_dde_counters;
extern PDDE_COUNTERS pdz_dde_counters;

//Backend Cache (BEC) Engine Counters
typedef struct dz_bec_engine_counters_s {
	U64 		bec_engine_memory_usage;
	ATOMIC64	bec_pba_page_count;
	ATOMIC64	bec_cache_hit;
	ATOMIC64	bec_cache_miss;
	ATOMIC64	bec_cache_delete;
	ULONG 		bec_size;
	INT   		bec_cutoff_nodes;
	INT   		bec_cutoff_percent;
	INT   		dz_bec_node_mempool_count;
	ATOMIC64 	bec_page_alloc_cnt;
	ATOMIC64 	bec_page_free_cnt;
	ATOMIC64 	bec_node_alloc_cnt;
	ATOMIC64 	bec_node_free_cnt;

/* Below macros are built for above data types. If a variable with new data type is added then
 * its corresponding macros needs to be created below (if not created below).
 */
#define	BEC_COUNTER_ATOMIC64_SET(var, val) 		ATOMIC64_SET(pdz_bec_counters->var, val)
#define	BEC_COUNTER_ATOMIC64_ADD(var, val) 		ATOMIC64_ADD(pdz_bec_counters->var, val)
#define	BEC_COUNTER_ATOMIC64_SUB(var, val) 		ATOMIC64_SUB(pdz_bec_counters->var, val)

#define	BEC_COUNTER_ATOMIC_INC(var) 			ATOMIC_INC(pdz_bec_counters->var)
#define	BEC_COUNTER_ATOMIC64_INC(var) 			ATOMIC64_INC(pdz_bec_counters->var)

#define	BEC_COUNTER_ATOMIC_DEC(var) 			ATOMIC_DEC(pdz_bec_counters->var)
#define	BEC_COUNTER_ATOMIC64_DEC(var) 			ATOMIC64_DEC(pdz_bec_counters->var)

#define	BEC_COUNTER_ATOMIC64_READ(var) 			ATOMIC64_READ(pdz_bec_counters->var)

#define	BEC_COUNTER_U64_GET(var) 				(pdz_bec_counters->var)
#define	BEC_COUNTER_U64_SET(var, val) 			(pdz_bec_counters->var) = (val)

#define	BEC_COUNTER_INT_GET(var) 				(pdz_bec_counters->var)
#define	BEC_COUNTER_INT_SET(var, val) 			(pdz_bec_counters->var) = (val)

}__attribute__((__packed__)) BEC_COUNTERS, *PBEC_COUNTERS;

extern BEC_COUNTERS  dz_bec_counters;
extern PBEC_COUNTERS pdz_bec_counters;

typedef struct dz_avm_counters_s {
	PBASIC_COUNTERS		basic_counters;
	PDDE_COUNTERS		dde_counters;
	PFEC_COUNTERS		fec_counters;
	PBEC_COUNTERS		bec_counters;
}__attribute__((__packed__)) AVM_COUNTERS, *PAVM_COUNTERS;

#endif  // COUNTERS

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

#define TEST_PASSED   100
#define TEST_FAILED   -1

extern int dz_switch_passthrough_mode;
extern int dz_switch_passthrough_read;
extern int dz_switch_passthrough_write;
extern USHORT num_align_read_threads;
extern USHORT num_align_write_threads;

extern INT dz_switch_fec_engine;
extern INT dz_switch_bec_engine;
extern INT dz_switch_dedupe_engine;
extern INT dz_switch_alignment_engine;
extern INT dz_switch_alignment_engine_multi_block_read;
extern INT dz_switch_alignment_engine_multi_block_write;
extern INT dz_switch_alignment_engine_partial_block_read;
extern INT dz_switch_alignment_engine_partial_block_write;


//extern ATOMIC64 dz_iorequest_alloc_cnt;
//extern ATOMIC64 dz_iorequest_free_cnt;
extern BASIC_COUNTERS  dz_basic_counters;
extern PBASIC_COUNTERS pdz_basic_counters;

//extern ATOMIC64 dz_page_alloc_cnt;
//extern ATOMIC64 dz_page_free_cnt;

extern SIZE 			TARGET_DEVICE_SIZE;

extern PAVM_DEVICE      aisa_device_object;
extern NAME             DATA_DISK_DEVICE_NAME;
extern OBJECT_LIST 	global_object_list;
extern U64 lba_table_size;
extern U64 pba_table_size;
extern U64 hash_table_size;
extern U32 number_of_longs_for_pba;
extern U32 number_of_longs_for_lba;
extern U32 number_of_longs_for_gpba;
extern ULONG            metadata_flush_interval;
extern U64 metadata_lba_table_size;
extern U64 metadata_lba_lock_table_size;
extern U64 avm_sysfs_entry_free_page_count;
extern CHAR avm_sysfs_entry_get_list_of_volumes[DZ_SYSFS_PAGE_SIZE];
extern CHAR avm_sysfs_entry_get_list_of_volumes_short[DZ_SYSFS_PAGE_SIZE];
extern PPBA_BLOCK		pba_table;
extern PHASH_BLOCK		hash_table;
extern PLBA_BLOCK		lba_table;

extern ATOMIC64			bec_pba_page_count;
extern ATOMIC64			bec_cache_hit;
extern ATOMIC64			bec_cache_miss;
extern ATOMIC64			bec_cache_delete;
extern ULONG			bec_size;
extern INT				bec_cutoff_nodes;
extern INT				bec_cutoff_percent;
extern INT				dz_bec_node_mempool_count;
extern U64			time_flush_prepare_start;
extern U64			time_flush_start;
extern U64			time_flush_end;
extern U64			time_fec_write_start;
extern U64			time_fec_write_switch;
extern PFEC_TABLE	fec_tables_flush;
extern ATOMIC64		fec_active_count_sequence;
extern ATOMIC64		fec_free_count_sequence;
extern ATOMIC64		fec_cur_flush_count;
extern ATOMIC64		fec_flush_ops_count;
extern ATOMIC64		fec_cur_flush_ops_count;
extern ATOMIC64		fec_memory_reads_count;
extern ATOMIC64		fec_forwarded_reads_count;
extern U64			fec_active_count;
extern ATOMIC64	dedupe_disk_reads;
extern ATOMIC64	dedupe_disk_writes;
extern ATOMIC64	dedupe_zeroed_out_page_reads;
extern ATOMIC64	dedupe_cache_hits;
extern ATOMIC64	dedupe_deleted_entries;
extern ATOMIC64	dedupe_cur_logical_space;
extern ATOMIC64	dedupe_cur_physical_space;
extern ATOMIC64	dedupe_cur_duplicate_space;
extern U64		dedupe_tot_logical_space;
extern U64		dedupe_tot_physical_space;
extern USHORT   dedupe_num_threads;
extern PDZ_THREAD_POOL dedupe_thread_pool;
extern PDZ_THREAD_POOL rdedupe_thread_pool;
extern PDZ_THREAD_POOL wdedupe_thread_pool;
extern PDZ_THREAD_POOL read_thread_pool;
extern PDZ_THREAD_POOL align_read_thread_pool;
extern PDZ_THREAD_POOL write_thread_pool; 
extern PDZ_THREAD_POOL align_write_thread_pool; 
extern PDZ_THREAD_POOL flush_thread_pool;
extern PDZ_THREAD_POOL metadata_flush_thread_pool;
extern ATOMIC64		tot_application_iocount;
extern ATOMIC64		tot_application_iocount_reads;
extern ATOMIC64 	tot_application_iocount_reads_partial_page;
extern ATOMIC64 	tot_application_iocount_reads_single_page;
extern ATOMIC64 	tot_application_iocount_reads_multi_page;
extern ATOMIC64 	fec_iocount_reads_partial_page;
extern ATOMIC64 	fec_iocount_reads_single_page;
extern ATOMIC64 	fec_iocount_reads_single_aligned_page;
extern ATOMIC64 	fec_iocount_reads_single_unaligned_page;
extern ATOMIC64 	fec_iocount_reads_multi_page;
extern ATOMIC64 	fec_iocount_reads_multi_aligned_page;
extern ATOMIC64 	fec_iocount_reads_multi_unaligned_page;

extern ATOMIC64 	align_iocount_disk_read;
extern ATOMIC64 	align_iocount_disk_read_internal;
extern ATOMIC64 	align_iocount_reads_partial_page;
extern ATOMIC64 	align_iocount_reads_single_page;
extern ATOMIC64 	align_iocount_reads_single_aligned_page;
extern ATOMIC64 	align_iocount_reads_single_unaligned_page;
extern ATOMIC64 	align_iocount_reads_multi_page;
extern ATOMIC64 	align_iocount_reads_multi_aligned_page;
extern ATOMIC64 	align_iocount_reads_multi_unaligned_page;

extern ATOMIC64		tot_application_iocount_writes;
extern ATOMIC64 	tot_application_iocount_writes_partial_page;
extern ATOMIC64 	tot_application_iocount_writes_single_page;
extern ATOMIC64 	tot_application_iocount_writes_multi_page;
extern ATOMIC64 	fec_iocount_writes_partial_page;
extern ATOMIC64 	fec_iocount_writes_single_page;
extern ATOMIC64 	fec_iocount_writes_single_aligned_page;
extern ATOMIC64 	fec_iocount_writes_single_unaligned_page;
extern ATOMIC64 	fec_iocount_writes_multi_page;
extern ATOMIC64 	fec_iocount_writes_multi_aligned_page;
extern ATOMIC64 	fec_iocount_writes_multi_unaligned_page;

extern ATOMIC64 	align_iocount_disk_write;
extern ATOMIC64 	align_iocount_disk_write_internal;
extern ATOMIC64 	align_iocount_writes_partial_page;
extern ATOMIC64 	align_iocount_writes_single_page;
extern ATOMIC64 	align_iocount_writes_single_aligned_page;
extern ATOMIC64 	align_iocount_writes_single_unaligned_page;
extern ATOMIC64 	align_iocount_writes_multi_page;
extern ATOMIC64 	align_iocount_writes_multi_aligned_page;
extern ATOMIC64 	align_iocount_writes_multi_unaligned_page;

extern PSUPERBLOCK 	superblock;

extern CHAR avm_sysfs_entry_get_align_counters[DZ_SYSFS_PAGE_SIZE];
INT avm_get_align_counters(CHAR *buf)
{
	INT count = 0;
	UINT rpending_iorequests;
	UINT rpending_threads;
	UINT rpending_jobs;

	UINT wpending_iorequests;
	UINT wpending_threads;
	UINT wpending_jobs;

	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_align_counters;
	memset(avm_sysfs_entry_get_align_counters, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "==============================================\n");
	ptr += sprintf(ptr, "ALIGNMENT ENGINE COUNTERS\n");
	ptr += sprintf(ptr, "==============================================\n");
	if (!superblock) {
		ptr += sprintf(ptr, "Superblock is NULL");
		return 0;
	}

	if (dz_switch_alignment_engine == DZ_DISABLED) {
		ptr += sprintf(ptr, "ALIGNMENT ENGINE DISABLED");
		return 0;
	}

	rpending_iorequests	=  atomic_read(&align_read_thread_pool->ioq_num_ios);
	rpending_threads	=  atomic_read(&align_read_thread_pool->num_active_threads);
	rpending_jobs		=  atomic_read(&align_read_thread_pool->num_jobs);

	wpending_iorequests	=  atomic_read(&align_write_thread_pool->ioq_num_ios);
	wpending_threads	=  atomic_read(&align_write_thread_pool->num_active_threads);
	wpending_jobs		=  atomic_read(&align_write_thread_pool->num_jobs);

	//Note: Value is NOT lock safe
	count = sprintf(ptr,
	"ALIGN Read Thread Pool Name....................= %s\n"
	"ALIGN No. of Read Threads......................= %u\n"
	"ALIGN Pending Read IO Requests.................= %u\n"
	"ALIGN Pending Read IO Threads..................= %u\n"
	"ALIGN Pending Read Jobs........................= %u\n"
	"ALIGN Read IO quiescing........................= %d\n"
	"ALIGN Read IO queue Opened.....................= %d\n"
	"ALIGN Write Thread Pool Name...................= %s\n"
	"ALIGN No. of Write Threads.....................= %u\n"
	"ALIGN Pending Write IO Requests................= %u\n"
	"ALIGN Pending Write IO Threads.................= %u\n"
	"ALIGN Pending Write Jobs.......................= %u\n"
	"ALIGN Write IO quiescing.......................= %d\n"
	"ALIGN Write IO queue Opened....................= %d\n"
	"ALIGN Current Time.............................= %lli\n"
	"ALIGN Diff wrt Current Time(secs)..............= %lli\n"
	"ALIGN Diff wrt Final(secs).....................= %lli\n"
	"ALIGN Application IO Count.....................= %lli\n"
	"ALIGN Application IO Count READS...............= %lli\n"
	"ALIGN Application IO Count READS Partial Page..= %lli\n"
	"ALIGN Application IO Count READS Single Page...= %lli\n"
	"ALIGN Application IO Count READS Multi Page....= %lli\n"
	"ALIGN Application IO Count WRITES..............= %lli\n"
	"ALIGN Application IO Count WRITES Partial Page.= %lli\n"
	"ALIGN Application IO Count WRITES Single Page..= %lli\n"
	"ALIGN Application IO Count WRITES Multi Page...= %lli\n"
	"ALIGN IO Count READS Partial Page..............= %lli\n"
	"ALIGN IO Count READS Single Page...............= %lli\n"
	"ALIGN IO Count READS Single Aligned Page.......= %lli\n"
	"ALIGN IO Count READS Single Unaligned Page.....= %lli\n"
	"ALIGN IO Count READS Multi Page................= %lli\n"
	"ALIGN IO Count READS Multi Aligned Page........= %lli\n"
	"ALIGN IO Count READS Multi Unaligned Page......= %lli\n"
	"ALIGN IO Count READS Disk Reads Direct.........= %lli\n"
	"ALIGN IO Count READS Disk Reads Indirect.......= %lli\n"
	"ALIGN IO Count WRITES Partial Page.............= %lli\n"
	"ALIGN IO Count WRITES Single Page..............= %lli\n"
	"ALIGN IO Count WRITES Single Aligned Page......= %lli\n"
	"ALIGN IO Count WRITES Single Unaligned Page....= %lli\n"
	"ALIGN IO Count WRITES Multi Page...............= %lli\n"
	"ALIGN IO Count WRITES Multi Aligned Page.......= %lli\n"
	"ALIGN IO Count WRITES Multi Unaligned Page.....= %lli\n"
	"ALIGN IO Count WRITES Disk Writes Direct.......= %lli\n"
	"ALIGN IO Count WRITES Disk Writes InDirect.....= %lli\n",
	//"Flush Completion Rate(psec) = %lli\n",
	align_read_thread_pool->name,
	align_read_thread_pool->num_threads,
	rpending_iorequests,
	rpending_threads,
	rpending_jobs,
	align_read_thread_pool->quiesce,
	align_read_thread_pool->ioq_open,

	write_thread_pool->name,
	write_thread_pool->num_threads,
	wpending_iorequests,
	wpending_threads,
	wpending_jobs,
	align_write_thread_pool->quiesce,
	align_write_thread_pool->ioq_open,

	(U64)GET_CURRENT_TIME(),
	(U64)(GET_TIME_DIFF_IN_SECONDS(time_flush_start, GET_CURRENT_TIME())/1000),
	(U64)((GET_TIME_DIFF_IN_SECONDS(time_flush_start, time_flush_end))/1000),
	(U64)READ_COUNTER(tot_application_iocount),
	(U64)READ_COUNTER(tot_application_iocount_reads),
	(U64)READ_COUNTER(tot_application_iocount_reads_partial_page),
	(U64)READ_COUNTER(tot_application_iocount_reads_single_page),
	(U64)READ_COUNTER(tot_application_iocount_reads_multi_page),
	(U64)READ_COUNTER(tot_application_iocount_writes),
	(U64)READ_COUNTER(tot_application_iocount_writes_partial_page),
	(U64)READ_COUNTER(tot_application_iocount_writes_single_page),
	(U64)READ_COUNTER(tot_application_iocount_writes_multi_page),
	(U64)READ_COUNTER(align_iocount_reads_partial_page),
	(U64)READ_COUNTER(align_iocount_reads_single_page),
	(U64)READ_COUNTER(align_iocount_reads_single_aligned_page),
	(U64)READ_COUNTER(align_iocount_reads_single_unaligned_page),
	(U64)READ_COUNTER(align_iocount_reads_multi_page),
	(U64)READ_COUNTER(align_iocount_reads_multi_aligned_page),
	(U64)READ_COUNTER(align_iocount_reads_multi_unaligned_page),
	(U64)READ_COUNTER(align_iocount_disk_read),
	(U64)READ_COUNTER(align_iocount_disk_read_internal),
	(U64)READ_COUNTER(align_iocount_writes_partial_page),
	(U64)READ_COUNTER(align_iocount_writes_single_page),
	(U64)READ_COUNTER(align_iocount_writes_single_aligned_page),
	(U64)READ_COUNTER(align_iocount_writes_single_unaligned_page),
	(U64)READ_COUNTER(align_iocount_writes_multi_page),
	(U64)READ_COUNTER(align_iocount_writes_multi_aligned_page),
	(U64)READ_COUNTER(align_iocount_writes_multi_unaligned_page),
	(U64)READ_COUNTER(align_iocount_disk_write),
	(U64)READ_COUNTER(align_iocount_disk_write_internal)
	);
	/*
	(U64) ((READ_COUNTER(fec_cur_flush_count)) / 
	(GET_TIME_DIFF_IN_SECONDS(time_flush_start, GET_CURRENT_TIME())/1000)));
	*/

	//LOGR("<------------ FEC Show ----------->\n");
	//LOGR("%s",buf);

	LOGD("Buffer Bytes used           = %d\n", count);
	//ptr += sprintf(ptr, "==============================================\n");

	return 0;
}

extern U64 avm_sysfs_entry_get_set_switch_dae;
INT avm_get_switch_dae(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_switch_dae, dz_switch_alignment_engine);
}

INT avm_set_switch_dae(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_alignment_engine, buf);
}

extern U64 avm_sysfs_entry_get_set_switch_dae_mbr;
INT avm_get_switch_dae_mbr(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_switch_dae_mbr, dz_switch_alignment_engine_multi_block_read);
}

INT avm_set_switch_dae_mbr(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_alignment_engine_multi_block_read, buf);
}

extern U64 avm_sysfs_entry_get_set_switch_dae_mbw;
INT avm_get_switch_dae_mbw(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_switch_dae_mbw, dz_switch_alignment_engine_multi_block_write);
}

INT avm_set_switch_dae_mbw(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_alignment_engine_multi_block_write, buf);
}

extern U64 avm_sysfs_entry_get_set_switch_dae_pbr;
INT avm_get_switch_dae_pbr(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_switch_dae_pbr, dz_switch_alignment_engine_partial_block_read);
}

INT avm_set_switch_dae_pbr(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_alignment_engine_partial_block_read, buf);
}

extern U64 avm_sysfs_entry_get_set_switch_dae_pbw;
INT avm_get_switch_dae_pbw(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_switch_dae_pbw, dz_switch_alignment_engine_partial_block_write);
}

INT avm_set_switch_dae_pbw(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_alignment_engine_partial_block_write, buf);
}

extern U64 avm_sysfs_entry_get_set_tunable_dae_rthreads;
INT avm_get_tunable_dae_read_threads(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_tunable_dae_rthreads, num_align_read_threads);

}

INT avm_set_tunable_dae_read_threads(U64 buf)
{
	DZ_SYS_SET_SWITCH(avm_sysfs_entry_get_set_tunable_dae_rthreads, buf);
}

extern U64 avm_sysfs_entry_get_set_tunable_dae_wthreads;
INT avm_get_tunable_dae_write_threads(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_tunable_dae_wthreads, num_align_write_threads);

}

INT avm_set_tunable_dae_write_threads(U64 buf)
{
	DZ_SYS_SET_SWITCH(avm_sysfs_entry_get_set_tunable_dae_wthreads, buf);
}

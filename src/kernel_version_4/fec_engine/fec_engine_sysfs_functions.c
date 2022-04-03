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

extern INT dz_switch_fec_engine;
extern INT dz_switch_bec_engine;
extern INT dz_switch_dedupe_engine;
extern INT dz_switch_alignment_engine;

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

extern CHAR avm_sysfs_entry_get_fec_counters[DZ_SYSFS_PAGE_SIZE];
INT avm_get_fec_counters(CHAR *buf)
{
	INT count = 0;
	UINT rpending_iorequests;
	UINT rpending_threads;
	UINT rpending_jobs;

	UINT wpending_iorequests;
	UINT wpending_threads;
	UINT wpending_jobs;

	UINT fpending_iorequests;
	UINT fpending_threads;
	UINT fpending_jobs;

	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_fec_counters;
	memset(avm_sysfs_entry_get_fec_counters, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "==============================================\n");
	ptr += sprintf(ptr, "FEC ENGINE COUNTERS\n");
	ptr += sprintf(ptr, "==============================================\n");
	if (!superblock) {
		ptr += sprintf(ptr, "Superblock is NULL");
		return 0;
	}

	if (dz_switch_fec_engine == DZ_DISABLED) {
		ptr += sprintf(ptr, "FEC ENGINE DISABLED");
		return 0;
	}

	rpending_iorequests	=  atomic_read(&read_thread_pool->ioq_num_ios);
	rpending_threads	=  atomic_read(&read_thread_pool->num_active_threads);
	rpending_jobs		=  atomic_read(&read_thread_pool->num_jobs);

	wpending_iorequests	=  atomic_read(&write_thread_pool->ioq_num_ios);
	wpending_threads	=  atomic_read(&write_thread_pool->num_active_threads);
	wpending_jobs		=  atomic_read(&write_thread_pool->num_jobs);

	fpending_iorequests	=  atomic_read(&flush_thread_pool->ioq_num_ios);
	fpending_threads	=  atomic_read(&flush_thread_pool->num_active_threads);
	fpending_jobs		=  atomic_read(&flush_thread_pool->num_jobs);

	//Note: Value is NOT lock safe
	count = sprintf(ptr,
	"FEC Active Count Sequence....................= %lli\n"
	"FEC Free   Count Sequence....................= %lli\n"
	"FEC Active Count.............................= %lli\n"
	"FEC Flush Ops Count..........................= %lli\n"
	"FEC Current Flush Ops Count..................= %lli\n"
	"FEC Memory Reads Count.......................= %lli\n"
	"FEC Forwarded Reads Count....................= %lli\n"
	"FEC Write Start..............................= %lli\n"
	"FEC Write Switch.............................= %lli\n"
	"FEC Current Time.............................= %lli\n"
	"FEC Diff wrt Current Time(secs)..............= %lli\n"
	"FEC Diff wrt Final(secs).....................= %lli\n"
	"FEC Total Flush Count Pending................= %lli\n"
	"FEC Current Flush Count......................= %lli\n"
	"FEC Flush Prepare Start......................= %lli\n"
	"FEC Flush Start Time.........................= %lli\n"
	"FEC Flush End Time...........................= %lli\n"
	"FEC Read Thread Pool Name....................= %s\n"
	"FEC No. of Read Threads......................= %u\n"
	"FEC Pending Read IO Requests.................= %u\n"
	"FEC Pending Read IO Threads..................= %u\n"
	"FEC Pending Read Jobs........................= %u\n"
	"FEC Read IO quiescing........................= %d\n"
	"FEC Read IO queue Opened.....................= %d\n"
	"FEC Write Thread Pool Name...................= %s\n"
	"FEC No. of Write Threads.....................= %u\n"
	"FEC Pending Write IO Requests................= %u\n"
	"FEC Pending Write IO Threads.................= %u\n"
	"FEC Pending Write Jobs.......................= %u\n"
	"FEC Write IO quiescing.......................= %d\n"
	"FEC Write IO queue Opened....................= %d\n"
	"FEC Flush Thread Pool Name...................= %s\n"
	"FEC No. of Flush Threads.....................= %u\n"
	"FEC Pending Flush IO Requests................= %u\n"
	"FEC Pending Flush IO Threads.................= %u\n"
	"FEC Pending Flush Jobs.......................= %u\n"
	"FEC Flush IO quiescing.......................= %d\n"
	"FEC Flush IO queue Opened....................= %d\n"
	"FEC Current Time.............................= %lli\n"
	"FEC Diff wrt Current Time(secs)..............= %lli\n"
	"FEC Diff wrt Final(secs).....................= %lli\n"
	"FEC Application IO Count.....................= %lli\n"
	"FEC Application IO Count READS...............= %lli\n"
	"FEC Application IO Count READS Partial Page..= %lli\n"
	"FEC Application IO Count READS Single Page...= %lli\n"
	"FEC Application IO Count READS Multi Page....= %lli\n"
	"FEC Application IO Count WRITES..............= %lli\n"
	"FEC Application IO Count WRITES Partial Page.= %lli\n"
	"FEC Application IO Count WRITES Single Page..= %lli\n"
	"FEC Application IO Count WRITES Multi Page...= %lli\n"
	"FEC IO Count READS Partial Page..............= %lli\n"
	"FEC IO Count READS Single Page...............= %lli\n"
	"FEC IO Count READS Single Aligned Page.......= %lli\n"
	"FEC IO Count READS Single Unaligned Page.....= %lli\n"
	"FEC IO Count READS Multi Page................= %lli\n"
	"FEC IO Count READS Multi Aligned Page........= %lli\n"
	"FEC IO Count READS Multi Unaligned Page......= %lli\n"
	"FEC IO Count WRITES Partial Page.............= %lli\n"
	"FEC IO Count WRITES Single Page..............= %lli\n"
	"FEC IO Count WRITES Single Aligned Page......= %lli\n"
	"FEC IO Count WRITES Single Unaligned Page....= %lli\n"
	"FEC IO Count WRITES Multi Page...............= %lli\n"
	"FEC IO Count WRITES Multi Aligned Page.......= %lli\n"
	"FEC IO Count WRITES Multi Unaligned Page.....= %lli\n",
	//"Flush Completion Rate(psec) = %lli\n",
	(U64)READ_COUNTER(fec_active_count_sequence),
	(U64)READ_COUNTER(fec_free_count_sequence),
	fec_active_count,
	(U64)READ_COUNTER(fec_flush_ops_count),
	(U64)READ_COUNTER(fec_cur_flush_ops_count),
	(U64)READ_COUNTER(fec_memory_reads_count),
	(U64)READ_COUNTER(fec_forwarded_reads_count),
	time_fec_write_start,
	time_fec_write_switch,
	GET_CURRENT_TIME(),
	(U64)(GET_TIME_DIFF_IN_SECONDS(time_fec_write_start, GET_CURRENT_TIME())/1000),
	(U64)(GET_TIME_DIFF_IN_SECONDS(time_fec_write_start, time_fec_write_switch)/1000),
	(U64)fec_tables_flush->total_flush_cnt,
	(U64)READ_COUNTER(fec_cur_flush_count),
	(U64)time_flush_prepare_start,
	(U64)time_flush_start,
	(U64)time_flush_end,
	read_thread_pool->name,
	read_thread_pool->num_threads,
	rpending_iorequests,
	rpending_threads,
	rpending_jobs,
	read_thread_pool->quiesce,
	read_thread_pool->ioq_open,

	write_thread_pool->name,
	write_thread_pool->num_threads,
	wpending_iorequests,
	wpending_threads,
	wpending_jobs,
	write_thread_pool->quiesce,
	write_thread_pool->ioq_open,

	flush_thread_pool->name,
	flush_thread_pool->num_threads,
	fpending_iorequests,
	fpending_threads,
	fpending_jobs,
	flush_thread_pool->quiesce,
	flush_thread_pool->ioq_open,
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
	(U64)READ_COUNTER(fec_iocount_reads_partial_page),
	(U64)READ_COUNTER(fec_iocount_reads_single_page),
	(U64)READ_COUNTER(fec_iocount_reads_single_aligned_page),
	(U64)READ_COUNTER(fec_iocount_reads_single_unaligned_page),
	(U64)READ_COUNTER(fec_iocount_reads_multi_page),
	(U64)READ_COUNTER(fec_iocount_reads_multi_aligned_page),
	(U64)READ_COUNTER(fec_iocount_reads_multi_unaligned_page),
	(U64)READ_COUNTER(fec_iocount_writes_partial_page),
	(U64)READ_COUNTER(fec_iocount_writes_single_page),
	(U64)READ_COUNTER(fec_iocount_writes_single_aligned_page),
	(U64)READ_COUNTER(fec_iocount_writes_single_unaligned_page),
	(U64)READ_COUNTER(fec_iocount_writes_multi_page),
	(U64)READ_COUNTER(fec_iocount_writes_multi_aligned_page),
	(U64)READ_COUNTER(fec_iocount_writes_multi_unaligned_page)
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

extern U64 avm_sysfs_entry_get_set_switch_fec;
INT avm_get_switch_fec(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_switch_fec, dz_switch_fec_engine);
}

INT avm_set_switch_fec(U64  buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_fec_engine, buf);
}

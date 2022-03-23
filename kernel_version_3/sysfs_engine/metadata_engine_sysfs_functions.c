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

extern CHAR avm_sysfs_entry_get_superblock[DZ_SYSFS_PAGE_SIZE];
INT avm_get_superblock(CHAR *buf)
{
	INT count = 0;
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_superblock;
	memset(avm_sysfs_entry_get_superblock, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "===================================================================\n");
	ptr += sprintf(ptr, "SUPERBLOCK STRUCTURE\n");
	ptr += sprintf(ptr, "===================================================================\n");

	if (!superblock) {
		ptr += sprintf(ptr, "Superblock is NULL\n");
		return 0;
	}

	count = sprintf(ptr,
	"                     version = %s\n"
	"        metadata_dev_magicno = %s\n"
	"           metadata_dev_name = %s\n"
	"           metadata_dev_uuid = %s\n"
	"             metadata_offset = %llu\n"
	"            data_dev_magicno = %s\n"
	"               data_dev_name = %s\n"
	"               data_dev_uuid = %s\n"
	"              lba_table_size = %llu\n"
	"              lba_block_size = %d bytes\n"
	"     metadata_lba_table_size = %llu\n"
	"     metadata_lba_block_size = %d bytes\n"
	"metadata_lba_lock_table_size = %llu\n"
	"metadata_lba_lock_block_size = %d bytes\n"
	"              pba_table_size = %llu\n"
	"              pba_block_size = %d bytes\n"
	"             hash_table_size = %llu\n",
	superblock->version,
	superblock->metadata_dev_magicno,
	superblock->metadata_dev_name,
	superblock->metadata_dev_uuid,
	superblock->metadata_offset,
	superblock->data_dev_magicno,
	superblock->data_dev_name,
	superblock->data_dev_uuid,
	superblock->lba_table_size,
	superblock->lba_block_size,
	superblock->metadata_lba_table_size,
	superblock->metadata_lba_block_size,
	superblock->metadata_lba_lock_table_size,
	superblock->metadata_lba_lock_block_size,
	superblock->pba_table_size,
	superblock->pba_block_size,
	superblock->hash_table_size
	);

	return 0;
}

extern CHAR avm_sysfs_entry_get_metadata_counters[DZ_SYSFS_PAGE_SIZE];
INT avm_get_metadata_counters(CHAR *buf)
{
	INT count = 0;
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_metadata_counters;
	memset(avm_sysfs_entry_get_metadata_counters, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "===================================\n");
	ptr += sprintf(ptr, "METADATA ENGINE (MDE) COUNTERS\n");
	ptr += sprintf(ptr, "===================================\n");


	if (!superblock) {
		ptr += sprintf(ptr, "Superblock is NULL");
		return 0;
	}

	//Note: Value is NOT lock safe
	count = sprintf(ptr,
	"MDE No. of Threads................= %u\n"
	"MDE Thread Pool Name..............= %s\n"
	"MDE Flush Interval (In Seconds)...= %lu\n",
	metadata_flush_thread_pool->num_threads,
	metadata_flush_thread_pool->name,
	(ULONG)metadata_flush_interval
	);

	ptr += count;
	ptr += sprintf(ptr, "MDE LBA Table Size................= %llu ", metadata_lba_table_size);
	ptr += sprintf(ptr, "%llu GB ", (metadata_lba_table_size * METADATA_BLOCK_SIZE/GB));
	ptr += sprintf(ptr, "%llu MB ", (metadata_lba_table_size * METADATA_BLOCK_SIZE/MB));
	ptr += sprintf(ptr, "%llu KB ", (metadata_lba_table_size * METADATA_BLOCK_SIZE/KB));
	ptr += sprintf(ptr, "%llu bytes\n", metadata_lba_table_size * METADATA_BLOCK_SIZE);

	ptr += sprintf(ptr, "MDE LBA Lock Table Size...........= %llu ", metadata_lba_lock_table_size);
	ptr += sprintf(ptr, "%llu GB ", (metadata_lba_lock_table_size * sizeof(MD_LOCK)/GB));
	ptr += sprintf(ptr, "%llu MB ", (metadata_lba_lock_table_size * sizeof(MD_LOCK)/MB));
	ptr += sprintf(ptr, "%llu KB ", (metadata_lba_lock_table_size * sizeof(MD_LOCK)/KB));
	ptr += sprintf(ptr, "%llu bytes\n", metadata_lba_lock_table_size * sizeof(MD_LOCK));
	//LOGR("<------------ BEC Show ----------->\n");
	//LOGR("%s",buf);

	LOGD("Buffer Bytes used           = %d\n", count);
	//return count;
	return 0;
}

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

extern CHAR avm_sysfs_entry_get_dedupe_counters[DZ_SYSFS_PAGE_SIZE];
INT avm_get_dedupe_counters(CHAR *buf)
{
	INT count = 0;
	UINT pending_iorequests;
	UINT pending_threads;
	UINT pending_jobs;

	UINT pending_riorequests;
	UINT pending_rthreads;
	UINT pending_rjobs;

	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_dedupe_counters;
	memset(avm_sysfs_entry_get_dedupe_counters, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "==============================================\n");
	ptr += sprintf(ptr, "DEDUPE ENGINE COUNTERS\n");
	ptr += sprintf(ptr, "==============================================\n");
	if (!superblock) {
		ptr += sprintf(ptr, "Superblock is NULL");
		return 0;
	}

	if (dz_switch_dedupe_engine == DZ_DISABLED) {
		ptr += sprintf(ptr, "DEDUPE ENGINE DISABLED");
		return 0;
	}

	pending_iorequests 	=  atomic_read(&wdedupe_thread_pool->ioq_num_ios);
	pending_threads		=  atomic_read(&wdedupe_thread_pool->num_active_threads);
	pending_jobs		=  atomic_read(&wdedupe_thread_pool->num_jobs);

	pending_riorequests	=  atomic_read(&rdedupe_thread_pool->ioq_num_ios);
	pending_rthreads	=  atomic_read(&rdedupe_thread_pool->num_active_threads);
	pending_rjobs		=  atomic_read(&rdedupe_thread_pool->num_jobs);

	//Note: Value is NOT lock safe
	count = sprintf(ptr,
	"DEDUPE Total Logical  Space..................= %lli\n"
	"DEDUPE Total Physical Space..................= %lli\n"
	"DEDUPE Current Logical  Space................= %lli\n"
	"DEDUPE Current Physical Space................= %lli\n"
	"DEDUPE Current Duplicate Space...............= %lli\n"
	"DEDUPE Deleted Entries.......................= %lli\n"
	"DEDUPE Cache hits............................= %lli\n"
	"DEDUPE Disk Reads............................= %lli\n"
	"DEDUPE Disk Writes...........................= %lli\n"
	"DEDUPE Zeroed Out Page Reads.................= %lli\n"
	"DEDUPE No. of Read Threads...................= %u\n"
	"DEDUPE Read Thread Pool Name.................= %s\n"
	"DEDUPE No. of Write Threads..................= %u\n"
	"DEDUPE Write Thread Pool Name................= %s\n"
	"DEDUPE Pending READ IO Requests..............= %u\n"
	"DEDUPE Pending READ Threads..................= %u\n"
	"DEDUPE Pending READ Jobs.....................= %u\n"
	"DEDUPE Pending WRITE IO Requests.............= %u\n"
	"DEDUPE Pending WRITE Threads.................= %u\n"
	"DEDUPE Pending WRITE Jobs....................= %u\n"
	"DEDUPE WRITE IO quiescing....................= %d\n"
	"DEDUPE WRITE IO queue Opened.................= %d\n"
	"DEDUPE Current Time..........................= %lli\n",
	(U64)dedupe_tot_logical_space,
	(U64)dedupe_tot_physical_space,
	(U64)READ_COUNTER(dedupe_cur_logical_space),
	(U64)READ_COUNTER(dedupe_cur_physical_space),
	(U64)READ_COUNTER(dedupe_cur_duplicate_space),
	(U64)READ_COUNTER(dedupe_deleted_entries),
	(U64)READ_COUNTER(dedupe_cache_hits),
	(U64)READ_COUNTER(dedupe_disk_reads),
	(U64)READ_COUNTER(dedupe_disk_writes),
	(U64)READ_COUNTER(dedupe_zeroed_out_page_reads),
	rdedupe_thread_pool->num_threads,
	rdedupe_thread_pool->name,
	wdedupe_thread_pool->num_threads,
	wdedupe_thread_pool->name,
	pending_riorequests,
	pending_rthreads,
	pending_rjobs,
	pending_iorequests,
	pending_threads,
	pending_jobs,
	wdedupe_thread_pool->quiesce,
	wdedupe_thread_pool->ioq_open,
	(U64)(GET_CURRENT_TIME()));

	ptr += count;
	ptr += sprintf(ptr, "DEDUPE LBA Table Size........................= %llu (4K size blocks) ", lba_table_size);
	ptr += sprintf(ptr, "%llu GB ", (lba_table_size * LBA_BLOCK_SIZE/GB));
	ptr += sprintf(ptr, "%llu MB ", (lba_table_size * LBA_BLOCK_SIZE/MB));
	ptr += sprintf(ptr, "%llu KB ", (lba_table_size * LBA_BLOCK_SIZE/KB));
	ptr += sprintf(ptr, "%llu bytes\n", lba_table_size * LBA_BLOCK_SIZE);

	ptr += sprintf(ptr, "LBA Bitmap: lba_table_size...................= %lli\n", lba_table_size);
	ptr += sprintf(ptr, "LBA Bitmap: number_of_longs_for_lba..........= %u\n", number_of_longs_for_lba);
	ptr += sprintf(ptr, "LBA Bitmap: Bitmap Size......................= %lu\n", number_of_longs_for_lba * sizeof(long));


	ptr += sprintf(ptr, "DEDUPE HASH Table Size.......................= %llu (4K size blocks) ", hash_table_size);
	ptr += sprintf(ptr, "%llu GB ", (hash_table_size * HBA_BLOCK_SIZE/GB));
	ptr += sprintf(ptr, "%llu MB ", (hash_table_size * HBA_BLOCK_SIZE/MB));
	ptr += sprintf(ptr, "%llu KB ", (hash_table_size * HBA_BLOCK_SIZE/KB));
	ptr += sprintf(ptr, "%llu bytes\n", hash_table_size * HBA_BLOCK_SIZE);

	ptr += sprintf(ptr, "DEDUPE PBA Table Size........................= %llu (4K size blocks) ", pba_table_size);
	ptr += sprintf(ptr, "%llu GB ", (pba_table_size * PBA_BLOCK_SIZE/GB));
	ptr += sprintf(ptr, "%llu MB ", (pba_table_size * PBA_BLOCK_SIZE/MB));
	ptr += sprintf(ptr, "%llu KB ", (pba_table_size * PBA_BLOCK_SIZE/KB));
	ptr += sprintf(ptr, "%llu bytes\n", pba_table_size * PBA_BLOCK_SIZE);
	ptr += sprintf(ptr, "PBA Bitmap: pba_table_size...................= %lli\n", pba_table_size);
	ptr += sprintf(ptr, "PBA Bitmap: number_of_longs_for_pba..........= %u\n", number_of_longs_for_pba);
	ptr += sprintf(ptr, "PBA Bitmap: Bitmap Size......................= %lu\n", number_of_longs_for_pba * sizeof(long));
    ptr += sprintf(ptr, "Garbage PBA Bitmap: pba_table_size...........= %lli\n", pba_table_size);
    ptr += sprintf(ptr, "Garbage PBA Bitmap: number_of_longs_for_gpba.= %u\n", number_of_longs_for_gpba);
    ptr += sprintf(ptr, "Garbage PBA Bitmap: Bitmap Size..............= %lu\n", number_of_longs_for_gpba * sizeof(long));
	//LOGD("Buffer Bytes used           = %d\n", count);
	return 0;
}

extern U64 avm_sysfs_entry_get_pba_table_size;
INT avm_pba_table_size_show(CHAR *buf)
{
	avm_sysfs_entry_get_pba_table_size = pba_table_size;
	return 0;
}

extern U64 avm_sysfs_entry_get_hash_table_size;
INT avm_get_hash_table_size(CHAR *buf)
{
	avm_sysfs_entry_get_hash_table_size = hash_table_size;
	return 0;
}

extern U64 avm_sysfs_entry_get_set_switch_dde;
INT avm_get_switch_dde(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_switch_dde, dz_switch_dedupe_engine);
}

INT avm_set_switch_dde(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_dedupe_engine, buf);
}

extern PPBA_BLOCK	pba_table;
extern PHASH_BLOCK	hash_table;
extern PLBA_BLOCK	lba_table;
extern U64 avm_sysfs_entry_get_set_lba_entry;
extern U64 avm_sysfs_entry_get_set_pba_entry;
extern BITMAP 			pba_bitmap;
INT avm_get_lba_entry(CHAR *buf)
{
	UINT i = 0;
	PLBA_BLOCK lba_entry       		= NULL;
	PPBA_BLOCK pba_entry       		= NULL;
	PHASH_BLOCK hash_entry     		= NULL;
	U64 total_pba_entries_allocated = 0;

	if (avm_sysfs_entry_get_set_lba_entry > lba_table_size) {
		LOGE("Index is greater than lba table size\n");
		return 0;
	}

	for_each_set_bit(i, pba_bitmap.bitmap, (pba_bitmap.num_of_longs) * sizeof(long)) {
		total_pba_entries_allocated++;
	}
	LOG("Total PBA Entries Allocated %llu\n", total_pba_entries_allocated);
	printk(KERN_ERR"\n");
	lba_entry = GET_LBA_ENTRY(avm_sysfs_entry_get_set_lba_entry);
	LOG("                                 lba = %llu\n", avm_sysfs_entry_get_set_lba_entry);
	LOG("               lba_entry->hash_index = %u\n", lba_entry->hash_index);
	LOG("     lba_entry->hash_collision_tag   = %llu\n", (U64)lba_entry->hash_collision_tag);
	printk(KERN_ERR"\n");

	hash_entry = GET_HASH_ENTRY(lba_entry->hash_index);
	LOG("                                hash = %llu\n", (U64)lba_entry->hash_index);
	LOG("               hash_entry->pba_index = %lu\n", (ULONG)hash_entry->pba_index);
	printk(KERN_ERR"\n");

	pba_entry  = GET_PBA_ENTRY(hash_entry->pba_index);
	LOG("                           pba_index = %lu\n", (ULONG)hash_entry->pba_index);
	LOG("     pba_entry->hash_collision_tag   = %llu\n", (U64)pba_entry->hash_collision_tag);
	LOG("                     pba_entry->size = %llu\n", (U64)pba_entry->size);
	LOG("                  pba_entry->ref_cnt = %llu\n", (U64)pba_entry->ref_cnt);
	LOG("pba_entry->next_hash_collision_entry = %llu\n", (U64)pba_entry->next_entry_in_collision_list);
	printk(KERN_ERR"\n");
	return 0;
}

extern CHAR avm_sysfs_entry_get_set_lba_table_index[DZ_SYSFS_PAGE_SIZE];
INT avm_get_set_lba_table_index(CHAR *buf)
{
	ssize_t ret;
	ULONG lba_index = 0;
	UINT i = 0;
	PLBA_BLOCK lba_entry 			= NULL;
	PPBA_BLOCK pba_entry 			= NULL;
	PHASH_BLOCK hash_entry          = NULL;
	PMDD_BLOCK  metadata_entry 		= NULL;
	U64 total_pba_entries_allocated = 0;
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_set_lba_table_index;
	ret = kstrtoul(ptr, 10, &lba_index);
	if (ret) {
		memset(avm_sysfs_entry_get_set_lba_table_index, 0, DZ_SYSFS_PAGE_SIZE);
		memcpy(ptr, "\n", 1);
		ptr += 1;
		ptr += sprintf(ptr, "=================================\n");
		if (ret == -EINVAL) {
			ptr += sprintf(ptr, "Unable to convert to index. Invalid Index\n");
		} else {
			ptr += sprintf(ptr, "Unable to convert to index. ret = %zd\n", ret) ;
		}
		ptr += sprintf(ptr, "=================================\n");
		return ret;
	}

	memset(avm_sysfs_entry_get_set_lba_table_index, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "========================================\n");
	ptr += sprintf(ptr, "LBA Table Entry for Index %lu\n", lba_index);
	ptr += sprintf(ptr, "========================================\n");
	ptr += sprintf(ptr, "LBA Table Entry for Index %lu\n", lba_index);

	if (lba_index > lba_table_size) {
		ptr += sprintf(ptr, "Index %lu is greater than lba table size %llu\n",
					lba_index, lba_table_size);
		return 0;
	}
	for_each_set_bit(i, pba_bitmap.bitmap, (pba_bitmap.num_of_longs) * sizeof(long)) {
		total_pba_entries_allocated++;
	}
	LOG("Total PBA Entries Allocated %llu\n", total_pba_entries_allocated);
	lba_entry = GET_LBA_ENTRY(lba_index);
	ptr += sprintf(ptr, "                                 lba   = %lu\n", lba_index);
	ptr += sprintf(ptr, "               lba_entry->hash_index   = %u\n", lba_entry->hash_index);
	ptr += sprintf(ptr, "     lba_entry->hash_collision_tag     = %llu\n", (U64)lba_entry->hash_collision_tag);

	ptr += sprintf(ptr, "========================================\n");
	ptr += sprintf(ptr, "Hash Table Entry for LBA Index %lu\n", lba_index);
	ptr += sprintf(ptr, "========================================\n");
	hash_entry = GET_HASH_ENTRY(lba_entry->hash_index);
	ptr += sprintf(ptr, "                                hash   = %llu\n", (U64)lba_entry->hash_index);
	ptr += sprintf(ptr, "               hash_entry->pba_index   = %lu\n", (ULONG)hash_entry->pba_index);

	ptr += sprintf(ptr, "========================================\n");
	ptr += sprintf(ptr, "PBA Table Entry for LBA Index %lu\n", lba_index);
	ptr += sprintf(ptr, "========================================\n");
	pba_entry  = GET_PBA_ENTRY(hash_entry->pba_index);
	ptr += sprintf(ptr, "                           pba_index   = %lu\n", (ULONG)hash_entry->pba_index);
	ptr += sprintf(ptr, "     pba_entry->hash_collision_tag     = %llu\n", (U64)pba_entry->hash_collision_tag);
	ptr += sprintf(ptr, "                     pba_entry->size   = %llu\n", (U64)pba_entry->size);
	ptr += sprintf(ptr, "                  pba_entry->ref_cnt   = %llu\n", (U64)pba_entry->ref_cnt);
	ptr += sprintf(ptr, "pba_entry->next_hash_collision_entry   = %llu\n", (U64)pba_entry->next_entry_in_collision_list);

	ptr += sprintf(ptr, "========================================\n");
	ptr += sprintf(ptr, "Metadata Table Entry for LBA Index %lu\n", lba_index);
	ptr += sprintf(ptr, "========================================\n");
	metadata_entry  = GET_MDB_ENTRY(lba_index);
	ptr += sprintf(ptr, "             metadata_entry->mdd_lba   = %llu\n", (U64)metadata_entry->mdd_lba);
	ptr += sprintf(ptr, "            metadata_entry->mdd_hash   = %llx %llx %llx %llx \n",
			(U64)metadata_entry->mdd_hash.hash32[0], (U64)metadata_entry->mdd_hash.hash32[1],
			(U64)metadata_entry->mdd_hash.hash32[2], (U64)metadata_entry->mdd_hash.hash32[3]);
	ptr += sprintf(ptr, "      metadata_entry->mdd_hash_index   = %u\n", (U32)metadata_entry->mdd_hash_index);
	ptr += sprintf(ptr, "         metadata_entry->mdd_pba_index = %llu\n", (U64)metadata_entry->mdd_pba_index);
	ptr += sprintf(ptr, "         metadata_entry->mdd_dd_sector = %llu\n", (U64)metadata_entry->mdd_dd_sector);
	ptr += sprintf(ptr, "             metadata_entry->mdd_dirty = %d\n",  (INT)metadata_entry->mdd_dirty);
	return 0;
}

INT avm_get_pba_entry(CHAR *buf)
{
	PPBA_BLOCK pba_entry = NULL;

	if (avm_sysfs_entry_get_set_pba_entry > pba_table_size) {
		LOGE("Index is greater than lba table size\n");
		return 0;
	}

	printk(KERN_ERR"\n");
	pba_entry  = GET_PBA_ENTRY(avm_sysfs_entry_get_set_pba_entry);
	LOG("                           pba_index = %lu\n", (ULONG)avm_sysfs_entry_get_set_pba_entry);
	LOG("     pba_entry->hash_collision_tag   = %llu\n", (U64)pba_entry->hash_collision_tag);
	LOG("                     pba_entry->size = %llu\n", (U64)pba_entry->size);
	LOG("                  pba_entry->ref_cnt = %llu\n", (U64)pba_entry->ref_cnt);
	LOG("pba_entry->next_hash_collision_entry = %llu\n", (U64)pba_entry->next_entry_in_collision_list);
	printk(KERN_ERR"\n");
	return 0;
}

extern CHAR avm_sysfs_entry_get_set_pba_table_index[DZ_SYSFS_PAGE_SIZE];
INT avm_get_set_pba_table_index(CHAR *buf)
{
	ssize_t ret;
	ULONG pba_index = 0;
	PPBA_BLOCK pba_entry = NULL;
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_set_pba_table_index;
	ret = kstrtoul(ptr, 10, &pba_index);
	if (ret) {
		memset(avm_sysfs_entry_get_set_pba_table_index, 0, DZ_SYSFS_PAGE_SIZE);
		memcpy(ptr, "\n", 1);
		ptr += 1;
		ptr += sprintf(ptr, "=================================\n");
		if (ret == -EINVAL) {
			ptr += sprintf(ptr, "Unable to convert to index. Invalid Index\n");
		} else {
			ptr += sprintf(ptr, "Unable to convert to index. ret = %zd\n", ret) ;
		}
		ptr += sprintf(ptr, "=================================\n");
		return ret;
	}
	memset(avm_sysfs_entry_get_set_pba_table_index, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "=================================\n");
	ptr += sprintf(ptr, "PBA Table Entry for Index %lu\n", pba_index);
	ptr += sprintf(ptr, "=================================\n");

	if (pba_index > pba_table_size) {
		LOGE("Index is greater than lba table size\n");
		ptr += sprintf(ptr, "Index %lu is greater than pba table size %llu\n",
					pba_index, pba_table_size);
		return 0;
	}

	pba_entry  = GET_PBA_ENTRY(pba_index);
	ptr += sprintf(ptr, "                           pba_index = %lu\n", (ULONG)pba_index);
	ptr += sprintf(ptr, "     pba_entry->hash_collision_tag   = %llu\n", (U64)pba_entry->hash_collision_tag);
	ptr += sprintf(ptr, "                     pba_entry->size = %llu\n", (U64)pba_entry->size);
	ptr += sprintf(ptr, "                  pba_entry->ref_cnt = %llu\n", (U64)pba_entry->ref_cnt);
	ptr += sprintf(ptr, "pba_entry->next_hash_collision_entry = %llu\n", (U64)pba_entry->next_entry_in_collision_list);
	return 0;
}

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

//Dedupe Mempool Functions
INT sys_dedupe_show(CHAR *buf)
{
	INT count = 0;
	//Note: Value is NOT lock safe
	count = sprintf(buf,
	"Total Logical  Space        = %lli\n"
	"Total Physical Space        = %lli\n"
	"Current Logical  Space      = %lli\n"
	"Current Physical Space      = %lli\n"
	"Current Duplicate Space     = %lli\n"
	"Deleted Entries             = %lli\n"
	"Cache hits                  = %lli\n"
	"Disk Reads                  = %lli\n"
	"Disk Writes                 = %lli\n"
	"Current Time                = %lli\n",
	(U64)dedupe_tot_logical_space,
	(U64)dedupe_tot_physical_space,
	(U64)ATOMIC64_READ(dedupe_cur_logical_space),
	(U64)ATOMIC64_READ(dedupe_cur_physical_space),
	(U64)ATOMIC64_READ(dedupe_cur_duplicate_space),
	(U64)ATOMIC64_READ(dedupe_deleted_entries),
	(U64)ATOMIC64_READ(dedupe_cache_hits),
	(U64)ATOMIC64_READ(dedupe_disk_reads),
	(U64)ATOMIC64_READ(dedupe_disk_writes),
	(U64)(GET_CURRENT_TIME()));

	LOGR("<------------ Dedupe Show ----------->\n");
	LOGR("%s",buf);

	LOGD("Buffer Bytes used           = %d\n", count);
	return count;

}

extern CHAR avm_sysfs_entry_get_table_size[DZ_SYSFS_PAGE_SIZE];
INT avm_get_table_size(CHAR *buf)
{
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_table_size;
	memset(avm_sysfs_entry_get_table_size, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "================================\n");
	ptr += sprintf(ptr, "InMemory Tables Size Information\n");
	ptr += sprintf(ptr, "================================\n");
	ptr += sprintf(ptr, "LBA Table Size............= %llu (%lu size blocks) ", lba_table_size, LBA_BLOCK_SIZE);
	ptr += sprintf(ptr, "%llu GB ", (lba_table_size * LBA_BLOCK_SIZE/GB));
	ptr += sprintf(ptr, "%llu MB ", (lba_table_size * LBA_BLOCK_SIZE/MB));
	ptr += sprintf(ptr, "%llu KB ", (lba_table_size * LBA_BLOCK_SIZE/KB));
	ptr += sprintf(ptr, "%llu bytes\n", lba_table_size * LBA_BLOCK_SIZE);

	ptr += sprintf(ptr, "HASH Table Size...........= %llu (%lu size blocks) ", hash_table_size, HBA_BLOCK_SIZE);
	ptr += sprintf(ptr, "%llu GB ", (hash_table_size * HBA_BLOCK_SIZE/GB));
	ptr += sprintf(ptr, "%llu MB ", (hash_table_size * HBA_BLOCK_SIZE/MB));
	ptr += sprintf(ptr, "%llu KB ", (hash_table_size * HBA_BLOCK_SIZE/KB));
	ptr += sprintf(ptr, "%llu bytes\n", hash_table_size * HBA_BLOCK_SIZE);

	ptr += sprintf(ptr, "PBA Table Size............= %llu (%lu size blocks) ", pba_table_size, PBA_BLOCK_SIZE);
	ptr += sprintf(ptr, "%llu GB ", (pba_table_size * PBA_BLOCK_SIZE/GB));
	ptr += sprintf(ptr, "%llu MB ", (pba_table_size * PBA_BLOCK_SIZE/MB));
	ptr += sprintf(ptr, "%llu KB ", (pba_table_size * PBA_BLOCK_SIZE/KB));
	ptr += sprintf(ptr, "%llu bytes\n", pba_table_size * PBA_BLOCK_SIZE);

	ptr += sprintf(ptr, "MDE LBA Table Size........= %llu ", metadata_lba_table_size);
	ptr += sprintf(ptr, "%llu GB ", (metadata_lba_table_size * METADATA_BLOCK_SIZE/GB));
	ptr += sprintf(ptr, "%llu MB ", (metadata_lba_table_size * METADATA_BLOCK_SIZE/MB));
	ptr += sprintf(ptr, "%llu KB ", (metadata_lba_table_size * METADATA_BLOCK_SIZE/KB));
	ptr += sprintf(ptr, "%llu bytes\n", metadata_lba_table_size * METADATA_BLOCK_SIZE);

	ptr += sprintf(ptr, "MDE LBA Lock Table Size...= %llu ", metadata_lba_lock_table_size);
	ptr += sprintf(ptr, "%llu GB ", (metadata_lba_lock_table_size * sizeof(MD_LOCK)/GB));
	ptr += sprintf(ptr, "%llu MB ", (metadata_lba_lock_table_size * sizeof(MD_LOCK)/MB));
	ptr += sprintf(ptr, "%llu KB ", (metadata_lba_lock_table_size * sizeof(MD_LOCK)/KB));
	ptr += sprintf(ptr, "%llu bytes\n", metadata_lba_lock_table_size * sizeof(MD_LOCK));

	ptr += sprintf(ptr, "TAGET_DEVICE_SIZE.........= %zu (%lu size blocks) ", TARGET_DEVICE_SIZE, LBA_BLOCK_SIZE);
	ptr += sprintf(ptr, "%zu GB ", (TARGET_DEVICE_SIZE * LBA_BLOCK_SIZE/GB));
	ptr += sprintf(ptr, "%zu MB ", (TARGET_DEVICE_SIZE * LBA_BLOCK_SIZE/MB));
	ptr += sprintf(ptr, "%zu KB ", (TARGET_DEVICE_SIZE * LBA_BLOCK_SIZE/KB));
	ptr += sprintf(ptr, "%zu bytes\n", TARGET_DEVICE_SIZE * LBA_BLOCK_SIZE);
	return 0;
}

extern PPBA_BLOCK	pba_table;
extern PHASH_BLOCK	hash_table;
extern PLBA_BLOCK	lba_table;
extern BITMAP 			pba_bitmap;

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

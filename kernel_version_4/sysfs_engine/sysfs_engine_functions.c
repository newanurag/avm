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

extern CHAR avm_sysfs_entry_get_sizeof_datastructures[DZ_SYSFS_PAGE_SIZE];
INT avm_get_sizeof_datastructures(CHAR *buf)
{
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_sizeof_datastructures;
	memset(avm_sysfs_entry_get_sizeof_datastructures, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;

	ptr += sprintf(ptr, "\n====================================\n");
	ptr += sprintf(ptr, "%s(): Basic Data Structures", __func__);
	ptr += sprintf(ptr, "\n====================================\n");
	ptr += sprintf(ptr, "Size of int......................= %zu\n", sizeof(int));
	ptr += sprintf(ptr, "Size of long int.................= %zu\n", sizeof( long int));
	ptr += sprintf(ptr, "Size of long unsigned int........= %zu\n", sizeof( long unsigned int));
	ptr += sprintf(ptr, "Size of Long.....................= %zu\n", sizeof(long));
	ptr += sprintf(ptr, "Size of U64......................= %zu\n", sizeof(U64));
	ptr += sprintf(ptr, "Size of unsigned long............= %zu\n", sizeof(unsigned long));
	ptr += sprintf(ptr, "Size of unsigned long long.......= %zu\n", sizeof(unsigned long long));
	ptr += sprintf(ptr, "Size of signed long..............= %zu\n", sizeof(signed long));
	ptr += sprintf(ptr, "Size of signed long long.........= %zu\n", sizeof(signed long long));
	ptr += sprintf(ptr, "Size of long long unsigned int...= %zu\n", sizeof(long long unsigned int));
	ptr += sprintf(ptr, "Size of long long signed int.....= %zu\n", sizeof(long long signed int));

	ptr += sprintf(ptr, "\n====================================\n");
	ptr += sprintf(ptr, "%s(): MACROs Value ", __func__);
	ptr += sprintf(ptr, "\n====================================\n");
	ptr += sprintf(ptr, "DATE_LENGTH.....= %d\n", DATE_LENGTH );
	ptr += sprintf(ptr, "TIME_LENGTH.....= %d\n", TIME_LENGTH );
	ptr += sprintf(ptr, "NAME_LENGTH.....= %d\n", NAME_LENGTH );
	ptr += sprintf(ptr, "UUID_LENGTH.....= %d\n", UUID_LENGTH );
	ptr += sprintf(ptr, "GUID_LENGTH.....= %d\n", GUID_LENGTH );
	ptr += sprintf(ptr, "MAGICNO_LENGTH..= %d\n", MAGICNO_LENGTH );
	ptr += sprintf(ptr, "BDEVNAME_SIZE...= %d\n", BDEVNAME_SIZE );
	ptr += sprintf(ptr, "METADATA_BLOCK_SIZE...............= %d\n", METADATA_BLOCK_SIZE );
	ptr += sprintf(ptr, "PBA_HASH_COLLISION_TAG_START......= %d\n", PBA_HASH_COLLISION_TAG_START );
	ptr += sprintf(ptr, "DZ_FEC_CONCURRENT_READS_LIMIT.....= %d\n", DZ_FEC_CONCURRENT_READS_LIMIT );
	ptr += sprintf(ptr, "DZ_FEC_FLUSH_INTERVAL.............= %d\n", DZ_FEC_FLUSH_INTERVAL );
	ptr += sprintf(ptr, "SUPERBLOCK_RESERVED_SECTORS.......= %d\n", SUPERBLOCK_RESERVED_SECTORS );
	ptr += sprintf(ptr, "SUPERBLOCK_SIZE...................= %lu\n", SUPERBLOCK_SIZE );
	ptr += sprintf(ptr, "DZ_NUMBER_OF_WRITE_PAGES_IN_A_BIO.= %d\n", DZ_NUMBER_OF_WRITE_PAGES_IN_A_BIO );
	ptr += sprintf(ptr, "DZ_NUMBER_OF_READ_PAGES_IN_A_BIO..= %d\n", DZ_NUMBER_OF_READ_PAGES_IN_A_BIO );

	ptr += sprintf(ptr, "\n============================================\n");
	ptr += sprintf(ptr, "%s(): Kernel Data Structures", __func__);
	ptr += sprintf(ptr, "\n============================================\n");
	ptr += sprintf(ptr, "Size of struct bio...............= %zu bytes\n", sizeof(struct bio));
	ptr += sprintf(ptr, "Size of struct bio_vec...........= %zu bytes\n", sizeof(struct bio_vec));
	ptr += sprintf(ptr, "Size of struct bio_set...........= %zu bytes\n", sizeof(struct bio_set));
	ptr += sprintf(ptr, "Size of struct block_device......= %zu bytes\n", sizeof(struct block_device));
	ptr += sprintf(ptr, "Size of struct list_head.........= %zu bytes\n", sizeof(struct list_head));
	ptr += sprintf(ptr, "Size of struct dm_dev............= %zu bytes\n", sizeof(struct dm_dev));
	ptr += sprintf(ptr, "Size of struct dm_target.........= %zu bytes\n", sizeof(struct dm_target));
	ptr += sprintf(ptr, "Size of struct page..............= %zu bytes\n", sizeof(struct page));
	ptr += sprintf(ptr, "Size of struct completion........= %zu bytes\n", sizeof(struct completion));
	ptr += sprintf(ptr, "Size of struct radix_tree_root...= %zu bytes\n", sizeof(struct radix_tree_root));

	ptr += sprintf(ptr, "\n===================================================\n");
	ptr += sprintf(ptr, "%s(): ZAIDSTOR AVM Data Structures", __func__);
	ptr += sprintf(ptr, "\n===================================================\n");
	ptr += sprintf(ptr, "Size of struct dz_bitmap_s.............= %zu bytes\n", sizeof(struct dz_bitmap_s));
	ptr += sprintf(ptr, "Size of struct dz_target_object_s......= %zu bytes\n", sizeof(struct dz_target_object_s));
	ptr += sprintf(ptr, "Size of struct dz_target_device_s......= %zu bytes\n", sizeof(struct dz_target_device_s));
	ptr += sprintf(ptr, "Size of struct dz_dedupe_volume_s......= %zu bytes\n", sizeof(struct dz_dedupe_volume_s));
	ptr += sprintf(ptr, "Size of struct dz_object_list_s........= %zu bytes\n", sizeof(struct dz_object_list_s));
	ptr += sprintf(ptr, "Size of struct dz_bvec_holder_s........= %zu bytes\n", sizeof(struct dz_bvec_holder_s));
	ptr += sprintf(ptr, "Size of struct dz_iorequest_s..........= %zu bytes\n", sizeof(struct dz_iorequest_s));
	ptr += sprintf(ptr, "Size of struct dz_dedupe_info_s........= %zu bytes\n", sizeof(struct dz_dedupe_info_s));
	ptr += sprintf(ptr, "Size of struct dz_data_pages_s.........= %zu bytes\n", sizeof(struct dz_data_pages_s));
	ptr += sprintf(ptr, "Size of union  dz_lba_block_s..........= %zu bytes\n", sizeof(union dz_lba_block_s));
	ptr += sprintf(ptr, "Size of union  dz_hash_block_s.........= %zu bytes\n", sizeof(union dz_hash_block_s));
	ptr += sprintf(ptr, "Size of union  dz_pba_block_s..........= %zu bytes\n", sizeof(union dz_pba_block_s));
	ptr += sprintf(ptr, "Size of union  dz_fec_data_write_s.....= %zu bytes\n", sizeof(union dz_fec_data_write_s));
	ptr += sprintf(ptr, "Size of struct dz_fec_table_s..........= %zu bytes\n", sizeof(struct dz_fec_table_s));
	ptr += sprintf(ptr, "Size of struct dz_completion_s.........= %zu bytes\n", sizeof(struct dz_completion_s));
	ptr += sprintf(ptr, "Size of struct dz_ddvol_lba_block_s....= %zu bytes\n", sizeof(struct dz_ddvol_lba_block_s));
	ptr += sprintf(ptr, "Size of struct dz_bec_request_s........= %zu bytes\n", sizeof(struct dz_bec_request_s));
	ptr += sprintf(ptr, "Size of struct dz_bec_node_s...........= %zu bytes\n", sizeof(struct dz_bec_node_s));
	ptr += sprintf(ptr, "Size of struct dz_hash_s...............= %zu bytes\n", sizeof(struct dz_hash_s));
	ptr += sprintf(ptr, "Size of struct dz_superblock_s.........= %zu bytes\n", sizeof(struct dz_superblock_s));
	ptr += sprintf(ptr, "Size of struct dz_job_s................= %zu bytes\n", sizeof(struct dz_job_s));
	ptr += sprintf(ptr, "Size of struct dz_thread_s.............= %zu bytes\n", sizeof(struct dz_thread_s));
	ptr += sprintf(ptr, "Size of struct dz_thread_pool_s........= %zu bytes\n", sizeof(struct dz_thread_pool_s));
	ptr += sprintf(ptr, "Size of struct dz_metadata_lock_s......= %zu bytes\n", sizeof(struct dz_metadata_lock_s));
	ptr += sprintf(ptr, "Size of struct dz_metadata_disk_block_s= %zu bytes\n", sizeof(struct dz_metadata_disk_block_s));
	ptr += sprintf(ptr, "\n===================================================\n");
	ptr += sprintf(ptr, "    \n");

	return 0;
}

extern CHAR avm_sysfs_entry_get_all_counters[DZ_SYSFS_PAGE_SIZE];
INT avm_get_all_counters(CHAR *buf)
{
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_all_counters;
	memset(avm_sysfs_entry_get_all_counters, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "ALL ENGINE COUNTERS\n");
	ptr += sprintf(ptr, "BEC ENGINE COUNTERS\n");
	ptr += sprintf(ptr, "bec_pba_page_count     = %lli\n", (U64)READ_COUNTER(bec_pba_page_count));
	ptr += sprintf(ptr, "bec_cache_hit          = %lli\n", (U64)ATOMIC64_READ(bec_cache_hit));
	ptr += sprintf(ptr, "bec_cache_miss         = %lli\n", (U64)ATOMIC64_READ(bec_cache_miss));
	ptr += sprintf(ptr, "bec_cache_delete       = %lli\n", (U64)ATOMIC64_READ(bec_cache_delete));
	ptr += sprintf(ptr, "bec_size               = %lli\n", (U64)bec_size);
	ptr += sprintf(ptr, "bec_cutoff_nodes       = %lli\n", (U64)bec_cutoff_nodes);
	ptr += sprintf(ptr, "bec_cutoff_percent     = %lli\n", (U64)bec_cutoff_percent);
	ptr += sprintf(ptr, "bec_node_mempool_count = %lli\n", (U64)dz_bec_node_mempool_count);
	ptr += sprintf(ptr, "AISA ZaidStor Minor   = %d\n", DZ_AISA_VERSION_MINOR);
	ptr += sprintf(ptr, "AISA ZaidStor Patch   = %d\n", DZ_AISA_VERSION_PATCH);
	ptr += sprintf(ptr, "AISA ZaidStor Release = %s\n", DZ_AISA_VERSION_RELEASE);
	ptr += sprintf(ptr, "LINUX_VERSION_CODE    = %d\n", LINUX_VERSION_CODE);
	return 0;
}

extern CHAR avm_sysfs_entry_get_memory_info[DZ_SYSFS_PAGE_SIZE];
//extern U64 fec_engine_memory_usage;
extern U64 bec_engine_memory_usage;
extern U64 dedupe_engine_memory_usage;
extern U64 infra_engine_memory_usage;
extern U64 md_engine_memory_usage;
INT avm_get_memory_info(CHAR *buf)
{
	U64 count = 0;
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_memory_info;
	memset(avm_sysfs_entry_get_memory_info, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "======================================================\n");
	ptr += sprintf(ptr, "AVM Memory Consumption in Kernel during Initialization\n");
	ptr += sprintf(ptr, "======================================================\n");
	count =		BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools);
	ptr += sprintf(ptr, "Total Memory Consumed................................= %llu MB %llu KB %llu (bytes)\n",
			count/MB,
			count/KB,
			count);
	ptr += sprintf(ptr, " |_ Total Memory Consumed by Infra Engine............= %llu MB %llu KB %llu (bytes)\n",
			BASE_COUNTER_U64_GET(infra_engine_memory_usage)/MB,
			BASE_COUNTER_U64_GET(infra_engine_memory_usage)/KB,
			BASE_COUNTER_U64_GET(infra_engine_memory_usage));
	ptr += sprintf(ptr, " |_ Total Memory Consumed by Dedupe Engine...........= %llu MB %llu KB %llu (bytes)\n",
			DDE_COUNTER_U64_GET(dedupe_engine_memory_usage)/MB,
			DDE_COUNTER_U64_GET(dedupe_engine_memory_usage)/KB,
			DDE_COUNTER_U64_GET(dedupe_engine_memory_usage));
	ptr += sprintf(ptr, " |_ Total Memory Consumed by FEC Engine..............= %llu MB %llu KB %llu (bytes)\n",
			FEC_COUNTER_U64_GET(fec_engine_memory_usage)/MB,
			FEC_COUNTER_U64_GET(fec_engine_memory_usage)/KB,
			FEC_COUNTER_U64_GET(fec_engine_memory_usage));
	ptr += sprintf(ptr, " |_ Total Memory Consumed by BEC Engine..............= %llu MB %llu KB %llu (bytes)\n",
			BEC_COUNTER_U64_GET(bec_engine_memory_usage)/MB,
			BEC_COUNTER_U64_GET(bec_engine_memory_usage)/KB,
			BEC_COUNTER_U64_GET(bec_engine_memory_usage));
	ptr += sprintf(ptr, " |_ Total Memory Consumed by MD Engine...............= %llu MB %llu KB %llu (bytes)\n",
			BASE_COUNTER_U64_GET(md_engine_memory_usage)/MB,
			BASE_COUNTER_U64_GET(md_engine_memory_usage)/KB,
			BASE_COUNTER_U64_GET(md_engine_memory_usage));

	ptr += sprintf(ptr, "dz_iorequest_alloc_cnt...............................= %ld\n",
			BASE_COUNTER_ATOMIC64_READ(dz_iorequest_alloc_cnt));

	ptr += sprintf(ptr, "dz_iorequest_free_cnt................................= %ld\n",
			BASE_COUNTER_ATOMIC64_READ(dz_iorequest_free_cnt));

	ptr += sprintf(ptr, "dz_iorequest_alloc_cnt - dz_iorequest_free_cnt.......= %ld\n", 
			(BASE_COUNTER_ATOMIC64_READ(dz_iorequest_alloc_cnt) - BASE_COUNTER_ATOMIC64_READ(dz_iorequest_free_cnt)));

	ptr += sprintf(ptr, "dz_page_alloc_cnt....................................= %ld\n",
			BASE_COUNTER_ATOMIC64_READ(dz_page_alloc_cnt));

	ptr += sprintf(ptr, "dz_page_free_cnt.....................................= %ld\n",
			BASE_COUNTER_ATOMIC64_READ(dz_page_free_cnt));

	ptr += sprintf(ptr, "dz_page_alloc_cnt - dz_page_free_cnt.................= %ld\n", 
			(BASE_COUNTER_ATOMIC64_READ(dz_page_alloc_cnt) - BASE_COUNTER_ATOMIC64_READ(dz_page_free_cnt)));

	ptr += sprintf(ptr, "bec_node_alloc_cnt...................................= %ld\n",
			BEC_COUNTER_ATOMIC64_READ(bec_node_alloc_cnt));

	ptr += sprintf(ptr, "bec_node_free_cnt....................................= %ld\n",
			BEC_COUNTER_ATOMIC64_READ(bec_node_free_cnt));

	ptr += sprintf(ptr, "bec_node_alloc_cnt - bec_node_free_cnt...............= %ld\n", 
			(BEC_COUNTER_ATOMIC64_READ(bec_node_alloc_cnt) - BEC_COUNTER_ATOMIC64_READ(bec_node_free_cnt)));

	ptr += sprintf(ptr, "bec_page_alloc_cnt...................................= %ld\n",
			BEC_COUNTER_ATOMIC64_READ(bec_page_alloc_cnt));

	ptr += sprintf(ptr, "bec_page_free_cnt....................................= %ld\n",
			BEC_COUNTER_ATOMIC64_READ(bec_page_free_cnt));

	ptr += sprintf(ptr, "bec_page_alloc_cnt - bec_page_free_cnt...............= %ld\n", 
			(BEC_COUNTER_ATOMIC64_READ(bec_page_alloc_cnt) - BEC_COUNTER_ATOMIC64_READ(bec_page_free_cnt)));
	return 0;
}

extern CHAR avm_sysfs_entry_get_version[DZ_SYSFS_PAGE_SIZE];
INT avm_get_version(CHAR *buf)
{
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_version;
	memset(avm_sysfs_entry_get_version, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "AISA ZaidStor Major   = %d\n", DZ_AISA_VERSION_MAJOR);
	ptr += sprintf(ptr, "AISA ZaidStor Minor   = %d\n", DZ_AISA_VERSION_MINOR);
	ptr += sprintf(ptr, "AISA ZaidStor Patch   = %d\n", DZ_AISA_VERSION_PATCH);
	ptr += sprintf(ptr, "AISA ZaidStor Release = %s\n", DZ_AISA_VERSION_RELEASE);
	ptr += sprintf(ptr, "LINUX_VERSION_CODE    = %d\n", LINUX_VERSION_CODE);
	return 0;
}

INT avm_get_free_page_count(CHAR *buf)
{
	//avm_sysfs_entry_free_page_count = atomic64_read(&avm_free_page_list.freepageq_num_pages);
	return 0;
}

INT avm_get_list_of_objects(CHAR *buf)
{
	LOGSYS("%s",__func__);
	print_object_list();
	return 0;
}

extern PPBA_BLOCK	pba_table;
extern PHASH_BLOCK	hash_table;
extern PLBA_BLOCK	lba_table;
extern BITMAP 			pba_bitmap;

extern U64 avm_sysfs_entry_tasb;
INT avm_get_tasb(CHAR *buf)
{
	/*
	INT	lock = 0x0;
	INT ret = 0;
	LOG("\nTesting test_and_set_bit()\n");
	lock = 0x0;
	LOG("Before lock =%d, 0x%x\n", lock, lock);
	ret = test_and_set_bit (0, &lock);
	LOG("ret =%d, 0x%x\n", ret, ret);
	LOG("After  lock =%d, 0x%x\n", lock, lock);

	lock = 0x1;
	LOG("Before lock =%d, 0x%x\n", lock, lock);
	ret = test_and_set_bit (0, &lock);
	LOG("ret =%d, 0x%x\n", ret, ret);
	LOG("After  lock =%d, 0x%x\n", lock, lock);

	lock = 0x2;
	LOG("Before lock =%d, 0x%x\n", lock, lock);
	ret = test_and_set_bit (0, &lock);
	LOG("ret =%d, 0x%x\n", ret, ret);
	LOG("After  lock =%d, 0x%x\n", lock, lock);

	lock = 0x3;
	LOG("Before lock =%d, 0x%x\n", lock, lock);
	ret = test_and_set_bit (0, &lock);
	LOG("ret =%d, 0x%x\n", ret, ret);
	LOG("After  lock =%d, 0x%x\n", lock, lock);

	*/

	//The above experiment shows that test_and_set_bit() will work for bit nr 0 
	//if the previous value is 0. This bit will be converted to 1
	//So for values 0x3 and 0x1, the above function will return -1
	//and for 0 and 2 it will return 0 and set the bit

	/*
	LOG("\nTesting test_and_clear_bit()\n");
	lock = 0x0;
	LOG("Before lock =%d, 0x%x\n", lock, lock);
	ret = test_and_clear_bit (0, &lock);
	LOG("ret =%d, 0x%x\n", ret, ret);
	LOG("After  lock =%d, 0x%x\n", lock, lock);

	lock = 0x1;
	LOG("Before lock =%d, 0x%x\n", lock, lock);
	ret = test_and_clear_bit (0, &lock);
	LOG("ret =%d, 0x%x\n", ret, ret);
	LOG("After  lock =%d, 0x%x\n", lock, lock);

	lock = 0x2;
	LOG("Before lock =%d, 0x%x\n", lock, lock);
	ret = test_and_clear_bit (0, &lock);
	LOG("ret =%d, 0x%x\n", ret, ret);
	LOG("After  lock =%d, 0x%x\n", lock, lock);
	
	lock = 0x3;
	LOG("Before lock =%d, 0x%x\n", lock, lock);
	ret = test_and_clear_bit (0, &lock);
	LOG("ret =%d, 0x%x\n", ret, ret);
	LOG("After  lock =%d, 0x%x\n", lock, lock);
	*/

	spinlock_t lba_spin;
	U64 start = 0;
	U64 end = 0;
	INT ret = 0;

	union a {
		struct {
			INT first:1;
			INT second:1;
			INT third:3;
			INT fourth:3;
			
		}__attribute__((__packed__));
		INT c;
	}__attribute__((__packed__));

	LBA_BLOCK lb;
	PLBA_BLOCK lbp = &lb;

	union a b;
	LOG("sizeofstruct a = %zu\n", sizeof(union a));
	b.first = 0;
	b.second = 0;
	b.third = 7;
	LOG("Before bit_spin_lock\n");
	LOG("b.first   = %x\n", b.first);
	LOG("b.second  = %x\n", b.second);
	LOG("b.third   = %x\n", b.third);
	LOG("b.c       = %d 0x%x\n", b.c, b.c);

	//bit_spin_lock(0, &b.c);
	LOG("After bit_spin_lock\n");
	LOG("b.first   = %x\n", b.first);
	LOG("b.second  = %x\n", b.second);
	LOG("b.third   = %x\n", b.third);
	LOG("b.c       = %d 0x%x\n", b.c, b.c);

	//bit_spin_unlock(0, &b.c);
	LOG("After bit_spin_unlock\n");
	LOG("b.first   = %x\n", b.first);
	LOG("b.second  = %x\n", b.second);
	LOG("b.third   = %x\n", b.third);
	LOG("b.c       = %d 0x%x\n", b.c, b.c);

	LOG("\nsize of lba_block = %zu\n", sizeof(LBA_BLOCK));
	LOG("\nsize of pba_block = %zu\n", sizeof(PBA_BLOCK));
	LOG("\nsize of hash_block = %zu\n", sizeof(HASH_BLOCK));
	//LBA_LOCK(&((GET_LBA_ENTRY(8))->lock));
	//LBA_UNLOCK((GET_LBA_ENTRY(8))->lock);
	MEMSET(&lb);
	
	spin_lock_init(&lba_spin);
	lb.allocated = true;
	lb.hash_index = 3;
	print_lba_block(&lb);
	LOG("test_bit = %d\n", test_bit(0, &lb.donotchange));
	LOG("test_bit = %d\n", test_bit(1, &lb.donotchange));
	LOG("test_bit = %d\n", test_bit(2, &lb.donotchange));
	LOG("test_bit = %d\n", test_bit(3, &lb.donotchange));
	LOG("test_bit = %d\n", test_bit(4, &lb.donotchange));
	start = get_jiffies_64();
	LBA_LOCK(&lb);
	ret = bit_spin_trylock(0, &lb.donotchange);
	LOG("\n ret = %d\n", ret);
	print_lba_block(&lb);
	LBA_UNLOCK(&lb);
	end = get_jiffies_64();
	LOG("start=%llx\n", start);
	LOG("end=%llx\n", end);
	LOG("diff=%x\n", jiffies_to_usecs(end-start));
	print_lba_block(&lb);

	start = get_jiffies_64();
	spin_lock(&lba_spin);
	if (lb.allocated == true ) {
		lb.allocated = false;
	}
	end = get_jiffies_64();
	spin_unlock(&lba_spin);

	lb.allocated = true;
	start = get_jiffies_64();
	spin_lock(&lba_spin);
	if (lbp->allocated == true ) {
		lbp->allocated = false;
	}
	end = get_jiffies_64();
	spin_unlock(&lba_spin);

	print_lba_block(&lb);

	bit_spin_unlock(0, &lb.donotchange);
	print_lba_block(&lb);
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

extern U64			fec_active_count;
extern U64			time_fec_write_start;
extern U64			time_fec_write_switch;
extern PFEC_TABLE	fec_tables_flush;
extern U64			time_flush_prepare_start;
extern U64			time_flush_start;
extern U64			time_flush_end;
extern ATOMIC64		fec_active_count_sequence;
extern ATOMIC64		fec_free_count_sequence;
extern ATOMIC64		fec_cur_flush_count;
extern ATOMIC64		fec_flush_ops_count;
extern ATOMIC64		fec_cur_flush_ops_count;
extern ATOMIC64		fec_memory_reads_count;
extern ATOMIC64		fec_forwarded_reads_count;
INT sys_fec_show(CHAR *buf)
{
	INT count = 0;
	//Note: Value is NOT lock safe
	count = sprintf(buf,
	"FEC Active Count Sequence   = %lli\n"
	"FEC Free   Count Sequence   = %lli\n"
	"FEC Active Count            = %lli\n"
	"FEC Flush Ops Count         = %lli\n"
	"Current Flush Ops Count     = %lli\n"
	"FEC Memory Reads Count      = %lli\n"
	"FEC Forwarded Reads Count   = %lli\n"
	"FEC Write Start             = %lli\n"
	"FEC Write Switch            = %lli\n"
	"Current Time                = %lli\n"
	"Diff wrt Current Time(secs) = %lli\n"
	"Diff wrt Final(secs)        = %lli\n"
	"Total Flush Count Pending   = %lli\n"
	"Current Flush Count         = %lli\n"
	"Flush Prepare Start         = %lli\n"
	"Flush Start Time            = %lli\n"
	"Flush End Time              = %lli\n"
	"Current Time                = %lli\n"
	"Diff wrt Current Time(secs) = %lli\n"
	"Diff wrt Final(secs)        = %lli\n",
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
	(U64)GET_CURRENT_TIME(),
	(U64)(GET_TIME_DIFF_IN_SECONDS(time_flush_start, GET_CURRENT_TIME())/1000),
	(U64)((GET_TIME_DIFF_IN_SECONDS(time_flush_start, time_flush_end))/1000)
	);
	/*
	(U64) ((READ_COUNTER(fec_cur_flush_count)) / 
	(GET_TIME_DIFF_IN_SECONDS(time_flush_start, GET_CURRENT_TIME())/1000)));
	*/

	LOGR("<------------ FEC Show ----------->\n");
	LOGR("%s",buf);

	LOGD("Buffer Bytes used           = %d\n", count);
	return 0;
}

extern ATOMIC64	bec_pba_page_count;
extern ATOMIC64	bec_cache_hit;
extern ATOMIC64	bec_cache_miss;
extern ATOMIC64	bec_cache_delete;
extern ULONG	bec_size;
extern INT		bec_cutoff_nodes;
extern INT		bec_cutoff_percent;
extern U64		bec_engine_memory_usage;
extern INT		dz_bec_node_mempool_count;


INT sys_bec_show(CHAR *buf)
{
	INT count = 0;
	//Note: Value is NOT lock safe
	count = sprintf(buf,
	"bec_pba_page_count          = %lli\n"
	"bec_cache_hit               = %lli\n"
	"bec_cache_miss              = %lli\n"
	"bec_cache_delete            = %lli\n"
	"bec_size                    = %lli\n"
	"bec_cutoff_nodes            = %lli\n"
	"bec_cutoff_percent          = %lli\n"
	"bec_node_mempool_count      = %lli\n",
	(U64)READ_COUNTER(bec_pba_page_count),
	(U64)ATOMIC64_READ(bec_cache_hit),
	(U64)ATOMIC64_READ(bec_cache_miss),
	(U64)ATOMIC64_READ(bec_cache_delete),
	(U64)bec_size,
	(U64)bec_cutoff_nodes,
	(U64)bec_cutoff_percent,
	(U64)dz_bec_node_mempool_count);

	LOGR("<------------ BEC Show ----------->\n");
	LOGR("%s",buf);

	LOGD("Buffer Bytes used           = %d\n", count);
	return count;

}

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "dedupe_engine.h"
#include "prototypes.h"

extern INT				dz_switch_fec_engine;
extern INT				dz_switch_dedupe_engine;
extern SIZE				TARGET_DEVICE_SIZE; // In terms of 4K blocks
extern PAVM_DEVICE 		aisa_device_object;

extern PBLOCK_DEVICE 	avm_metadata_bdev;
extern PMD_LOCK	md_lock_table;	// Array of spinlocks for MetaData Lock

extern U32 number_of_longs_for_pba;
extern U32 number_of_longs_for_lba;
extern U32 number_of_longs_for_gpba;
extern INT	dz_hash_mempool_count;
extern INT	dz_dedupe_mempool_count;

extern U64 	pba_table_size; 
extern U64	hash_table_size;
extern U64 	lba_table_size;

extern PPBA_BLOCK	dedupe_pba_table;
extern PHASH_BLOCK	dedupe_hash_table;
extern PLBA_BLOCK	dedupe_lba_table;

SPINLOCK 	lba_table_lock; 
SPINLOCK 	hash_table_lock; 
SPINLOCK 	pba_table_lock; 

extern BITMAP 		pba_bitmap;
extern BITMAP 		gpba_bitmap;
extern BITMAP 		lba_bitmap;
//U64			dedupe_engine_memory_usage = 0;
//EXPORT_SYMBOL(dedupe_engine_memory_usage);

extern USHORT 	dedupe_num_threads;
extern ATOMIC64	dedupe_disk_reads;
extern ATOMIC64	dedupe_disk_writes;
extern ATOMIC64	dedupe_zeroed_out_page_reads;
extern ATOMIC64	dedupe_cache_hits;
extern ATOMIC64	dedupe_deleted_entries;

extern U64		dedupe_tot_logical_space;
extern U64		dedupe_tot_physical_space;
extern ATOMIC64	dedupe_cur_logical_space;
extern ATOMIC64	dedupe_cur_physical_space;
extern ATOMIC64	dedupe_cur_duplicate_space;


U64 dz_dedupe_get_lba_table_size(VOID)
{
	return lba_table_size;
}
EXPORT_SYMBOL(dz_dedupe_get_lba_table_size);

U64 dz_dedupe_get_pba_table_size(VOID)
{
	return pba_table_size;
}
EXPORT_SYMBOL(dz_dedupe_get_pba_table_size);

U64 dz_dedupe_get_hash_table_size(VOID)
{
	return hash_table_size;
}
EXPORT_SYMBOL(dz_dedupe_get_hash_table_size);


BOOL compare_pages(PPAGE s1 , PPAGE s2)
{
	PVOID	s1pagebuf				= NULL;
	PVOID	s2pagebuf				= NULL;

	s1pagebuf = kmap(s1);
	s2pagebuf = kmap(s2);
	if (memcmp(s1pagebuf, s2pagebuf, PAGE_SIZE) == 0) {
		kunmap(s1);
		kunmap(s2);
		return true;
	} else {
		kunmap(s1);
		kunmap(s2);
		return false;
	}
}

PPBA_BLOCK dz_find_mapped_pba(U64 hash_collision_tag, U64 pba_index)
{
	PPBA_BLOCK	tmp_pba_entry		= NULL;


	while(true) {
		tmp_pba_entry = GET_PBA_ENTRY(pba_index);
		if (tmp_pba_entry->hash_collision_tag == hash_collision_tag) {
			return tmp_pba_entry;
		}

		pba_index = tmp_pba_entry->next_entry_in_collision_list;
		if (pba_index == 0) {
			break;
		} 
	}

	return NULL;
}

RVOID dz_dedupe_io_old(PIOREQUEST iorequest)
{
	PBIO		bio 	= iorequest->ior_bio; // Parent or Original bio

	if (bio_data_dir(bio) == WRITE) {
		if (!iorequest->ior_parent) {
			//LOG("FAKE IO DONE 1\n");
			FAKE_IO_DONE(iorequest);
			dz_io_free(iorequest);
		} else {
			FAKE_IO_DONE(iorequest);
		}
	} else {
		//IF there is no parent then release the memory as well
		//because they will be direct application ios wrapped 
		//inside iorequest
		if (!iorequest->ior_parent) {
			FAKE_IO_DONE(iorequest);
			//LOG("FAKE IO DONE 2 for lba %lli, sector %lli, size %u\n",
				//iorequest->ior_lba, (U64)iorequest->ior_sector, iorequest->size);
			dz_io_free(iorequest);
		} else {
			FAKE_IO_DONE(iorequest);
		}
	}
}

VOID 	dz_dedupe_print_counters(VOID)
{
	LOG("lba_table_size           = %lli\n", (U64)lba_table_size);
	LOG("hash_table_size          = %lli\n", (U64)hash_table_size);
	LOG("pba_table_size           = %lli\n", (U64)pba_table_size);
	LOG("number_of_longs_for_pba  = %lli\n", (U64)number_of_longs_for_pba);
	LOG("number_of_longs_for_lba  = %lli\n", (U64)number_of_longs_for_lba);
}
VOID 	dz_dedupe_init_counters(VOID)
{
	ATOMIC64_SET(dedupe_disk_reads,     0);
	ATOMIC64_SET(dedupe_disk_writes,     0);
	ATOMIC64_SET(dedupe_cache_hits,       0);
	ATOMIC64_SET(dedupe_deleted_entries ,  0);
	ATOMIC64_SET(dedupe_cur_logical_space,  0);
	ATOMIC64_SET(dedupe_cur_physical_space,  0);
	ATOMIC64_SET(dedupe_cur_duplicate_space,  0);
	ATOMIC64_SET(dedupe_zeroed_out_page_reads, 0);
	// Set PBA Table size for this device
	//pba_table_size = 100;
	//pba_table_size must not be less than lba_table_size
	if (pba_table_size == 0) {
		pba_table_size = TARGET_DEVICE_SIZE;
	}
	
	// Set HASH Table size for this device
	//hash_table_size = 100;
	//hash_table_size must not be less than lba_table_size
	if (hash_table_size == 0) {
		hash_table_size = TARGET_DEVICE_SIZE;
	}
	
	// Set LBA Table size for this device
	lba_table_size = TARGET_DEVICE_SIZE;

	dedupe_tot_logical_space	= lba_table_size;
	dedupe_tot_physical_space	= pba_table_size;

	DDE_COUNTER_U64_SET(dedupe_engine_memory_usage, 0);
	dz_dedupe_print_counters();
}

INT dz_dedupe_engine_init(VOID)
{
	INT ret = SUCCESS;
	MARK_MEMORY_USAGE();

	if (dz_switch_dedupe_engine == DZ_DISABLED) {
		LOGDDEF(" ______________________________________________________ \n");
		LOGDDEF("|                                                      |\n");
		LOGDDEF("|                Dedupe Engine Disabled                |\n");
		LOGDDEF("|______________________________________________________|\n");
		PRINT_MEMORY_USAGE( DDE_COUNTER_U64_GET(dedupe_engine_memory_usage), "Dedupe Engine");
		RETURNS;
	}

	LOGDDE(" ______________________________________________________ \n");
	LOGDDE("|                                                      |\n");
	LOGDDE("|         Dedupe Engine Initialization Begin           |\n");
	LOGDDE("|______________________________________________________|\n");

	dz_dedupe_init_counters();
	ret = dz_create_dedupe_thread_pool(dedupe_num_threads);
	//TODO: Handle ret failure
	
	// Create a PBA Table for this device
	dedupe_pba_table = dz_create_pba_table(pba_table_size);
	if (!dedupe_pba_table) {
		goto exit_failure;
	}

	// Create a HASH Table for this device
	dedupe_hash_table = dz_create_hash_table(hash_table_size);
	if (!dedupe_hash_table) {
		goto exit_failure;
	}


	// Create a LBA Table for this device
	dedupe_lba_table = dz_create_lba_table(lba_table_size);
	if (!dedupe_lba_table) {
		goto exit_failure;
	}

	ret = dz_create_hash_pool		(dz_hash_mempool_count);
	ret = dz_create_dedupe_pool		(dz_dedupe_mempool_count);
	//TODO: Handle ret failure
	PRINT_MEMORY_USAGE( DDE_COUNTER_U64_GET(dedupe_engine_memory_usage), "Dedupe Engine");
	LOGDDE(" ______________________________________________________ \n");
	LOGDDE("|                                                      |\n");
	LOGDDE("|         Dedupe Engine Initialization End             |\n");
	LOGDDE("|______________________________________________________|\n");
	RETURNS;

exit_failure:
	dz_dedupe_engine_exit();
	LOGDDE(" ______________________________________________________ \n");
	LOGDDE("|                                                      |\n");
	LOGDDE("|         Dedupe Engine Initialization Failed          |\n");
	LOGDDE("|______________________________________________________|\n");
	RETURNF;
}
EXPORT_SYMBOL(dz_dedupe_engine_init);

RVOID dz_dedupe_engine_exit(VOID)
{
	MARK_MEMORY_USAGE();
	if (dz_switch_dedupe_engine == DZ_DISABLED) {
		LOGDDEF(" ______________________________________________________ \n");
		LOGDDEF("|                                                      |\n");
		LOGDDEF("|                Dedupe Engine Disabled                |\n");
		LOGDDEF("|______________________________________________________|\n");
		PRINT_MEMORY_USAGE( DDE_COUNTER_U64_GET(dedupe_engine_memory_usage), "Dedupe Engine");
		return;
	}

	LOGDDE(" ______________________________________________________ \n");
	LOGDDE("|                                                      |\n");
	LOGDDE("|         Dedupe Engine Exit Begin                     |\n");
	LOGDDE("|______________________________________________________|\n");

	dz_dedupe_print_counters();

	dz_destroy_pba_table(dedupe_pba_table);
	dz_destroy_hash_table(dedupe_hash_table);
	dz_destroy_lba_table(dedupe_lba_table);
	dz_destroy_hash_pool(dz_hash_mempool_count);
	dz_destroy_dedupe_pool(dz_dedupe_mempool_count);
	dz_destroy_dedupe_thread_pool();

	PRINT_MEMORY_FREED( DDE_COUNTER_U64_GET(dedupe_engine_memory_usage), "Dedupe Engine");
	LOGDDE(" ______________________________________________________ \n");
	LOGDDE("|                                                      |\n");
	LOGDDE("|         Dedupe Engine Exit End                       |\n");
	LOGDDE("|______________________________________________________|\n");
}
EXPORT_SYMBOL(dz_dedupe_engine_exit);

RVOID dz_dedupe_io_old2(PIOREQUEST iorequest)
{
	if (bio_data_dir(iorequest->ior_bio) == WRITE) {
		if (!iorequest->ior_parent) {
			//LOG("FAKE IO DONE 1\n");
			FAKE_IO_DONE(iorequest);
			dz_io_free(iorequest);
		} else {
			FAKE_IO_DONE(iorequest);
		}
	} else {
		dz_dedupe_read_io(iorequest);
	}

	/*
	bio_data_dir(iorequest->bio) == WRITE ? 
		dz_dedupe_write_io(iorequest) : 
		dz_dedupe_read_io(iorequest);
	*/
}

RVOID dz_dedupe_io(PIOREQUEST iorequest)
{
	/*
	bio_data_dir(iorequest->ior_bio) == WRITE ? 
		dz_dedupe_write_io(iorequest) : 
		dz_dedupe_read_io(iorequest);
	*/
//	if (iorequest->ior_bio == NULL) {
	IF_WRITE_IOREQUEST(iorequest) {
		//bio is set to NULL for Write Application IO inside dz_fec_align_write_for_single_block()
		dz_dedupe_write_io(iorequest);
	} else {
		dz_dedupe_read_io(iorequest);
	}
}

inline PLBA_BLOCK	GET_LBA_ENTRY(U64 lba)
{
		return (&dedupe_lba_table[lba]);
}
EXPORT_SYMBOL(GET_LBA_ENTRY);

inline PPBA_BLOCK	GET_PBA_ENTRY(U64 pba)
{
		return (&dedupe_pba_table[pba]);
}
EXPORT_SYMBOL(GET_PBA_ENTRY);

inline PHASH_BLOCK	GET_HASH_ENTRY(U64 hash)
{
		return (&dedupe_hash_table[hash]);
}
EXPORT_SYMBOL(GET_HASH_ENTRY);

inline PHASH_BLOCK	GET_HBA_ENTRY(U64 hash)
{
	return GET_HASH_ENTRY(hash);
}
EXPORT_SYMBOL(GET_HBA_ENTRY);

static int dz_dedupe_engine_module_init(void)
{
	LOGDDE(" ______________________________________________________ \n");
	LOGDDE("|                                                      |\n");
	LOGDDE("|           ZAIDSTOR Dedupe Engine Module Loaded       |\n");
	LOGDDE("|______________________________________________________|\n");

	RETURNS;
}

static void dz_dedupe_engine_module_exit(void)
{
	LOGDDE(" ______________________________________________________ \n");
	LOGDDE("|                                                      |\n");
	LOGDDE("|           ZAIDSTOR Dedupe Engine Module Unloaded     |\n");
	LOGDDE("|______________________________________________________|\n");
}

module_init(dz_dedupe_engine_module_init);
module_exit(dz_dedupe_engine_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

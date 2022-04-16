/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

#define dz_md_engine_init	dz_metadata_engine_init
extern PBLOCK_DEVICE	METADATA_DISK_DEVICE;
extern PBLOCK_DEVICE	DATA_DISK_DEVICE;
extern U64 				lba_table_size;
extern PSUPERBLOCK		superblock;
extern INT 				dz_switch_passthrough_mode;
extern USHORT 			num_metadata_flush_threads;
extern THREAD           *metadata_flush_timer_thread;

extern INT				dz_switch_dedupe_engine;

//U64			md_engine_memory_usage = 0;
//EXPORT_SYMBOL(md_engine_memory_usage);

extern U64  			metadata_lba_table_size;
extern PMDD_BLOCK  		metadata_lba_table;

extern U64 				metadata_lba_lock_table_size;
extern PMD_LOCK			metadata_lba_lock_table;	// Array of spinlocks for MetaData Lock

INLINE PMD_BLOCK   GET_MDB_ENTRY(U64 lba)
{
	return (&metadata_lba_table[lba]);
}
EXPORT_SYMBOL(GET_MDB_ENTRY);

VOID dz_generate_hash(VOID *data, SIZE size, PVOID output_hash)
{
	U32 seed = 0;
       
	//TODO: We need to change the constant values inside this function
	MurmurHash3_x64_128 (data, size, seed, output_hash);
}
EXPORT_SYMBOL(dz_generate_hash);

VOID    dz_md_init_counters(VOID)
{
	BASE_COUNTER_U64_SET(md_engine_memory_usage, 0);
}

RINT dz_metadata_block_init(VOID)
{

	//Create an inmemory table of metadata blocks */
	//No of entries of metadata will be equal to no. of lba blocks exposed
	//Hence metadata_lba_table_size will be equal to lba_table_size

	if (dz_switch_dedupe_engine == DZ_DISABLED) {
		LOGMETF("Dedupe Engine is Disabled. MetaData Block Initialization Disabled \n");
		RETURNS;
	}

	metadata_lba_table_size = lba_table_size;

	if (!(metadata_lba_table = dz_create_metadata_table(metadata_lba_table_size))) {
		LOGMETE("AISA-Error: Unable to create metadata table\n");
		return -ENOMEM;
	}

	//Store this table size in superblock
	superblock->metadata_lba_table_size 		= metadata_lba_table_size;
	superblock->metadata_lba_block_size 		= METADATA_BLOCK_SIZE;

	//Create an array of locks for Metadata
	//metadata_lba_lock_table_size = (metadata_lba_table_size / (SECTOR_SIZE / METADATA_BLOCK_SIZE)) * sizeof(SPINLOCK);
	//Since disk sector is typically 512 bytes size so lets take a lock of complete SECTOR
	//METADATA_BLOCK_SIZE is 64 bytes max and there can be 8 METADATA_BLOCKS can be accomodated in one SECTOR
	//
	metadata_lba_lock_table_size = metadata_lba_table_size / (SECTOR_SIZE / METADATA_BLOCK_SIZE);
	metadata_lba_lock_table = dz_create_metadata_lock_table(metadata_lba_lock_table_size);
	if (!metadata_lba_lock_table) {
		LOGMETE("AISA-Error: Unable to create metadata lock array \n");
		return -ENOMEM;
	}
	superblock->metadata_lba_lock_table_size	= metadata_lba_lock_table_size;
	superblock->metadata_lba_lock_block_size 	= SECTOR_SIZE;
	RETURNS;
}

VOID dz_metadata_block_exit(VOID)
{
	if (dz_switch_dedupe_engine == DZ_DISABLED) {
		LOGMETF("Dedupe Engine is Disabled. MetaData Block Initialization Disabled \n");
		return;
	}
	dz_destroy_metadata_lock_table();
	dz_destroy_metadata_table();
}

RINT dz_metadata_engine_init(VOID)
{
	INT ret = SUCCESS;
	MARK_MEMORY_USAGE();
	LOGMETF(" ______________________________________________________ \n");
	LOGMETF("|                                                      |\n");
	LOGMETF("|       Metadata (MD) Engine Initialization Begin      |\n");
	LOGMETF("|______________________________________________________|\n");

	ret = dz_metadata_create_flush_thread_pool(num_metadata_flush_threads);
	//TODO:Check for failure
	
	//Start Timer
	//Sleeping so that timer gets started
	SLEEP_MILLI_SECONDS(300);
	metadata_flush_timer_thread = kthread_run (dz_metadata_flush_timer, NULL, "flush_timer_thread");
	if (IS_ERR(metadata_flush_timer_thread)) {
		LOGMETE("Unable to create flush timer thread\n");
		goto exit_failure;
	}

	//Initialize the superblock and write the header into DATA DISK */
	ret = dz_superblock_init();
	if (ret) {
		ret = -EIO;
		goto exit_failure;
	}
	LOGMETF("Superblock Initialized. Printing Superblock\n");
	print_superblock(superblock);
	ret = dz_metadata_block_init();
	if (ret) {
		goto exit_failure;
	}
	LOGMETF("Metadata Block Initialized.\n");

	PRINT_MEMORY_USAGE(BASE_COUNTER_U64_GET(md_engine_memory_usage), "MD Engine");
	LOGMETF(" ______________________________________________________ \n");
	LOGMETF("|                                                      |\n");
	LOGMETF("|       Metadata (MD) Engine Initialization End        |\n");
	LOGMETF("|______________________________________________________|\n");

	RETURNS;

exit_failure:
	dz_metadata_engine_exit();
	LOGMETF(" ______________________________________________________ \n");
	LOGMETF("|                                                      |\n");
	LOGMETF("|       Metadata (MD) Engine Initialization Failed     |\n");
	LOGMETF("|______________________________________________________|\n");
	RETURN;
}
EXPORT_SYMBOL(dz_metadata_engine_init);

VOID dz_metadata_engine_exit(VOID)
{
	MARK_MEMORY_USAGE();
	LOGMETF(" ______________________________________________________ \n");
	LOGMETF("|                                                      |\n");
	LOGMETF("|            Metadata (MD) Engine Exit Begin           |\n");
	LOGMETF("|______________________________________________________|\n");

	dz_metadata_flush_timer_stop();
	dz_destroy_metadata_flush_thread_pool();
	dz_superblock_exit();
	dz_metadata_block_exit();

	PRINT_MEMORY_FREED(BASE_COUNTER_U64_GET(md_engine_memory_usage), "MD Engine");
	LOGMETF(" ______________________________________________________ \n");
	LOGMETF("|                                                      |\n");
	LOGMETF("|            Metadata (MD) Engine Exit End             |\n");
	LOGMETF("|______________________________________________________|\n");

	return;
}
EXPORT_SYMBOL(dz_metadata_engine_exit);



static int dz_metadata_engine_module_init(void)
{
    LOGMETF(" ______________________________________________________ \n");
    LOGMETF("|                                                      |\n");
    LOGMETF("|           ZAIDSTOR Metadata Engine Module Loaded     |\n");
    LOGMETF("|______________________________________________________|\n");

    RETURNS;
}

static void dz_metadata_engine_module_exit(void)
{
    LOGMETF(" ______________________________________________________ \n");
    LOGMETF("|                                                      |\n");
    LOGMETF("|           ZAIDSTOR Metadata Engine Module Unloaded   |\n");
    LOGMETF("|______________________________________________________|\n");
}

module_init(dz_metadata_engine_module_init);
module_exit(dz_metadata_engine_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

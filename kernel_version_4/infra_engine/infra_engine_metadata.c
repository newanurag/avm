/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

//Metadata Engine Infrastructure


///////////////////////////////////////
//Device related holders
///////////////////////////////////////
PBLOCK_DEVICE avm_metadata_bdev = NULL;
EXPORT_SYMBOL(avm_metadata_bdev);

///////////////////////////////////////
//Table related holders
///////////////////////////////////////
U64         metadata_lba_table_size  = 0;
EXPORT_SYMBOL(metadata_lba_table_size);

//metadata_lba_lock_table_size = (metadata_lba_table_size / (SECTOR_SIZE / METADATA_BLOCK_SIZE))
//Since disk sector is typically 512 bytes size so lets take a lock of complete SECTOR
//METADATA_BLOCK_SIZE is 64 bytes max and there can be 8 METADATA_BLOCKS can be accomodated in one SECTOR
U64         metadata_lba_lock_table_size  = 0;
EXPORT_SYMBOL(metadata_lba_lock_table_size);

//Actual Table holders

//Array of metadata blocks
//Each lba in DATA DISK will correspond to one MDD_BLOCK
PMDD_BLOCK  metadata_lba_table;
EXPORT_SYMBOL(metadata_lba_table);

//Array of spinlocks for metadata lba. Each lock entry will be capable
//of locking 8 metadata blocks i.e. MDD_BLOCK
PMDD_LOCK  metadata_lba_lock_table;
EXPORT_SYMBOL(metadata_lba_lock_table);

///////////////////////////////////////
//Thread and Flush related holders
///////////////////////////////////////

#define DZ_METADATA_FLUSH_INTERVAL   		15 //Metadata Flush interval in secs i.e. 15 seconds
#define DZ_METADATA_NUMBER_OF_FLUSH_THREADS 1 

PCHAR METADATA_FLUSH_THREAD_POOL_NAME      	= "metadata_flush_thread_pool";
EXPORT_SYMBOL(METADATA_FLUSH_THREAD_POOL_NAME);

PCHAR METADATA_FLUSH_THREAD_POOL_SHORT_NAME = "mdpool";
EXPORT_SYMBOL(METADATA_FLUSH_THREAD_POOL_SHORT_NAME);

USHORT num_metadata_flush_threads           = DZ_METADATA_NUMBER_OF_FLUSH_THREADS;
EXPORT_SYMBOL(num_metadata_flush_threads);

PDZ_THREAD_POOL  metadata_flush_thread_pool	= NULL;
EXPORT_SYMBOL(metadata_flush_thread_pool);

BOOL             metadata_flush_timer_stop;
EXPORT_SYMBOL(metadata_flush_timer_stop);

ULONG            metadata_flush_interval 	= DZ_METADATA_FLUSH_INTERVAL;
EXPORT_SYMBOL(metadata_flush_interval);

THREAD           *metadata_flush_timer_thread;
EXPORT_SYMBOL(metadata_flush_timer_thread);

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

//DEDUPE Engine Infrastructure

///////////////////////////////////////
//Dedupe Threads Related
///////////////////////////////////////
#define NUMBER_OF_DEDUPE_THREADS        4

USHORT      dedupe_num_threads      = NUMBER_OF_DEDUPE_THREADS;
EXPORT_SYMBOL(dedupe_num_threads);

PCHAR DEDUPE_THREAD_POOL_NAME         = "dedupe_thread_pool";
PCHAR DEDUPE_THREAD_POOL_SHORT_NAME   = "ddpool";

PCHAR DEDUPE_READ_THREAD_POOL_NAME         = "rdedupe_thread_pool";
PCHAR DEDUPE_READ_THREAD_POOL_SHORT_NAME   = "rddpool";

PCHAR DEDUPE_WRITE_THREAD_POOL_NAME         = "wdedupe_thread_pool";
PCHAR DEDUPE_WRITE_THREAD_POOL_SHORT_NAME   = "wddpool";

EXPORT_SYMBOL(DEDUPE_THREAD_POOL_NAME);
EXPORT_SYMBOL(DEDUPE_THREAD_POOL_SHORT_NAME);

EXPORT_SYMBOL(DEDUPE_READ_THREAD_POOL_NAME);
EXPORT_SYMBOL(DEDUPE_READ_THREAD_POOL_SHORT_NAME);

EXPORT_SYMBOL(DEDUPE_WRITE_THREAD_POOL_NAME);
EXPORT_SYMBOL(DEDUPE_WRITE_THREAD_POOL_SHORT_NAME);

///////////////////////////////////////
//Stats Counters
///////////////////////////////////////
DDE_COUNTERS    dz_dde_counters;
PDDE_COUNTERS   pdz_dde_counters;
EXPORT_SYMBOL(dz_dde_counters);
EXPORT_SYMBOL(pdz_dde_counters);

///////////////////////////////////////
//Stats Reads/Writes
///////////////////////////////////////

//Reads issued for unallocated LBAs or Unwritten LBAs
ATOMIC64    dedupe_zeroed_out_page_reads;
EXPORT_SYMBOL(dedupe_zeroed_out_page_reads);

ATOMIC64    dedupe_disk_reads;
EXPORT_SYMBOL(dedupe_disk_reads);
 
ATOMIC64    dedupe_disk_writes;
EXPORT_SYMBOL(dedupe_disk_writes);

//If the read during comparison is served from bec cache instead from disk
ATOMIC64    dedupe_cache_hits;
EXPORT_SYMBOL(dedupe_cache_hits);

//If there is an Overwrite happens on an LBA and inturns if dedupe
//needs to delete the previously allocated pba
ATOMIC64    dedupe_deleted_entries;
EXPORT_SYMBOL(dedupe_deleted_entries);

///////////////////////////////////////
//Table related holders
///////////////////////////////////////
U64         pba_table_size  = 0;
EXPORT_SYMBOL(pba_table_size);
 
U64         hash_table_size = 0;
EXPORT_SYMBOL(hash_table_size);
 
U64         lba_table_size  = 0;
EXPORT_SYMBOL(lba_table_size);

//No. of longs for pbas which are processed by Garbage Collector
U32 number_of_longs_for_gpba;
EXPORT_SYMBOL(number_of_longs_for_gpba);

U32 number_of_longs_for_pba;
EXPORT_SYMBOL(number_of_longs_for_pba);

U32 number_of_longs_for_lba;
EXPORT_SYMBOL(number_of_longs_for_lba);

//Actual Table holders
PPBA_BLOCK  dedupe_pba_table;
EXPORT_SYMBOL(dedupe_pba_table);

PLBA_BLOCK  dedupe_lba_table;
EXPORT_SYMBOL(dedupe_lba_table);

PHASH_BLOCK dedupe_hash_table;
EXPORT_SYMBOL(dedupe_hash_table);

///////////////////////////////////////
//Bitmaps
///////////////////////////////////////
BITMAP      pba_bitmap;
EXPORT_SYMBOL(pba_bitmap);
 
BITMAP      lba_bitmap;
EXPORT_SYMBOL(lba_bitmap);

BITMAP      gpba_bitmap;
EXPORT_SYMBOL(gpba_bitmap);

///////////////////////////////////////
//Memory Pool related
///////////////////////////////////////

///////////////////////////////////////
//Dedupe Storage Space Related
///////////////////////////////////////

//Initially how much total logical space available
U64         dedupe_tot_logical_space;
EXPORT_SYMBOL(dedupe_tot_logical_space);
 
//Initially how much total physical space available
U64         dedupe_tot_physical_space;
EXPORT_SYMBOL(dedupe_tot_physical_space);
 
//Currently how much total logical space available
ATOMIC64    dedupe_cur_logical_space;
EXPORT_SYMBOL(dedupe_cur_logical_space);
 
//Currently how much total physical space available
ATOMIC64    dedupe_cur_physical_space;
EXPORT_SYMBOL(dedupe_cur_physical_space);

//Currently how much duplicate space identified(saved)
ATOMIC64    dedupe_cur_duplicate_space;
EXPORT_SYMBOL(dedupe_cur_duplicate_space);


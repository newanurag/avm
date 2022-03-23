/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"
//FEC Engine

///////////////////////////////////////
//Stats Counters
///////////////////////////////////////

FEC_COUNTERS    dz_fec_counters;
PFEC_COUNTERS   pdz_fec_counters;
EXPORT_SYMBOL(dz_fec_counters);
EXPORT_SYMBOL(pdz_fec_counters);

U64         fec_active_threshold_perc   = 50;
EXPORT_SYMBOL(fec_active_threshold_perc);
 
U64         fec_active_threshold        = 0;
EXPORT_SYMBOL(fec_active_threshold);
 
U64         fec_active_count            = 0;
EXPORT_SYMBOL( fec_active_count);
 
U64         fec_flush_count             = 0;
EXPORT_SYMBOL(fec_flush_count);

ATOMIC64 fec_active_count_sequence;
EXPORT_SYMBOL(fec_active_count_sequence);

ATOMIC64    fec_free_count_sequence;
EXPORT_SYMBOL(fec_free_count_sequence);

ATOMIC64    fec_cur_flush_count;
EXPORT_SYMBOL(fec_cur_flush_count);

ATOMIC64    fec_flush_ops_count;
EXPORT_SYMBOL(fec_flush_ops_count);
 
ATOMIC64    fec_cur_flush_ops_count;
EXPORT_SYMBOL(fec_cur_flush_ops_count);
 
ATOMIC64    fec_memory_reads_count;
EXPORT_SYMBOL(fec_memory_reads_count);
 
ATOMIC64    fec_forwarded_reads_count;
EXPORT_SYMBOL(fec_forwarded_reads_count);

ATOMIC64    fec_iocount_reads_partial_page;
EXPORT_SYMBOL(fec_iocount_reads_partial_page);
 
ATOMIC64    fec_iocount_reads_single_page;
EXPORT_SYMBOL(fec_iocount_reads_single_page);
 
ATOMIC64    fec_iocount_reads_single_aligned_page;
EXPORT_SYMBOL(fec_iocount_reads_single_aligned_page);
 
ATOMIC64    fec_iocount_reads_single_unaligned_page;
EXPORT_SYMBOL(fec_iocount_reads_single_unaligned_page);
 
ATOMIC64    fec_iocount_reads_multi_page;
EXPORT_SYMBOL(fec_iocount_reads_multi_page);
 
ATOMIC64    fec_iocount_reads_multi_aligned_page;
EXPORT_SYMBOL(fec_iocount_reads_multi_aligned_page);
 
ATOMIC64    fec_iocount_reads_multi_unaligned_page;
EXPORT_SYMBOL(fec_iocount_reads_multi_unaligned_page);
 
ATOMIC64    fec_iocount_reads_multi_tail_unaligned_page;
EXPORT_SYMBOL(fec_iocount_reads_multi_tail_unaligned_page);

ATOMIC64    fec_iocount_reads_multi_head_unaligned_page;
EXPORT_SYMBOL(fec_iocount_reads_multi_head_unaligned_page);
 
ATOMIC64    fec_iocount_reads_multi_both_unaligned_page;
EXPORT_SYMBOL(fec_iocount_reads_multi_both_unaligned_page);
 
ATOMIC64    fec_iocount_writes_partial_page;
EXPORT_SYMBOL(fec_iocount_writes_partial_page);
 
ATOMIC64    fec_iocount_writes_single_page;
EXPORT_SYMBOL(fec_iocount_writes_single_page);
 
ATOMIC64    fec_iocount_writes_single_aligned_page;
EXPORT_SYMBOL(fec_iocount_writes_single_aligned_page);
 
ATOMIC64    fec_iocount_writes_single_unaligned_page;
EXPORT_SYMBOL(fec_iocount_writes_single_unaligned_page);
 
ATOMIC64    fec_iocount_writes_multi_page;
EXPORT_SYMBOL(fec_iocount_writes_multi_page);
 
ATOMIC64    fec_iocount_writes_multi_aligned_page;
EXPORT_SYMBOL(fec_iocount_writes_multi_aligned_page);

ATOMIC64    fec_iocount_writes_multi_unaligned_page;
EXPORT_SYMBOL(fec_iocount_writes_multi_unaligned_page);

///////////////////////////////////////
//Tables
///////////////////////////////////////
U64         fec_table_size              = 0;
EXPORT_SYMBOL(fec_table_size);

//For operational purpose
//Actually this is an Active table but in comments 
//it is referenced as Active Buffer used for storing writes
PFEC_TABLE  fec_tables_active           = NULL;
EXPORT_SYMBOL(fec_tables_active);
 
PFEC_TABLE  fec_tables_flush            = NULL;
EXPORT_SYMBOL(fec_tables_flush);

///////////////////////////////////////
//Time Related 
///////////////////////////////////////
U64         time_fec_write_start        = 0;
EXPORT_SYMBOL(time_fec_write_start);
 
U64         time_fec_write_switch       = 0;
EXPORT_SYMBOL(time_fec_write_switch);

U64     time_flush_prepare_start = 0;
EXPORT_SYMBOL(time_flush_prepare_start);
 
U64     time_flush_start   = 0;
EXPORT_SYMBOL(time_flush_start);
 
U64     time_flush_end   = 0;
EXPORT_SYMBOL(time_flush_end);

///////////////////////////////////////
//Threads Related
///////////////////////////////////////

#define NUMBER_OF_READ_THREADS          8
#define NUMBER_OF_WRITE_THREADS         8
#define NUMBER_OF_FLUSH_THREADS         1


PCHAR FLUSH_THREAD_POOL_NAME          = "flush_thread_pool";
PCHAR FLUSH_THREAD_POOL_SHORT_NAME    = "ftpool";
EXPORT_SYMBOL(FLUSH_THREAD_POOL_NAME);
EXPORT_SYMBOL(FLUSH_THREAD_POOL_SHORT_NAME);

PCHAR WRITE_THREAD_POOL_NAME          = "write_thread_pool";
PCHAR WRITE_THREAD_POOL_SHORT_NAME    = "wtpool";
EXPORT_SYMBOL(WRITE_THREAD_POOL_NAME);
EXPORT_SYMBOL(WRITE_THREAD_POOL_SHORT_NAME);

PCHAR READ_THREAD_POOL_NAME           = "read_thread_pool";
PCHAR READ_THREAD_POOL_SHORT_NAME     = "rtpool";
EXPORT_SYMBOL(READ_THREAD_POOL_NAME);
EXPORT_SYMBOL(READ_THREAD_POOL_SHORT_NAME);

USHORT num_read_threads             = NUMBER_OF_READ_THREADS;
EXPORT_SYMBOL(num_read_threads);

USHORT num_write_threads            = NUMBER_OF_WRITE_THREADS;
EXPORT_SYMBOL(num_write_threads);

USHORT num_flush_threads            = NUMBER_OF_FLUSH_THREADS;
EXPORT_SYMBOL(num_flush_threads);

PDZ_THREAD_POOL read_thread_pool = NULL;
EXPORT_SYMBOL(read_thread_pool);

PDZ_THREAD_POOL write_thread_pool = NULL;
EXPORT_SYMBOL(write_thread_pool);

PDZ_THREAD_POOL flush_thread_pool = NULL;
EXPORT_SYMBOL(flush_thread_pool);


PDZ_THREAD_POOL fec_read_thread_pool = NULL;
EXPORT_SYMBOL(fec_read_thread_pool);

PDZ_THREAD_POOL fec_write_thread_pool = NULL;
EXPORT_SYMBOL(fec_write_thread_pool);

PDZ_THREAD_POOL fec_flush_thread_pool = NULL;
EXPORT_SYMBOL(fec_flush_thread_pool);


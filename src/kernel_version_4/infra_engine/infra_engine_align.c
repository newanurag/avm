/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"
//Alignment Engine

///////////////////////////////////////
//Enable / Disable Alignment Engine
///////////////////////////////////////

INT dz_alignment_engine_switch = DZ_ENABLED;
EXPORT_SYMBOL(dz_alignment_engine_switch);

///////////////////////////////////////
//Stats Counters
///////////////////////////////////////

ALIGN_COUNTERS    dz_align_counters;
PALIGN_COUNTERS   pdz_align_counters;
EXPORT_SYMBOL(dz_align_counters);
EXPORT_SYMBOL(pdz_align_counters);

ATOMIC64    align_iocount_disk_read;
EXPORT_SYMBOL(align_iocount_disk_read);

ATOMIC64    align_iocount_disk_read_internal;
EXPORT_SYMBOL(align_iocount_disk_read_internal);

ATOMIC64    align_iocount_reads_partial_page;
EXPORT_SYMBOL(align_iocount_reads_partial_page);
 
ATOMIC64    align_iocount_reads_single_page;
EXPORT_SYMBOL(align_iocount_reads_single_page);
 
ATOMIC64    align_iocount_reads_single_aligned_page;
EXPORT_SYMBOL(align_iocount_reads_single_aligned_page);
 
ATOMIC64    align_iocount_reads_single_unaligned_page;
EXPORT_SYMBOL(align_iocount_reads_single_unaligned_page);
 
ATOMIC64    align_iocount_reads_multi_page;
EXPORT_SYMBOL(align_iocount_reads_multi_page);
 
ATOMIC64    align_iocount_reads_multi_aligned_page;
EXPORT_SYMBOL(align_iocount_reads_multi_aligned_page);
 
ATOMIC64    align_iocount_reads_multi_unaligned_page;
EXPORT_SYMBOL(align_iocount_reads_multi_unaligned_page);
 
ATOMIC64    align_iocount_reads_multi_tail_unaligned_page;
EXPORT_SYMBOL(align_iocount_reads_multi_tail_unaligned_page);

ATOMIC64    align_iocount_reads_multi_head_unaligned_page;
EXPORT_SYMBOL(align_iocount_reads_multi_head_unaligned_page);
 
ATOMIC64    align_iocount_reads_multi_both_unaligned_page;
EXPORT_SYMBOL(align_iocount_reads_multi_both_unaligned_page);

ATOMIC64    align_iocount_disk_write;
EXPORT_SYMBOL(align_iocount_disk_write);

ATOMIC64    align_iocount_disk_write_internal;
EXPORT_SYMBOL(align_iocount_disk_write_internal);
 
ATOMIC64    align_iocount_writes_partial_page;
EXPORT_SYMBOL(align_iocount_writes_partial_page);
 
ATOMIC64    align_iocount_writes_single_page;
EXPORT_SYMBOL(align_iocount_writes_single_page);
 
ATOMIC64    align_iocount_writes_single_aligned_page;
EXPORT_SYMBOL(align_iocount_writes_single_aligned_page);
 
ATOMIC64    align_iocount_writes_single_unaligned_page;
EXPORT_SYMBOL(align_iocount_writes_single_unaligned_page);
 
ATOMIC64    align_iocount_writes_multi_page;
EXPORT_SYMBOL(align_iocount_writes_multi_page);
 
ATOMIC64    align_iocount_writes_multi_aligned_page;
EXPORT_SYMBOL(align_iocount_writes_multi_aligned_page);

ATOMIC64    align_iocount_writes_multi_unaligned_page;
EXPORT_SYMBOL(align_iocount_writes_multi_unaligned_page);

///////////////////////////////////////
//Threads Related
///////////////////////////////////////

#define NUMBER_OF_ALIGN_READ_THREADS          2
#define NUMBER_OF_ALIGN_WRITE_THREADS         2

PCHAR ALIGN_WRITE_THREAD_POOL_NAME          = "awrite_thread_pool";
PCHAR ALIGN_WRITE_THREAD_POOL_SHORT_NAME    = "awtpool";
EXPORT_SYMBOL(ALIGN_WRITE_THREAD_POOL_NAME);
EXPORT_SYMBOL(ALIGN_WRITE_THREAD_POOL_SHORT_NAME);

PCHAR ALIGN_READ_THREAD_POOL_NAME           = "aread_thread_pool";
PCHAR ALIGN_READ_THREAD_POOL_SHORT_NAME     = "artpool";
EXPORT_SYMBOL(ALIGN_READ_THREAD_POOL_NAME);
EXPORT_SYMBOL(ALIGN_READ_THREAD_POOL_SHORT_NAME);

USHORT num_align_read_threads             = NUMBER_OF_ALIGN_READ_THREADS;
EXPORT_SYMBOL(num_align_read_threads);

USHORT num_align_write_threads            = NUMBER_OF_ALIGN_WRITE_THREADS;
EXPORT_SYMBOL(num_align_write_threads);

PDZ_THREAD_POOL align_read_thread_pool = NULL;
EXPORT_SYMBOL(align_read_thread_pool);

PDZ_THREAD_POOL align_write_thread_pool = NULL;
EXPORT_SYMBOL(align_write_thread_pool);

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

//BEC (Back End Cache) Engine

///////////////////////////////////////
//Stats Counters
///////////////////////////////////////
BEC_COUNTERS    dz_bec_counters;
PBEC_COUNTERS   pdz_bec_counters;
EXPORT_SYMBOL(dz_bec_counters);
EXPORT_SYMBOL(pdz_bec_counters);

///////////////////////////////////////
//Thread Pool Related 
///////////////////////////////////////
#define NUMBER_OF_BEC_THREADS           4

PCHAR BEC_THREAD_POOL_NAME            = "bec_thread_pool";
PCHAR BEC_THREAD_POOL_SHORT_NAME      = "bcpool";
EXPORT_SYMBOL(BEC_THREAD_POOL_NAME);
EXPORT_SYMBOL(BEC_THREAD_POOL_SHORT_NAME);


PDZ_THREAD_POOL bec_thread_pool         = NULL;
EXPORT_SYMBOL(bec_thread_pool);

USHORT num_bec_threads                  = NUMBER_OF_BEC_THREADS;
EXPORT_SYMBOL(num_bec_threads);

//Semaphore lock
DECLARE_RWSEM(bec_lock);

///////////////////////////////////////
//BEC Radix Tree
///////////////////////////////////////
LIST_HEAD           bec_list_head;
EXPORT_SYMBOL(bec_list_head);

PRADIX_TREE_ROOT    bec_radix_tree;
EXPORT_SYMBOL(bec_radix_tree);

ATOMIC64            bec_pba_page_count;
EXPORT_SYMBOL(bec_pba_page_count);

//Used when cache is completely full and there is a need to create some space
//for accomodating recent pages.
//This is a percentenage which tells how many lru pages needs to be evicted
INT                 bec_cutoff_nodes;
EXPORT_SYMBOL(bec_cutoff_nodes);
 
INT                 bec_cutoff_percent;
EXPORT_SYMBOL(bec_cutoff_percent);

ULONG       bec_size                    = 0;
EXPORT_SYMBOL(bec_size);

///////////////////////////////////////
//Cache Counters :-)
///////////////////////////////////////
ATOMIC64    bec_cache_hit;
EXPORT_SYMBOL(bec_cache_hit);
 
ATOMIC64    bec_cache_miss;
EXPORT_SYMBOL(bec_cache_miss);
 
ATOMIC64    bec_cache_delete;
EXPORT_SYMBOL(bec_cache_delete);

///////////////////////////////////////
//BEC Mempool related
///////////////////////////////////////
#define NUMBER_OF_BEC_NODE_POOL_ENTRIES     _128K
INT         dz_bec_node_mempool_count       = NUMBER_OF_BEC_NODE_POOL_ENTRIES;
EXPORT_SYMBOL(dz_bec_node_mempool_count);

INT         dz_pba_page_mempool_count       = NUMBER_OF_BEC_NODE_POOL_ENTRIES;
EXPORT_SYMBOL(dz_pba_page_mempool_count);

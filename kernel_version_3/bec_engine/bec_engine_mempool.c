/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "bec_engine.h"
#include "memory_pool_engine.h"
#include "thread_pool_engine.h"
#include "prototypes.h"

#define NUMBER_OF_BEC_NODE_POOL_ENTRIES		_128K
extern INT			dz_bec_node_mempool_count;
extern INT			dz_pba_page_mempool_count;

static struct kmem_cache *dz_bec_node_cache		= NULL;
PMEMPOOL		dz_bec_node_mempool				= NULL;		
PMEMPOOL		dz_bec_page_mempool				= NULL;		

// BEC Node Mempool Functions
RINT dz_create_bec_node_pool(SIZE num_nodes)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct dz_bec_node_s))

	dz_bec_node_cache	= KMEM_CACHE(dz_bec_node_s, 0);		
	dz_bec_node_mempool	= mempool_create_slab_pool(num_nodes, dz_bec_node_cache);
	if (dz_bec_node_mempool) {
		LOGBEC("BEC Node pool created for %d entries\n", (INT)num_nodes);
		log_memory_usage(num_nodes * DZ_UNIT_SIZE);
		RETURNS;
	} else {
		LOGBECE("BEC Node pool creation failed\n");
		RETURNF;
	}

	#undef DZ_UNIT_SIZE
}

RVOID dz_destroy_bec_node_pool(SIZE num_nodes)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct dz_bec_node_s))
	LOGBEC("Destroying BEC Node Pool\n");
	if (dz_bec_node_mempool) {
		mempool_destroy(dz_bec_node_mempool);
		kmem_cache_destroy(dz_bec_node_cache);
		reduce_memory_usage(num_nodes * DZ_UNIT_SIZE);
	} else {
		LOGBEC("dz_bec_node_mempool is already NULL\n");
	}
	#undef DZ_UNIT_SIZE
}

RPVOID dz_bec_node_alloc(VOID)
{
	PVOID ptr = NULL;
	if (!dz_bec_node_mempool) return NULL;
	ptr = mempool_alloc(dz_bec_node_mempool, GFP_NOIO);
	if (ptr) {
		BEC_COUNTER_ATOMIC64_INC(bec_node_alloc_cnt);
		return ptr;

	} else {
		return NULL;
	}
}

RVOID dz_bec_node_free(PBEC_NODE node) 
{
	if (!node) {
		LOGBECW("node is NULL\n");
		return;
	}
	BEC_COUNTER_ATOMIC64_INC(bec_node_free_cnt);
	mempool_free(node, dz_bec_node_mempool);
}

// BEC Page or Physical Block Page Page Mempool Functions
RINT dz_create_bec_page_pool(SIZE num_pages)
{
	#define	DZ_UNIT_SIZE		( PAGE_SIZE )

	dz_bec_page_mempool = mempool_create_page_pool(num_pages, 0);
	if (dz_bec_page_mempool) {
		LOGBEC("PB  Page pool created for %d pages\n", (INT)num_pages);
		log_memory_usage(num_pages * DZ_UNIT_SIZE);
		RETURNS;
	} else {
		LOGBECE("PB  Page pool creation failed\n");
		RETURNF;
	}

	#undef DZ_UNIT_SIZE
}

RVOID dz_destroy_bec_page_pool(SIZE num_pages)
{
	LOGBEC("Destroying PB  Page Pool\n");
	if (dz_bec_page_mempool) {
		mempool_destroy(dz_bec_page_mempool);
		reduce_memory_usage(num_pages * PAGE_SIZE);
	} else {
		LOGBEC("dz_bec_page_mempool is already NULL\n");
	}
}

RPVOID dz_bec_page_alloc(VOID)
{
	PVOID ptr = NULL;
	if (!dz_bec_page_mempool) return NULL;
	ptr = mempool_alloc(dz_bec_page_mempool, GFP_NOIO);
	if (ptr) {
		BEC_COUNTER_ATOMIC64_INC(bec_page_alloc_cnt);
		return ptr;
	} else {
		return NULL;
	}
}
EXPORT_SYMBOL(dz_bec_page_alloc);

RVOID dz_bec_page_free(PPAGE page) 
{
	if (!page) {
		LOGBECW("Page is NULL\n");
		return;
	}
	BEC_COUNTER_ATOMIC64_INC(bec_page_free_cnt);
	mempool_free(page, dz_bec_page_mempool);
}
EXPORT_SYMBOL(dz_bec_page_free);

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

extern PPBA_BLOCK			pba_table;
extern USHORT 				num_bec_threads;
extern INT					dz_bec_node_mempool_count;
extern INT					dz_pba_page_mempool_count;

extern LIST_HEAD			bec_list_head;
extern PRADIX_TREE_ROOT		bec_radix_tree;

extern ATOMIC64				bec_pba_page_count;
extern ATOMIC64				pba_cache_job_count;
extern INT					bec_cutoff_nodes;

extern ULONG				bec_size;
extern struct rw_semaphore bec_lock;

PBEC_NODE dz_bec_radix_tree_node_find(U64 pba_index)
{
	PBEC_NODE	node = NULL;

	node = radix_tree_lookup(bec_radix_tree, pba_index);
	if (node) {
		//Make the node to recent one so place it to start of list
		list_del(&node->pba_list);
		BEC_ADD_NODE_TO_LIST(node);
	}

	return node;
}

RINT dz_bec_radix_tree_node_insert(PPBA_CACHE_NODE node_entry)
{
	return radix_tree_insert(bec_radix_tree, node_entry->pba_index, node_entry);
}

RINT dz_bec_radix_tree_node_delete(U64 pba_index)
{
	PBEC_NODE	node = NULL;
	node = radix_tree_delete(bec_radix_tree, pba_index);
	if (node) {
		list_del(&node->pba_list);
		dz_bec_page_free(node->page);
		dz_bec_node_free(node);
		ATOMIC64_DEC(bec_pba_page_count);
		LOGBEC("Page deleted from Page Cache for pba_index = %lli, TC = %li\n",
			node->pba_index, ATOMIC64_READ(bec_pba_page_count));
		RETURNS;
	} else {
		LOGBECW("Unable to delete page @pba_index %lli. Looks like Page not found\n", pba_index);
	}
	RETURNF;
}

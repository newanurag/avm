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

extern 	INT      dz_switch_bec_engine;
extern 	INT      dz_switch_fec_engine;
extern	PCHAR BEC_THREAD_POOL_NAME;
extern	PCHAR BEC_THREAD_POOL_SHORT_NAME;
extern 	USHORT num_bec_threads;

extern 	PDZ_THREAD_POOL bec_thread_pool;
extern 	INT			dz_bec_node_mempool_count;
extern 	INT			dz_pba_page_mempool_count;
extern 	INT			dz_bec_request_mempool_count;
extern 	PPBA_BLOCK	pba_table;
extern 	USHORT 		num_bec_threads;
extern 	INT			dz_bec_node_mempool_count;
extern 	INT			dz_pba_page_mempool_count;

extern LIST_HEAD			bec_list_head;
extern PRADIX_TREE_ROOT	bec_radix_tree;

extern ATOMIC64			bec_pba_page_count;

//Used when cache is completely full and there is a need to create some space
//for accomodating recent pages.
//This is a percentenage which tells how many lru pages needs to be evicted
extern INT					bec_cutoff_nodes;
extern INT					bec_cutoff_percent;


extern ULONG		bec_size;

//U64			bec_engine_memory_usage		= 0;
//EXPORT_SYMBOL(bec_engine_memory_usage);

extern ATOMIC64	bec_cache_hit;
extern ATOMIC64	bec_cache_miss;
extern ATOMIC64	bec_cache_delete;

//DEFINE_SPINLOCK(bec_lock);
//DECLARE_RWSEM(bec_lock);

RINT dz_create_bec_thread_pool(INT num_threads)
{
	PDZ_THREAD_POOL tp = NULL;
	tp = dz_create_thread_pool(
		BEC_THREAD_POOL_NAME, BEC_THREAD_POOL_SHORT_NAME,
		num_threads, NULL, NULL, dz_bec_io);
	if (tp) {
		LOGBEC("%s created successfully for %d threads \n", 
			BEC_THREAD_POOL_NAME, num_threads);
		bec_thread_pool = tp;
		RETURNS;
	} else {
		LOGBECE("%s creation failed \n", BEC_THREAD_POOL_NAME);
		RETURNF;
	}
	return SUCCESS;
}

RVOID dz_destroy_bec_thread_pool(VOID)
{
	if (bec_thread_pool) {
		dz_destroy_thread_pool(bec_thread_pool);
	}
}

VOID dz_bec_print_counters(VOID)
{
	LOGBEC("bec_pba_page_count     = %lli\n", (U64)ATOMIC64_READ(bec_pba_page_count));
	LOGBEC("bec_cache_hit          = %lli\n", (U64)ATOMIC64_READ(bec_cache_hit));
	LOGBEC("bec_cache_miss         = %lli\n", (U64)ATOMIC64_READ(bec_cache_miss));
	LOGBEC("bec_cache_delete       = %lli\n", (U64)ATOMIC64_READ(bec_cache_delete));
	LOGBEC("bec_size               = %lli\n", (U64)bec_size);
	LOGBEC("bec_cutoff_nodes       = %lli\n", (U64)bec_cutoff_nodes);
	LOGBEC("bec_cutoff_percent     = %lli\n", (U64)bec_cutoff_percent);
	LOGBEC("bec_node_mempool_count = %lli\n", (U64)dz_bec_node_mempool_count);
}

VOID dz_bec_init_counters(VOID)
{
	ATOMIC64_SET(bec_pba_page_count, 0);
	ATOMIC64_SET(bec_cache_hit, 0);
	ATOMIC64_SET(bec_cache_miss, 0);
	ATOMIC64_SET(bec_cache_delete, 0);

	bec_cutoff_percent 	= BEC_PAGE_EVICT_PERCENTAGE;
	bec_size			= dz_bec_node_mempool_count;
	bec_cutoff_nodes 	= (bec_size * bec_cutoff_percent) / 100;

	BEC_COUNTER_U64_SET(bec_engine_memory_usage, 0);
	BEC_COUNTER_ATOMIC64_SET(bec_page_alloc_cnt, 0);
	BEC_COUNTER_ATOMIC64_SET(bec_page_free_cnt, 0);
	BEC_COUNTER_ATOMIC64_SET(bec_node_alloc_cnt, 0);
	BEC_COUNTER_ATOMIC64_SET(bec_node_free_cnt, 0);

	dz_bec_print_counters();
}

RINT dz_bec_engine_init(VOID)
{
	INT ret = SUCCESS;
	MARK_MEMORY_USAGE();

	if (dz_switch_bec_engine == DZ_DISABLED) {
		LOGBECF(" ______________________________________________________ \n");
		LOGBECF("|                                                      |\n");
		LOGBECF("|          BackEnd Cache (BEC) Engine Disabled         |\n");
		LOGBECF("|______________________________________________________|\n");
		PRINT_MEMORY_USAGE(BEC_COUNTER_U64_GET(bec_engine_memory_usage), "BEC Engine");
		RETURNS;
    }



	//bec_pba_page_count = ATOMIC64_INIT(0);
	LOGBECF(" ______________________________________________________ \n");
	LOGBECF("|                                                      |\n");
	LOGBECF("|   Backend Cache (BEC) Engine Initialization Begin    |\n");
	LOGBECF("|______________________________________________________|\n");
	INIT_LIST_HEAD(&bec_list_head);
	dz_bec_init_counters();

	ret = dz_create_bec_thread_pool(num_bec_threads);

	bec_radix_tree = dz_kmalloc(sizeof(RADIX_TREE_ROOT), GFP_KERNEL|__GFP_ZERO);
	if (!bec_radix_tree) {
		return -ENOMEM;
	}
	INIT_RADIX_TREE(bec_radix_tree, GFP_ATOMIC);
	ret = dz_create_bec_node_pool	(dz_bec_node_mempool_count);
	ret = dz_create_bec_page_pool	(dz_bec_node_mempool_count);
	if (radix_tree_preload(GFP_KERNEL)) {
		LOGBECE("Radix tree preload failed\n");
	} else {
		LOGBEC("Radix tree preloaded successfully\n");
	}
	PRINT_MEMORY_USAGE( BEC_COUNTER_U64_GET(bec_engine_memory_usage), "BEC Engine");
	LOGBECF(" ______________________________________________________ \n");
	LOGBECF("|                                                      |\n");
	LOGBECF("|     Backend Cache (BEC) Engine Initialization End    |\n");
	LOGBECF("|______________________________________________________|\n");
	RETURNS;
}
EXPORT_SYMBOL(dz_bec_engine_init);

RINT dz_bec_engine_exit(VOID)
{

	MARK_MEMORY_USAGE();
	if (dz_switch_bec_engine == DZ_DISABLED) {
		LOGBECF(" ______________________________________________________ \n");
		LOGBECF("|                                                      |\n");
		LOGBECF("|          BackEnd Cache (BEC) Engine Disabled         |\n");
		LOGBECF("|______________________________________________________|\n");
		PRINT_MEMORY_USAGE(BEC_COUNTER_U64_GET(bec_engine_memory_usage), "BEC Engine");
		RETURNS;
    }

	LOGBECF(" ______________________________________________________ \n");
	LOGBECF("|                                                      |\n");
	LOGBECF("|        Backend Cache (BEC) Engine Exit Begin         |\n");
	LOGBECF("|______________________________________________________|\n");
	dz_bec_print_counters();
	dz_destroy_bec_thread_pool();
	if (bec_radix_tree) {
		dz_kfree(bec_radix_tree, sizeof(RADIX_TREE_ROOT));
	}
	dz_destroy_bec_node_pool	(dz_bec_node_mempool_count);
	dz_destroy_bec_page_pool	(dz_bec_node_mempool_count);
	PRINT_MEMORY_FREED(BEC_COUNTER_U64_GET(bec_engine_memory_usage), "BEC Engine");
	LOGBECF(" ______________________________________________________ \n");
	LOGBECF("|                                                      |\n");
	LOGBECF("|        Backend Cache (BEC) Engine Exit End           |\n");
	LOGBECF("|______________________________________________________|\n");
	RETURNS;
}
EXPORT_SYMBOL(dz_bec_engine_exit);

RVOID dz_bec_proceed_pba_page_add_read(PIOREQUEST iorequest)
{
	iorequest_get(iorequest);		
	iorequest->ior_bec_request_type = BEC_REQUEST_TYPE_ADD_READ;
	iorequest->ior_thread_pool = bec_thread_pool;
	dz_q_iorequest(bec_thread_pool, iorequest);
}
EXPORT_SYMBOL(dz_bec_proceed_pba_page_add_read);

RVOID dz_bec_proceed_pba_page_add_write(PIOREQUEST iorequest)
{
	iorequest_get(iorequest);		
	iorequest->ior_bec_request_type = BEC_REQUEST_TYPE_ADD_WRITE;
	//iorequest->ior_thread_pool = bec_thread_pool;
	dz_q_iorequest(bec_thread_pool, iorequest);
}
EXPORT_SYMBOL(dz_bec_proceed_pba_page_add_write);

RVOID dz_bec_proceed_pba_page_delete(PIOREQUEST iorequest)
{
	//Note : There is no bio involved
	//iorequest_get(iorequest);		
	//iorequest_get is invoked when there is a need to update
	//old entries during dedupe write.
	iorequest->ior_bec_request_type = BEC_REQUEST_TYPE_DELETE;
	//iorequest->ior_thread_pool = bec_thread_pool;
	dz_q_iorequest(bec_thread_pool, iorequest);
}
EXPORT_SYMBOL(dz_bec_proceed_pba_page_delete);

RVOID dz_bec_io(PIOREQUEST iorequest)
{	
	switch(iorequest->ior_bec_request_type) {
		case BEC_REQUEST_TYPE_ADD_READ:
			if (dz_switch_fec_engine == DZ_DISABLED) {
				dz_bec_pba_page_add_read_without_fec(iorequest->ior_lba, iorequest);
				return;
			}
			dz_bec_pba_page_add_read(iorequest->ior_sector, iorequest, DZ_BEC_PAGE_SIZE);
		break;

		case BEC_REQUEST_TYPE_ADD_WRITE:
			dz_bec_pba_page_add_write(iorequest->ior_lba, iorequest, DZ_BEC_PAGE_SIZE);
		break;

		case BEC_REQUEST_TYPE_DELETE:
			dz_bec_pba_page_delete(iorequest->ior_sector, iorequest, DZ_BEC_PAGE_SIZE);
		break;

		default:
		//dz_bec_request_free(becr);
		LOGBECW("BEC Request Unknown\n");
		return;
	}
}

RINT dz_bec_pba_page_add_read(U64 pba_index, PIOREQUEST iorequest, UINT data_size)
{
	PBIO			bio		= iorequest->ior_bio;
	PPBA_CACHE_NODE	node	= NULL;
	INT ret					= SUCCESS;

	_ACQUIRE_BEC_LOCK_READ();
	node = dz_bec_radix_tree_node_find(pba_index);
	if (node) {
		if (bio->bi_vcnt == 1) {
			COPY_SINGLE_PAGE_FROM_BIO(node->page, bio);
		} else {
			COPY_PAGE_FROM_BIO(node->page, bio);
		}
		DZ_OS_KERNEL_BIO_PUT(bio);
		iorequest_put(iorequest);
		_RELEASE_BEC_LOCK_READ();
		RETURNS;
	}
	_RELEASE_BEC_LOCK_READ();

	node = dz_bec_node_alloc();
	if (!node) {
		LOGBECW("READ:All PBA Cache nodes are exhausted. Time to evict LRU pages\n");
		goto exit_failure;
	} else {
		node->page = dz_bec_page_alloc();
		if (!node->page) {
			LOGBECW("READ:All PBA Cache pages are exhausted. Time to evict LRU pages\n");
			goto exit_failure;
		}
	}
	if (bio->bi_vcnt == 1) {
		COPY_SINGLE_PAGE_FROM_BIO(node->page, bio);
	} else {
		COPY_PAGE_FROM_BIO(node->page, bio);
	}
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(iorequest);
	node->pba_index = pba_index;
	
	_ACQUIRE_BEC_LOCK_WRITE();
	ret = dz_bec_radix_tree_node_insert(node);
	if (ret == SUCCESS) {
		BEC_ADD_NODE_TO_LIST(node);
		_RELEASE_BEC_LOCK_WRITE();

		ATOMIC64_INC(bec_pba_page_count);
		//LOGBEC("Page added in Page Cache for pba_index = %lli, TC = %li\n",
		//	node->pba_index, ATOMIC64_READ(bec_pba_page_count));
		if (ATOMIC64_READ(bec_pba_page_count) >= bec_size) {
			//TODO:Evict LRU pages from cache
			//We need to do it asynchronously
			LOGBEC("READ:Invoking bec clear since page cache is full\n");
			dz_bec_print_counters();

			_ACQUIRE_BEC_LOCK_WRITE();
			dz_bec_clear(bec_cutoff_nodes);
			_RELEASE_BEC_LOCK_WRITE();
		} else {
			if (radix_tree_preload(GFP_KERNEL)) {
				LOGBECE("READ:Radix tree preload failed\n");
			}

		}
		RETURNS;
	} else {
		_RELEASE_BEC_LOCK_WRITE();

		LOGBECW("READ:Unable to add node into radix tree. Invoking radix_tree_preload\n");
		dz_bec_print_counters();
		if (radix_tree_preload(GFP_KERNEL)) {
			//dz_bec_clear(bec_cutoff_nodes);
			LOGBECE("READ:Radix tree preload failed\n");
		}
	}
	RETURNF;

exit_failure:
	DZ_OS_KERNEL_BIO_PUT(bio);
	iorequest_put(iorequest);
	if (node) {
		if (node->page) {
			dz_bec_page_free(node->page);
		}
		dz_bec_node_free(node);
	}
	dz_bec_print_counters();

	_ACQUIRE_BEC_LOCK_WRITE();
	dz_bec_clear(bec_cutoff_nodes);
	_RELEASE_BEC_LOCK_WRITE();
	
	RETURNF;
}




//This function is applicable when FEC is disabled and/or Dedupe Engine is also disabled
// Note cache_page is passed from the caller, where the disk read is already happened
//Note that no extra cache_page is allocated. If the passed cache_page is utilized
//then unused page from bec is freed
RINT dz_bec_pba_page_add_read_without_fec(U64 pba_index, PIOREQUEST iorequest)
{
	PPBA_CACHE_NODE	node	= NULL;
	PPAGE	cache_page		= iorequest->ior_cache_page;
	INT ret					= SUCCESS;

	_ACQUIRE_BEC_LOCK_READ();
	node = dz_bec_radix_tree_node_find(pba_index);
	LOGBEC("Inside dz_bec_pba_page_add_read_without_fec: node = %p, pba_index = %llu\n", node, pba_index);
	if (node) {
		dz_bec_page_free(node->page);
		node->page = cache_page;
		_RELEASE_BEC_LOCK_READ();
		iorequest_put(iorequest);
		RETURNS;
	}
	_RELEASE_BEC_LOCK_READ();

	LOGBEC("AVERMA:BEC1\n");

	node = dz_bec_node_alloc();
	if (!node) {
		dz_bec_page_free(cache_page);
		LOGBECW("READ:All PBA Cache nodes are exhausted. Time to evict LRU pages\n");
		goto exit_failure;
	} else {
		node->page = cache_page;
	}
	node->pba_index = pba_index;

	LOGBEC("AVERMA:BEC2\n");
	iorequest_put(iorequest);
	
	_ACQUIRE_BEC_LOCK_WRITE();
	ret = dz_bec_radix_tree_node_insert(node);
	LOGBEC("AVERMA:BEC3\n");
	if (ret == SUCCESS) {
		BEC_ADD_NODE_TO_LIST(node);
		_RELEASE_BEC_LOCK_WRITE();

		ATOMIC64_INC(bec_pba_page_count);
		LOGBEC("Page added in Page Cache for pba_index = %lli, TC = %li\n",
			node->pba_index, ATOMIC64_READ(bec_pba_page_count));
		if (ATOMIC64_READ(bec_pba_page_count) >= bec_size) {
			//TODO:Evict LRU pages from cache
			//We need to do it asynchronously
			LOGBEC("READ:Invoking bec clear since page cache is full\n");
			dz_bec_print_counters();

			_ACQUIRE_BEC_LOCK_WRITE();
			dz_bec_clear(bec_cutoff_nodes);
			_RELEASE_BEC_LOCK_WRITE();
		} else {
			if (radix_tree_preload(GFP_KERNEL)) {
				LOGBECE("READ:Radix tree preload failed\n");
			}

		}
		RETURNS;
	} else {
		_RELEASE_BEC_LOCK_WRITE();

		LOGBECW("READ:Unable to add node into radix tree. Invoking radix_tree_preload\n");
		dz_bec_print_counters();
		if (radix_tree_preload(GFP_KERNEL)) {
			//dz_bec_clear(bec_cutoff_nodes);
			LOGBECE("READ:Radix tree preload failed\n");
		}
	}
	LOGBEC("AVERMA:BEC4\n");
	RETURNF;

exit_failure:
	iorequest_put(iorequest);
	if (node) {
		if (node->page) {
			dz_bec_page_free(node->page);
		}
		dz_bec_node_free(node);
	}
	dz_bec_print_counters();

	_ACQUIRE_BEC_LOCK_WRITE();
	dz_bec_clear(bec_cutoff_nodes);
	_RELEASE_BEC_LOCK_WRITE();
	
	RETURNF;
}

//Note: This function is copying the fec buffer 
//to pba node page. This is different than add_read functionality.
//This is because it is an internally generated iorequest whose parent is flush io.
RINT dz_bec_pba_page_add_write(U64 pba_index, PIOREQUEST iorequest, UINT data_size)
{
	PPBA_CACHE_NODE	node	= NULL;
	INT				ret		= SUCCESS;

	_ACQUIRE_BEC_LOCK_READ();
	node = dz_bec_radix_tree_node_find(pba_index);
	if (node) {
		COPY_BUFFER_TO_PAGE(node->page, iorequest->ior_bv_buffer);
		_RELEASE_BEC_LOCK_READ();
		DZ_BEC_IOREQUEST_DONE(iorequest);
		RETURNS;
	}
	_RELEASE_BEC_LOCK_READ();

	node = dz_bec_node_alloc();
	if (!node) {
exit_here:
		DZ_BEC_IOREQUEST_DONE(iorequest);
		LOGBECW("Write:All PBA Cache nodes are exhausted. Time to evict LRU pages\n");
		dz_bec_print_counters();

		_ACQUIRE_BEC_LOCK_WRITE();
		dz_bec_clear(bec_cutoff_nodes);
		_RELEASE_BEC_LOCK_WRITE();

		RETURNF;
	} else {
		node->page = dz_bec_page_alloc();
		if (!node->page) {
			LOGBECW("READ:All PBA Cache pages are exhausted. Time to evict LRU pages\n");
			goto exit_here;
		}
	}

	COPY_BUFFER_TO_PAGE(node->page, iorequest->ior_bv_buffer);
	DZ_BEC_IOREQUEST_DONE(iorequest);
	node->pba_index = pba_index;

	_ACQUIRE_BEC_LOCK_WRITE();
	ret = dz_bec_radix_tree_node_insert(node);
	if (ret == SUCCESS) {
		BEC_ADD_NODE_TO_LIST(node);
		_RELEASE_BEC_LOCK_WRITE();

		LOGD("Node added to BEC Cache:pba_index = %lli\n", pba_index);
		ATOMIC64_INC(bec_pba_page_count);
		if (ATOMIC64_READ(bec_pba_page_count) >= bec_size) {
			LOGBEC("Write:Invoking bec clear since page cache is full\n");
			dz_bec_print_counters();

			_ACQUIRE_BEC_LOCK_WRITE();
			dz_bec_clear(bec_cutoff_nodes);
			_RELEASE_BEC_LOCK_WRITE();

		} else {
			if (radix_tree_preload(GFP_KERNEL)) {
				LOGBECE("Write:Radix tree preload failed\n");
			}

		}
		RETURNS;
	} else {
		_RELEASE_BEC_LOCK_WRITE();

		LOGBECW("Write:Unable to add node into radix tree. Invoking radix_tree_preload\n");
		dz_bec_page_free(node->page);
		dz_bec_node_free(node);
		dz_bec_print_counters();
		if (radix_tree_preload(GFP_KERNEL)) {
			LOGBECE("Write:Radix tree preload failed\n");
		}
	}
	RETURNF;
}

RINT dz_bec_pba_page_delete(U64 pba_index, PIOREQUEST iorequest , UINT data_size)
{
	INT ret = SUCCESS;
	_ACQUIRE_BEC_LOCK_WRITE();
	ret = dz_bec_radix_tree_node_delete(iorequest->ior_sector);
	_RELEASE_BEC_LOCK_WRITE();

	iorequest_put(iorequest);
	if (ret == SUCCESS) {
		ATOMIC64_INC(bec_cache_delete);
	}
	RETURNS;
}

//Remove cutoff count pages from cache
//TODO: Refine the return codes
RINT dz_bec_clear(INT node_count_cutoff)
{
	PBEC_NODE	node = NULL;
	INT			cleared_nodes = 0;
	INT			ret = SUCCESS;
	INT			count = 0;

	LOGBEC("Deleting %d pages from bec\n", node_count_cutoff);

	//bec_pba_page_count is getting decremented inside radix_tree_node_delete function
	//
	//LOGBEC("Initial Page count %li \n", ATOMIC64_READ(bec_pba_page_count));
	while( count++ < node_count_cutoff) {
		if (!list_empty(&bec_list_head)) {
			node = list_first_entry(&bec_list_head, struct dz_bec_node_s, pba_list);
			if ((ret = dz_bec_radix_tree_node_delete(node->pba_index))) {
				//TODO:Add log message
				LOGBEC("Page deletion failure for pba_index = %lli, TC = %li\n",
				node->pba_index, ATOMIC64_READ(bec_pba_page_count));
				return -EINVAL;
			} else {
				cleared_nodes++;
			}
		} else {
			break;
		}
	}
	//LOGBEC("Final Page count %li \n", ATOMIC64_READ(bec_pba_page_count));

	if (!ret) {
		return BEC_CLEARED;
	} else {
		if (cleared_nodes) {
			return BEC_PARTIAL_CLEARED;
		} else {
			return BEC_FAILED_TO_CLEAR;
		}
	}

}

BOOL dz_bec_pba_page_lookup(U64 pba_index, PBIO bio)
{
	PBEC_NODE	node 		= NULL;
	BOOL		status		= TRUE;

	_ACQUIRE_BEC_LOCK_READ();
	node = dz_bec_radix_tree_node_find(pba_index);
	if (!node) {
		_RELEASE_BEC_LOCK_READ();
		//LOGBEC("Page NOT found in Page Cache for pba_index = %lli\n", pba_index);
		ATOMIC64_INC(bec_cache_miss);
		return FALSE;
	}

	_RELEASE_BEC_LOCK_READ();

	//LOGBEC("Page Served from Page Cache for pba_index = %lli\n", node->pba_index);
	COPY_PAGE_TO_BIO_ATOMIC(bio, node->page, status);
	ATOMIC64_INC(bec_cache_hit);
	return status;
}
EXPORT_SYMBOL(dz_bec_pba_page_lookup);

static int dz_bec_engine_module_init(void)
{
	LOGBEC(" ______________________________________________________ \n");
	LOGBEC("|                                                      |\n");
	LOGBEC("|           ZAIDSTOR BEC Engine Module Loaded          |\n");
	LOGBEC("|______________________________________________________|\n");

	RETURNS;
}

static void dz_bec_engine_module_exit(void)
{
	LOGBEC(" ______________________________________________________ \n");
	LOGBEC("|                                                      |\n");
	LOGBEC("|           ZAIDSTOR BEC Engine Module UnLoaded        |\n");
	LOGBEC("|______________________________________________________|\n");
}

module_init(dz_bec_engine_module_init);
module_exit(dz_bec_engine_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

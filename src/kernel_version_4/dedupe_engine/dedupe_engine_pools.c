/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "dedupe_engine.h"
#include "prototypes.h"

extern INT		dz_switch_fec_engine;


extern PCHAR	DEDUPE_THREAD_POOL_NAME;
extern PCHAR	DEDUPE_THREAD_POOL_SHORT_NAME;

extern PCHAR	DEDUPE_READ_THREAD_POOL_NAME;
extern PCHAR	DEDUPE_READ_THREAD_POOL_SHORT_NAME;

extern PCHAR	DEDUPE_WRITE_THREAD_POOL_NAME;
extern PCHAR	DEDUPE_WRITE_THREAD_POOL_SHORT_NAME;

extern PDZ_THREAD_POOL read_thread_pool;
extern PDZ_THREAD_POOL write_thread_pool;

#define NUMBER_OF_HASH_POOL_ENTRIES		_256K
#define NUMBER_OF_DEDUPE_POOL_ENTRIES	_256K

extern SIZE				TARGET_DEVICE_SIZE; // In terms of 4K blocks
extern PAVM_DEVICE 		aisa_device_object;

PDZ_THREAD_POOL dedupe_thread_pool 			= NULL;
EXPORT_SYMBOL(dedupe_thread_pool);

PDZ_THREAD_POOL rdedupe_thread_pool 		= NULL;
EXPORT_SYMBOL(rdedupe_thread_pool);

PDZ_THREAD_POOL wdedupe_thread_pool 		= NULL;
EXPORT_SYMBOL(wdedupe_thread_pool);

PMEMPOOL		dz_hash_mempool				= NULL;		
PMEMPOOL		dz_dedupe_mempool			= NULL;		
static struct 	kmem_cache *dz_hash_cache	= NULL;
static struct 	kmem_cache *dz_dedupe_cache	= NULL;
INT	dz_hash_mempool_count			= NUMBER_OF_HASH_POOL_ENTRIES;
INT	dz_dedupe_mempool_count			= NUMBER_OF_DEDUPE_POOL_ENTRIES;


//Dedupe Mempool Functions
RINT dz_create_dedupe_pool(UINT num_dedupes)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct dz_dedupe_info_s))
	dz_dedupe_cache = KMEM_CACHE(dz_dedupe_info_s, 0);		
	dz_dedupe_mempool = mempool_create_slab_pool(num_dedupes, dz_dedupe_cache);
	if (dz_dedupe_mempool) {
		LOG("dedupe  pool created for accomodating %u units. Unit Size %lu \n",
			num_dedupes, DZ_UNIT_SIZE);
		log_memory_usage(num_dedupes * DZ_UNIT_SIZE);
		RETURNS;
	} else {
		LOGE("dedupe pool creation failed\n");
		RETURNF;
	}
	#undef DZ_UNIT_SIZE
}

RVOID dz_destroy_dedupe_pool(UINT num_dedupes)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct dz_dedupe_info_s))
	LOG("Destroying dedupe Pool\n");
	mempool_destroy(dz_dedupe_mempool);
	kmem_cache_destroy(dz_dedupe_cache);
	reduce_memory_usage(num_dedupes * DZ_UNIT_SIZE);
	#undef DZ_UNIT_SIZE
}

RPVOID dz_dedupe_alloc(VOID)
{
	LOGD("dedupe Space Allocated\n");
	if (!dz_dedupe_mempool) return NULL;
	return mempool_alloc(dz_dedupe_mempool	, GFP_KERNEL);
}

RVOID dz_dedupe_free(PVOID dedupe)
{
	if (!dedupe) {
		LOGW("dedupe is NULL\n");
		return;
	}
	mempool_free(dedupe, dz_dedupe_mempool	);
	LOGD("dedupe Space Freed %p\n", dedupe);
}

// Hash Mempool Functions
RINT dz_create_hash_pool(UINT num_hashs)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct dz_hash_s))
	dz_hash_cache = KMEM_CACHE(dz_hash_s, 0);		
	dz_hash_mempool = mempool_create_slab_pool(num_hashs, dz_hash_cache);
	if (dz_hash_mempool) {
		LOG("Hash  pool created for accomodating %u units. Unit Size %lu \n",
			num_hashs, DZ_UNIT_SIZE);
		log_memory_usage(num_hashs * DZ_UNIT_SIZE);
		RETURNS;
	} else {
		LOGE("hash pool creation failed\n");
		RETURNF;
	}
	#undef DZ_UNIT_SIZE
}

RVOID dz_destroy_hash_pool(UINT num_hashs)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct dz_hash_s))
	LOG("Destroying hash Pool\n");
	mempool_destroy(dz_hash_mempool);
	kmem_cache_destroy(dz_hash_cache);
	reduce_memory_usage(num_hashs * DZ_UNIT_SIZE);
	#undef DZ_UNIT_SIZE
}

RPVOID dz_hash_alloc(VOID)
{
	LOG("Hash Space Allocated\n");
	if (!dz_hash_mempool) return NULL;
	return mempool_alloc(dz_hash_mempool	, GFP_KERNEL);
}

RVOID dz_hash_free(PVOID hash)
{
	if (!hash) {
		LOGW("Hash is NULL\n");
		return;
	}
	mempool_free(hash, dz_hash_mempool	);
	LOG("Hash Space Freed %p\n", hash);
}

//Thread Pool functions
RVOID dz_destroy_dedupe_thread_pool(VOID)
{
	//dz_destroy_thread_pool(dedupe_thread_pool);
	dz_destroy_thread_pool(rdedupe_thread_pool);
	dz_destroy_thread_pool(wdedupe_thread_pool);
}

RINT dz_create_dedupe_thread_pool(INT num_threads)
{
	PDZ_THREAD_POOL tp = NULL;
	/*
	tp = dz_create_thread_pool(
		DEDUPE_THREAD_POOL_NAME, DEDUPE_THREAD_POOL_SHORT_NAME,
		num_threads, NULL, NULL, dz_dedupe_io);
	if (tp) {
		LOG("%s created successfully for %d threads \n", 
			DEDUPE_THREAD_POOL_NAME, num_threads);
		dedupe_thread_pool = tp;
		RETURNS;
	} else {
		LOGE("%s creation failed \n", DEDUPE_THREAD_POOL_NAME);
		RETURNF;
	}
	*/
	
	//Create thread pool for Reads
	tp = dz_create_thread_pool(
		DEDUPE_READ_THREAD_POOL_NAME, DEDUPE_READ_THREAD_POOL_SHORT_NAME,
		num_threads, NULL, NULL, dz_dedupe_read_io);
	if (tp) {
		LOG("%s created successfully for %d threads \n", 
			DEDUPE_READ_THREAD_POOL_NAME, num_threads);
		rdedupe_thread_pool = tp;
		//RETURNS;
	} else {
		LOGE("%s creation failed \n", DEDUPE_READ_THREAD_POOL_NAME);
		RETURNF;
	}

	//Create thread pool for Writes
	tp = dz_create_thread_pool(
		DEDUPE_WRITE_THREAD_POOL_NAME, DEDUPE_WRITE_THREAD_POOL_SHORT_NAME,
		num_threads, NULL, NULL, dz_dedupe_write_io);
	if (tp) {
		LOG("%s created successfully for %d threads \n", 
			DEDUPE_WRITE_THREAD_POOL_NAME, num_threads);
		wdedupe_thread_pool = tp;
		RETURNS;
	} else {
		LOGE("%s creation failed \n", DEDUPE_WRITE_THREAD_POOL_NAME);
		dz_destroy_thread_pool(rdedupe_thread_pool);
		RETURNF;
	}

	/*
	if (dz_fec_engine_switch == DZ_DISABLED) {

		read_thread_pool 	= rdedupe_thread_pool;
		write_thread_pool 	= wdedupe_thread_pool;
	}
	*/
	
	return SUCCESS;
}

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

#define NUMBER_OF_BIO_POOL_ENTRIES			_256K
#define NUMBER_OF_PAGE_POOL_ENTRIES			_512K
#define NUMBER_OF_READ_PAGE_POOL_ENTRIES	_256K
#define NUMBER_OF_WRITE_PAGE_POOL_ENTRIES	_256K
#define NUMBER_OF_IO_POOL_ENTRIES			_256K

typedef RINT 	(*	CREATE_FUN	)(UINT 	);
typedef RVOID 	(*	DESTROY_FUN	)(VOID 	);
typedef RPVOID 	(*	ALLOC_FUN	)(VOID 	);
typedef RVOID 	(*	FREE_FUN	)(PVOID );

static INT 	dz_bio_mempool_count			= NUMBER_OF_BIO_POOL_ENTRIES;
static INT	dz_page_mempool_count			= NUMBER_OF_PAGE_POOL_ENTRIES;
//static INT	dz_read_page_mempool_count		= NUMBER_OF_READ_PAGE_POOL_ENTRIES;
//static INT	dz_write_page_mempool_count		= NUMBER_OF_WRITE_PAGE_POOL_ENTRIES;
static INT	dz_io_mempool_count				= NUMBER_OF_IO_POOL_ENTRIES;

//ATOMIC64	dz_iorequest_alloc_cnt;
//ATOMIC64	dz_iorequest_free_cnt;
//extern BASIC_COUNTERS  dz_basic_counters;
//extern PBASIC_COUNTERS pdz_basic_counters;

//EXPORT_SYMBOL(dz_iorequest_alloc_cnt);
//EXPORT_SYMBOL(dz_iorequest_free_cnt);

//ATOMIC64	dz_page_alloc_cnt;
//ATOMIC64	dz_page_free_cnt;
//EXPORT_SYMBOL(dz_page_alloc_cnt);
//EXPORT_SYMBOL(dz_page_free_cnt);

PBIO_SET		dz_bio_mempool			= NULL;		
PMEMPOOL		dz_page_mempool			= NULL;		
PMEMPOOL		dz_read_page_mempool	= NULL;		
PMEMPOOL		dz_write_page_mempool	= NULL;		
PMEMPOOL		dz_io_mempool			= NULL;		

static struct kmem_cache *dz_io_cache;

//ATOMIC64 dz_total_memory_consumed_by_all_pools;
//EXPORT_SYMBOL(dz_total_memory_consumed_by_all_pools);

// Function for summing up total memory consumption
INLINE VOID log_memory_usage(U64 count)
{
	BASE_COUNTER_ATOMIC64_ADD(dz_total_memory_consumed_by_all_pools, count);
	count = BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools);
	LOGINF("Memory Usage: %llu\n", count);
}
EXPORT_SYMBOL(log_memory_usage);

// Reverse of log_memory_usage
INLINE VOID reduce_memory_usage(U64 count)
{
	BASE_COUNTER_ATOMIC64_SUB(dz_total_memory_consumed_by_all_pools, count);
	count = BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools);
	LOGINF("Reduced Memory Usage: %llu\n", count);
}
EXPORT_SYMBOL(reduce_memory_usage);

U64 dz_get_memory_usage(VOID)
{
	return BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools);
}
EXPORT_SYMBOL(dz_get_memory_usage);

RVOID dz_memory_consumption(U64 initial, U64 *usage, CHAR *marker)
{
	U64 final = BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools);
	*usage = final - initial;

	LOGINF("\n");
	LOGINF("Total Memory Consumed by %s:\n", marker);
	LOGINF("Enter :    %lli MB | %lli KB | %lli Bytes\n",
		((initial)/MB), ((initial)/KB), (initial));
	LOGINF("Exit  :    %lli MB | %lli KB | %lli Bytes\n",
		((final)/MB), ((final)/KB), (final));
	LOGINF("Usage :    %lli MB | %lli KB | %lli Bytes\n",
		((*usage)/MB), ((*usage)/KB), (*usage));
}
EXPORT_SYMBOL(dz_memory_consumption);

RVOID dz_memory_reduction(U64 initial, U64 *reduced, CHAR *marker)
{
	U64 final = BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools);
	*reduced = initial - final;

	LOGINF("\nTotal Memory Reduced by %s:\n", marker);
	LOGINF("Enter   :    %lli MB | %lli KB | %lli Bytes\n",
		((initial)/MB), ((initial)/KB), (initial));
	LOGINF("Exit    :    %lli MB | %lli KB | %lli Bytes\n",
		((final)/MB), ((final)/KB), (final));
	LOGINF("Reduced :    %lli MB | %lli KB | %lli Bytes\n",
		((*reduced)/MB), ((*reduced)/KB), (*reduced));
}
EXPORT_SYMBOL(dz_memory_reduction);

RVOID dz_total_memory_consumption(VOID)
{
	U64 final = BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools);
	LOGINF("\n");
	LOGINF("Total Memory Consumed :%lli MB | %lli KB | %lli Bytes\n\n",
		((final)/MB), ((final)/KB), (final));
}
EXPORT_SYMBOL(dz_total_memory_consumption);

RVOID dz_check_memory_leak(VOID)
{
	if (BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools) > 0) {
		LOGW("<-------Memory Leaks Encountered------->\n");
		//LOGW("<-------Memory Leaks in Bytes %lld --->\n", //TODO:Earlier it was %lu
		LOGW("<-------Memory Leaks in Bytes %lu --->\n", //TODO:Earlier it was %lu
			BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools));
	} else if (BASE_COUNTER_ATOMIC64_READ(dz_total_memory_consumed_by_all_pools) < 0) {
		LOGW("<-------Memory Freed More than once ------->\n");
	} else {
		LOGINF("Congratulations ! No Memory leaks encountered\n");
	}
}
EXPORT_SYMBOL(dz_check_memory_leak);

// bio mempool functions
RINT dz_create_bio_pool(UINT num_bios)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct bio))
	//dz_bio_mempool = bioset_create(num_bios, 0);
	dz_bio_mempool = NULL;//TODO:
	if (dz_bio_mempool) {
		LOGINF("bio pool created for %d bios\n", num_bios);
		log_memory_usage(num_bios * DZ_UNIT_SIZE);
		return SUCCESS;
	} else {
		LOGINF("bio pool creation failed\n");
		return FAILURE;
	}
	#undef DZ_UNIT_SIZE
}
EXPORT_SYMBOL(dz_create_bio_pool);

RVOID dz_destroy_bio_pool(UINT num_bios)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct bio))
	LOGINF("Destroying bioset / biopool\n");
	if (dz_bio_mempool) {
		//bioset_free(dz_bio_mempool);TODO:
		reduce_memory_usage(num_bios * DZ_UNIT_SIZE);
	} else {
		LOGINF("dz_bio_mempool is NULL\n");
	}
	dz_bio_mempool = NULL;
	#undef DZ_UNIT_SIZE
}
EXPORT_SYMBOL(dz_destroy_bio_pool);

PBIO dz_bio_alloc(UINT nr_iovecs) 
{
	if (!dz_bio_mempool) return NULL;
	return bio_alloc_bioset (GFP_NOIO, nr_iovecs, dz_bio_mempool);
}
EXPORT_SYMBOL(dz_bio_alloc);

RVOID dz_bio_alloc_test(VOID)
{
	INT i = 0;
	for (i = 0; i < 2 * dz_bio_mempool_count; i++) {
		PBIO bio = dz_bio_alloc(1);
		if (bio == NULL) {
			LOG("Bio Allocation Failed at count %d\n", i);
		} else {
			LOGINF("Bio Allocation at %d is %p\n", i, (void *)bio);
		}
	}

	//NOTE : The outcome of this test is that bio allocation continued
	//for more than twice of dz_mempool_bio_count. 
	//It means that bioset can allow to allocatre more no. of bios than
	//the pool elements given at the time of bioset creation 
}
EXPORT_SYMBOL(dz_bio_alloc_test);

RVOID dz_bio_free(PBIO bio)
{
	//Kernel itself frees the bio when its reference count reaches to zero.
	//See the bio_put() code 
	//
	//	if (atomic_dec_and_test(&bio->bi_cnt))
	//	    bio_free(bio);
};
EXPORT_SYMBOL(dz_bio_free);

// IO Info mempool Functions
RINT dz_create_io_pool(UINT num_ios)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct dz_iorequest_s))
	dz_io_cache = KMEM_CACHE(dz_iorequest_s, 0);		
	dz_io_mempool	 = mempool_create_slab_pool(num_ios, dz_io_cache);


	if (dz_io_mempool	) {
		LOGINF("IO Info pool created for accomodating %u IOs. Unit Size %lu \n",
			num_ios, DZ_UNIT_SIZE);
		log_memory_usage(num_ios * DZ_UNIT_SIZE);
		BASE_COUNTER_ATOMIC64_SET(dz_iorequest_alloc_cnt, 0);
		BASE_COUNTER_ATOMIC64_SET(dz_iorequest_free_cnt, 0);
		RETURNS;
	} else {
		LOGE("IO Info pool creation failed\n");
		RETURNF;
	}

	#undef DZ_UNIT_SIZE
}
EXPORT_SYMBOL(dz_create_io_pool);

RVOID dz_destroy_io_pool(UINT num_ios)
{
	#define	DZ_UNIT_SIZE		(sizeof(struct dz_iorequest_s))
	LOGINF("Destroying IO Info Pool\n");
	if (dz_io_mempool) {
		mempool_destroy(dz_io_mempool);
		kmem_cache_destroy(dz_io_cache);
		reduce_memory_usage(num_ios * DZ_UNIT_SIZE);
	}
	#undef DZ_UNIT_SIZE
}
EXPORT_SYMBOL(dz_destroy_io_pool);

RPVOID dz_io_alloc(VOID)
{
	PIOREQUEST ptr = NULL;
	if (!dz_io_mempool	) return NULL;

action:
	ptr = mempool_alloc(dz_io_mempool, GFP_KERNEL);
	//alloc_cnt++;
	//LOG("Alloc Count=%u,ptr=%p, min_nr=%d, curr_nr=%d\n",
	//	alloc_cnt, ptr, dz_io_mempool->min_nr, dz_io_mempool->curr_nr);
	if (ptr) {
		//LOG("IO Info Space Allocated %p\n", ptr);
		BASE_COUNTER_ATOMIC64_INC(dz_iorequest_alloc_cnt);
		return ptr;
	} else {
		DELAY_MICRO_SECONDS(10);
		goto action;
	}
}
EXPORT_SYMBOL(dz_io_alloc);

RVOID dz_io_free(PVOID io)
{
	if (!io) {
		LOGW("IO Info is NULL\n");
		return;
	}
	BASE_COUNTER_ATOMIC64_INC(dz_iorequest_free_cnt);
	//LOG("Free Count=%u,ptr=%p, min_nr=%d, curr_nr=%d\n",
	//	free_cnt, io, dz_io_mempool->min_nr, dz_io_mempool->curr_nr);
	mempool_free(io, dz_io_mempool	);
	//LOG("IO request Freed %p\n", io);
}
EXPORT_SYMBOL(dz_io_free);

// Page Mempool Functions

RINT dz_create_page_pool(SIZE num_pages)
{
	#define	DZ_UNIT_SIZE		( PAGE_SIZE )

	LOGINF("Creating Page Pool for accomodating %u pages\n",
		(UINT)num_pages);
	dz_page_mempool = mempool_create_page_pool(num_pages, 0);
	if (dz_page_mempool) {
		LOGINF("Page  pool created for accomodating %u pages. Unit Size %lu \n",
			(UINT)num_pages, DZ_UNIT_SIZE);
		log_memory_usage(num_pages * DZ_UNIT_SIZE);
		BASE_COUNTER_ATOMIC64_SET(dz_page_alloc_cnt, 0);
		BASE_COUNTER_ATOMIC64_SET(dz_page_free_cnt, 0);
		RETURNS;
	} else {
		LOGE("page pool creation failed\n");
		RETURNF;
	}

	#undef DZ_UNIT_SIZE
}
EXPORT_SYMBOL(dz_create_page_pool);

RVOID dz_destroy_page_pool(SIZE num_pages)
{
	LOGINF("Destroying Page Pool\n");
	if (dz_page_mempool) {
		mempool_destroy(dz_page_mempool);
		reduce_memory_usage(num_pages * PAGE_SIZE);
	} else {
		LOG("dz_page_mempool is NULL\n");
	}
}
EXPORT_SYMBOL(dz_destroy_page_pool);

RPVOID dz_page_alloc(VOID)
{
	PVOID ptr = NULL;
	//LOG("Page Allocated\n");
	if (!dz_page_mempool) return NULL;
	//return mempool_alloc(dz_page_mempool, GFP_NOIO);
	ptr = mempool_alloc(dz_page_mempool, GFP_NOIO);
	if (ptr) {
		BASE_COUNTER_ATOMIC64_INC(dz_page_alloc_cnt);
		return ptr;
	} else {
		return NULL;
	}
}
EXPORT_SYMBOL(dz_page_alloc);

RVOID dz_page_free(PPAGE page) 
{
	if (!page) {
		LOGW("Page is NULL\n");
		return;
	}
	BASE_COUNTER_ATOMIC64_INC(dz_page_free_cnt);
	mempool_free(page, dz_page_mempool);
	//LOG("Read Page Freed %p\n", page);
}
EXPORT_SYMBOL(dz_page_free);

// Read Page Mempool Functions

// In order to allocate/free pages from this pool,
// it has alloc macro as DZ_MEMPOOL_READ_PAGE_ALLOC
// and free macro as DZ_MEMPOOL_READ_PAGE_FREE

RINT dz_create_read_page_pool(SIZE num_pages)
{
	#define	DZ_UNIT_SIZE		( PAGE_SIZE )

	dz_read_page_mempool = mempool_create_page_pool(num_pages, 0);
	if (dz_read_page_mempool) {
		LOGINF("Read Page  pool created for accomodating %u pages. Unit Size %lu \n",
			(UINT)num_pages, DZ_UNIT_SIZE);
		log_memory_usage(num_pages * DZ_UNIT_SIZE);
		RETURNS;
	} else {
		LOGE("read_page pool creation failed\n");
		RETURNF;
	}

	#undef DZ_UNIT_SIZE
}
EXPORT_SYMBOL(dz_create_read_page_pool);

RVOID dz_destroy_read_page_pool(SIZE num_pages)
{
	LOGINF("Destroying Read Page Pool\n");
	if (dz_read_page_mempool) {
		mempool_destroy(dz_read_page_mempool);
		reduce_memory_usage(num_pages * PAGE_SIZE);
	} else {
		LOG("dz_read_page_mempool is NULL\n");
	}
}
EXPORT_SYMBOL(dz_destroy_read_page_pool);

RPVOID dz_read_page_alloc(VOID)
{
	return dz_page_alloc();
}
EXPORT_SYMBOL(dz_read_page_alloc);

RVOID dz_read_page_free(PPAGE page) 
{
	dz_page_free(page);
}
EXPORT_SYMBOL(dz_read_page_free);

// Write Page Mempool Functions
RINT dz_create_write_page_pool(SIZE num_pages)
{
	#define	DZ_UNIT_SIZE		( PAGE_SIZE )

	dz_write_page_mempool = mempool_create_page_pool(num_pages, 0);
	if (dz_write_page_mempool) {
		LOGINF("Write Page  pool created for accomodating %u pages. Unit Size %lu \n",
			(UINT)num_pages, DZ_UNIT_SIZE);
		log_memory_usage(num_pages * DZ_UNIT_SIZE);
		RETURNS;
	} else {
		LOGE("write_page pool creation failed\n");
		RETURNF;
	}

	#undef DZ_UNIT_SIZE
}

RVOID dz_destroy_write_page_pool(SIZE num_pages)
{
	//NOTE: Kernel does not check the pending allocation while destroyin
	//the mempool. Atleast this we noticed in our test
	LOGINF("Destroying Write Page Pool\n");
	if (dz_write_page_mempool) {
		mempool_destroy(dz_write_page_mempool);
		reduce_memory_usage(num_pages * PAGE_SIZE);
	} else {
		LOG("dz_write_page_mempool is NULL\n");
	}
	dz_write_page_mempool = NULL;
}

RPVOID dz_write_page_alloc(VOID)
{
	/*
	if (!dz_write_page_mempool) return NULL;
	//LOG("Write Page Allocated\n");
	return mempool_alloc(dz_write_page_mempool, GFP_NOIO);
	*/
	return dz_page_alloc();
}
EXPORT_SYMBOL(dz_write_page_alloc);

RVOID dz_write_page_free(PPAGE page) 
{
	/*
	if (!page) {
		LOGW("Page is NULL\n");
		return;
	}
	mempool_free(page, dz_write_page_mempool);
	//LOG("Write Page Freed %p\n", page);
	*/
	dz_page_free(page);
}
EXPORT_SYMBOL(dz_write_page_free);

RVOID dz_write_page_free_debug(PPAGE page) 
{
	mempool_free(page, dz_write_page_mempool);
	LOGINF("dz_write_page_mempool:free:curr_nr = %d, ptr = %p\n", 
		dz_write_page_mempool->curr_nr, page);
}
EXPORT_SYMBOL(dz_write_page_free_debug);

//General wrapper function for kmalloc and kfree
RPVOID dz_kmalloc(SIZE size, INT flags)
{
	PVOID ptr = NULL;
	ptr = kmalloc(size, flags);
	//TODO:Record memory usage count
	if (ptr) {
		log_memory_usage(size);
	}
	return ptr;
}
EXPORT_SYMBOL(dz_kmalloc);

RVOID dz_kfree(PVOID ptr, SIZE size)
{
	if (ptr) {
		kfree(ptr);
		ptr = NULL;
		reduce_memory_usage(size);
	} else {
		LOGW("Memory is already freed or unallocated\n");
	}
}
EXPORT_SYMBOL(dz_kfree);

PBIO dz_bio_page_alloc(PBLOCK_DEVICE bdev)
{
	PBIO 	bio 	= NULL;
	PPAGE	page 	= NULL;
bioalloc:
	bio = dz_bio_alloc(1);
	if (!bio) {
		LOGE("Unable to get free bio\n");
		DELAY_MICRO_SECONDS(10);
		goto bioalloc;
	}
pagealloc:
	page = dz_page_alloc();
	if (!page) {
		LOGE("Unable to get free page\n");
		DELAY_MICRO_SECONDS(10);
		goto pagealloc;
	}
	DZ_BIO_SET_DEV(bio, bdev);
	if (!bio_add_page(bio, page, PAGE_SIZE, 0)) {
		//TODO: There is NO reason to have this failure.
		LOGE("Unable to add page to bio\n");
		BUG_ON(1);
	}
	return bio;
}
EXPORT_SYMBOL(dz_bio_page_alloc);

RVOID dz_bio_page_free(PBIO bio)
{
	//TODO:Please ensure that the page through bio_page(bio) is not returning NULL.
	//We have seen that in case of write bio, when the bio is done the bio's iovec is
	//also set to NULL which  results in bio_page(bio) to NULL
	dz_page_free(bio_page(bio));
	DZ_OS_KERNEL_BIO_PUT(bio);
}
EXPORT_SYMBOL(dz_bio_page_free);

// Function for creating common memory pools
RINT dz_create_memory_pools(VOID)
{
	INT ret = 0;
	dz_bio_mempool			= NULL;		
	dz_read_page_mempool	= NULL;		
	dz_write_page_mempool	= NULL;		

	LOGINF(" ____________________________________________ \n");
	LOGINF("|                                            |\n");
	LOGINF("|           Creating Memory Pools            |\n");
	LOGINF("|____________________________________________|\n");

	ret = dz_create_bio_pool		(dz_bio_mempool_count);
	if (ret == FAILURE) {
		RETURNF;
	}
	//ret = dz_create_write_page_pool	(dz_write_page_mempool_count	);
	//ret = dz_create_read_page_pool	(dz_read_page_mempool_count);
	ret = dz_create_page_pool		(dz_page_mempool_count);
	if (ret == FAILURE) {
		RETURNF;
	}
	ret = dz_create_io_pool			(dz_io_mempool_count);
	if (ret == FAILURE) {
		RETURNF;
	}

	LOGINF(" ____________________________________________ \n");
	LOGINF("|____________________________________________|\n");

	return SUCCESS;
}
EXPORT_SYMBOL(dz_create_memory_pools);

// Function for destroying common memory pools
RINT dz_destroy_memory_pools(VOID)
{
	dz_destroy_io_pool			(dz_io_mempool_count);
	dz_destroy_page_pool		(dz_page_mempool_count);
	//dz_destroy_read_page_pool	(dz_read_page_mempool_count);
	//dz_destroy_write_page_pool	(dz_write_page_mempool_count);
	dz_destroy_bio_pool			(dz_bio_mempool_count);
	return SUCCESS;
}
EXPORT_SYMBOL(dz_destroy_memory_pools);

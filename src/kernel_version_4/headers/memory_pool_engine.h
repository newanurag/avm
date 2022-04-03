/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _MEMORY_POOL_ENGINE_H_
#define _MEMORY_POOL_ENGINE_H_

#include "bec_engine.h"

// Memory Pools Function Declarations
INLINE VOID log_memory_usage(U64 count);
INLINE VOID reduce_memory_usage(U64 count);

RPVOID 	dz_kmalloc(SIZE size, INT flags);
RVOID 	dz_kfree(PVOID ptr, SIZE size);

RINT 	dz_create_memory_pools(VOID);
RINT   	dz_destroy_memory_pools(VOID);

RINT 	dz_create_bio_pool(UINT num_bios);
RVOID  	dz_destroy_bio_pool(UINT num_bios) ;
PBIO   	dz_bio_alloc(UINT nr_iovecs);
RVOID  	dz_bio_free(PBIO bio);
RVOID  	dz_bio_alloc_test(VOID);

RINT 	dz_create_write_page_pool(SIZE num_pages);
RVOID  	dz_destroy_write_page_pool(SIZE);
RPVOID 	dz_write_page_alloc(VOID);
RPVOID 	dz_write_page_alloc_debug(VOID);
RVOID  	dz_write_page_free(PPAGE page);
RVOID  	dz_page_free(PPAGE page);
RVOID  	dz_write_page_free_debug(PPAGE page);

RINT 	dz_create_read_page_pool(SIZE num_pages);
RVOID  	dz_destroy_read_page_pool(SIZE);
RPVOID 	dz_read_page_alloc(VOID);
RVOID  	dz_read_page_free(PPAGE page);
RINT   	dz_bio_bvec_write_page_free(PBIO bio);
RINT   	dz_bio_bvec_read_page_free(PBIO bio);

RPVOID 	dz_page_alloc(VOID);
RINT 	dz_create_io_pool(UINT);
RVOID 	dz_destroy_io_pool(UINT);
RPVOID 	dz_io_alloc(VOID);
RVOID 	dz_io_free(PVOID);

RINT 	dz_create_read_io_pool(UINT);
RVOID 	dz_destroy_read_io_pool(UINT);
RPVOID 	dz_read_io_alloc(VOID);
RVOID 	dz_read_io_free(PVOID);

RINT 	dz_create_write_io_pool(UINT);
RVOID 	dz_destroy_write_io_pool(UINT);
RPVOID 	dz_write_io_alloc(VOID);
RVOID 	dz_write_io_free(PVOID);


PBIO 	dz_bio_page_alloc(PBLOCK_DEVICE bdev);
RVOID 	dz_bio_page_free(PBIO bio);

RVOID 	dz_memory_consumption(U64 initial, U64 *final_mem, CHAR *marker);
RVOID 	dz_memory_reduction(U64 initial, U64 *final_mem, CHAR *marker);
RVOID 	dz_total_memory_consumption(VOID);
U64 	dz_get_memory_usage(VOID);
RVOID 	dz_check_memory_leak(VOID);
#endif //_MEMORY_POOL_ENGINE_H_

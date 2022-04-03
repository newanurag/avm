/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _BEC_ENGINE_H_
#define _BEC_ENGINE_H_
#include "dedupe_engine.h"
#include "thread_pool_engine.h"

#define	BEC_CLEARED			SUCCESS
#define	BEC_FAILED_TO_CLEAR	1
#define	BEC_PARTIAL_CLEARED	2

#define BEC_REQUEST_TYPE_ADD_READ	1 //Addition of Page in BEC through READ IO
#define BEC_REQUEST_TYPE_ADD_WRITE	2 //Addition of Page in BEC through WRITE IO
#define BEC_REQUEST_TYPE_DELETE		3

#define	BEC_PAGE_EVICT_PERCENTAGE	10

#define	BEC_ADD_NODE_TO_LIST(node)	\
		list_add_tail(&((node)->pba_list), &bec_list_head)
//#define	ISSUE_IO_TO_DATA_DISK(bio)	generic_make_request((bio))

typedef struct dz_bec_request_s {
	UCHAR			type;	// Type of request;
	U64 			pba_index;
	PBIO			bio;	//Bio whose page to be added to Cache
	LIST_HEAD 		ioq; // BEC  request will be queued through this field.

} BECREQUEST, BEC_REQUEST, *PBECREQUEST, *PBEC_REQUEST;

typedef struct dz_bec_node_s {
	union {
		UINT data_size;
		UINT size;
	};
	union {
		PPAGE	page;
		PPAGE	data_page;
	};
	U64 pba_index;
	LIST_HEAD	pba_list;
} BEC_NODE, *PBEC_NODE, PBA_CACHE_NODE, *PPBA_CACHE_NODE;


PBEC_NODE dz_bec_radix_tree_node_find(U64 pba_index);
RINT dz_bec_radix_tree_node_insert(PPBA_CACHE_NODE node_entry);
RINT dz_bec_radix_tree_node_delete(U64 pba_index);

RINT 	dz_bec_engine_init(VOID);
RINT 	dz_bec_engine_exit(VOID);
//RVOID 	dz_bec_io(PBEC_REQUEST bec_request);
RVOID 	dz_bec_io(PIOREQUEST iorequest);
RVOID 	dz_bec_read_io(PIOREQUEST iorequest);
RVOID 	dz_bec_write_io(PIOREQUEST iorequest);
BOOL 	dz_bec_pba_page_lookup(U64 pba_index, PBIO bio);
RVOID 	dz_bec_write_biodone(PBIO bio, INT error);
RINT 	dz_bec_pba_page_add_read(U64 pba_index, PIOREQUEST iorequest , UINT data_size);
RINT    dz_bec_pba_page_add_read_without_fec(U64 pba_index, PIOREQUEST iorequest);
RINT 	dz_bec_pba_page_add_write(U64 pba_index, PIOREQUEST iorequest , UINT data_size);
RINT 	dz_bec_pba_page_delete(U64 pba_index, PIOREQUEST iorequest , UINT data_size);
RINT 	dz_bec_pba_parent_page_add(U64 pba_index, PBIO bio , UINT data_size);
RINT 	dz_bec_clear(INT node_count_cutoff);
RVOID	dz_bec_update_from_disk(U64 pba_index, UINT data_size);

RINT 	dz_create_bec_node_pool(SIZE num_nodes);
RVOID 	dz_destroy_bec_node_pool(SIZE num_nodes);
RPVOID 	dz_bec_node_alloc(VOID);
RVOID 	dz_bec_node_free(PBEC_NODE node);

RINT 	dz_create_bec_page_pool(SIZE num_pages);
RVOID 	dz_destroy_bec_page_pool(SIZE num_pages);
RPVOID 	dz_bec_page_alloc(VOID);
RVOID 	dz_bec_page_free(PPAGE page);

RINT 	dz_create_bec_request_pool(UINT num_requests);
RVOID 	dz_destroy_bec_request_pool(UINT num_requests);
RPVOID 	dz_bec_request_alloc(VOID);
RVOID 	dz_bec_request_free(PVOID request);

RVOID 	dz_bec_proceed_pba_page_add_read(PIOREQUEST iorequest);
RVOID 	dz_bec_proceed_pba_page_add_write(PIOREQUEST iorequest);
RVOID 	dz_bec_proceed_pba_page_delete(PIOREQUEST iorequest);

VOID 	dz_bec_print_counters(VOID);
VOID 	dz_bec_init_counters(VOID);
#endif //_BEC_ENGINE_H_

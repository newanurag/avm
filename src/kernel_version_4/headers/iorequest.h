#ifndef _IOREQUEST_H_
#define _IOREQUEST_H_

#include "thread_pool_engine.h"
struct dz_dedupe_info_s;

typedef struct dz_iorequest_s {  
	UINT                ior_type;

//IO received directly from application during _map function
#define DZ_IOR_TYPE_APPLICATION_IO_READ                     1 

//IO received directly from application during _map function
#define DZ_IOR_TYPE_APPLICATION_IO_WRITE                    2 

//IO created internally and acting as Parent
#define DZ_IOR_TYPE_INTERNAL_PARENT_IO_READ                 3
#define DZ_IOR_TYPE_INTERNAL_PARENT_IO_WRITE                4

//Allocates new page for reading purpose
#define DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ                  5 

//Uses Parent's bio page for reading purpose
#define DZ_IOR_TYPE_INTERNAL_CHILD_IO_READ_SHARE_PAGE       6 

//Allocates new page for writing purpose
#define DZ_IOR_TYPE_INTERNAL_CHILD_IO_WRITE                 7

//Uses Parent's bio page for writing purpose
#define DZ_IOR_TYPE_INTERNAL_CHILD_IO_WRITE_SHARE_PAGE      8

	PBIO                ior_bio;
	ATOMIC              ior_ref_cnt; //Free if zero
	SECTOR              ior_sector;

	union {
		UINT            ior_iosize;
		UINT            ior_size;
		UINT            ior_len;
	};

	PPAGE				ior_cache_page; //Used when FEC is disabled and BEC is enabled;

	union {
		PPAGE           ior_bv_page;
		PVOID           ior_bv_buffer;
	};
	UINT                ior_bv_len:18;  //Max 256K Page Size
	UINT                ior_bv_offset:12;
    //Used when BEC pba page addition/deletion required
	UINT                ior_bec_request_type:2;
    
	IOREQUEST_DONE      ior_donefun;
	ATOMIC                  ior_child_cnt;
	LBA                     ior_lba;
	PVOID                   ior_private;

	TIME                    ior_tstart;
	TIME                    ior_tend;
	struct dz_thread_pool_s *ior_thread_pool;
	struct dz_iorequest_s   *ior_parent;
 
	//PMEMPOOL              io_mempool; // Type of iomem pool
	
	union {
		struct list_head        ior_ioq; // IO request will be queued through this field.
        // Applicable for Parent - Child relation ship. Parent will have ior_ioq_head_local active.
        // Child will represent ior_ioq
		struct list_head        ior_ioq_head_local; 
	};
	LIST_HEAD               	ior_flushq_head;
    

    //Structure holding Dedupe information used by Dedupe Engine
    struct dz_dedupe_info_s 	*ior_ddinfo; 
                    
} DZIO, *PDZIO, DZIO_T, *PDZIO_T, IOREQUEST, *PIOREQUEST;






RVOID iorequest_get(PIOREQUEST iorequest);
RVOID iorequest_put(PIOREQUEST iorequest);
// Other Read / Write / Bio  Functions
RVOID 	dz_child_biodone(PBIO bio, INT error);
RVOID 	dz_init_child_bio(PBIO parent_bio, PBIO child_bio, SECTOR sector, BIO_CALLBACK done, PVOID private);
RINT 	dz_write_split_bio_without_alignment_processing(PIOREQUEST piorequest);
PIOREQUEST dz_alloc_and_init_iorequest(PBIO bio, PBIO child_bio, SECTOR sector, BOOL syncflag);

PIOREQUEST dz_create_iorequest_without_bio_bvec(PIOREQUEST parent_iorequest, UINT nr_bvecs, SECTOR sector);
PIOREQUEST dz_create_iorequest_with_bio_bvec(PIOREQUEST parent_iorequest, SECTOR sector);

RVOID 	dz_q_iorequest(DZ_THREAD_POOL *pool, PDZIO iorequest);
RVOID 	dz_q_iorequest_thread_pool(PDZIO iorequest);
//RVOID dz_q_iorequest_list(DZ_THREAD_POOL *pool, struct list_head *newlist);
RVOID 	dz_q_iorequest_list(DZ_THREAD_POOL *pool, struct list_head *newlist, UINT count);
PDZIO 	dz_dq_iorequest(DZ_THREAD_POOL *pool);
UINT 	dz_outstanding_iorequests(DZ_THREAD_POOL *pool);
RVOID 	dz_wait_for_all_iorequests_finished(DZ_THREAD_POOL *pool);
RVOID 	dz_q_child_iorequest_to_parent_q(PDZIO parent_iorequest, PDZIO child_iorequest);
//RVOID dz_merge_parent_local_q_to_threadpool_q(DZ_THREAD_POOL *pool, struct list_head *parent_iorequest_list);
RVOID 	dz_merge_parent_local_q_to_threadpool_q(DZ_THREAD_POOL *pool, PIOREQUEST parent_iorequest);


RVOID	 dz_create_iorequest_for_single_block_read(PIOREQUEST, SECTOR , BIODONE);
RVOID 	print_iorequest(PIOREQUEST p, char *str);
RVOID 	print_iorequest_lite(PIOREQUEST p, char *str);
RVOID 	dz_fake_iodone(PIOREQUEST iorequest);
RVOID 	dz_iodone(PIOREQUEST iorequest);
RVOID 	dz_write_iodone(PIOREQUEST iorequest);
RVOID 	dz_read_iodone(PIOREQUEST iorequest);
RVOID 	dz_iodone_error(PIOREQUEST iorequest, INT error);

#endif //_IOREQUEST_H_

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _THREAD_POOL_ENGINE_H_
#define _THREAD_POOL_ENGINE_H_

struct dz_iorequest_s;

typedef struct dz_job_s {
	void *work;
	struct completion done;
	struct list_head list;
} DZ_JOB_T, DZ_JOB, *PDZ_JOB, JOB, *PJOB;

struct dz_thread_ops_s;
struct dz_thread_s;

//typedef VOID (*THREAD_FUNC)(struct dz_thread_s *, struct dz_job_s *);
//typedef VOID (*THREAD_FUNC)(struct dz_thread_s *, struct dz_iorequest_s *);
//typedef VOID (*THREAD_FUNC)(struct dz_thread_s *, VOID *);
typedef VOID (*THREAD_FUNC)(struct dz_iorequest_s *);
typedef VOID (*THREAD_INIT)(struct dz_thread_s *, int *, int);
typedef VOID (*THREAD_FREE)(struct dz_thread_s *);

typedef struct dz_thread_s {
	struct task_struct 		*thread;
	struct dz_thread_pool_s *pool;
	VOID 					*private;

	// alive means to ensure that some threads are already woken up when IO arrives.
	// Normaly wakeup takes time so this flag is used to ensure that not all threads
	// are sleeping at the time when IO comes up. Its like proactive wakeup
	BOOL 					alive;			
	BOOL 					cleanup_pending;
	VOID 					*resources;
	THREAD_FUNC				thread_func;
	THREAD_INIT				thread_init;
	THREAD_FREE				thread_free;

} DZ_THREAD_T, DZ_THREAD, *PDZ_THREAD, WORKER_THREAD;


typedef struct dz_thread_pool_s {
	//WORKER_THREAD   	**worker_threads; // Pointer to each worker thread
	WORKER_THREAD   	*worker_thread; // Pointer to each worker thread
	struct completion   halt_active_threads;
	ATOMIC              num_active_threads; // Active means if it is in thread_func()
	UINT                num_threads;
	VOID (*free_job_resource)(struct dz_job_s *); 
	wait_queue_head_t   all_active_threads_finished_waitq;
	CHAR                name[NAME_LENGTH];
	BOOL				destroy;	// Signalling Pool Destruction. Default is False
	BOOL                quiesce;	// Signalling IO Quiescing. Default is False

	BOOL				jobs;		// Create Jobs. Default is True
	struct completion   job_addition;
	SPINLOCK            job_list_lock; // Lock for serializing concurrent access to
	struct list_head    job_list_head;
	ATOMIC              num_jobs;
	wait_queue_head_t   all_jobs_finished_waitq;

	BOOL				ioq_open;	// Queue Open/Closed for IO Requests
	struct completion   ioq_addition;
	SPINLOCK            ioq_lock; // Lock for serializing concurrent access to ioq in DZIO
	struct list_head    ioq_head;
	ATOMIC              ioq_num_ios;
	wait_queue_head_t   all_ios_finished_waitq;
} DZ_WORKER_THREAD_POOL_T, DZ_THREAD_POOL, THREAD_POOL, *PTHREAD_POOL, *PDZ_THREAD_POOL, *PDZ_WORKER_THREAD_POOL, PDZ_TP;


// Thread/ThreadPool Related Function Declarations
RINT 	dz_create_thread_pools(VOID);
PDZ_WORKER_THREAD_POOL dz_create_thread_pool(CHAR *, CHAR *, UINT, THREAD_INIT, THREAD_FREE, THREAD_FUNC);


RINT 	dz_create_align_read_thread_pool(INT num_threads);
RINT 	dz_create_align_write_thread_pool(INT num_threads);
RVOID 	dz_destroy_align_read_thread_pool(VOID);
RVOID 	dz_destroy_align_write_thread_pool(VOID);


RINT 	dz_create_read_thread_pool(INT num_threads);
RINT 	dz_create_write_thread_pool(INT num_threads);
RINT 	dz_create_flush_thread_pool(INT num_threads);

RVOID 	dz_destroy_thread_pools(VOID);
RVOID 	dz_destroy_thread_pool(PDZ_WORKER_THREAD_POOL pool);
RVOID 	dz_destroy_read_thread_pool(VOID);
RVOID 	dz_destroy_write_thread_pool(VOID);
RVOID 	dz_destroy_flush_thread_pool(VOID);

RVOID 	dz_thread_exit(VOID);
RVOID 	dz_q_jobs(DZ_THREAD_POOL *pool, DZ_JOB *job);
PJOB	dz_dq_jobs(DZ_THREAD_POOL *pool);
INT 	dz_worker_thread(void *data);
INT 	dz_io_daemon(void *data);
UINT 	dz_outstanding_jobs(DZ_THREAD_POOL *pool);
RVOID 	dz_wait_for_all_jobs_finished(DZ_THREAD_POOL *pool);
RVOID 	dz_wait_for_all_active_threads_finished(DZ_THREAD_POOL *pool);

RINT 	dz_create_bec_thread_pool(INT num_threads);
RVOID 	dz_destroy_bec_thread_pool(VOID);
#endif // _THREAD_POOL_ENGINE_H_

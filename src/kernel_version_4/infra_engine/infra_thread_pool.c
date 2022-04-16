/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"


#define	JOB_COMPLETION_TIMEOUT_VALUE	msecs_to_jiffies(1000)	//1000 Milli Seconds converted into jiffies
#define	IOREQUEST_COMPLETION_TIMEOUT_VALUE	msecs_to_jiffies(1000)	//1000 Milli Seconds converted into jiffies
#define	IO_COMPLETION_TIMEOUT_VALUE		msecs_to_jiffies(100)	//100  Milli Seconds converted into jiffies

PDZ_THREAD_POOL dz_create_thread_pool(CHAR *name, CHAR *sname,
	UINT num_threads, 
	THREAD_INIT init,
	THREAD_FREE free,
	THREAD_FUNC func)
{
	int count=0;
	PDZ_THREAD_POOL dz_thread_pool = NULL;
	//DZ_THREAD **worker_threads = NULL;
	DZ_THREAD *worker_thread   = NULL;

	dz_thread_pool = dz_kmalloc(sizeof(DZ_WORKER_THREAD_POOL_T), GFP_KERNEL);
	if (!dz_thread_pool) {
		LOGE("Unable to allocate memory for dz_thread_pool\n");
		return NULL;
	}
	/*
	worker_threads = dz_kmalloc(num_threads * sizeof(DZ_THREAD_T *) , GFP_KERNEL);
	if (!worker_threads) {
		LOGE("Unable to allocate memory for worker_threads pointer \n");
		dz_kfree(dz_thread_pool);
		return NULL;
	}
	dz_thread_pool->worker_threads = worker_threads;
	*/

	worker_thread = dz_kmalloc(num_threads * sizeof(WORKER_THREAD), GFP_KERNEL);
	if(!worker_thread){
		LOGE("Unable to allocate memory for worker threads \n");
		dz_kfree(dz_thread_pool, sizeof(DZ_WORKER_THREAD_POOL_T));
		return NULL;
	}
	dz_thread_pool->worker_thread = worker_thread;

	strncpy(dz_thread_pool->name, name, NAME_LENGTH);

	spin_lock_init(&dz_thread_pool->job_list_lock);
	spin_lock_init(&dz_thread_pool->ioq_lock);

	INIT_LIST_HEAD(&dz_thread_pool->job_list_head);
	INIT_LIST_HEAD(&dz_thread_pool->ioq_head);

	init_waitqueue_head(&dz_thread_pool->all_jobs_finished_waitq);
	init_waitqueue_head(&dz_thread_pool->all_ios_finished_waitq);
	init_waitqueue_head(&dz_thread_pool->all_active_threads_finished_waitq);

	init_completion(&dz_thread_pool->job_addition);
	init_completion(&dz_thread_pool->ioq_addition);
	init_completion(&dz_thread_pool->halt_active_threads);
	complete(&dz_thread_pool->halt_active_threads);

	atomic_set(&dz_thread_pool->num_jobs, 0);
	atomic_set(&dz_thread_pool->num_active_threads, 0);
	atomic_set(&dz_thread_pool->ioq_num_ios, 0);

	dz_thread_pool->quiesce 	= DZ_ACTIVE_IO;
	dz_thread_pool->destroy 	= false;
	dz_thread_pool->jobs    	= true;
	dz_thread_pool->ioq_open   	= true;

	for(count=0; count<num_threads; count++) {
		worker_thread->thread_init 		= init;
		worker_thread->thread_free 		= free;
		worker_thread->thread_func 		= func;
		worker_thread->pool 			= dz_thread_pool;
		worker_thread->alive 			= true;
		worker_thread->cleanup_pending	= false;

		//worker_thread->thread = kthread_create(dz_worker_thread,
		worker_thread->thread = kthread_create(dz_io_daemon,
		worker_thread, "%s_%d/%d", sname, (count+1), num_threads);


		if (IS_ERR(worker_thread->thread)) {
			//TODO: Should we exit or continue
			//dz_kfree(dz_thread_pool);
			//dz_kfree(worker_threads);

		} else {
			LOGINF("Kthread_create:pid %d, tgid %d, Name %s \n", 
				worker_thread->thread->pid,
				worker_thread->thread->tgid,
				worker_thread->thread->comm);
			wake_up_process(worker_thread->thread);
	
			dz_thread_pool->num_threads = count + 1;
		}


		//worker_threads[count]			= worker_thread;
		worker_thread++;
	}
	return dz_thread_pool;
}
EXPORT_SYMBOL(dz_create_thread_pool);

/*
 * Destruction of Pool will involved these activies:
 * 1. Stop Accepting new IO Request into IOQ
 * 2. Wait for inflight IOS i.e. active threads busy in IO processing
 * 4. Signal IO Daemons to wakeup
 * 5. kthread_stop IO Daemon threads
 * 6. Free up resources
 */

RVOID dz_destroy_thread_pool(PDZ_THREAD_POOL pool)
{
	int 	count	 = 0;
	UINT num_threads = pool->num_threads;
	//DZ_THREAD **worker_threads = NULL;
	DZ_THREAD *worker_thread   = pool->worker_thread;

	LOGINF("Destroying %s\n", pool->name);

	// Stop Accepting new IO Request
	pool->ioq_open	= false;

	// Stop Quiescing IOs ?? No need. It will stop pending jobs IOs
	//pool->quiesce = true;

	if (dz_outstanding_iorequests(pool)) {
		//Wait for all IO Requests to finish
		dz_wait_for_all_iorequests_finished(pool);
	}

	// Until now all jobs are completed but gone for sleep
	// waiting for new arrival of jobs

	// Signal Pool Destroy flag to Worker Thread.
	// We cant signal earlier as there may be pending jobs.
	// This signal will help to come out of while loop of worker
	// thread execution
	pool->destroy = true;

	// Signal completion event to worker threads.
	// These threads might be sleeping for this event,
	// waiting for new jobs arrival/additions
	
	complete_all(&pool->ioq_addition);

	// Stop all Kernel Threads
	for(count=0; count<num_threads; count++) {
		kthread_stop(worker_thread->thread);
		LOGINF("Kthread_stop:pid %d, tgid %d, Name %s \n", 
			worker_thread->thread->pid,
			worker_thread->thread->tgid,
			worker_thread->thread->comm);

		if (worker_thread->thread_free) {
			worker_thread->thread_free(worker_thread);
		}
		worker_thread++;
	}

	// Free DZ Threads Memory
	dz_kfree(pool->worker_thread, num_threads * sizeof(WORKER_THREAD));

	// Free Pool Memory
	dz_kfree(pool, sizeof(DZ_WORKER_THREAD_POOL_T));
}
EXPORT_SYMBOL(dz_destroy_thread_pool);


RVOID dz_thread_exit(VOID)
{
	set_current_state(TASK_INTERRUPTIBLE);
	while(!kthread_should_stop()) {
		schedule();
		set_current_state(TASK_INTERRUPTIBLE);
	}
	__set_current_state(TASK_RUNNING);
}
EXPORT_SYMBOL(dz_thread_exit);

RINT dz_io_daemon(void *data)
{
	DZ_THREAD_POOL 	*pool;
	DZ_THREAD 		*io_daemon;
	PDZIO 			iorequest;

	io_daemon = (DZ_THREAD *)data;
	pool = io_daemon->pool;

action:
	while(!kthread_should_stop() && !pool->destroy) {
		if (pool->quiesce == DZ_QUIESCE_IO) {
			//TODO:Stop all IOs
		}
		if ( io_daemon->alive == false ) {
			//TODO
		}

		iorequest = dz_dq_iorequest(pool);
		if(iorequest) {
			atomic_inc(&pool->num_active_threads);

			//Actual Execution of thread_func gets started here
			if(io_daemon->thread_func) {
				//io_daemon->thread_func(io_daemon, iorequest);
				io_daemon->thread_func(iorequest);
			}

			if (atomic_dec_and_test(&pool->num_active_threads)) {
				wake_up(&pool->all_active_threads_finished_waitq);
			}
			if (atomic_dec_and_test(&pool->ioq_num_ios)) {
				wake_up(&pool->all_ios_finished_waitq);
			}

			goto action;
        } else { //End of If job
			/*
			LOGINF("IO Request NULL for io_daemon:pid %d, tgid %d, Name %s \n", 
				io_daemon->thread->pid,
				io_daemon->thread->tgid,
				io_daemon->thread->comm);
			*/
			wait_for_completion_interruptible(&pool->ioq_addition);
		}
	} //End of While loop

	if (pool->destroy) {
		dz_thread_exit();
	}
	/*
	LOGINF("Exiting Kthread:pid %d, tgid %d, Name %s \n", 
		io_daemon->thread->pid,
		io_daemon->thread->tgid,
		io_daemon->thread->comm);
	*/
	RETURNS;
}

UINT dz_outstanding_jobs(DZ_THREAD_POOL *pool)
{
	return atomic_read(&pool->num_jobs);
}


RVOID dz_wait_for_all_jobs_finished(DZ_THREAD_POOL *pool)
{
	UINT pending_jobs;
	while((pending_jobs =  atomic_read(&pool->num_jobs))) {
		LOGINF("Num of Pending JOBS for Pool %s are %u\n", pool->name, pending_jobs);
		wait_event_timeout(pool->all_jobs_finished_waitq, (pending_jobs == 0), JOB_COMPLETION_TIMEOUT_VALUE);
	}
}

RVOID dz_wait_for_all_iorequests_finished(DZ_THREAD_POOL *pool)
{
	UINT pending_iorequests;
	while((pending_iorequests =  atomic_read(&pool->ioq_num_ios))) {
		LOGINF("Num of Pending IO Requests for Pool %s are %u\n", pool->name, pending_iorequests);
		wait_event_timeout(pool->all_jobs_finished_waitq, (pending_iorequests == 0), IOREQUEST_COMPLETION_TIMEOUT_VALUE);
	}
}

RVOID dz_wait_for_all_active_threads_finished(DZ_THREAD_POOL *pool)
{
	UINT pending_threads;
	while((pending_threads =  atomic_read(&pool->num_active_threads))) {
		LOGINF("Num of Pending Active Threads for Pool %s are %u\n", pool->name, pending_threads);
		wait_event_timeout(pool->all_active_threads_finished_waitq, (pending_threads == 0), IO_COMPLETION_TIMEOUT_VALUE);
	}
}

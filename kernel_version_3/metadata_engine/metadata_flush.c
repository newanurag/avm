/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"
extern PDZ_THREAD_POOL 	metadata_flush_thread_pool;

//For operational purpose
extern BOOL				metadata_flush_timer_stop;
extern ULONG 			metadata_flush_interval;
extern THREAD 			*metadata_flush_timer_thread;

extern U64				time_flush_prepare_start;
extern U64				time_flush_start;
extern U64				time_flush_end  ;

extern PCHAR    		METADATA_FLUSH_THREAD_POOL_NAME;
extern PCHAR    		METADATA_FLUSH_THREAD_POOL_SHORT_NAME;

// This function will actually start the flushing of inmemory metadata
// to metadata disk, when invoked from timer
RVOID dz_metadata_flush_start_from_timer_context(VOID)
{
	//dz_q_iorequest(metadata_flush_thread_pool, flush_iorequest);
}

// This timer will be running in the background and  start the
// flushing of inmemory metadata table to metadata disk
RINT dz_metadata_flush_timer(PVOID data)
{
	unsigned long scheduled_time 	= 0;
	LOGMETF("Metadata Flush Timer Started\n");

	while(!kthread_should_stop() && !metadata_flush_timer_stop) {

		scheduled_time = msecs_to_jiffies(metadata_flush_interval * 1000);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(scheduled_time);
		if (kthread_should_stop()) {
			break;
		}

		if (metadata_flush_timer_stop) {
			LOGMET("Metadata Flush Timer Thread getting exited\n");
			dz_thread_exit();
		} else {
			//dz_fec_flush_start_from_timer_context();
		}

	}
	LOGMETF("Metadata Flush Timer Stopped\n");
	RETURNS;
}

RVOID 	dz_metadata_flush_timer_stop(VOID)
{
	if (metadata_flush_timer_thread) {
		metadata_flush_timer_stop = TRUE;
		kthread_stop(metadata_flush_timer_thread);
	}
}

//ret = dz_create_flush_thread_pool(num_flush_threads);

RINT dz_metadata_create_flush_thread_pool(INT num_threads)
{
    PDZ_THREAD_POOL tp = NULL;
    tp = dz_create_thread_pool(
        METADATA_FLUSH_THREAD_POOL_NAME, METADATA_FLUSH_THREAD_POOL_SHORT_NAME,
        num_threads, NULL, NULL, dz_metadata_flush_io);
    if (tp) {
        LOGMETF("%s created successfully for %d threads \n", 
            METADATA_FLUSH_THREAD_POOL_NAME, num_threads);
        metadata_flush_thread_pool = tp; 
        RETURNS;
    } else {
        LOGMETE("%s creation failed \n", METADATA_FLUSH_THREAD_POOL_NAME);
        RETURNF;
    }   
    return SUCCESS;
}

RVOID dz_destroy_flush_thread_pool(VOID)
{
    dz_destroy_thread_pool(metadata_flush_thread_pool);
}

RVOID dz_metadata_flush_io(PIOREQUEST parent_iorequest)
{

}

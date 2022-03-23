/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "fec_engine.h"
extern PMEMPOOL			fec_data_read_mempool;
extern INT				fec_data_read_mempool_count;

extern U64 				fec_table_size;
extern U64				fec_active_count;

extern PDZ_THREAD_POOL 	read_thread_pool;
extern PDZ_THREAD_POOL 	write_thread_pool;
extern PDZ_THREAD_POOL 	dedupe_thread_pool;
extern PDZ_THREAD_POOL 	flush_thread_pool;

extern SPINLOCK			fec_active_lock;
extern SPINLOCK			fec_flush_lock;

//For operational purpose
extern PFEC_TABLE 		fec_tables_active;
extern PFEC_TABLE 		fec_tables_flush;

extern BOOL				fec_flush_timer_stop;
extern ULONG 			fec_flush_interval;
extern THREAD 			*fec_flush_timer_thread;
extern PIOREQUEST		fec_flush_iorequest;

extern PAVM_DEVICE 		aisa_device_object;
extern PLBA_BLOCK		lba_table;
extern ATOMIC64			fec_cur_flush_count;
extern ATOMIC64			fec_flush_ops_count;
extern ATOMIC64			fec_cur_flush_ops_count;

extern U64		time_flush_prepare_start;
extern U64		time_flush_start;
extern U64		time_flush_end  ;

//This function will add the fec buffer to flush queue.
// This will be invoked from fec engine especially when
// application IO is received
RVOID dz_fec_add_write_buffer_to_flushq(PFEC_WRITE fecw)
{
	spin_lock(&fec_flush_lock);
	list_add_tail(&(fecw)->flushq, &(fec_tables_flush)->flushq_head);
	fec_tables_flush->total_flush_cnt++;
	spin_unlock(&fec_flush_lock);
}

// This function will actually start the flushing
// when invoked from timer
RVOID dz_fec_flush_start_from_timer_context(VOID)
{	
	//Call the flush function
	spin_lock(&fec_active_lock);
	if (fec_active_count == 0) {
		LOGFEC("Nothing in Active Buffer to flush\n");
		spin_unlock(&fec_active_lock);
		return;

	}

	//Its rare, but check if previous flush is in progress
	//Previous flush may be pending if there are reads being served
	if (fec_tables_flush->flush_in_progress) {
		spin_unlock(&fec_active_lock);
		//We should retry this in short duration, otherwise, flushing
		//will be delayed until next cycle, or if writes are going on
		//then flushing will be started from write context whenever active
		//buffer becomes full
		return;
	}
	LOGFEC("Flushing Started From Timer Context\n");
	dz_fec_flush_switchover();
	fec_tables_flush->flush_in_progress = TRUE;

	spin_unlock(&fec_active_lock);			
	dz_q_iorequest(fec_flush_iorequest->ior_thread_pool, fec_flush_iorequest);
}

// This timer will be running in the background and  start the
// flushing if required by calling dz_fec_flush_start_from_timer_context()
RINT dz_fec_flush_timer(PVOID data)
{
	unsigned long scheduled_time 	= 0;
	LOGD("Flush Timer Started\n");

	while(!kthread_should_stop() && !fec_flush_timer_stop) {

		scheduled_time = msecs_to_jiffies(fec_flush_interval * 1000);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(scheduled_time);
		if (kthread_should_stop()) {
			break;
		}

		if (fec_flush_timer_stop) {
			LOGFEC("Flush Timer Thread getting exited\n");
			dz_thread_exit();
		} else {
			//dz_fec_flush_start_from_timer_context();
		}

	}
	LOGFEC("Flush Timer Stopped\n");
	RETURNS;
}

RVOID 	dz_fec_flush_timer_stop(VOID)
{
	if (fec_flush_timer_thread) {
		fec_flush_timer_stop = TRUE;
		kthread_stop(fec_flush_timer_thread);
	}
}

// This function MUST be protected by active_lock
// Also if we are in function, then it means that the previous
// flush buffer is free and reset
RVOID dz_fec_flush_switchover(VOID)
{
	PFEC_TABLE	tmp_tables 				= NULL;

	//Table Switching
	tmp_tables 							= fec_tables_active;
	fec_tables_active 					= fec_tables_flush;
	fec_tables_flush 					= tmp_tables;

	//Reset Counters
	fec_tables_flush->total_flush_cnt 	= (fec_active_count > fec_table_size) ? fec_table_size : fec_active_count;
	LOGD("Total Flush Count = %lli\n", fec_tables_flush->total_flush_cnt);
	fec_active_count					= 0;
}

RVOID dz_flush_iodone(PVOID iorequest , INT error)
{
	PIOREQUEST		ior 	= (PIOREQUEST)iorequest;
	PIOREQUEST		parent_iorequest 	= ior->ior_parent;
	PFEC_WRITE		fecw 	= (PFEC_WRITE)ior->ior_private;
	//ior->error = error;

	//This thread will continue to busywait till the 
	//read_ref_cnt become zero
check_reads:
	FEC_ENTRY_WRITE_LOCK(fecw);
	if (fecw->read_ref_cnt) {
		FEC_ENTRY_WRITE_UNLOCK(fecw);
		//Busy Wait of 10 micro second
		DELAY_MICRO_SECONDS(10);
		goto check_reads;
		
	}
	if (error) {
		fecw->state 	= FECBUF_STATE_FLUSH_ERROR;
	} else {
		fecw->state 	= FECBUF_STATE_FLUSH_COMPLETED;
	}
	FEC_ENTRY_WRITE_UNLOCK(fecw);

	//Now remove the fecw entry from read table
	FEC_DELETE_WRITE_BUFFER_FROM_LBA_TABLE(fecw);

	if (error) {
		//TODO: We need to check error scenario as well
		//We should retry flush operation for this entry
		print_iorequest(iorequest, "Flush Request with error");
		BUG_ON(1);
	}

	LOGD("Child done count = %u, pba_index = %lli, lba = %lli, iorequest = %p, fecw = %p\n",
		ATOMIC_READ(parent_iorequest->ior_child_cnt),
		ior->lba, fecw->lba, ior, fecw);

	ATOMIC64_INC(fec_cur_flush_count);
	if (atomic_dec_and_test(&parent_iorequest->ior_child_cnt)) {

		LOGFEC("Flush Completed\n");
		spin_lock(&fec_active_lock);			
		fec_tables_flush->flush_in_progress = FALSE;
		spin_unlock(&fec_active_lock);			
		time_flush_end = get_jiffies_64();	
		iorequest_put(parent_iorequest);
	}
	fecw->state 			= FECBUF_STATE_IDLE;
	fecw->lba 				= 0;
	fecw->read_ref_cnt 		= 0;
	memset(fecw->bv_page, 0, PAGE_SIZE);
	dz_fec_put_buffer_index(fecw->index, fec_table_size);

	//freeing page will be taken care
	//by bec engine during dz_bec_pba_page_add_write.
	//we could have added this page to bec cache from here itself
	//but this would lead to redundant insertion especially when
	//there are lots of dedupes and overwrites.
	//If we have to process page addition in cache from here then 
	//it would require to communicate the status of each iorequest
	//here. Based on that status a decision could be made for bec addition.
	//This seems to be an overhead.
	//dz_bio_bvec_write_page_free(bio);
	iorequest_put(ior);
	return;
}

PIOREQUEST dz_fec_create_flush_io(PFEC_WRITE fec_data_entry)
{
	PIOREQUEST		iorequest;

	iorequest = dz_io_alloc();
	if (!iorequest) {
		LOGFECE("Unable to allocate memory for iorequest\n");
		return NULL;
	}

	MEMSET(iorequest);
	iorequest->ior_bv_buffer	= fec_data_entry->bv_page;
	iorequest->ior_donefun = dz_flush_iodone;
	iorequest->ior_bio 			= NULL;
	//By this time, the correct pba_index is already set because
	//disk write is completed. So iorequest->lba will point the exact
	//disk pba index number. This will passed to bec layer since radix tree
	//needs pba_index.
	iorequest->ior_lba 			= (GET_LBA_ENTRY(fec_data_entry->lba))->pba_index;
	iorequest->ior_private		= fec_data_entry;
	ATOMIC_SET(iorequest->ior_ref_cnt, 0);
	//iorequest_get(iorequest);
	return iorequest;
}

//This function will create a flush io request which will be responsible
//for flushing the fecw's (frontend cache write buffer) into the lower layer.
// Here lower layer means backend cache engine
//It will eventually call dz_fec_flush_io

RVOID dz_fec_create_flush_iorequest(VOID)
{
	PFEC_WRITE	fecw 		= NULL;
	U64 count;
	U64			local_flush_cnt = 0;
	PIOREQUEST  flush_iorequest = dz_io_alloc();
	if (!flush_iorequest) {
		LOGFECE("Unable to allocate memory for iorequest\n");
		//TODO:
		BUG_ON(1);
	}

	MEMSET(flush_iorequest);

	INIT_LIST_HEAD(&flush_iorequest->ior_flushq_head);
	spin_lock(&fec_flush_lock);
	local_flush_cnt = fec_tables_flush->total_flush_cnt;
	if (local_flush_cnt <= 0) {
		spin_unlock(&fec_flush_lock);
		LOGFECW("Flush count is Zero %lli\n", local_flush_cnt);
		dz_io_free(flush_iorequest);
		return;
	}
	fec_tables_flush->total_flush_cnt = 0;

	for (count = 0; count < local_flush_cnt; count++) {
		fecw = list_first_entry(&fec_tables_flush->flushq_head, union dz_fec_data_write_s, flushq);
		list_del(&fecw->flushq);
		list_add_tail(&fecw->flushq, &flush_iorequest->ior_flushq_head);
	}
	INIT_LIST_HEAD(&fec_tables_flush->flushq_head);
	spin_unlock(&fec_flush_lock);

	ATOMIC_SET(flush_iorequest->ior_child_cnt, local_flush_cnt);

	ATOMIC64_INC(fec_flush_ops_count);

	LOGFEC("Flush Request No. %lli Ready to be processed %p\n",
		(U64)ATOMIC64_READ(fec_flush_ops_count), flush_iorequest);

	fecw = list_first_entry(&flush_iorequest->ior_flushq_head, union dz_fec_data_write_s, flushq);
	dz_q_iorequest(flush_thread_pool, flush_iorequest); // It will call dz_fec_flush_io()
}

// This function will create child iorequests for each fecw buffers.
// Argument parent_iorequest stores no. of childs as child_cnt.
// These child iorequests will eventually be flushed/stored into 
// backend cache engine. This will save time to fill the backend cache.
// Here the actual meaning of flushing means, flushing of fec buffers to
// backend cache engine.

RVOID dz_fec_flush_io(PIOREQUEST parent_iorequest)
{
	PFEC_WRITE	fecw 		= NULL;
	PIOREQUEST	fiorequest 			= NULL;
	U64 count;
	U64			local_flush_cnt = 0;

	if (list_empty(&parent_iorequest->ior_flushq_head)) {
		LOGFEC("Flushq can never be empty\n");
		BUG_ON(1);
	}
	local_flush_cnt = (U64)ATOMIC_READ(parent_iorequest->ior_child_cnt);
	time_flush_prepare_start = get_jiffies_64();
	
	ATOMIC64_INC(fec_cur_flush_ops_count);
	LOGFEC("Serving Flush request [%lli/%lli] %p in progress with %lli FEC Write Buffers\n",
		(U64)ATOMIC64_READ(fec_flush_ops_count),
		(U64)ATOMIC64_READ(fec_cur_flush_ops_count),
		parent_iorequest, local_flush_cnt);
		


	for (count = 0; count < local_flush_cnt; count++) {
		fecw = list_first_entry(&parent_iorequest->ior_flushq_head, union dz_fec_data_write_s, flushq);
		list_del(&fecw->flushq);

		switch(fecw->state) {


			case FECBUF_STATE_DEDUPE_WRITE_COMPLETED:
			//This is the case which needs flushing
				//LOGFEC("Found State : FEC_BUFFER_STATE_ACTIVE\n");
				break;

			case FECBUF_STATE_DEDUPE_WRITE_IN_PROGRESS:
				LOGD("%lli:Flush:WIP LBA %lli with iorequest %p\n", count, fecw->lba, fiorequest);
				wait_for_completion(&fecw->io_completion.completion);
			//This is the case which needs flushing
				//LOGFEC("Found State : FEC_BUFFER_STATE_ACTIVE\n");
				break;

			case FECBUF_STATE_IDLE:
			case FECBUF_STATE_ALLOCATED:
			default:
				LOGR("%lli:Flush IO Start for LBA %lli with iorequest %p, child_cnt = %u\n",
					count, fecw->lba, parent_iorequest,
					ATOMIC_READ(parent_iorequest->ior_child_cnt));
				print_fecws(fecw,"Unexpected buffer state");
				BUG_ON(1);
		}

		//Change the current state to Flush started
		FEC_ENTRY_WRITE_LOCK(fecw);
		fecw->state 	= FECBUF_STATE_FLUSH_STARTED;
		FEC_ENTRY_WRITE_UNLOCK(fecw);

start_fiorequest:
		fiorequest = dz_fec_create_flush_io(fecw);
		if (!fiorequest) {
			//TODO:Need to find out if 1 milli second delay is a good one
			LOGFEC("Unable to get fiorequest, Retrying it\n");
			DELAY_MICRO_SECONDS(100);
			goto start_fiorequest;
		}
		fiorequest->ior_parent = parent_iorequest;
		LOGD("%lli:Flush IO Start for LBA %lli with iorequest %p\n", count, fecw->lba, fiorequest);
		dz_bec_proceed_pba_page_add_write(fiorequest);
		//SL1;
		//SLEEP_MILLI_SECONDS(200);
	}
	return;
}

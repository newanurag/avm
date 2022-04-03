/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "fec_engine.h"

extern SPINLOCK	print_iorequest_lock;
SPINLOCK	fec_active_lock;
SPINLOCK	fec_flush_lock;			//Used in flush biodone
SPINLOCK	fec_read_flush_lock;	//Used between Read and flush completed context

extern INT 		dz_switch_fec_engine;
extern U64 		fec_table_size;
extern U64		fec_active_threshold_perc;
extern U64		fec_active_threshold;
extern U64		fec_active_count;
extern U64		fec_flush_count;
extern ATOMIC64	fec_active_count_sequence;
extern ATOMIC64	fec_free_count_sequence;
extern ATOMIC64	fec_cur_flush_count;
extern ATOMIC64	fec_flush_ops_count;
extern ATOMIC64	fec_cur_flush_ops_count;
extern ATOMIC64	fec_memory_reads_count;
extern ATOMIC64	fec_forwarded_reads_count;
extern ATOMIC64	fec_iocount_reads_partial_page;
extern ATOMIC64	fec_iocount_reads_single_page;
extern ATOMIC64	fec_iocount_reads_single_aligned_page;
extern ATOMIC64	fec_iocount_reads_single_unaligned_page;
extern ATOMIC64	fec_iocount_reads_multi_page;
extern ATOMIC64	fec_iocount_reads_multi_aligned_page;
extern ATOMIC64	fec_iocount_reads_multi_unaligned_page;
extern ATOMIC64	fec_iocount_reads_multi_tail_unaligned_page;
extern ATOMIC64	fec_iocount_reads_multi_head_unaligned_page;
extern ATOMIC64	fec_iocount_reads_multi_both_unaligned_page;
extern ATOMIC64	fec_iocount_writes_partial_page;
extern ATOMIC64	fec_iocount_writes_single_page;
extern ATOMIC64	fec_iocount_writes_single_aligned_page;
extern ATOMIC64	fec_iocount_writes_single_unaligned_page;
extern ATOMIC64	fec_iocount_writes_multi_page;
extern ATOMIC64	fec_iocount_writes_multi_aligned_page;
extern ATOMIC64	fec_iocount_writes_multi_unaligned_page;


//For allocation and freeing purpose
PFEC_TABLE 	fec_tables_1 				= NULL;
PFEC_WRITE 	fec_table_write_active_1	= NULL;

//For operational purpose
//Actually this is an Active table but in comments 
//it is referenced as Active Buffer used for storing writes
extern PFEC_TABLE 	fec_tables_active;
extern PFEC_TABLE 	fec_tables_flush ;

THREAD 		*fec_flush_timer_thread		= NULL;

BOOL		fec_flush_timer_stop		= FALSE;
ULONG 		fec_flush_interval			= DZ_FEC_FLUSH_INTERVAL;
PIOREQUEST	fec_flush_iorequest 		= NULL;
IOREQUEST	_fec_flush_iorequest;		//Fake IORequest for scheduling Flush thread

extern PDZ_THREAD_POOL flush_thread_pool;
extern USHORT num_read_threads;
extern USHORT num_write_threads;
extern USHORT num_flush_threads;

//U64			fec_engine_memory_usage		= 0;
//EXPORT_SYMBOL(fec_engine_memory_usage);


extern U64 		time_fec_write_start;
extern U64 		time_fec_write_switch;

VOID dz_fec_init_counters(VOID)
{
	fec_table_size 				= ONE_MILLION_IOPS;
	//fec_table_size 				= TWO_MILLION_IOPS;
	fec_active_count			= 0;
	fec_flush_count				= 0;
	fec_active_threshold		= (fec_active_threshold_perc * fec_table_size) / 100;
	SET_COUNTER(fec_active_count_sequence);
	SET_COUNTER(fec_free_count_sequence);
	SET_COUNTER(fec_cur_flush_count);
	SET_COUNTER(fec_flush_ops_count);
	SET_COUNTER(fec_cur_flush_ops_count);
	SET_COUNTER(fec_memory_reads_count);
	SET_COUNTER(fec_forwarded_reads_count);

	SET_COUNTER(fec_iocount_reads_partial_page);
	SET_COUNTER(fec_iocount_reads_single_page);
	SET_COUNTER(fec_iocount_reads_single_aligned_page);
	SET_COUNTER(fec_iocount_reads_single_unaligned_page);
	SET_COUNTER(fec_iocount_reads_multi_page);
	SET_COUNTER(fec_iocount_reads_multi_aligned_page);
	SET_COUNTER(fec_iocount_reads_multi_unaligned_page);
	SET_COUNTER(fec_iocount_reads_multi_tail_unaligned_page);
	SET_COUNTER(fec_iocount_reads_multi_head_unaligned_page);
	SET_COUNTER(fec_iocount_reads_multi_both_unaligned_page);

	SET_COUNTER(fec_iocount_writes_partial_page);
	SET_COUNTER(fec_iocount_writes_single_page);
	SET_COUNTER(fec_iocount_writes_single_aligned_page);
	SET_COUNTER(fec_iocount_writes_single_unaligned_page);
	SET_COUNTER(fec_iocount_writes_multi_page);
	SET_COUNTER(fec_iocount_writes_multi_aligned_page);
	SET_COUNTER(fec_iocount_writes_multi_unaligned_page);

	FEC_COUNTER_U64_SET(fec_engine_memory_usage, 0);
}
EXPORT_SYMBOL(dz_fec_init_counters);

INT dz_fec_engine_init(VOID)
{
	INT ret = SUCCESS;
	MARK_MEMORY_USAGE();

	if (dz_switch_fec_engine == DZ_DISABLED) {
		LOGFECF(" ______________________________________________________ \n");
		LOGFECF("|                                                      |\n");
		LOGFECF("|         Frontend Cache (FEC) Engine Disabled         |\n");
		LOGFECF("|______________________________________________________|\n");
		PRINT_MEMORY_USAGE(FEC_COUNTER_U64_GET(fec_engine_memory_usage), "FEC Engine");
		RETURNS;
	}

	LOGFECF(" ______________________________________________________ \n");
	LOGFECF("|                                                      |\n");
	LOGFECF("|   Frontend Cache (FEC) Engine Initialization Begin   |\n");
	LOGFECF("|______________________________________________________|\n");
	dz_fec_init_counters();
	ret = dz_create_read_thread_pool(num_read_threads);
	//TODO: Check for failure
	ret = dz_create_write_thread_pool(num_write_threads);
	ret = dz_create_flush_thread_pool(num_flush_threads);

	fec_table_write_active_1 = dz_create_fec_data_write_table(fec_table_size);
	if (fec_table_write_active_1 == NULL)
		goto exit_failure;

	//Create a table container for Current one
	fec_tables_1 = dz_kmalloc(sizeof(FEC_TABLE), GFP_KERNEL);
	if (fec_tables_1 == NULL) {
		goto exit_failure;

	}
	INIT_LIST_HEAD(&fec_tables_1->flushq_head);

	fec_tables_1->write_table 		= fec_table_write_active_1;

	fec_tables_active 				= fec_tables_1;
	
	fec_tables_flush 				= fec_tables_1;
	fec_tables_flush->flush_in_progress = FALSE;
	fec_tables_flush->total_flush_cnt = 0;

	spin_lock_init(&print_iorequest_lock);
	spin_lock_init(&fec_active_lock);
	spin_lock_init(&fec_flush_lock);
	spin_lock_init(&fec_read_flush_lock);


	//Sleeping so that timer gets started
	SLEEP_MILLI_SECONDS(300);
	fec_flush_timer_thread = kthread_run (dz_fec_flush_timer, NULL, "flush_timer_thread");
	if (IS_ERR(fec_flush_timer_thread)) {
		LOGFECE("Unable to create flush timer thread\n");
		goto exit_failure;
	}
	MEMSET(&_fec_flush_iorequest);
	fec_flush_iorequest = & _fec_flush_iorequest;
	fec_flush_iorequest->ior_thread_pool = flush_thread_pool;

	PRINT_MEMORY_USAGE(FEC_COUNTER_U64_GET(fec_engine_memory_usage), "FEC Engine");

	LOGFECF(" ______________________________________________________ \n");
	LOGFECF("|                                                      |\n");
	LOGFECF("|   Frontend Cache (FEC) Engine Initialization End     |\n");
	LOGFECF("|______________________________________________________|\n");
	RETURNS;

exit_failure:
	if (fec_table_write_active_1) {
		dz_destroy_fec_data_write_table(fec_table_write_active_1, fec_table_size);
	}

	if (fec_tables_1) {
		dz_kfree(fec_tables_1, sizeof(FEC_TABLE));
	}
	dz_destroy_read_thread_pool();
	dz_destroy_write_thread_pool();
	dz_destroy_flush_thread_pool();
	RETURNF;
}
EXPORT_SYMBOL(dz_fec_engine_init);

RVOID dz_fec_engine_exit(VOID)
{
	MARK_MEMORY_USAGE();
	if (dz_switch_fec_engine == DZ_DISABLED) {
		LOGFECF(" ______________________________________________________ \n");
		LOGFECF("|                                                      |\n");
		LOGFECF("|         Frontend Cache (FEC) Engine Disabled         |\n");
		LOGFECF("|______________________________________________________|\n");
		PRINT_MEMORY_USAGE(FEC_COUNTER_U64_GET(fec_engine_memory_usage), "FEC Engine");
		return;
	}
	LOGFECF(" ______________________________________________________ \n");
	LOGFECF("|                                                      |\n");
	LOGFECF("|       Frontend Cache (FEC) Engine Exit Begin         |\n");
	LOGFECF("|______________________________________________________|\n");

	dz_fec_flush_timer_stop();

	if (fec_tables_1) {
		dz_kfree(fec_tables_1, sizeof(FEC_TABLE));
	}

	dz_destroy_fec_data_write_table(fec_table_write_active_1, fec_table_size);
	dz_destroy_read_thread_pool();
	dz_destroy_write_thread_pool();
	dz_destroy_flush_thread_pool();
	PRINT_MEMORY_FREED(FEC_COUNTER_U64_GET(fec_engine_memory_usage), "FEC Engine");
	LOGFECF(" ______________________________________________________ \n");
	LOGFECF("|                                                      |\n");
	LOGFECF("|       Frontend Cache (FEC) Engine Exit End           |\n");
	LOGFECF("|______________________________________________________|\n");

}
EXPORT_SYMBOL(dz_fec_engine_exit);


RVOID dz_fec_table_reset(PFEC_TABLE tables)
{
	tables->total_flush_cnt = 0;
	//memset(tables->table_write, 0, fec_table_size * sizeof(FEC_DATA_WRITE));
}


RVOID print_fecws(PFEC_WRITE fecw, CHAR *str)
{
	if (str) {
		LOGFEC(" ______________________________________________________ \n");
		LOGFEC("|                                                      |\n");
		LOGFEC("|          Printing fecw Begin %s                      |\n", str);
		LOGFEC("|______________________________________________________|\n");

	} else {

		LOGFEC(" ______________________________________________________ \n");
		LOGFEC("|                                                      |\n");
		LOGFEC("|          Printing fecw Begin                         |\n");
		LOGFEC("|______________________________________________________|\n");

	}
	LOGFEC("fecw               = %p\n", fecw);
	LOGFEC("fecw->state        = %d\n", (UINT)fecw->state);
	LOGFEC("fecw->read_ref_cnt = %d\n", (UINT)fecw->read_ref_cnt);
	LOGFEC("fecw->lba          = %lli\n", fecw->lba);
	print_data(fecw->bv_page);

	if (str) {
		LOGFEC(" ______________________________________________________ \n");
		LOGFEC("|          Printing fecw End %s                        |\n", str);
		LOGFEC("|______________________________________________________|\n");
	} else {
		LOGFEC(" ______________________________________________________ \n");
		LOGFEC("|          Printing fecw End                           |\n");
		LOGFEC("|______________________________________________________|\n");
	}
}

RVOID dz_fec_io(PIOREQUEST iorequest)
{
	bio_data_dir(iorequest->ior_bio) == WRITE ? 
		dz_fec_align_write_io(iorequest) : 
		dz_fec_align_read_io(iorequest);
}

static int dz_fec_engine_module_init(void)
{
	LOGFEC(" ______________________________________________________ \n");
	LOGFEC("|                                                      |\n");
	LOGFEC("|           ZAIDSTOR FEC Engine Module Loaded          |\n");
	LOGFEC("|______________________________________________________|\n");

	RETURNS;
}


static void dz_fec_engine_module_exit(void)
{
	LOGFEC(" ______________________________________________________ \n");
	LOGFEC("|                                                      |\n");
	LOGFEC("|           ZAIDSTOR FEC Engine Module UnLoaded        |\n");
	LOGFEC("|______________________________________________________|\n");
}

module_init(dz_fec_engine_module_init);
module_exit(dz_fec_engine_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

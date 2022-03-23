/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

//#include "alignment_engine.h"
#include "prototypes.h"

extern SPINLOCK	print_iorequest_lock;

extern INT 		dz_alignment_engine_switch;
extern ATOMIC64	align_iocount_reads_partial_page;
extern ATOMIC64	align_iocount_reads_single_page;
extern ATOMIC64	align_iocount_reads_single_aligned_page;
extern ATOMIC64	align_iocount_reads_single_unaligned_page;
extern ATOMIC64	align_iocount_reads_multi_page;
extern ATOMIC64	align_iocount_reads_multi_aligned_page;
extern ATOMIC64	align_iocount_reads_multi_unaligned_page;
extern ATOMIC64	align_iocount_reads_multi_tail_unaligned_page;
extern ATOMIC64	align_iocount_reads_multi_head_unaligned_page;
extern ATOMIC64	align_iocount_reads_multi_both_unaligned_page;
extern ATOMIC64	align_iocount_writes_partial_page;
extern ATOMIC64	align_iocount_writes_single_page;
extern ATOMIC64	align_iocount_writes_single_aligned_page;
extern ATOMIC64	align_iocount_writes_single_unaligned_page;
extern ATOMIC64	align_iocount_writes_multi_page;
extern ATOMIC64	align_iocount_writes_multi_aligned_page;
extern ATOMIC64	align_iocount_writes_multi_unaligned_page;

extern USHORT num_align_read_threads;
extern USHORT num_align_write_threads;

extern ATOMIC64    align_iocount_disk_read;
extern ATOMIC64    align_iocount_disk_read_internal;
extern ATOMIC64    align_iocount_disk_write;
extern ATOMIC64    align_iocount_disk_write_internal;

static VOID dz_align_init_counters(VOID)
{
	SET_COUNTER(align_iocount_disk_read);
	SET_COUNTER(align_iocount_disk_write);

	SET_COUNTER(align_iocount_disk_read_internal);
	SET_COUNTER(align_iocount_disk_write_internal);

	SET_COUNTER(align_iocount_reads_partial_page);
	SET_COUNTER(align_iocount_reads_single_page);
	SET_COUNTER(align_iocount_reads_single_aligned_page);
	SET_COUNTER(align_iocount_reads_single_unaligned_page);
	SET_COUNTER(align_iocount_reads_multi_page);
	SET_COUNTER(align_iocount_reads_multi_aligned_page);
	SET_COUNTER(align_iocount_reads_multi_unaligned_page);
	SET_COUNTER(align_iocount_reads_multi_tail_unaligned_page);
	SET_COUNTER(align_iocount_reads_multi_head_unaligned_page);
	SET_COUNTER(align_iocount_reads_multi_both_unaligned_page);

	SET_COUNTER(align_iocount_writes_partial_page);
	SET_COUNTER(align_iocount_writes_single_page);
	SET_COUNTER(align_iocount_writes_single_aligned_page);
	SET_COUNTER(align_iocount_writes_single_unaligned_page);
	SET_COUNTER(align_iocount_writes_multi_page);
	SET_COUNTER(align_iocount_writes_multi_aligned_page);
	SET_COUNTER(align_iocount_writes_multi_unaligned_page);

	ALIGN_COUNTER_U64_SET(align_engine_memory_usage, 0);
}
//EXPORT_SYMBOL(dz_align_init_counters);

INT dz_align_engine_init(VOID)
{
	INT ret = SUCCESS;
	MARK_MEMORY_USAGE();

	//if (dz_align_engine_switch == DZ_DISABLED) {
	//	RETURNS;
	//}

	LOGALIGNF(" ______________________________________________________ \n");
	LOGALIGNF("|                                                      |\n");
	LOGALIGNF("|        Alignment Engine Initialization Begin         |\n");
	LOGALIGNF("|______________________________________________________|\n");
	dz_align_init_counters();
	ret = dz_create_align_read_thread_pool(num_align_read_threads);
	//TODO: Check for failure
	if (ret != SUCCESS) {
		goto exit_failure;
	}

	ret = dz_create_align_write_thread_pool(num_align_write_threads);
	//TODO: Check for failure
	if (ret != SUCCESS) {
		goto exit_failure;
	}


	PRINT_MEMORY_USAGE(ALIGN_COUNTER_U64_GET(align_engine_memory_usage), "ALIGN Engine");

	LOGALIGNF(" ______________________________________________________ \n");
	LOGALIGNF("|                                                      |\n");
	LOGALIGNF("|         Alignment Engine Initialization End          |\n");
	LOGALIGNF("|______________________________________________________|\n");
	RETURNS;

exit_failure:
	dz_destroy_align_read_thread_pool();
	dz_destroy_align_write_thread_pool();
	RETURNF;
}
EXPORT_SYMBOL(dz_align_engine_init);

RVOID dz_align_engine_exit(VOID)
{
	MARK_MEMORY_USAGE();
	LOGALIGNF(" ______________________________________________________ \n");
	LOGALIGNF("|                                                      |\n");
	LOGALIGNF("|             Alignment Engine Exit Begin              |\n");
	LOGALIGNF("|______________________________________________________|\n");

	dz_destroy_align_read_thread_pool();
	dz_destroy_align_write_thread_pool();
	PRINT_MEMORY_FREED(ALIGN_COUNTER_U64_GET(align_engine_memory_usage), "ALIGN Engine");
	LOGALIGNF(" ______________________________________________________ \n");
	LOGALIGNF("|                                                      |\n");
	LOGALIGNF("|             Alignment Engine Exit End                |\n");
	LOGALIGNF("|______________________________________________________|\n");

}
EXPORT_SYMBOL(dz_align_engine_exit);

RVOID dz_align_io(PIOREQUEST iorequest)
{
	bio_data_dir(iorequest->ior_bio) == WRITE ? 
		dz_align_write_io(iorequest) : 
		dz_align_read_io(iorequest);
}

static int dz_align_engine_module_init(void)
{
	LOGALIGN(" ______________________________________________________ \n");
	LOGALIGN("|                                                      |\n");
	LOGALIGN("|      ZAIDSTOR AVM Alignment Engine Module Loaded     |\n");
	LOGALIGN("|______________________________________________________|\n");

	RETURNS;
}


static void dz_align_engine_module_exit(void)
{
	LOGALIGN(" ______________________________________________________ \n");
	LOGALIGN("|                                                      |\n");
	LOGALIGN("|    ZAIDSTOR AVM Alignment Engine Module UnLoaded     |\n");
	LOGALIGN("|______________________________________________________|\n");
}

module_init(dz_align_engine_module_init);
module_exit(dz_align_engine_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

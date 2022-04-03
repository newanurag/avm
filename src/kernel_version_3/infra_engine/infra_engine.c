/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PPBA_BLOCK	pba_table;
extern PHASH_BLOCK	hash_table;
extern PLBA_BLOCK	lba_table;
extern CHAR sysfs_avm_get_target_name[DZ_SYSFS_PAGE_SIZE];
extern SIZE				dz_pba_page_mempool_count;
extern U64 				lba_table_size;
extern U64 				pba_table_size;
extern U64 				hash_table_size;
extern BITMAP 			pba_bitmap;
extern PDZ_THREAD_POOL read_thread_pool;
extern PDZ_THREAD_POOL write_thread_pool;
extern SPINLOCK			print_iorequest_lock;

//U64		infra_engine_memory_usage		= 0;
//EXPORT_SYMBOL(infra_engine_memory_usage);
//
VOID    dz_infra_engine_init_counters(VOID)
{
	BASE_COUNTER_U64_SET(infra_engine_memory_usage, 0);
}

RVOID dz_infra_engine_exit(VOID)
{
	MARK_MEMORY_USAGE();
	LOGINFF(" ______________________________________________________ \n");
	LOGINFF("|                                                      |\n");
	LOGINFF("|       Infrastructure (Infra) Engine Exit Begin       |\n");
	LOGINFF("|______________________________________________________|\n");

	dz_destroy_memory_pools();

	PRINT_MEMORY_FREED(BASE_COUNTER_U64_GET(infra_engine_memory_usage), "Infrastructure Engine");
	LOGINFF(" ______________________________________________________ \n");
	LOGINFF("|                                                      |\n");
	LOGINFF("|       Infrastructure (Infra) Engine Exit End         |\n");
	LOGINFF("|______________________________________________________|\n");

}
EXPORT_SYMBOL(dz_infra_engine_exit);

INT dz_infra_engine_init(VOID)
{
	INT ret = SUCCESS;
	MARK_MEMORY_USAGE();
	LOGINFF(" ______________________________________________________ \n");
	LOGINFF("|                                                      |\n");
	LOGINFF("|  Infrastructure (Infra) Engine Initialization Begin  |\n");
	LOGINFF("|______________________________________________________|\n");

	dz_infra_engine_init_counters();

	ret = dz_object_init();
	if (ret == FAILURE) {
		goto exit_failure;
	}
	ret = dz_create_memory_pools();
	if (ret == FAILURE) {
		goto exit_failure;
	}
	PRINT_MEMORY_USAGE(BASE_COUNTER_U64_GET(infra_engine_memory_usage), "Infrastructure Engine");

	LOGINFF(" ______________________________________________________ \n");
	LOGINFF("|                                                      |\n");
	LOGINFF("|  Infrastructure (Infra) Engine Initialization End    |\n");
	LOGINFF("|______________________________________________________|\n");
	RETURNS;

exit_failure:
	dz_infra_engine_exit();
	RETURNF;
}
EXPORT_SYMBOL(dz_infra_engine_init);

#define DZ_AVM_LOG_FILE		"/dev/dzlog"

static int dz_infra_engine_module_init(void)
{
	INT ret;
	//Open the kernel log file so that loggins should start immediately
	ret = dz_avm_log_file_open();
    LOGINFF(" ______________________________________________________ \n");
    LOGINFF("|                                                      |\n");
    LOGINFF("|           ZAIDSTOR Infra Engine Module Loaded        |\n");
    LOGINFF("|______________________________________________________|\n");

    RETURNS;
}

static void dz_infra_engine_module_exit(void)
{
    LOGINFF(" ______________________________________________________ \n");
    LOGINFF("|                                                      |\n");
    LOGINFF("|           ZAIDSTOR Infra Engine Module UnLoaded      |\n");
    LOGINFF("|______________________________________________________|\n");
	dz_avm_log_file_close();
}

module_init(dz_infra_engine_module_init);
module_exit(dz_infra_engine_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern OBJECT_LIST 	global_object_list;
extern U64 lba_table_size;
extern U64 pba_table_size;
extern U64 hash_table_size;
extern U64 sysfs_avm_free_page_count;
extern CHAR sysfs_avm_get_list_of_volumes[DZ_SYSFS_PAGE_SIZE];
extern PPBA_BLOCK		pba_table;
extern PHASH_BLOCK		hash_table;
extern PLBA_BLOCK		lba_table;

struct kobject *sysfs_avm_kernel_kobj;
EXPORT_SYMBOL(sysfs_avm_kernel_kobj);


static struct attribute * kernel_attrs[] = {
    NULL,
    NULL,
    NULL,
#ifdef CONFIG_PROFILING
    NULL,
#endif
#ifdef CONFIG_KEXEC
    NULL,
    NULL,
    NULL,
    NULL,
#endif
    NULL,
    NULL
};



static struct attribute_group kernel_attr_group = {
    .attrs = kernel_attrs,
};



INT avm_sysfs_engine_init(VOID)
{
	int error;

	sysfs_avm_kernel_kobj = kobject_create_and_add("avm", NULL);
	if (!sysfs_avm_kernel_kobj) {
		error = -ENOMEM;
		return -1;
	}
	error = sysfs_create_group(sysfs_avm_kernel_kobj, &kernel_attr_group);
	if (error) { 
		kobject_put(sysfs_avm_kernel_kobj);
	}

	return SUCCESS;
}

INT avm_sysfs_engine_exit(VOID)
{
	sysfs_remove_group(sysfs_avm_kernel_kobj, &kernel_attr_group);
	kobject_put(sysfs_avm_kernel_kobj);
	return SUCCESS;
}

static int dz_sysfs_engine_module_init(void)
{
	LOGSYS("%s", " ______________________________________________________ \n");
	LOGSYS("%s", "|                                                      |\n");
	LOGSYS("%s", "|           ZAIDSTOR Sysfs Engine Module Registering   |\n");
	LOGSYS("%s", "|______________________________________________________|\n");

	avm_sysfs_engine_init();
	avm_sysfs_kernel_init();
	avm_sysfs_target_engine_init();
	avm_sysfs_alignment_engine_init();
	avm_sysfs_bec_engine_init();
	avm_sysfs_fec_engine_init();
	avm_sysfs_dedupe_engine_init();
	avm_sysfs_volume_engine_init();
	avm_sysfs_metadata_engine_init();

	LOGSYS("%s", " ______________________________________________________ \n");
	LOGSYS("%s", "|                                                      |\n");
	LOGSYS("%s", "|           ZAIDSTOR Sysfs Engine Module Registered    |\n");
	LOGSYS("%s", "|______________________________________________________|\n");
	RETURNS;
}

static void dz_sysfs_engine_module_exit(void)
{
	LOGSYS("%s", " ______________________________________________________ \n");
	LOGSYS("%s", "|                                                      |\n");
	LOGSYS("%s", "|           ZAIDSTOR Sysfs Engine Module UnRegistering |\n");
	LOGSYS("%s", "|______________________________________________________|\n");

	avm_sysfs_kernel_exit();
	avm_sysfs_target_engine_exit();
	avm_sysfs_alignment_engine_exit();
	avm_sysfs_bec_engine_exit();
	avm_sysfs_fec_engine_exit();
	avm_sysfs_dedupe_engine_exit();
	avm_sysfs_volume_engine_exit();
	avm_sysfs_metadata_engine_exit();
	avm_sysfs_engine_exit();

	LOGSYS("%s", " ______________________________________________________ \n");
	LOGSYS("%s", "|                                                      |\n");
	LOGSYS("%s", "|           ZAIDSTOR Sysfs Engine Module UnRegistered  |\n");
	LOGSYS("%s", "|______________________________________________________|\n");
}

module_init(dz_sysfs_engine_module_init);
module_exit(dz_sysfs_engine_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

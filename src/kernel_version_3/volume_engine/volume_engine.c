/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PBLOCK_DEVICE	DATA_DISK_DEVICE;
extern NAME				DATA_DISK_DEVICE_NAME;

extern PBLOCK_DEVICE	METADATA_DISK_DEVICE;
extern NAME				METADATA_DISK_DEVICE_NAME;

extern PBLOCK_DEVICE	TARGET_DEVICE;
extern NAME				TARGET_DEVICE_NAME;

extern SIZE				TARGET_DEVICE_SIZE;
extern PAVM_DEVICE 		aisa_device_object;
extern PBLOCK_DEVICE 	avm_metadata_bdev;

extern OBJECT_LIST 	global_object_list;
extern BITMAP		pba_bitmap;

extern U64 			lba_table_size;
extern U64 			pba_table_size;
extern U64 			hash_table_size;

extern PPBA_BLOCK	pba_table;
extern PHASH_BLOCK	hash_table;
extern PLBA_BLOCK	lba_table;

extern U64 			md_lock_table_size;
extern PMD_LOCK		md_lock_table;

extern SPINLOCK 	lba_table_lock; 
extern SPINLOCK 	hash_table_lock; 
extern SPINLOCK 	pba_table_lock; 

extern PDZ_THREAD_POOL read_thread_pool;
extern PDZ_THREAD_POOL write_thread_pool;
extern PSUPERBLOCK	superblock;
extern U64 dedupe_tot_logical_space;
extern U64	tot_free_space;

U64 dz_get_free_space(VOID)
{
	return tot_free_space;
}
EXPORT_SYMBOL(dz_get_free_space);

RVOID dz_volume_decrease_free_space(U64 space)
{
	tot_free_space -= space;
}

RVOID dz_volume_increase_free_space(U64 space)
{
	tot_free_space += space;
}

RINT dz_volume_set_used_space(PDM_TARGET target, PDDVOL ddvol)
{
	DDVOL_LBA_BLOCK	*lba_table_local 	= NULL;
	U64			table_size 	= 0;
	U64			count 		= 0;
	S64			lba_index 	= 0;
	S64		prev_lba_index 	= 0;
	INT			ret			= SUCCESS;
	BOOL		used		= TRUE;

	//TODO:Protect it through spin lock
	
	table_size = ((ddvol->ddvol_obj.obj_end_lba - ddvol->ddvol_obj.obj_start_lba + 1) * SECTOR_SIZE) / LBA_BLOCK_SIZE;
	if (table_size > dz_get_free_space()) {
		target->error = "AISA-Error: Insufficient free space to create volume";
		LOGE("AISA-Error: Insufficient free space to create volume\n");
		LOGE("Requested Space = %lli (4K Blocks), Current Free Space = %lli (4K Blocks)\n",
		table_size, dz_get_free_space());
		ret 	= -ENOMEM;
		used 	= FALSE;
		goto exit_failure;
	}
	PRINT_U64(dz_get_free_space());
	dz_volume_decrease_free_space(table_size);
	ddvol->ddvol_lba_table = NULL;
	ddvol->ddvol_table_size = table_size;

	lba_table_local = dz_volume_create_lba_table(table_size);
	if (!lba_table_local) {
		target->error = "AISA-Error: Unable to create lba table";
		LOGVOLE("AISA-Error: Unable to create lba table\n");
		ret = -ENOMEM;
		goto exit_failure;
	}

	//TODO:Currently volumes are created for contiguous lbas
	//We need to provide non-contagious as well
	for (count = 0; count < table_size; count++) {
		lba_index = dz_get_free_lba_index();
		
		ret = lba_index - prev_lba_index;
		if ((lba_index > 0) && (prev_lba_index > 0) && (ret != 1)) {
			target->error = "AISA-Error: Insufficient contagious free blocks to create volume";
			LOGE("AISA-Error: Insufficient contagious free blocks to create volume\n");
			LOGE("prev_lba_index = %lli, lba_index = %lli\n", prev_lba_index, lba_index);
			ret = -ENOMEM;
			goto exit_failure;

		}
	
		if (lba_index >= 0) {
			lba_table_local[count].lba_index = lba_index;
		} else {
			target->error = "AISA-Error: Insufficient free blocks to create volume";
			LOGE("AISA-Error: Insufficient free blocks to create volume\n");
			ret = -ENOMEM;
			goto exit_failure;
		}
		prev_lba_index	= lba_index;
	}
	PRINT_U64(dz_get_free_space());
	ddvol->ddvol_lba_table = lba_table_local;
	RETURNS;

exit_failure:
	if (ddvol->ddvol_lba_table) {
		dz_volume_destroy_lba_table(ddvol);
	}
	if (used) {
		dz_volume_increase_free_space(table_size);
	}
	return ret;
}

RVOID dz_volume_freeup_space(PDDVOL ddvol)
{
	DDVOL_LBA_BLOCK	*lba_table_local 	= NULL;
	U64			table_size 	= 0;
	U64			count 		= 0;
	S64			lba_index 	= 0;

	//TODO:Protect it through spin lock
	table_size = ddvol->ddvol_table_size;
	lba_table_local = ddvol->ddvol_lba_table;
	for (count = 0; count < table_size; count++) {
		lba_index = lba_table_local[count].lba_index;
		dz_put_lba_index(lba_index);
	}
	dz_volume_increase_free_space(table_size);
}

static int dz_volume_engine_module_init(void)
{
	int result;
    LOGVOL(" ______________________________________________________ \n");
    LOGVOL("|                                                      |\n");
    LOGVOL("|           ZAIDSTOR Volume Engine Registering         |\n");
    LOGVOL("|______________________________________________________|\n");

    result = dz_register_datavol_target();
    if (result < 0) {
        RETURNF;
    }
    result = dz_register_vmvol_target();
    if (result < 0) {
        RETURNF;
    }
    LOGVOL(" ______________________________________________________ \n");
    LOGVOL("|                                                      |\n");
    LOGVOL("|           ZAIDSTOR Volume Engine Registered          |\n");
    LOGVOL("|______________________________________________________|\n");
    //dz_sysfs_init();
    RETURNS;
}


static void dz_volume_engine_module_exit(void)
{
	LOGVOL(" ______________________________________________________ \n");
	LOGVOL("|                                                      |\n");
	LOGVOL("|     ZAIDSTOR Volume Engine Module UnRegistering      |\n");
	LOGVOL("|______________________________________________________|\n");
	//dz_sysfs_exit();
	//dz_check_memory_leak();
	dz_unregister_datavol_target();
	dz_unregister_vmvol_target();

    LOGVOL(" ______________________________________________________ \n");
    LOGVOL("|                                                      |\n");
    LOGVOL("|     ZAIDSTOR Volume Engine Module UnRegistered       |\n");
    LOGVOL("|______________________________________________________|\n");
}


module_init(dz_volume_engine_module_init);
module_exit(dz_volume_engine_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

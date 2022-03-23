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

RPVOID  dz_ddvol_create_snapshot(PDM_TARGET target, UINT argc, CHAR ** argv, INT *error)
{
	/* 
	Here it is how the information is provided from cli
	Start Sector Passed from cli (dmsetup cli)
	Size in terms of sectors passed from cli (dmsetup cli)
	argv[0] : Underlying device or Backstore Name e.g. /dev/loop0 
	argv[1] : Start sector of Underlying Device. It should be ignored as it will be set by us only
	argv[2] : Product Type; AISA, NORMAL, SNAPSHOT_READ, SNAPSHOT_WRITE
	argv[3] : Name of virtual device
	argv[4] : Magic No.
	argv[5] : UUID. Easier to be passed from cli
	argv[6] : Name of Volume for which snapshot to be taken
	*/
	PDDVOL	ddvol 						= NULL;
	PDEVICE_OBJECT 	parent_object 		= NULL;
	CHAR			*parent_vol_name 	= NULL;
	PDDVOL			parent_ddvol 		= NULL;
	PDDVOL_LBA_BLOCK lba_table			= NULL;
	PDDVOL_LBA_BLOCK lba_entry			= NULL;
	PDDVOL_LBA_BLOCK parent_lba_table	= NULL;
	PDDVOL_LBA_BLOCK parent_lba_entry	= NULL;
	PLBA_BLOCK 		aisa_lba_entry		= NULL;
	PPBA_BLOCK 		aisa_pba_entry		= NULL;
	PPBA_BLOCK 		aisa_pba_entry_col	= NULL;
	PHBA_BLOCK 		aisa_hba_entry		= NULL;
	U64					pba_index		= 0;
	U64				hash_pba_index		= 0;
	U64				table_size			= 0;
	U64				lba_index			= 0;
	INT			ret						= SUCCESS;
	INT			len = 0;
	INT		len_max = 0;
	BOOL	dm_flag = TRUE;
	U64		count	= 0;
	//Security Check //TODO:We need to check argv[0] with "/dev/mapper/AISA";
	/*
	if (strcmp(argv[0], aisa_device_object->obj.name) != 0) {
		target->error = "AISA-Error: Target Device Name is wrong. Can't proceed";
		LOG("AISA-Error: Target Device Name is wrong. Looking for %s\n", aisa_device_object->obj.name);
		ret = -ENODEV;
		goto exit_failure;
	}
	*/
	if (argc != 8) {
		target->error = "AISA-Error: Parent Volume is Missing";
		LOG("AISA-Error: Parent Volume is Missing\n");
		ret = -ENODEV;
		goto exit_failure;
	}

	parent_vol_name = (CHAR *) argv[6];
	parent_object = dz_find_object(&global_object_list, parent_vol_name);
	if (!parent_object) {
		target->error = "AISA-Error: Parent Volume not found";
		LOG("AISA-Error: Parent Volume %s not found\n", parent_vol_name);
		ret = -ENODEV;
		goto exit_failure;
	}

	ddvol = dz_kmalloc(sizeof(DDVOL), GFP_KERNEL);
	if (!ddvol) {
		target->error = "AISA-Error: memory allocation failed";
		ret = -ENOMEM;
		goto exit_failure;
	}
	MEMSET(ddvol);
	ddvol->ddvol_obj.obj_start_lba	= target->begin; 
	strncpy(ddvol->ddvol_obj.obj_dm_dev_name, argv[0], strlen(argv[0]));

	ddvol->ddvol_obj.obj_end_lba	= ddvol->ddvol_obj.obj_start_lba  + target->len - 1;
	
	len_max = NAME_LENGTH;
	len 	= strlen(argv[3]);
	len = len > len_max ? len_max: len;

	strncpy(ddvol->ddvol_obj.obj_name, 		argv[3], len);

	len_max = MAGICNO_LENGTH;
	len 	= strlen(argv[4]);
	len = len > len_max ? len_max: len;

	strncpy(ddvol->ddvol_obj.obj_magicno, 	argv[4], len);

	len_max = UUID_LENGTH;
	len 	= strlen(argv[5]);
	len = len > len_max ? len_max: len;

	strncpy(ddvol->ddvol_obj.obj_uuid, 		argv[5], len);

	ret = dm_get_device(target, ddvol->ddvol_obj.obj_dm_dev_name, dm_table_get_mode(target->table),
		&ddvol->ddvol_obj.obj_dm_dev);
	if (ret) {
		target->error = "AISA-Error: Lookup for the backstore device is failed";
		LOG("AISA-Error: Lookup for the backstore device %s is failed with errno %d\n",
			ddvol->ddvol_obj.obj_dm_dev_name, ret);
		ret = -ENODEV;
		dm_flag = FALSE;
		goto exit_failure;
	}

	parent_ddvol		= (PDDVOL)parent_object;
	table_size			= parent_ddvol->ddvol_table_size;
	parent_lba_table	= parent_ddvol->ddvol_lba_table;
	lba_table 			= dz_volume_create_lba_table(table_size);

	if (!lba_table) {

		target->error = "AISA-Error: Not Enough Memory";
		ret = -ENODEV;
		goto exit_failure;
	}
	ddvol->ddvol_table_size = table_size;
	ddvol->ddvol_lba_table 	= lba_table;
	ddvol->ddvol_parent		= parent_ddvol;

	for (count = 0; count < table_size; count++) {
		lba_entry 			= &lba_table[count];	
		parent_lba_entry 	= &parent_lba_table[count];	
		lba_index			= parent_lba_entry->lba_index;

		aisa_lba_entry		= GET_LBA_ENTRY(lba_index);
		_ACQUIRE_LBA_LOCK_WRITE(aisa_lba_entry);

		aisa_hba_entry		= GET_HBA_ENTRY(aisa_lba_entry->hash_index);
		_ACQUIRE_HBA_LOCK(aisa_hba_entry);

		hash_pba_index		= aisa_hba_entry->hash_pba_index;
		aisa_pba_entry		= GET_PBA_ENTRY(hash_pba_index);


		_ACQUIRE_PBA_LOCK(aisa_pba_entry);

		pba_index			= aisa_lba_entry->pba_index;
		aisa_pba_entry_col	= GET_PBA_ENTRY(pba_index);
		lba_entry->pba_index= pba_index;
		aisa_pba_entry_col->ref_cnt++;

		_DZ_DDE_RELEASE_PBA_LOCK(aisa_pba_entry);
		_DZ_DDE_RELEASE_HBA_LOCK(aisa_hba_entry);
		_RELEASE_LBA_LOCK_WRITE(aisa_lba_entry);
	}

	//Assign Creation Date
	strncpy(ddvol->ddvol_obj.obj_date,      argv[7], DATE_LENGTH);

	//Assign Creation Time
	ddvol->ddvol_obj.obj_time[0]    =       argv[7][DATE_LENGTH];
	ddvol->ddvol_obj.obj_time[1]    =       argv[7][DATE_LENGTH+1];
	ddvol->ddvol_obj.obj_time[2]    =       argv[7][DATE_LENGTH+2];
	ddvol->ddvol_obj.obj_time[3]    =       argv[7][DATE_LENGTH+3];
	ddvol->ddvol_obj.obj_time[4]    =       argv[7][DATE_LENGTH+4];
	ddvol->ddvol_obj.obj_time[5]    =       argv[7][DATE_LENGTH+5];

	// Assign this to private storage of target
	target->private = ddvol;

	// Enqueue this to the object list for traversal and deletion purpose
	dz_q_object(&global_object_list, &ddvol->ddvol_obj);

	LOG("Successfully created snapshot volume %s\n", ddvol->ddvol_obj.obj_name);
	return ddvol;

exit_failure:
	if (ddvol) {
		if (dm_flag) {
			dm_put_device(target, ddvol->ddvol_obj.obj_dm_dev);
		}
		if (ddvol->ddvol_lba_table) {
			dz_volume_destroy_lba_table(ddvol);
		}
		dz_kfree(ddvol, sizeof(*ddvol));
	}
	*error = ret;
	return NULL;
}

RINT  dz_volume_create_datavol_sread(PDM_TARGET target, UINT argc, CHAR ** argv)
{
	INT	error 		= 0;
	PDDVOL ddvol 	= dz_ddvol_create_snapshot(target, argc, argv, &error);
	if (ddvol) {
		ddvol->ddvol_obj.obj_type = OBJECT_TYPE_DATA_VOLUME_SNAPSHOT_READ;
		RETURNS;
	} else {
		return error;
	}
}
EXPORT_SYMBOL(dz_volume_create_datavol_sread);

RINT  dz_volume_create_datavol_swrite(PDM_TARGET target, UINT argc, CHAR ** argv)
{
	//ddvol->ddvol_obj.type = OBJECT_TYPE_DATA_VOLUME_SNAPSHOT_WRITE;
	INT	error 		= 0;
	PDDVOL ddvol 	= dz_ddvol_create_snapshot(target, argc, argv, &error);
	if (ddvol) {
		ddvol->ddvol_obj.obj_type = OBJECT_TYPE_DATA_VOLUME_SNAPSHOT_WRITE;
		RETURNS;
	} else {
		return error;
	}
	RETURNS;
}
EXPORT_SYMBOL(dz_volume_create_datavol_swrite);

RVOID	dz_volume_destroy_datavol_sread(PDDVOL ddvol)
{
	if (ddvol) {
		dz_get_free_space();
		LOG("Destroying SNAPSHOT_READ volume %s\n", ddvol->ddvol_obj.obj_name);
		dz_kfree(ddvol, sizeof(DDVOL));
		dz_get_free_space();
	}
}
EXPORT_SYMBOL(dz_volume_destroy_datavol_sread);

RVOID	dz_volume_destroy_datavol_swrite(PDDVOL ddvol)
{
	if (ddvol) {
		dz_get_free_space();
		LOG("Destroying SNAPSHOT_WRITE volume %s\n", ddvol->ddvol_obj.obj_name);
		dz_kfree(ddvol, sizeof(DDVOL));
		dz_get_free_space();
	}
}
EXPORT_SYMBOL(dz_volume_destroy_datavol_swrite);

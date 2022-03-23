/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern INT dz_switch_passthrough_mode;
extern INT dz_switch_passthrough_read;
extern INT dz_switch_passthrough_write;

extern AISA_DEVICE 	*aisa_device_object;
extern U64			dedupe_tot_logical_space;
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

extern PSUPERBLOCK	superblock;
static int 	dz_target_ctr_datavol(PDM_TARGET target, unsigned int argc,char **argv);
static int 	dz_target_map_datavol(struct dm_target *target, struct bio *bio);
static void	dz_target_dtr_datavol(PDM_TARGET target);
static int 	dz_target_map_datavol_sread(struct dm_target *target, struct bio *bio);
static int 	dz_target_map_datavol_swrite(struct dm_target *target, struct bio *bio);

//Target Type for Data Volumes
static struct 	target_type aisa_datavol = {

	.name 		= "AVM_VOLUME",
	.version	= {1,0,0},
	.module 	= THIS_MODULE,
	.ctr 		= dz_target_ctr_datavol,
	.dtr 		= dz_target_dtr_datavol,
	.map 		= dz_target_map_datavol,
};

//Target Type Table for Read Only Snapshots
//for DAta volumes
static struct 	target_type aisa_datavol_sr = {

	.name 		= "AVM_VOLUME_SR",
	.version	= {1,0,0},
	.module 	= THIS_MODULE,
	.ctr 		= dz_target_ctr_datavol,
	.dtr 		= dz_target_dtr_datavol,
	.map 		= dz_target_map_datavol_sread,
};

//Target Type Table for Writable Snapshots
//for Data Volumes
static struct 	target_type aisa_datavol_sw = {

	.name 		= "AVM_VOLUME_SW",
	.version	= {1,0,0},
	.module 	= THIS_MODULE,
	.ctr 		= dz_target_ctr_datavol,
	.dtr 		= dz_target_dtr_datavol,
	.map 		= dz_target_map_datavol_swrite,
};

RINT dz_process_datavol_io(PDM_TARGET target, PBIO bio)
{
	PDDVOL			ddvol 				= NULL;
	U64			lba_index				= 0;
	PDDVOL_LBA_BLOCK ddvol_lba_table	= NULL;
	PIOREQUEST iorequest 				= dz_io_alloc();

	ddvol 	= (PDDVOL)target->private;

	if (!iorequest) {
		LOGE("Unable to allocate memory for completion\n");
		return -ENOMEM;
	}

	//TODO:Only Aligned IO requests entertained.
	//Need to fix, unaligned and contagious IO requests
	ddvol_lba_table = ddvol->ddvol_lba_table;
	lba_index	= ddvol_lba_table[dz_convert_sector_to_lba(bio->bi_sector)].lba_index;
	//PRINT_U64(lba_index);
	iorequest->ior_lba 	= lba_index;

	//memset(iorequest, 0, sizeof(IOREQUEST));
	iorequest->ior_bio 			= bio;
	iorequest->ior_parent 		= NULL;
	ATOMIC_SET(iorequest->ior_ref_cnt, 0);

	//print_iorequest(iorequest, "aisa_init");

	//iorequest_get(iorequest);
	if ((bio->bi_rw & WRITE) == WRITE) {
		dz_q_iorequest(write_thread_pool, iorequest);
	} else {
		dz_q_iorequest(read_thread_pool, iorequest);
	}


	return DM_MAPIO_SUBMITTED;
}

//Map function for Snapshot Read i.e Readonly Snapshots
RINT dz_process_datavol_sread_io(PDM_TARGET target, PBIO bio)
{
	PDDVOL			ddvol 				= NULL;
	U64			lba_index				= 0;
	PDDVOL_LBA_BLOCK ddvol_lba_table	= NULL;
	PIOREQUEST iorequest 				= NULL;

	if ((bio->bi_rw & WRITE) == WRITE) {
			//BIODONE(bio, IO_ERROR_ENOSYS);
			BIODONE(bio, IO_ERROR_EINVAL);
			//return IO_ERROR_ENOSYS;
			return DM_MAPIO_SUBMITTED;
	}

	iorequest 				= dz_io_alloc();
	if (!iorequest) {
		LOGE("Unable to allocate memory for completion\n");
		return -ENOMEM;
	}
	ddvol 	= (PDDVOL)target->private;

	//TODO:Only Aligned IO requests entertained.
	//Need to fix, unaligned and contagious IO requests
	ddvol_lba_table = ddvol->ddvol_lba_table;
	lba_index	= ddvol_lba_table[dz_convert_sector_to_lba(bio->bi_sector)].lba_index;
	//PRINT_U64(lba_index);
	iorequest->ior_lba 	= lba_index;

	//memset(iorequest, 0, sizeof(IOREQUEST));
	iorequest->ior_bio 			= bio;
	iorequest->ior_parent 		= NULL;
	ATOMIC_SET(iorequest->ior_ref_cnt, 0);

	//print_iorequest(iorequest, "aisa_init");

	//iorequest_get(iorequest);
	dz_q_iorequest(read_thread_pool, iorequest);
	return DM_MAPIO_SUBMITTED;
}

RINT dz_process_datavol_swrite_io(PDM_TARGET target, PBIO bio)
{
	PDDVOL			ddvol 				= NULL;
	U64			lba_index				= 0;
	PDDVOL_LBA_BLOCK ddvol_lba_table	= NULL;
	PIOREQUEST iorequest 				= dz_io_alloc();
	if (!iorequest) {
		LOGE("Unable to allocate memory for completion\n");
		return -ENOMEM;
	}
	ddvol 	= (PDDVOL)target->private;

	//TODO:Only Aligned IO requests entertained.
	//Need to fix, unaligned and contagious IO requests
	ddvol_lba_table = ddvol->ddvol_lba_table;
	lba_index	= ddvol_lba_table[dz_convert_sector_to_lba(bio->bi_sector)].lba_index;
	iorequest->ior_lba 	= lba_index;

	//memset(iorequest, 0, sizeof(IOREQUEST));
	iorequest->ior_bio 			= bio;
	iorequest->ior_parent 		= NULL;
	ATOMIC_SET(iorequest->ior_ref_cnt, 0);

	//print_iorequest(iorequest, "aisa_init");

	//iorequest_get(iorequest);
	if ((bio->bi_rw & WRITE) == WRITE) {
		dz_q_iorequest(write_thread_pool, iorequest);
	} else {
		dz_q_iorequest(read_thread_pool, iorequest);
	}

	return DM_MAPIO_SUBMITTED;
}

static int dz_target_map_datavol(PDM_TARGET target, struct bio *bio)
{
	INT 			ret = SUCCESS;

	if (!aisa_device_object) {
		target->error = "AISA-Error: IO is not supported until AISA device online";
		ret = -EACCES;
		return ret;
	}

	if (dz_switch_passthrough_mode) {
		return dz_demo_passthrough_io(target, bio);
	}

	return dz_process_datavol_io(target, bio);
}

static int dz_target_map_datavol_sread(PDM_TARGET target, struct bio *bio)
{
	INT 			ret = SUCCESS;

	if (!aisa_device_object) {
		target->error = "AISA-Error: IO is not supported until AISA device online";
		ret = -EACCES;
		return ret;
	}

	if (dz_switch_passthrough_mode) {
		return dz_demo_passthrough_io(target, bio);
	}

	return dz_process_datavol_sread_io(target, bio);
}

static int dz_target_map_datavol_swrite(PDM_TARGET target, struct bio *bio)
{
	INT 			ret = SUCCESS;

	if (!aisa_device_object) {
		target->error = "AISA-Error: IO is not supported until AISA device online";
		ret = -EACCES;
		return ret;
	}

	if (dz_switch_passthrough_mode) {
		return dz_demo_passthrough_io(target, bio);
	}

	return dz_process_datavol_swrite_io(target, bio);
}

static int dz_target_ctr_datavol(PDM_TARGET target, unsigned int argc,char **argv)
{
	//return dz_target_create(target, argc, argv);
	/* 
	Here it is how the information is provided from cli
	Start Sector Passed from cli (dmsetup cli)
	Size in terms of sectors passed from cli (dmsetup cli)
	argv[0] : Underlying device or Backstore Name e.g. /dev/md0 
	argv[1] : Start sector of Underlying Device. It should be ignored as set by module only
	argv[2] : Product Type; AISA, NORMAL, SNAPSHOT_READ, SNAPSHOT_WRITE
	argv[3] : Name of Virtual Device or Volume
	argv[4] : Magic No.
	argv[5] : UUID. Easier to be passed from cli
	*/
    char    *device_type = NULL; 
    print_cli_args(target, argc, argv);

    device_type = (char *)argv[2];

	if (!aisa_device_object) {
		target->error = "AISA_Error: AISA Target Device needs to be created first";
		return -EINVAL;
    }

	// If the cli request is to create normal volume aka data volume

	if(strcmp(device_type, DEVICE_TYPE_DATA_VOLUME) == 0) {
		return dz_volume_create_datavol(target, argc, argv);
	}

	// If the cli request is to create readable snapshot volume

	if(strcmp(device_type, DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_READ) == 0) {
		return dz_volume_create_datavol_sread(target, argc, argv);
	}

	// If the cli request is to create writable snapshot volume

	if(strcmp(device_type, DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_WRITE) == 0) {
		return dz_volume_create_datavol_swrite(target, argc, argv);
	}

	RETURNF;

}

static void dz_target_dtr_datavol(PDM_TARGET target)
{
	//dz_target_destroy(target);
	DEVICE_OBJECT *obj = (DEVICE_OBJECT *) target->private;
	LOGVOL("Destroying Volume Object %p\n", obj);
	dz_dq_object(&global_object_list, obj->obj_name);
	dm_put_device(target, obj->obj_dm_dev);

	switch(obj->obj_type) {
	
		case OBJECT_TYPE_DATA_VOLUME:
			dz_volume_destroy_datavol((PDDVOL )obj);
			return;

		case OBJECT_TYPE_DATA_VOLUME_SNAPSHOT_READ:
			dz_volume_destroy_datavol_sread((PDDVOL )obj);
			return;

		case OBJECT_TYPE_DATA_VOLUME_SNAPSHOT_WRITE:
			dz_volume_destroy_datavol_swrite((PDDVOL )obj);
			return;

		case OBJECT_TYPE_VM_VOLUME:
			dz_volume_destroy_datavol((PDDVOL )obj);
			return;

		case OBJECT_TYPE_VM_VOLUME_SNAPSHOT_READ:
			dz_volume_destroy_datavol((PDDVOL )obj);
			return;

		case OBJECT_TYPE_VM_VOLUME_SNAPSHOT_WRITE:
			dz_volume_destroy_datavol((PDDVOL )obj);
			return;
	}
}

/*
static int aisa_datavol_ioctl(struct dm_target *ti, unsigned int cmd, unsigned long arg)
{
	return SUCCESS;
}
*/

RINT dz_register_datavol_target()
{
	int result;
	//TODO:Check for proper error return scenarios
	result = dm_register_target(&aisa_datavol);
	if (result < 0) {
		LOGVOLE("\nError in Registering Data Volume Target\n");
	} else {
		LOGVOL("AISA_NORMAL Target Registered\n");
	}

	result = dm_register_target(&aisa_datavol_sr);
	if (result < 0) {
		LOGVOLE("\nError in Registering Data Volume Snapshot Read Target\n");
	} else {
		LOGVOL("AISA_SNAPSHOT_READ Target Registered\n");
	}

	result = dm_register_target(&aisa_datavol_sw);
	if (result < 0) {
		LOGVOLE("\nError in Registering Data Volume Snapshot Write Target\n");
	} else {
		LOGVOL("AISA_SNAPSHOT_WRITE Target Registered\n");
	}

	RETURNS;
}

RVOID dz_unregister_datavol_target()
{
	dm_unregister_target(&aisa_datavol_sw);
	dm_unregister_target(&aisa_datavol_sr);
	dm_unregister_target(&aisa_datavol);
}

RINT  dz_volume_create_datavol(PDM_TARGET target, UINT argc, CHAR ** argv)
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
	*/
	INT			ret = 0;
	PDDVOL	ddvol 	= NULL;
	INT			len = 0;
	INT		len_max = 0;
	BOOL	dm_flag = TRUE;
	//Security Check //TODO:We need to check argv[0] with "/dev/mapper/AISA";
	/*
	if (strcmp(argv[0], aisa_device_object->obj.name) != 0) {
		target->error = "AISA-Error: Target Device Name is wrong. Can't proceed";
		LOGVOLE("AISA-Error: Target Device Name is wrong. Looking for %s\n", aisa_device_object->obj.name);
		ret = -ENODEV;
		goto exit_failure;
	}
	*/

	_ACQUIRE_TRANSACTION_LOCK();

	ddvol = dz_kmalloc(sizeof(DDVOL), GFP_KERNEL);
	if (!ddvol) {
		target->error = "AISA-Error: fvm vol allocation failed";
		ret = -ENOMEM;
		goto exit_failure;
	}
	MEMSET(ddvol);
	ddvol->ddvol_obj.obj_start_lba	= target->begin; 

	// Assign the name of Parent volume passed from cli i.e. /dev/mapper/AISA
	strncpy(ddvol->ddvol_obj.obj_dm_dev_name, argv[0], strlen(argv[0]));

	ddvol->ddvol_obj.obj_end_lba	= ddvol->ddvol_obj.obj_start_lba  + target->len - 1;
	
	len_max = NAME_LENGTH;
	len 	= strlen(argv[3]);
	len = len > len_max ? len_max: len;

	// Assign the name of volume passed from cli
	strncpy(ddvol->ddvol_obj.obj_name, 		argv[3], len);

	len_max = MAGICNO_LENGTH;
	len 	= strlen(argv[4]);
	len = len > len_max ? len_max: len;

	strncpy(ddvol->ddvol_obj.obj_magicno, 	argv[4], len);

	len_max = UUID_LENGTH;
	len 	= strlen(argv[5]);
	len = len > len_max ? len_max: len;

	strncpy(ddvol->ddvol_obj.obj_uuid, 		argv[5], len);

	//Assign Creation Date
	strncpy(ddvol->ddvol_obj.obj_date,		argv[6], DATE_LENGTH);

	//Assign Creation Time
	ddvol->ddvol_obj.obj_time[0]	=		argv[6][DATE_LENGTH];
	ddvol->ddvol_obj.obj_time[1]	=		argv[6][DATE_LENGTH+1];
	ddvol->ddvol_obj.obj_time[2]	=		argv[6][DATE_LENGTH+2];
	ddvol->ddvol_obj.obj_time[3]	=		argv[6][DATE_LENGTH+3];
	ddvol->ddvol_obj.obj_time[4]	=		argv[6][DATE_LENGTH+4];
	ddvol->ddvol_obj.obj_time[5]	=		argv[6][DATE_LENGTH+5];
	

	//Check if the backstore device or Parent device /dev/mapper/AISA exists
	ret = dm_get_device(target, ddvol->ddvol_obj.obj_dm_dev_name, dm_table_get_mode(target->table),
		&ddvol->ddvol_obj.obj_dm_dev);
	if (ret) {
		target->error = "AISA-Error: Lookup for the backstore device is failed";
		LOGVOLE("AISA-Error: Lookup for the backstore device %s is failed with errno %d\n",
			ddvol->ddvol_obj.obj_dm_dev_name, ret);
		ret = -ENODEV;
		dm_flag = FALSE;
		goto exit_failure;
	}

	ret = dz_volume_set_used_space(target, ddvol);
	if (ret) {
		goto exit_failure;
	}
	
	// Now mark this object as normal volume
	ddvol->ddvol_obj.obj_type = OBJECT_TYPE_DATA_VOLUME;

	// Assign this to private storage of target
	target->private = ddvol;

	// Enqueue this to the object list for traversal and deletion purpose
	dz_q_object(&global_object_list, &ddvol->ddvol_obj);

	LOGVOL("Successfully created dedupe volume %s\n", ddvol->ddvol_obj.obj_name);
	_RELEASE_TRANSACTION_LOCK();
	RETURNS;

exit_failure:
	_RELEASE_TRANSACTION_LOCK();
	if (ddvol) {
		if (dm_flag) {
			dm_put_device(target, ddvol->ddvol_obj.obj_dm_dev);
		}
		if (ddvol->ddvol_lba_table) {
			dz_volume_destroy_lba_table(ddvol);
		}
		dz_kfree(ddvol, sizeof(*ddvol));
	}

	return ret;
}

RVOID	dz_volume_destroy_datavol(PDDVOL ddvol)
{
	if (ddvol) {
		dz_get_free_space();
		LOGVOL("Destroying NORMAL volume %s\n", ddvol->ddvol_obj.obj_name);
		if (ddvol->ddvol_lba_table) {
			dz_volume_freeup_space(ddvol);
			dz_volume_destroy_lba_table(ddvol);
		}
		dz_kfree(ddvol, sizeof(DDVOL));
		dz_get_free_space();
	}
}
EXPORT_SYMBOL(dz_volume_destroy_datavol);

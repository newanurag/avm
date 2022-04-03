/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

//passthrough_read and passthrough_write will be active if the passthrough_mode is enabled
extern INT dz_switch_passthrough_mode;
extern INT dz_switch_passthrough_read;
extern INT dz_switch_passthrough_write;
extern INT dz_switch_fake_write;
extern INT dz_switch_fake_read;

extern INT dz_switch_fec_engine;

module_param(dz_switch_passthrough_mode, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
module_param(dz_switch_passthrough_read, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
module_param(dz_switch_passthrough_write, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
module_param(dz_switch_fake_read, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
module_param(dz_switch_fake_write, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

extern BASIC_COUNTERS 	dz_basic_counters;
extern PBASIC_COUNTERS pdz_basic_counters;

extern DDE_COUNTERS 	dz_dde_counters;
extern PDDE_COUNTERS 	pdz_dde_counters;

extern FEC_COUNTERS	dz_fec_counters;
extern PFEC_COUNTERS	pdz_fec_counters;

extern BEC_COUNTERS	dz_bec_counters;
extern PBEC_COUNTERS	pdz_bec_counters;

extern AVM_COUNTERS	dz_avm_counters;
extern PAVM_COUNTERS	pdz_avm_counters;

//Total READ Application IOs received.
extern ATOMIC64 tot_application_iocount_reads;
extern ATOMIC64 tot_application_iocount_reads_single_page;
extern ATOMIC64 tot_application_iocount_reads_partial_page;
extern ATOMIC64 tot_application_iocount_reads_multi_page;

//Total WRITE Application IOs received.
extern ATOMIC64 tot_application_iocount_writes;
extern ATOMIC64 tot_application_iocount_writes_single_page;
extern ATOMIC64 tot_application_iocount_writes_partial_page;
extern ATOMIC64 tot_application_iocount_writes_multi_page;

//Total Application IOs (including READs and WRITEs) received.
extern ATOMIC64 tot_application_iocount;

extern U64				tot_free_space;

extern U64		dedupe_tot_logical_space;
extern PBLOCK_DEVICE	DATA_DISK_DEVICE;
extern NAME		DATA_DISK_DEVICE_NAME;

extern PBLOCK_DEVICE	METADATA_DISK_DEVICE;
extern NAME			METADATA_DISK_DEVICE_NAME;

extern PBLOCK_DEVICE	TARGET_DEVICE;
extern NAME			TARGET_DEVICE_NAME;

extern SIZE			TARGET_DEVICE_SIZE;
extern PAVM_DEVICE 	aisa_device_object;

extern PBLOCK_DEVICE 	avm_metadata_bdev;

//extern ATOMIC64 dz_total_memory_consumed_by_all_pools;
extern OBJECT_LIST 	global_object_list;
extern BITMAP		pba_bitmap;

extern PPBA_BLOCK	dedupe_pba_table;
extern PHASH_BLOCK	dedupe_hash_table;
extern PLBA_BLOCK	dedupe_lba_table;

extern U64 			metadata_lba_lock_table_size;
extern PMD_LOCK		metadata_lba_lock_table;

extern U64 			metadata_lba_table_size;
extern PMD_BLOCK	metadata_lba_table;

extern SPINLOCK 	lba_table_lock; 
extern SPINLOCK 	hash_table_lock; 
extern SPINLOCK 	pba_table_lock; 

extern PDZ_THREAD_POOL read_thread_pool;
extern PDZ_THREAD_POOL write_thread_pool;

extern PDZ_THREAD_POOL align_read_thread_pool;
extern PDZ_THREAD_POOL align_write_thread_pool;

extern PDZ_THREAD_POOL fec_read_thread_pool;
extern PDZ_THREAD_POOL fec_write_thread_pool;


extern PSUPERBLOCK	superblock;

extern PSUPERBLOCK	superblock;

PDM_TARGET	AVM_DM_TARGET = NULL;

struct 	target_type dzavm = {

	.name 		= "AVM_TARGET",
	.version	= {1,0,0},
	.module 	= THIS_MODULE,
	.ctr 		= dz_avm_target_ctr,
	.dtr 		= dz_avm_target_dtr,
	//.map 		= dz_avm_target_map,
	.map 		= dz_avm_target_default_map,
};

static RINT dz_avm_target_aisa_register(VOID);

RINT dz_process_io(PDM_TARGET target, PBIO bio)
{
	UINT  io_size   = bio->bi_size; // Parent or Original bio size

	PIOREQUEST iorequest 				= dz_io_alloc();
	if (!iorequest) {
		LOGE("Unable to allocate memory for IO completion\n");
		return -ENOMEM;
	}

	iorequest->ior_lba 	= dz_convert_sector_to_lba(bio->bi_sector);


	//memset(iorequest, 0, sizeof(IOREQUEST));
	iorequest->ior_bio 			= bio;
	
	//Since it is an application IO so its parent is NULL
	iorequest->ior_parent 		= NULL;

	//Since this is the first iorequest for this bio so its
	// ref_cnt is set to zero
	ATOMIC_SET(iorequest->ior_ref_cnt, 0);

	IF_WRITE_BIO(bio)  {
		iorequest->ior_type 		= DZ_IOR_TYPE_APPLICATION_IO_WRITE;
		iorequest->ior_thread_pool = write_thread_pool;
		ATOMIC64_INC(tot_application_iocount_writes);

		if (io_size < LBA_BLOCK_SIZE) {
			ATOMIC64_INC(tot_application_iocount_writes_partial_page);
		} else if (io_size == LBA_BLOCK_SIZE) {
			ATOMIC64_INC(tot_application_iocount_writes_single_page);
		} else if (io_size > LBA_BLOCK_SIZE) {
			ATOMIC64_INC(tot_application_iocount_writes_multi_page);
		}
		dz_q_iorequest(write_thread_pool, iorequest);
	} else {
		iorequest->ior_type 		= DZ_IOR_TYPE_APPLICATION_IO_READ;
		iorequest->ior_thread_pool = read_thread_pool;

		//print_iorequest_lite(iorequest,"AVERMA:Application IO");

		ATOMIC64_INC(tot_application_iocount_reads);
		if (io_size < LBA_BLOCK_SIZE) {
			ATOMIC64_INC(tot_application_iocount_reads_partial_page);

		} else if (io_size == LBA_BLOCK_SIZE) {
			ATOMIC64_INC(tot_application_iocount_reads_single_page);

		} else if (io_size > LBA_BLOCK_SIZE) {
			ATOMIC64_INC(tot_application_iocount_reads_multi_page);
		}
		dz_q_iorequest(read_thread_pool, iorequest);
	}

	return DM_MAPIO_SUBMITTED;
}

RINT dz_demo_passthrough_writeonly(PDM_TARGET target, struct bio *bio)
{

	bio->bi_bdev = aisa_device_object->obj.obj_dm_dev->bdev;
	if((bio->bi_rw & WRITE) == WRITE) {
		LOGTGT("AISA-Info: dz_demo_passthrough_writeonly : bio is a WRITE request.... \n");
		//dz_process_interval_tree((bio->bi_sector * SECTOR_SIZE), bio->bi_size);
		//dz_process_lab_engine_interval((bio->bi_sector * SECTOR_SIZE), bio->bi_size);
		//dz_process_lab_engine_linked_list((bio->bi_sector * SECTOR_SIZE), bio->bi_size);
		//dz_process_lab_engine_bplus_tree((bio->bi_sector * SECTOR_SIZE), bio->bi_size);
		//dz_process_lab_engine_locks((bio->bi_sector * SECTOR_SIZE), bio->bi_size);
		dz_process_lab_engine_pipe((bio->bi_sector * SECTOR_SIZE), bio->bi_size);
		//dz_process_lab_engine_binary_tree((bio->bi_sector * SECTOR_SIZE), bio->bi_size);
	} else {
		LOGTGT("AISA-Info: dz_demo_passthrough_writeonly : bio is a READ request.... \n");
	}

	DZ_OS_KERNEL_SUBMIT_BIO(bio->bi_rw,bio);
	return DM_MAPIO_SUBMITTED;
}

RINT dz_demo_passthrough_readonly(PDM_TARGET target, struct bio *bio)
{
	bio->bi_bdev = aisa_device_object->obj.obj_dm_dev->bdev;
	if((bio->bi_rw & WRITE) == WRITE) {
		LOGTGT("AISA-Info: dz_demo_passthrough_readonly : bio is a WRITE request.... \n");
		BIODONE(bio, IO_ERROR_EIO);
		return DM_MAPIO_SUBMITTED;
	} else {
		LOGTGT("AISA-Info: dz_demo_passthrough_readonly : bio is a READ request.... \n");
	}

	DZ_OS_KERNEL_SUBMIT_BIO(bio->bi_rw,bio);
	return DM_MAPIO_SUBMITTED;
}

RINT dz_demo_passthrough_io(PDM_TARGET target, struct bio *bio)
{
	if (dz_switch_passthrough_read) {
		return dz_demo_passthrough_readonly(target, bio);
	}
	if (dz_switch_passthrough_write) {
		return dz_demo_passthrough_writeonly(target, bio);
	}

	bio->bi_bdev = aisa_device_object->obj.obj_dm_dev->bdev;
	if((bio->bi_rw & WRITE) == WRITE) {
		LOGTGT("AISA-Info: dz_demo_passthrough_io : bio is a WRITE request.... \n");
	} else {
		LOGTGT("AISA-Info: dz_demo_passthrough_io : bio is a READ request.... \n");
	}

	DZ_OS_KERNEL_SUBMIT_BIO(bio->bi_rw,bio);
	return DM_MAPIO_SUBMITTED;
}
EXPORT_SYMBOL(dz_demo_passthrough_io);


RINT dz_avm_target_map(PDM_TARGET target, struct bio *bio)
{
	INT 			ret = SUCCESS;

	if (!aisa_device_object) {
		target->error = "AISA-Error: IO is not supported until avm device online";
		ret = -EACCES;
		return ret;
	}
	ATOMIC64_INC(tot_application_iocount);

	if (dz_switch_passthrough_mode) {
		return dz_demo_passthrough_io(target, bio);

	} else if (dz_switch_fake_write) {
		BIODONE(bio, SUCCESS);
		return DM_MAPIO_SUBMITTED;

	} else if (dz_switch_fake_read) {
		BIODONE(bio, SUCCESS);
		return DM_MAPIO_SUBMITTED;
	}

	return dz_process_io(target, bio);
}

RINT dz_avm_target_default_map(PDM_TARGET target, struct bio *bio)
{

	if((bio->bi_rw & WRITE) == WRITE) {
		LOGTGT("AISA-Info: dz_avm_target_default_map : Failing. bio is a WRITE request.... \n");
		BIODONE(bio, IO_ERROR_EIO);
		return DM_MAPIO_SUBMITTED;
	} else {
		LOGTGT("AISA-Info: dz_avm_target_default_map : Fake Completion of READ IOs. \n");
		BIODONE(bio, SUCCESS);
		return DM_MAPIO_SUBMITTED;
		//DZ_OS_KERNEL_SUBMIT_BIO(bio->bi_rw,bio);
		//return DM_MAPIO_SUBMITTED;
	}

}

RINT dz_avm_target_init_counters(VOID)
{

	pdz_basic_counters 	= &dz_basic_counters;
	MEMSET(pdz_basic_counters);

	pdz_dde_counters 	= &dz_dde_counters;
	MEMSET(pdz_dde_counters);

	pdz_fec_counters 	= &dz_fec_counters;
	MEMSET(pdz_fec_counters);

	pdz_align_counters 	= &dz_align_counters;
	MEMSET(pdz_align_counters);

	pdz_bec_counters 	= &dz_bec_counters;
	MEMSET(pdz_bec_counters);

	pdz_avm_counters 	= &dz_avm_counters;
	MEMSET(pdz_avm_counters);

	BASE_COUNTER_ATOMIC64_SET(dz_total_memory_consumed_by_all_pools, 0);
	BASE_COUNTER_ATOMIC64_SET(dz_iorequest_alloc_cnt, 0);
	BASE_COUNTER_ATOMIC64_SET(dz_iorequest_free_cnt, 0);
	BASE_COUNTER_ATOMIC64_SET(dz_page_alloc_cnt, 0);
	BASE_COUNTER_ATOMIC64_SET(dz_page_free_cnt, 0);
	BASE_COUNTER_U64_SET(fec_engine_memory_usage, 0);
	
	//Initialize Counters
	ATOMIC64_SET(tot_application_iocount, 0);
	ATOMIC64_SET(tot_application_iocount_reads, 0);
	ATOMIC64_SET(tot_application_iocount_reads_single_page, 0);
	ATOMIC64_SET(tot_application_iocount_reads_partial_page, 0);
	ATOMIC64_SET(tot_application_iocount_reads, 0);

	ATOMIC64_SET(tot_application_iocount_writes, 0);
	ATOMIC64_SET(tot_application_iocount_writes_single_page, 0);
	ATOMIC64_SET(tot_application_iocount_writes_partial_page, 0);
	ATOMIC64_SET(tot_application_iocount_writes_multi_page, 0);

	dz_fec_init_counters();

	RETURNS;
}

RINT dz_avm_target_ctr(PDM_TARGET target, unsigned int argc,char **argv)
{
	int ret = 0;
	//ret = dz_target_init_counters();
	if (ret != SUCCESS) {
		goto exit;
	}

	if (dz_switch_passthrough_mode) {
		LOGTGT("AISA-Info: IO Passthrough is enabled\n");
	} else {
		LOGTGT("AISA-Info: IO Passthrough is disabled. IOs will be allowed to AISA device\n");
	}
	
	ret = dz_target_create(target, argc, argv);
	if (ret != SUCCESS) {
		goto exit;
	}

	if (dz_switch_fec_engine == DZ_DISABLED) {
		write_thread_pool = align_write_thread_pool; //dz_align_write_io()
		read_thread_pool  = align_read_thread_pool;  //dz_align_read_io()
	} else {
		write_thread_pool = fec_write_thread_pool;   //dz_fec_align_write_io()
		read_thread_pool  = fec_read_thread_pool;    //dz_fec_align_read_io()
	}

	AVM_DM_TARGET = target;

  exit:
	return ret;
}

VOID dz_avm_target_dtr(PDM_TARGET target)
{
	dz_target_destroy(target);
}

/*
static RINT aisa_ioctl(struct dm_target *ti, unsigned int cmd, unsigned long arg)
{
	return SUCCESS;
}
*/

/* This function is common for creating following Targets:
 * AISA Target : Parent of all volumes
 * Volume Target : For creating data volumes aka volumes,
 *                 Readable snapshot volumes,
 *                 Writable snapshot volumes
 * 
 */

RINT  dz_target_create(struct dm_target *target, UINT argc, CHAR ** argv)
{
	/* 
	Here it is how the information is provided from cli
	Start Sector Passed from cli (dmsetup cli)
	Size in terms of sectors passed from cli (dmsetup cli)
	argv[0] : Underlying device or Backstore Name e.g. /dev/md0 or /dev/sda 
	argv[1] : Start sector of Underlying Device. It should be ignored as set by module only
	argv[2] : Product Type; AISA, NORMAL, SNAPSHOT_READ, SNAPSHOT_WRITE
	argv[3] : Name of Virtual Device or Volume
	argv[4] : Magic No.
	argv[5] : UUID. Easier to be passed from cli
	*/
	char	*device_type = NULL;
	print_cli_args(target, argc, argv);

	device_type = (char *)argv[2];

	// If the cli request is to create AISA Target device (Parent of all volumes).
	// i.e. /dev/mapper/AISA
	// This needs to be created first and only once.

	if(strcmp(device_type, DEVICE_TYPE_AISA) == 0) {
		if (aisa_device_object) {
			target->error = "AISA_Error: AISA Target Device already exists. It needs to be created only once";
			return -EINVAL;
		}
		// Get ready to create AISA Target device i.e. /dev/mapper/AISA
		return dz_target_create_aisa(target, argc, argv);
	}

	/*

	// Below devices are dependent on aisa_device_object. So they will fail
	// if aisa_device_object does not exist.

	if (!aisa_device_object) {
		target->error = "AISA_Error: AISA Target Device needs to be created first";
		return -EINVAL;
	}
	// Also below device requests will be served from volume engine

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
	*/


	RETURNF;
}
EXPORT_SYMBOL(dz_target_create);

RVOID	dz_target_destroy(PDM_TARGET target )
{
	DEVICE_OBJECT *obj = (DEVICE_OBJECT *) target->private;

	LOGTGT("Destroying AISA Object %p\n", obj);
	dz_dq_object(&global_object_list, obj->obj_name);
	dm_put_device(target, obj->obj_dm_dev);

	switch(obj->obj_type) {

		case OBJECT_TYPE_AISA:
			dz_avm_aisa_destroy(obj);
			return;

		/*
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
		*/
	}
}
EXPORT_SYMBOL(dz_target_destroy);

/* This function will create aisa target device inside the kernel.
 * It will also initialize all the engines
 */

RINT  dz_target_create_aisa(struct dm_target *target, UINT argc, CHAR ** argv)
{
	/* 
	Here it is how the information is provided from cli
	Start Sector Passed from cli (dmsetup cli) e.g. target->begin
	Size in terms of sectors passed from cli (dmsetup cli) e.g. target->len
	argv[0] : Underlying device or Backstore Name e.g. /dev/md0
	argv[1] : Start sector of Underlying Device. It should be ignored as it will be set by us only
	argv[2] : Product Type; AISA, NORMAL, SNAPSHOT_READ, SNAPSHOT_WRITE
	argv[3] : Name of Virtual Device
	argv[4] : Magic No.
	argv[5] : UUID. Easier to be passed from cli
	argv[6] : Name of Metadata disk/device
	argv[7] : MagicNo of Metadat disk/device
	argv[8] : UUID of Metadata disk/device
	*/
	INT			ret 			= 0;
	struct request_queue *queue	= NULL;
	PAVM_DEVICE	aisa_device = dz_kmalloc(sizeof(AVM_DEVICE), GFP_KERNEL);
	char b[BDEVNAME_SIZE];

	if (!aisa_device) {
		target->error = "AISA-Error: avm device allocation failed";
		ret = -ENOMEM;
		goto exit_failure;
	}

	MEMSET(aisa_device);
	aisa_device_object 			= aisa_device;

	if (strlen(argv[6]) > NAME_LENGTH) {
		target->error = "AISA-Error: Metadevice name is longer than expected";
		ret = -EINVAL;
		goto exit_failure;
	}

	if (strlen(argv[7]) > MAGICNO_LENGTH) {
		target->error = "AISA-Error: Metadevice magicno length is longer than expected";
		ret = -EINVAL;
		goto exit_failure;
	}
	if (strlen(argv[8]) > UUID_LENGTH) {
		target->error = "AISA-Error: Metadevice UUID length is longer than expected";
		ret = -EINVAL;
		goto exit_failure;
	}
	if (strlen(argv[9]) > (DATE_LENGTH + TIME_LENGTH)) {
		target->error = "AISA-Error: DateTime length is longer than expected";
		ret = -EINVAL;
		goto exit_failure;
	} else {
	    //Assign Creation Date
		strncpy(aisa_device->obj.obj_date,      argv[9], DATE_LENGTH);

		//Assign Creation Time
		aisa_device->obj.obj_time[0]    =       argv[9][DATE_LENGTH];
		aisa_device->obj.obj_time[1]    =       argv[9][DATE_LENGTH+1];
		aisa_device->obj.obj_time[2]    =       argv[9][DATE_LENGTH+2];
		aisa_device->obj.obj_time[3]    =       argv[9][DATE_LENGTH+3];
		aisa_device->obj.obj_time[4]    =       argv[9][DATE_LENGTH+4];
		aisa_device->obj.obj_time[5]    =       argv[9][DATE_LENGTH+5];
	}

	// Assign the metadata device name passed from cli(dmsetup command) to the kernel
	strncpy (aisa_device->metadata_dev_name, argv[6], strlen(argv[6]));

	//Open a Metadata device first and get its bdev. It is mandatory that
	//metadata device must exist. Ideally we should have check in cli whether 
	//it exists.

	LOGTGT("Opening MetaData Device %s\n", aisa_device->metadata_dev_name);
	aisa_device->metadata_bdev = dz_bdev_open(aisa_device->metadata_dev_name);
	if (! aisa_device->metadata_bdev) {
		target->error = "AISA-Error: Unable to open Metadata device";
		LOGE("Unable to open Metadata device %s\n", aisa_device->metadata_dev_name);
		ret = -ENODEV;
		goto exit_failure;
	} else {
		METADATA_DISK_DEVICE 		= aisa_device->metadata_bdev;
		strcpy(METADATA_DISK_DEVICE_NAME, aisa_device->metadata_dev_name);
		LOGTGT("Metadata device %s Opened successfully\n", METADATA_DISK_DEVICE_NAME);
	}
	avm_metadata_bdev = aisa_device->metadata_bdev;
	strncpy (aisa_device->metadata_dev_magicno, argv[7], strlen(argv[7]));
	strncpy (aisa_device->metadata_dev_uuid,    argv[8], strlen(argv[8]));

	aisa_device->obj.obj_start_pba	= target->begin; 
	aisa_device->obj.obj_start_lba	= target->begin; 
	strncpy(aisa_device->obj.obj_dm_dev_name, argv[0], strlen(argv[0]));

	//TODO: One of these two are redundant. Will remove later
	strncpy(aisa_device->obj.obj_name, 	argv[3], strlen(argv[3]) );
	strncpy(aisa_device->obj.obj_magicno, 	argv[4], strlen(argv[4]) );
	strncpy(aisa_device->obj.obj_uuid, 	argv[5], strlen(argv[5]) );


	strncpy(aisa_device->data_dev_name, 	argv[0], strlen(argv[0]) );
	strncpy(aisa_device->data_dev_magicno, 	argv[4], strlen(argv[4]) );
	strncpy(aisa_device->data_dev_uuid, 	argv[5], strlen(argv[5]) );


	aisa_device->obj.obj_end_pba	= aisa_device->obj.obj_start_pba + target->len - 1;
	aisa_device->obj.obj_end_lba	= aisa_device->obj.obj_start_lba + target->len - 1;
	
	LOGTGT("Opening Data Device %s\n", aisa_device->obj.obj_dm_dev_name);
	if (dm_get_device(target, aisa_device->obj.obj_dm_dev_name,
			dm_table_get_mode(target->table),
			&aisa_device->obj.obj_dm_dev)) {

		target->error = "AISA-Error: Unable to open Data Device";
		LOGTGT("AISA-Error: Unable to open Data Device %s. Check if any app/fs is using it.\n",
			aisa_device->obj.obj_dm_dev_name);
		LOGTGT("AISA-Error: If filesystem is mounted on it, then unmount it and try again\n");

		ret = -ENODEV;
		goto exit_failure;
	}

	if (aisa_device->obj.obj_dm_dev->bdev == NULL) {
		ret = -ENODEV;
		target->error = "AISA-Error: Lookup for the backstore device is failed twice";
		goto exit_failure;
	}

	queue = bdev_get_queue(aisa_device->obj.obj_dm_dev->bdev);
	if (!queue) {
		ret = -ENODEV;
		target->error = "AISA-Error: Unable to get device queue";
		goto exit_failure;

	}
	blk_queue_merge_bvec(queue, NULL);
	blk_queue_max_hw_sectors(queue, UINT_MAX);

	DATA_DISK_DEVICE			= aisa_device->obj.obj_dm_dev->bdev;
	strcpy(DATA_DISK_DEVICE_NAME, bdevname(DATA_DISK_DEVICE, b));

	TARGET_DEVICE_SIZE = (((aisa_device->obj.obj_end_lba - aisa_device->obj.obj_start_lba + 1)
						 * SECTOR_SIZE) / (LBA_BLOCK_SIZE));

	LOGTGT("TARGET_DEVICE_SIZE = %zu (blocks. Block Size = 4096 bytes)\n", TARGET_DEVICE_SIZE);
						 
	//TODO:Get Target device name e.g /dev/mapper/AISA
	//strcpy(TARGET_DEVICE_NAME, bdevname(TARGET_DEVICE, b));


	/************************************************************/
	/***************** Starting All Engines *********************/
	/************************************************************/

	LOGTGT(" ______________________________________________________ \n");
	LOGTGT("|                                                      |\n");
	LOGTGT("|    Starting All Engines in Storage Stack(ZaidStor)   |\n");
	LOGTGT("|______________________________________________________|\n");
	
	//Initialize Infrastructure Engine
	if (dz_infra_engine_init()) {
		target->error = "AISA-Error: Unable to initialize Infrastructure Engine";
		ret = -ENOMEM;
		goto exit_failure;
	}

	//Initialize Alignment Engine
	if ((ret = dz_align_engine_init())) {
		target->error = "AISA-Error: Unable to initialize Alignment Engine";
		goto exit_failure;
	}

	//Initialize Dedupe Engine
	if (dz_dedupe_engine_init()) {
		target->error = "AISA-Error: Unable to initialize Dedupe Engine";
		ret = -ENOMEM;
		goto exit_failure;
	}

	//Initialize FrontEnd Cache Engine
	if (dz_fec_engine_init()) {
		target->error = "AISA-Error: Unable to initialize Frontend Cache Engine";
		ret = -ENOMEM;
		goto exit_failure;
	}

	//Initialize BackEnd Cache Engine
	if (dz_bec_engine_init()) {
		target->error = "AISA-Error: Unable to initialize Backend Cache Engine";
		ret = -ENOMEM;
		goto exit_failure;
	}
	
	//Initialize Metadata Engine
	if ((ret = dz_metadata_engine_init())) {
		target->error = "AISA-Error: Unable to initialize Metadata Engine";
		goto exit_failure;
	}


	/************************************************************/
	/***************** Starting All Engines Done ****************/
	/************************************************************/
	LOGTGT(" ______________________________________________________ \n");
	LOGTGT("|                                                      |\n");
	LOGTGT("|    Started  All Engines in Storage Stack(ZaidStor)   |\n");
	LOGTGT("|______________________________________________________|\n");
	
	//Now initialize pba_disk_offset and its lock;
	aisa_device->pba_disk_offset = 0;
	spin_lock_init(&aisa_device->pba_disk_offset_lock);

	// Now mark this object as avm device and not volume
	aisa_device->obj.obj_type 			= OBJECT_TYPE_AISA;
	aisa_device->pba_table 				= dedupe_pba_table;
	aisa_device->hash_table 			= dedupe_hash_table;
	aisa_device->lba_table 				= dedupe_lba_table;
	aisa_device->metadata_lba_table 	= metadata_lba_table;
	aisa_device->metadata_lba_lock_table= metadata_lba_lock_table;
	aisa_device->superblock 	= superblock;

	// Assign this to private storage of target
	target->private = aisa_device_object;

	// Enqueue this to the object list for traversal and deletion purpose
	dz_q_object(&global_object_list, &aisa_device_object->obj);

	tot_free_space = dedupe_tot_logical_space;
	LOGTGT("Successfully created AISA Target Device of size %lli (4K blocks)\n",
		(U64)TARGET_DEVICE_SIZE);

	dz_total_memory_consumption();
	RETURNS;

exit_failure:
	dz_avm_aisa_destroy(&aisa_device->obj);
	/*
	if (aisa_device) {
		dz_kfree(aisa_device, sizeof(AVM_DEVICE));
	}
	dz_md_engine_exit();
	dz_bec_engine_exit();
	dz_fec_engine_exit();
	dz_dedupe_engine_exit();
	dz_infra_engine_exit();
	dz_total_memory_consumption();
	*/
	return ret;
}

RVOID	dz_avm_aisa_destroy(PDEVICE_OBJECT obj)
{
	if (obj->obj_type == true) {
		PAVM_DEVICE aisa_device = (PAVM_DEVICE) obj;

		dz_metadata_engine_exit();
		dz_bec_engine_exit();
		dz_fec_engine_exit();
		dz_dedupe_engine_exit();
		dz_align_engine_exit();
		dz_infra_engine_exit();
		
		if (aisa_device->metadata_bdev) {
			dz_bdev_close(aisa_device->metadata_bdev);
			aisa_device->metadata_bdev = NULL;
			LOGTGT("Metadata device %s closed\n", aisa_device->metadata_dev_name);
		}


		if (memcmp(obj->obj_uuid, aisa_device_object->obj.obj_uuid, UUID_LENGTH) == 0) {
			aisa_device_object = NULL;
		}
		dz_kfree(obj, sizeof(AVM_DEVICE));
		dz_total_memory_consumption();
	}
}

static RINT dz_avm_target_aisa_register(VOID)
{
	int result;
	result = dm_register_target(&dzavm);
	if (result < 0) {
		LOGTGTE("\nError in Registering AVM AISA Target\n");
		RETURNF;
	} else {
		LOGTGT("AISA Target Registered\n");
	}

	RETURNS;
}


static int dz_avm_target_module_init(void)
{
	int result;
	//atomic64_set(&dz_total_memory_consumed_by_all_pools, 0);
	LOGTGT(" ______________________________________________________ \n");
	LOGTGT("|                                                      |\n");
	LOGTGT("|           ZAIDSTOR AVM AISA Target Registering       |\n");
	LOGTGT("|______________________________________________________|\n");
	
	dz_avm_target_init_counters();
	result = dz_avm_target_aisa_register();
	if (result < 0) {
		RETURNF;
	}

	LOGTGT(" ______________________________________________________ \n");
	LOGTGT("|                                                      |\n");
	LOGTGT("|           ZAIDSTOR AVM AISA Target Registered        |\n");
	LOGTGT("|______________________________________________________|\n");
	//result = dz_register_datavol_target();
	//result = dz_register_vmvol_target();
	//dz_sysfs_init();
	RETURNS;
}

static void dz_avm_target_module_exit(void)
{
	LOGTGT(" ______________________________________________________ \n");
	LOGTGT("|                                                      |\n");
	LOGTGT("|           ZAIDSTOR AVM AISA Target UnRegistering     |\n");
	LOGTGT("|______________________________________________________|\n");
	//dz_sysfs_exit();
	dz_check_memory_leak();
	//dz_unregister_datavol_target();
	//dz_unregister_vmvol_target();
	dm_unregister_target(&dzavm);

	LOGTGT(" ______________________________________________________ \n");
	LOGTGT("|                                                      |\n");
	LOGTGT("|           ZAIDSTOR AVM AISA Target UnRegistered      |\n");
	LOGTGT("|______________________________________________________|\n");
}

module_init(dz_avm_target_module_init);
module_exit(dz_avm_target_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

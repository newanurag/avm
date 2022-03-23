/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

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
static int 	dz_target_ctr_vmvol(PDM_TARGET target, unsigned int argc,char **argv);
static int 	dz_target_map_vmvol(struct dm_target *target, struct bio *bio);
static void	dz_target_dtr_vmvol(PDM_TARGET target);
static int 	dz_target_map_vmvol_sread(struct dm_target *target, struct bio *bio);
static int 	dz_target_map_vmvol_swrite(struct dm_target *target, struct bio *bio);

//Target Type for Data Volumes
static struct 	target_type aisa_vmvol = {

	.name 		= "AVM_VOLUME_VM",
	.version	= {1,0,0},
	.module 	= THIS_MODULE,
	.ctr 		= dz_target_ctr_vmvol,
	.dtr 		= dz_target_dtr_vmvol,
	.map 		= dz_target_map_vmvol,
};

//Target Type Table for Read Only Snapshots
//for Virtual Machine volumes
static struct 	target_type aisa_vmvol_sr = {

	.name 		= "AVM_VOLUME_VMSR",
	.version	= {1,0,0},
	.module 	= THIS_MODULE,
	.ctr 		= dz_target_ctr_vmvol,
	.dtr 		= dz_target_dtr_vmvol,
	.map 		= dz_target_map_vmvol_sread,
};

//Target Type Table for Writable Snapshots
//for Virtual Machine Volumes
static struct 	target_type aisa_vmvol_sw = {

	.name 		= "AVM_VOLUME_VMSW",
	.version	= {1,0,0},
	.module 	= THIS_MODULE,
	.ctr 		= dz_target_ctr_vmvol,
	.dtr 		= dz_target_dtr_vmvol,
	.map 		= dz_target_map_vmvol_swrite,
};

RINT dz_process_vmvol_io(PDM_TARGET target, PBIO bio)
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
	lba_index	= ddvol_lba_table[dz_convert_sector_to_lba(DZ_BIO_GET_SECTOR(bio))].lba_index;
	//PRINT_U64(lba_index);
	iorequest->ior_lba 	= lba_index;

	//memset(iorequest, 0, sizeof(IOREQUEST));
	iorequest->ior_bio 			= bio;
	iorequest->ior_parent 		= NULL;
	ATOMIC_SET(iorequest->ior_ref_cnt, 0);

	//print_iorequest(iorequest, "aisa_init");

	//iorequest_get(iorequest);
	if (bio_op(bio) == REQ_OP_WRITE) {
		dz_q_iorequest(write_thread_pool, iorequest);
	} else {
		dz_q_iorequest(read_thread_pool, iorequest);
	}


	return DM_MAPIO_SUBMITTED;
}

//Map function for Snapshot Read i.e Readonly Snapshots
RINT dz_process_vmvol_sread_io(PDM_TARGET target, PBIO bio)
{
	PDDVOL			ddvol 				= NULL;
	U64			lba_index				= 0;
	PDDVOL_LBA_BLOCK ddvol_lba_table	= NULL;
	PIOREQUEST iorequest 				= NULL;

	if (bio_op(bio) == REQ_OP_WRITE) {
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
	lba_index	= ddvol_lba_table[dz_convert_sector_to_lba(DZ_BIO_GET_SECTOR(bio))].lba_index;
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

RINT dz_process_vmvol_swrite_io(PDM_TARGET target, PBIO bio)
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
	lba_index	= ddvol_lba_table[dz_convert_sector_to_lba(DZ_BIO_GET_SECTOR(bio))].lba_index;
	iorequest->ior_lba 	= lba_index;

	//memset(iorequest, 0, sizeof(IOREQUEST));
	iorequest->ior_bio 			= bio;
	iorequest->ior_parent 		= NULL;
	ATOMIC_SET(iorequest->ior_ref_cnt, 0);

	//print_iorequest(iorequest, "aisa_init");

	//iorequest_get(iorequest);
	if (bio_op(bio) == REQ_OP_WRITE) {
		dz_q_iorequest(write_thread_pool, iorequest);
	} else {
		dz_q_iorequest(read_thread_pool, iorequest);
	}

	return DM_MAPIO_SUBMITTED;
}

static int dz_target_map_vmvol(PDM_TARGET target, struct bio *bio)
{
	INT 			ret = SUCCESS;

	if (!aisa_device_object) {
		target->error = "AISA-Error: IO is not supported until AISA device online";
		ret = -EACCES;
		return ret;
	}

	return dz_process_vmvol_io(target, bio);
}

static int dz_target_map_vmvol_sread(PDM_TARGET target, struct bio *bio)
{
	INT 			ret = SUCCESS;

	if (!aisa_device_object) {
		target->error = "AISA-Error: IO is not supported until AISA device online";
		ret = -EACCES;
		return ret;
	}

	return dz_process_vmvol_sread_io(target, bio);
}

static int dz_target_map_vmvol_swrite(PDM_TARGET target, struct bio *bio)
{
	INT 			ret = SUCCESS;

	if (!aisa_device_object) {
		target->error = "AISA-Error: IO is not supported until AISA device online";
		ret = -EACCES;
		return ret;
	}

	return dz_process_vmvol_swrite_io(target, bio);
}

static int dz_target_ctr_vmvol(PDM_TARGET target, unsigned int argc,char **argv)
{
	return dz_target_create(target, argc, argv);
}

static void dz_target_dtr_vmvol(PDM_TARGET target)
{
	dz_target_destroy(target);
}

/*
static int aisa_vmvol_ioctl(struct dm_target *ti, unsigned int cmd, unsigned long arg)
{
	return SUCCESS;
}
*/

RINT dz_register_vmvol_target()
{
	int result;
	result = dm_register_target(&aisa_vmvol);
	if (result < 0) {
		LOGVOLE("\nError in Registering VM Volume Target\n");
	} else {
		LOGVOL("VM Volume Target Registered\n");
	}

	result = dm_register_target(&aisa_vmvol_sr);
	if (result < 0) {
		LOGVOLE("\nError in Registering VM Volume Snapshot Read Target\n");
	} else {
		LOGVOL("VM Volume Snapshot Read Target Registered\n");
	}

	result = dm_register_target(&aisa_vmvol_sw);
	if (result < 0) {
		LOGVOLE("\nError in Registering VM Volume Snapshot Write Target\n");
	} else {
		LOGVOL("VM Volume Snapshot Write Target Registered\n");
	}

	RETURNS;
}

RVOID dz_unregister_vmvol_target()
{
	dm_unregister_target(&aisa_vmvol_sw);
	dm_unregister_target(&aisa_vmvol_sr);
	dm_unregister_target(&aisa_vmvol);
}

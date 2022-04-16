/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

#define TEST_PASSED   100
#define TEST_FAILED   -1

extern int dz_switch_passthrough_mode;
extern int dz_switch_passthrough_read;
extern int dz_switch_passthrough_write;

extern INT dz_switch_fec_engine;
extern INT dz_switch_bec_engine;
extern INT dz_switch_dedupe_engine;
extern INT dz_switch_alignment_engine;

extern INT dz_switch_target_iopath;

//extern ATOMIC64 dz_iorequest_alloc_cnt;
//extern ATOMIC64 dz_iorequest_free_cnt;
extern BASIC_COUNTERS  dz_basic_counters;
extern PBASIC_COUNTERS pdz_basic_counters;

//extern ATOMIC64 dz_page_alloc_cnt;
//extern ATOMIC64 dz_page_free_cnt;

extern SIZE 			TARGET_DEVICE_SIZE;

extern PAVM_DEVICE      aisa_device_object;
extern NAME             DATA_DISK_DEVICE_NAME;
extern OBJECT_LIST 	global_object_list;

extern PSUPERBLOCK 	superblock;
extern struct 	target_type dzavm;

extern CHAR avm_sysfs_entry_get_target_name[DZ_SYSFS_PAGE_SIZE];
extern PAVM_DEVICE 	aisa_device_object;
INT avm_get_target_name(CHAR *buf)
{
	UINT len = 0;
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_target_name;
	if (!aisa_device_object) {
		return 0;
	}
	memset(avm_sysfs_entry_get_target_name, 0, DZ_SYSFS_PAGE_SIZE);

	len = strlen(aisa_device_object->obj.obj_name);

	memcpy(ptr, aisa_device_object->obj.obj_name, len);
	ptr += len;
	memcpy(ptr, "|", 1);
	ptr += 1;

	len = strlen(aisa_device_object->obj.obj_uuid);
	memcpy(ptr, aisa_device_object->obj.obj_uuid, len);
	ptr += len;
	return 0;
}

extern CHAR avm_sysfs_entry_get_target_backstore_name[DZ_SYSFS_PAGE_SIZE];
INT avm_get_target_backstore_name(CHAR *buf)
{
	UINT len = 0;
	if (!aisa_device_object) {
		return 0;
	}
	memset(avm_sysfs_entry_get_target_backstore_name, 0, DZ_SYSFS_PAGE_SIZE);
	len = strlen(aisa_device_object->obj.obj_dm_dev_name);
	memcpy(avm_sysfs_entry_get_target_backstore_name, aisa_device_object->obj.obj_dm_dev_name, len);
	return 0;
}

extern CHAR avm_sysfs_entry_get_target_device_size_bytes[DZ_SYSFS_PAGE_SIZE];
INT avm_get_target_device_size_bytes(CHAR *buf)
{
	CHAR *device_name 	= avm_sysfs_entry_get_target_device_size_bytes;
	INT ret = 0;
	ULONG	size_in_bytes = 0;
	LOGSYS("Input Device Name : %s\n", device_name);

	ret = dz_get_device_size(device_name, &size_in_bytes);	
	if (ret == 0) {
		sprintf(avm_sysfs_entry_get_target_device_size_bytes, "\n%lu", size_in_bytes);
		LOGSYS("Size of device is %lu bytes\n", size_in_bytes);
	} else {
		sprintf(avm_sysfs_entry_get_target_device_size_bytes, "%d", -1 );
		LOGSYS("Size of device %s is -1 byte. ", device_name);
		LOGSYS("IOCTL failed with error code %d\n", ret);
	}

	return 0;
}

extern CHAR avm_sysfs_entry_get_target_device_size_blocks[DZ_SYSFS_PAGE_SIZE];
INT avm_get_target_device_size_blocks(CHAR *buf)
{
	CHAR *device_name 	= avm_sysfs_entry_get_target_device_size_blocks;
	INT ret = 0;
	ULONG	size_in_blocks = 0;
	LOGSYS("Input Device Name : %s\n", device_name);

	ret = dz_get_device_size(device_name, &size_in_blocks);	
	if (ret == 0) {
		size_in_blocks = (size_in_blocks / SECTOR_SIZE);
		sprintf(avm_sysfs_entry_get_target_device_size_blocks, "\n%lu", size_in_blocks);
		LOGSYS("Size of device is %lu blocks (512 bytes block)\n", size_in_blocks);
	} else {
		sprintf(avm_sysfs_entry_get_target_device_size_blocks, "%d", -1 );
		LOGSYS("Size of device %s is -1 block. ", device_name);
		LOGSYS("IOCTL failed with error code %d\n", ret);
	}

	return 0;
}

extern U64 avm_sysfs_entry_get_set_target_passthrough_mode;
INT avm_get_target_passthrough_mode(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_target_passthrough_mode, dz_switch_passthrough_mode);
}

INT avm_set_target_passthrough_mode(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_passthrough_mode, buf);
}

extern U64 avm_sysfs_entry_get_set_target_passthrough_read;
INT avm_get_target_passthrough_read(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_target_passthrough_read, dz_switch_passthrough_read);
}

INT avm_set_target_passthrough_read(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_passthrough_read, buf);
}

extern U64 avm_sysfs_entry_get_set_target_passthrough_write;
INT avm_get_target_passthrough_write(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_target_passthrough_write, dz_switch_passthrough_write);
}

INT avm_set_target_passthrough_write(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_passthrough_write, buf);
}

extern U64 avm_sysfs_entry_get_set_target_lba_block_size;
INT avm_get_target_lba_block_size(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_target_lba_block_size, dz_switch_passthrough_write);
}

INT avm_set_target_lba_block_size(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_passthrough_write, buf);
}


INT avm_do_write_io(CHAR *buf)
{
	LOGSYS("%s",__func__);
	dz_generate_write_io();
	return 0;
}

INT avm_do_read_io(CHAR *buf)
{
	LOGSYS("%s",__func__);
	dz_generate_read_io();
	return 0;
}

INT avm_do_zero_io(CHAR *buf)
{
	LOGSYS("%s",__func__);
	dz_generate_zero_io();
	return 0;
}

extern INT dz_double_page;
INT avm_do_write_verify_io(CHAR *buf)
{
	LOGSYS("%s",__func__);
	printk(KERN_ERR"\n");
	LOGSYS("Generate Write Verify IO With Normal Pages Allocation\n");
	dz_double_page = 0;
	dz_generate_write_verify_io();
	LOGSYS("Generate Write Verify IO With Double Pages Allocation\n");
	dz_double_page = 1;
	dz_generate_write_verify_io();
	printk(KERN_ERR"\n");
	return 0;
}

extern CHAR avm_sysfs_entry_issue_read_io_on_device[DZ_SYSFS_PAGE_SIZE];
extern U64 avm_sysfs_entry_issue_read_io_at_sector;
extern INT avm_sysfs_entry_issue_read_io_with_size;

//This function will read the data in the form pages by using bio framework. 
// It essentially means that it will not go through dmtarget map function
// There are three main inputs to this function which is provided through
// sysfs interface
// Device Name : avm_sysfs_entry_issue_read_io_on_device
// IO Sector   : avm_sysfs_entry_issue_read_io_at_sector
// IO Size     : avm_sysfs_entry_issue_read_io_with_size
//
// As an output, the read pages are printed into syslog and will be visible 
// through dmesg
RINT  dz_generate_read_io(VOID)
{
	INT ret = SUCCESS;
	IOREQUEST iorequest;
	DATA_PAGES_T data_pages;
	MEMSET(&iorequest);


	LOGSYS("avm_sysfs_entry_issue_read_io_on_device = %s\n", avm_sysfs_entry_issue_read_io_on_device);
	LOGSYS("avm_sysfs_entry_issue_read_io_at_sector = %lli\n", avm_sysfs_entry_issue_read_io_at_sector);
	LOGSYS("avm_sysfs_entry_issue_read_io_with_size = %d\n", avm_sysfs_entry_issue_read_io_with_size);

	iorequest.ior_sector	= avm_sysfs_entry_issue_read_io_at_sector;
	iorequest.ior_size 		= avm_sysfs_entry_issue_read_io_with_size;
	ret = dz_read_data_from_block_device(&iorequest, avm_sysfs_entry_issue_read_io_on_device, &data_pages);
	if (ret != SUCCESS) {
		LOGSYSE("IO Read Failure on device %s \n", avm_sysfs_entry_issue_read_io_on_device);
	} else {
		LOGSYS("IO Read successfully\n");
		print_data_pages(&data_pages);
		dz_free_data_pages(&data_pages, false);
	}
	RETURN;
}
EXPORT_SYMBOL(dz_generate_read_io);

extern CHAR avm_sysfs_entry_issue_write_io_on_device[DZ_SYSFS_PAGE_SIZE];
extern U64 avm_sysfs_entry_issue_write_io_at_sector;
extern INT avm_sysfs_entry_issue_write_io_with_size;

RINT dz_generate_write_io(VOID)
{
	INT ret = SUCCESS;
	IOREQUEST iorequest;
	MEMSET(&iorequest);

	if (avm_sysfs_entry_issue_write_io_with_size > _16K) {

		LOGW("IO Size is higher than 16K. Limiting it to 16K only\n");
		avm_sysfs_entry_issue_write_io_with_size = _16K;
	}

	LOGSYS("avm_sysfs_entry_issue_write_io_on_device = %s\n", avm_sysfs_entry_issue_write_io_on_device);
	LOGSYS("avm_sysfs_entry_issue_write_io_at_sector = %lli\n", avm_sysfs_entry_issue_write_io_at_sector);
	LOGSYS("avm_sysfs_entry_issue_write_io_with_size = %d\n", avm_sysfs_entry_issue_write_io_with_size);
	
	iorequest.ior_sector	= avm_sysfs_entry_issue_write_io_at_sector;
	iorequest.ior_size 		= avm_sysfs_entry_issue_write_io_with_size;
	iorequest.ior_private		= dz_get_io_data(iorequest.ior_size);
	//strncpy(DATA_DISK_DEVICE_NAME, avm_sysfs_entry_issue_write_io_on_device, strlen(avm_sysfs_entry_issue_write_io_on_device));

	//ret = dz_write_data(&iorequest);
	ret = dz_write_data_to_block_device(&iorequest, avm_sysfs_entry_issue_write_io_on_device);
	if (ret != SUCCESS) {
		LOGSYSE("IO Write Failure on device \n");
	} else {
		LOGSYS("IO written successfully\n");
	}
	RETURNS ;
}
EXPORT_SYMBOL(dz_generate_write_io);



extern CHAR avm_sysfs_entry_issue_zero_io_on_device[DZ_SYSFS_PAGE_SIZE];
extern U64 avm_sysfs_entry_issue_zero_io_at_sector;
extern INT avm_sysfs_entry_issue_zero_io_with_size;

RINT dz_generate_zero_io(VOID)
{
	INT ret = SUCCESS;
	IOREQUEST iorequest;
	MEMSET(&iorequest);

	if (avm_sysfs_entry_issue_zero_io_with_size > _16K) {

		LOGW("IO Size is higher than 16K. Limiting it to 16K only\n");
		avm_sysfs_entry_issue_zero_io_with_size = _16K;
	}

	LOGSYS("avm_sysfs_entry_issue_zero_io_on_device = %s\n", avm_sysfs_entry_issue_zero_io_on_device);
	LOGSYS("avm_sysfs_entry_issue_zero_io_at_sector = %lli\n", avm_sysfs_entry_issue_zero_io_at_sector);
	LOGSYS("avm_sysfs_entry_issue_zero_io_with_size = %d\n", avm_sysfs_entry_issue_zero_io_with_size);
	
	iorequest.ior_sector	= avm_sysfs_entry_issue_zero_io_at_sector;
	iorequest.ior_size 		= avm_sysfs_entry_issue_zero_io_with_size;

	iorequest.ior_private		= dz_kmalloc(iorequest.ior_size, GFP_KERNEL|__GFP_ZERO);

	//ret = dz_zero_data(&iorequest);
	ret = dz_write_data_to_block_device(&iorequest, avm_sysfs_entry_issue_zero_io_on_device);
	if (ret != SUCCESS) {
		LOGSYSE("Zeroed IO Failure on device \n");
	} else {
		LOGSYS("Zeroed IO written successfully\n");
	}
	dz_kfree(iorequest.ior_private, iorequest.ior_size);
	RETURNS ;
}
EXPORT_SYMBOL(dz_generate_zero_io);


extern CHAR avm_sysfs_entry_issue_read_io_on_device[DZ_SYSFS_PAGE_SIZE];
extern U64 avm_sysfs_entry_issue_read_io_at_sector;
extern INT avm_sysfs_entry_issue_read_io_with_size;
extern INT avm_sysfs_entry_write_verify_io;

RINT dz_generate_write_verify_io(VOID)
{
	INT ret 				= SUCCESS;
	UINT	pgcnt			= 0;
	PVOID pagedata 			= NULL;
	CHAR	*readbuf		= NULL;
	CHAR	*preadbuf		= NULL;
	INT 				i 	= 0;
	BOOL			same	= true;
	DATA_PAGES_T	data_pages;

	IOREQUEST iorequest;
	MEMSET(&iorequest);
	readbuf = vmalloc(_16K); //Max is 16K buffer so far.
	if (!readbuf) {
		LOGSYSE("Unable to allocate memory for readbuf, hence quitting\n");
		avm_sysfs_entry_write_verify_io = TEST_FAILED;
		RETURNF;
	}
	MEMSET(readbuf);
	preadbuf = readbuf;
	

	if (avm_sysfs_entry_issue_write_io_with_size > _16K) {
		LOGW("IO Size is higher than 16K. Limiting it to 16K only for writes\n");
		avm_sysfs_entry_issue_write_io_with_size = _16K;
	}
	
	//Issue a write first
	iorequest.ior_sector	= avm_sysfs_entry_issue_write_io_at_sector;
	iorequest.ior_size 		= avm_sysfs_entry_issue_write_io_with_size;
	iorequest.ior_private		= dz_get_io_data(iorequest.ior_size);
	strncpy(DATA_DISK_DEVICE_NAME, avm_sysfs_entry_issue_write_io_on_device, strlen(avm_sysfs_entry_issue_write_io_on_device));

	if ((avm_sysfs_entry_issue_write_io_with_size % SECTOR_SIZE) != 0) {
		LOGSYSE("IO MUST be in multiples of 512 bytes i.e. SECTOR_SIZE, hence quitting\n");
		avm_sysfs_entry_write_verify_io = TEST_FAILED;
		RETURNF;

	}

	ret = dz_write_data(&iorequest);
	if (ret != SUCCESS) {
		LOGSYSE("IO Write Failure on device \n");
		avm_sysfs_entry_write_verify_io = TEST_FAILED;
		RETURNF;
	} else {
		LOGSYS("IO written successfully\n");
	}

	MEMSET(&iorequest);

	if (avm_sysfs_entry_issue_write_io_with_size > _16K) {
		LOGW("IO Size is higher than 16K. Limiting it to 16K only for reads as well\n");
		avm_sysfs_entry_issue_read_io_with_size = _16K;
	}
	//Issue a read second. issue_read_io_at_sector anad read_io_size will be same as 
	//write_io_sector and write_io_size
	iorequest.ior_sector	= avm_sysfs_entry_issue_read_io_at_sector;
	iorequest.ior_size 		= avm_sysfs_entry_issue_read_io_with_size;
	strncpy(DATA_DISK_DEVICE_NAME, avm_sysfs_entry_issue_read_io_on_device, strlen(avm_sysfs_entry_issue_read_io_on_device));
	ret = dz_read_data(&iorequest, &data_pages);
	if (ret != SUCCESS) {
		LOGSYSE("IO Read Failure on device \n");
		avm_sysfs_entry_write_verify_io = TEST_FAILED;
		RETURNF;
	} else {
		LOGSYS("IO Read successfully\n");
		pgcnt = data_pages.pgcnt;
		for (i=0; i < pgcnt; i++){
			PBIOVEC bvec = &(data_pages.pagevec[i]);
			pagedata = kmap(bvec->bv_page);
			pagedata += bvec->bv_offset;
			memcpy(preadbuf, pagedata, bvec->bv_len);
			kunmap(bvec->bv_page);
			preadbuf += bvec->bv_len;
		}
		LOGSYS("Buffer is read successfully and copied locally\n");
		if (strncmp(dz_get_io_data(iorequest.ior_size), readbuf, avm_sysfs_entry_issue_read_io_with_size) == 0 ) {
			same = true;
		} else {
			same = false;
		}
		vfree(readbuf);
		dz_free_data_pages(&data_pages, false);
		if (same) {
			LOGSYS("\nRESULT:PASSED: Write Data and Readback Data are same\n");
			avm_sysfs_entry_write_verify_io = TEST_PASSED;
		} else {
			LOGSYS("\nRESULT:FAILED: Write Data and Readback Data are different\n");
			avm_sysfs_entry_write_verify_io = TEST_FAILED;
		}
	}
	RETURNS ;
}

EXPORT_SYMBOL(dz_generate_write_verify_io);

extern U64 avm_sysfs_entry_get_set_target_switch_iopath;

INT avm_get_target_switch_iopath(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_target_switch_iopath, dz_switch_target_iopath);
}

extern PDM_TARGET   AVM_DM_TARGET;
INT avm_set_target_switch_iopath(U64 buf)
{
	//struct target_type *type = &dzavm;
	DZ_SYS_SET_SWITCH(dz_switch_target_iopath, buf);
	dzavm.map = dz_avm_target_map;
	dzavm.ctr = NULL;
}

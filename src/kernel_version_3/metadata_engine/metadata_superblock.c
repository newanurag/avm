/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern INT 				dz_switch_passthrough_mode;
extern PBLOCK_DEVICE	METADATA_DISK_DEVICE;
extern PBLOCK_DEVICE	DATA_DISK_DEVICE;
extern PAVM_DEVICE 		aisa_device_object;
extern U64 				lba_table_size;
extern U64 				pba_table_size;
extern U64 				hash_table_size;

extern PSUPERBLOCK		superblock;

//U64			md_engine_memory_usage = 0;
//EXPORT_SYMBOL(md_engine_memory_usage);

extern U64  			metadata_lba_table_size;
extern PMDD_BLOCK  		metadata_lba_table;

extern U64 				metadata_lba_lock_table_size;
extern PMD_LOCK			metadata_lba_lock_table;	// Array of spinlocks for MetaData Lock

PSUPERBLOCK dz_superblock_alloc(VOID)
{
	PVOID		pagebuf;
	PPAGE		page	 = NULL;
	//PSUPERBLOCK	superblock = NULL;

	// Allocating a page for superblock instead of kmalloc
	// so that there is no need to allocate a new page when
	// flushing/writing to disk.
	// Simply attach this page to bio
	page = dz_write_page_alloc();
	if (page) {
		pagebuf = kmap(page);
		memset(pagebuf, 0, PAGE_SIZE);
		kunmap(page);

		superblock = dz_kmalloc(sizeof(SUPERBLOCK), GFP_KERNEL);
		if (!superblock) {
			BUG_ON(1);
		}
		MEMSET(superblock);
		superblock->page = page;
		return superblock;
	} else {
		LOGE("Unable to allocate superblock\n");
		BUG_ON(1);	
		return NULL;
	}
}

RVOID dz_superblock_free(PSUPERBLOCK ptr)
{
	if (ptr) {
		dz_write_page_free(ptr->page);
		dz_kfree(ptr, sizeof(SUPERBLOCK));
	}
}

RVOID print_superblock(PSUPERBLOCK sb)
{
	LOGMET("Printing Superblock Information\n");
	LOGMET("             version = %s\n", sb->version);
	LOGMET("metadata_dev_magicno = %s\n", sb->metadata_dev_magicno);
	LOGMET("   metadata_dev_name = %s\n", sb->metadata_dev_name);
	LOGMET("   metadata_dev_uuid = %s\n", sb->metadata_dev_uuid);
	LOGMET("   metadata_offset   = %llu\n", sb->metadata_offset);

	LOGMET("    data_dev_magicno = %s\n", sb->data_dev_magicno);
	LOGMET("       data_dev_name = %s\n", sb->data_dev_name);
	LOGMET("       data_dev_uuid = %s\n", sb->data_dev_uuid);

	LOGMET("      lba_table_size = %llu\n", sb->lba_table_size);
	LOGMET("      pba_table_size = %llu\n", sb->pba_table_size);
	LOGMET("     hash_table_size = %llu\n", sb->hash_table_size);
}

RINT dz_superblock_read(VOID)
{
	PSUPERBLOCK superblock	= NULL;
	SECTOR superblk_sector 	= 0;
	SECTOR superblk_size 	= PAGE_SIZE;
	PPAGE	read_page		= NULL;
	PVOID		pagebuf;
	PVOID		pagebuf2;
	read_page = dz_read_page_from_raw_disk_sync(METADATA_DISK_DEVICE, superblk_sector, superblk_size);

	if (read_page) {
		pagebuf = kmap(read_page);
		pagebuf2 = kmap(superblock->page);
		memcpy(superblock, pagebuf, sizeof(SUPERBLOCK));
		memcpy(pagebuf2, pagebuf, PAGE_SIZE);
		kunmap(superblock->page);
		kunmap(read_page);
		RETURNS;
	} else {
		RETURNF;
	}
}

/* This function writes the header on the DATA DISK.
 * Header size is typically SUPERBLOCK_SIZE and it is
 * written at sector SUPERBLOCK_SECTOR
 */ 
RINT dz_superblock_write(PSUPERBLOCK superblock)
{
	SECTOR	sector		= SUPERBLOCK_SECTOR;

	//Since SUPERBLOCK_SIZE is always lesser than or equal to PAGE_SIZE
	if(dz_write_page_to_raw_disk_sync(DATA_DISK_DEVICE, sector, SUPERBLOCK_SIZE, superblock->page)) {
		LOGMETE("Error in writing superblock information to metadata disk\n");
		RETURNF;
	} else {
		LOGMET("Superblock written to metadata device %s at sector %llu\n", superblock->metadata_dev_name, (U64)sector);
		RETURNS;
	}
}

/* Initialize the superblock header
 * Also dont write it, if there is already header placed on DATA DISK.
 * It will lead to some inconsistencies
 */
RINT dz_superblock_init(VOID)
{
	PVOID		pagebuf	= NULL;
	U32		*hash_array	= NULL;
	INT			ret		= SUCCESS;
	superblock = dz_superblock_alloc();

	memcpy(superblock->metadata_dev_magicno,
		aisa_device_object->metadata_dev_magicno, MAGICNO_LENGTH); 
	memcpy(superblock->metadata_dev_name,
		aisa_device_object->metadata_dev_name, NAME_LENGTH); 
	memcpy(superblock->metadata_dev_uuid,
		aisa_device_object->metadata_dev_uuid, UUID_LENGTH); 

	memcpy(superblock->data_dev_magicno,
		aisa_device_object->data_dev_magicno, MAGICNO_LENGTH); 
	memcpy(superblock->data_dev_name,
		aisa_device_object->data_dev_name, NAME_LENGTH); 
	memcpy(superblock->data_dev_uuid,
		aisa_device_object->data_dev_uuid, UUID_LENGTH); 

	superblock->lba_table_size 	= lba_table_size;
	superblock->lba_block_size 	= LBA_BLOCK_SIZE;
	superblock->pba_table_size 	= pba_table_size;
	superblock->pba_block_size 	= PBA_BLOCK_SIZE;
	superblock->hash_table_size = hash_table_size;
	superblock->hash_size 		= sizeof(HASH);
	superblock->metadata_offset = (SUPERBLOCK_RESERVED_SECTORS * SECTOR_SIZE);

	strcpy(superblock->version, DZ_VERSION);
	hash_array = (U32 *)&superblock->checksum;
	dz_generate_hash(superblock, sizeof(SUPERBLOCK), hash_array);
	memcpy((PVOID)&(superblock->checksum), hash_array, sizeof(HASH));

	pagebuf = kmap(superblock->page);
	memcpy(pagebuf, superblock, sizeof(SUPERBLOCK)); 
	kunmap(superblock->page);

	if (dz_switch_passthrough_mode) {
		// There is no need to write anything on the disk if it is in passthrough mode
		LOGMET("AISA-Info: Passthrough mode is enabled. Superblock WILL not be written\n");
		RETURNS;
	}
	dz_md_init_counters();

	//This is where superblock header is actually written on DATA DISK
	ret = dz_superblock_write(superblock);
	RETURN;
}

VOID dz_superblock_exit(VOID)
{
	dz_superblock_free(superblock);
}

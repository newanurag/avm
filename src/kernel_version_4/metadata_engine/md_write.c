/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

#define dz_md_engine_init	dz_metadata_engine_init
extern int dz_switch_passthrough_mode;

extern PBLOCK_DEVICE 	avm_metadata_bdev;

//U64			md_engine_memory_usage = 0;
//EXPORT_SYMBOL(md_engine_memory_usage);

extern U64  		metadata_lba_table_size;
extern PMDD_BLOCK  	metadata_lba_table;

extern U64 		metadata_lba_lock_table_size;
extern PMD_LOCK	metadata_lba_lock_table;	// Array of spinlocks for MetaData Lock


// offset is irrelevant if all flag is set true
RVOID dz_read_and_print_metadata_block(UINT offset, U64 sector, BOOL all) 
{
	PVOID		pagebuf;
	INT			i = 0;
	PMDD_BLOCK		pmdd = NULL;
	PPAGE		read_page = NULL;

	if (!(read_page = dz_read_page_from_raw_disk_sync(avm_metadata_bdev, sector, SECTOR_SIZE)) ) {
		LOGE("Read Page Failed\n");
		return;
	}
	if (all) goto all;

	LOG("Printing single MD Block at offset %u\n", offset);
	pagebuf = kmap(read_page);
	pagebuf += offset;
	kunmap(read_page);

	print_metadata_block(pagebuf);
	dz_read_page_free(read_page);
	return;

all:
	LOG("Printing All MD Blocks\n");
	pagebuf = kmap(read_page);
	pmdd = pagebuf;
	for (i=0; i < SECTOR_SIZE; i = i + METADATA_BLOCK_SIZE) {
		LOG("MD Block at offset %u\n", i);
		print_metadata_block(pagebuf);
		pagebuf = pagebuf + METADATA_BLOCK_SIZE;
	}
	kunmap(read_page);
	dz_read_page_free(read_page);
	return;


}

/*******************************************************
    Layout of MetaData Disk Block
    Total 8 metadata blocks will be accomoted
    in one SECTOR
********************************************************

    <---METADATA_BLOCK_SIZE---->
    <-------- 64 bytes -------->
^   +--------------------------+
|   |       MDD_BLOCK          |     For LBA - 0
    +--------------------------+
S   |       MDD_BLOCK          |     For LBA - 1
E   +--------------------------+
C   |       MDD_BLOCK          |     For LBA - 2
T   +--------------------------+
O   |       MDD_BLOCK          |     For LBA - 3
R   +--------------------------+
    |       MDD_BLOCK          |     For LBA - 4
S   +--------------------------+
|   |       MDD_BLOCK          |     For LBA - 5
Z   +--------------------------+
E   |       MDD_BLOCK          |     For LBA - 6
    +--------------------------+
|   |       MDD_BLOCK          |     For LBA - 7
V   +--------------------------+
    .                          .     For LBA - 8
    .                          .
    +--------------------------+


TODO:
Writing of metadata on a seperate disk involves
1. One Read
2. One Lock. 
3. One Write

We need to keep this metadata on the NVDIMM 
first. And then flush to metadata disk in 
batches or in bulk.

Since sizeof MDD_BLOCK is 64 bytes and this space is 
occupied by one single LBA. So Size of Metadata disk 
should be 1/64th of the Data disk because a 4K size 
space on Metadata disk can accomodate 64 LBA's metadata

E.g if Data Disk size is 1 TB then
Metadata Disk size will be 16 GB ((1024 * 1024 * 1024 * 1024 ) / 64 )

******************************************************/

//This function will update the inmemory data structure of metadata
//i.e. metadata_lba_table
RINT dz_metadata_update(PIOREQUEST iorequest, U64 pba_index, SECTOR dd_sector)
{
	U64			lba_index 	= iorequest->ior_lba;
	PMDD_BLOCK	pmdd 		= GET_MDB_ENTRY(lba_index);

	pmdd->mdd_lba 			= lba_index;
	pmdd->mdd_pba_index 	= pba_index;
	pmdd->mdd_hash_index 	= iorequest->ior_ddinfo->new_hash_index;
	pmdd->mdd_dd_sector 	= dd_sector;
	pmdd->mdd_hash_collision_tag = iorequest->ior_ddinfo->lba_entry->hash_collision_tag;
	memcpy((PVOID)&pmdd->mdd_hash, iorequest->ior_ddinfo->hash, sizeof(HASH));

	RETURNS;
}
EXPORT_SYMBOL(dz_metadata_update);

RINT dz_metadata_write(PIOREQUEST iorequest, U64 pba_index, SECTOR dd_sector)
{

	INT			ret  		= SUCCESS;
	UINT		io_size 	= SECTOR_SIZE;
	U64			sector 		= 0;
	U64			lba_index 	= iorequest->ior_lba;
	U64			mdb_bytes	= lba_index * METADATA_BLOCK_SIZE;
	UINT		offset		= 0;
	PPAGE		read_page 	= NULL;
	PVOID		pagebuf;
	PMDD_BLOCK	pmdd 		= GET_MDB_ENTRY(lba_index);

	U64 mdb_to_sector		= mdb_bytes / SECTOR_SIZE;

	// Each metadata size is 64 bytes (METADATA_BLOCK_SIZE) at max, 
	// so one SECTOR can accomodate 8 metadata blocks.
	// Each lba is propotional to 1 metadata block
	// offset means the location of mbb_block for the corresponding lba
	// inside that sector
	if (mdb_to_sector == 0) {
		offset = mdb_bytes;
		sector = 0;
	} else {
		offset = mdb_bytes - (mdb_to_sector * SECTOR_SIZE); // mdb_bytes will always be more 
		sector = mdb_to_sector;

	}
	/* 
	We are going to set the start sector of Metadata disk as 
	the SUPERBLOCK_RESERVED_SECTORS. This is achieved at the time of
	configuring the metadata disk. i.e. during dmsetup where the
	start sector value is provided
	
	//First two pages are reserved for superblock so just
	//lba 0 will be written from lba 2
	sector += SUPERBLOCK_RESERVED_SECTORS;
	*/
	

	LOG("Metadata starts at sector %lli\n", sector);
	PRINT_SIZE(sizeof(MDD_BLOCK));
	MEMSET(pmdd);
	PRINT_UINT(offset);

	pmdd->mdd_lba 			= lba_index;
	pmdd->mdd_pba_index 	= pba_index;
	pmdd->mdd_hash_index 	= iorequest->ior_ddinfo->new_hash_index;
	pmdd->mdd_dd_sector 	= dd_sector;
	pmdd->mdd_hash_collision_tag = iorequest->ior_ddinfo->lba_entry->hash_collision_tag;
	memcpy((PVOID)&pmdd->mdd_hash, iorequest->ior_ddinfo->hash, sizeof(HASH));
	

	//Note that, the spin lock is sector wise, i.e. one sector can accomodate
	// 8 LBAs (64 * 8). So it will be good if we can do read and writes of 
	// 8 contiguous LBA. This will save locking, disk reads and disk writes
	// Although it all depends on the IOs we received. If there are IOs which
	// are 32K in size or more then it is possible.
	spin_lock(&metadata_lba_lock_table[sector].md_lock);

	//First read the existing data from metadata disk. io_size is SECTOR_SIZE i.e. 512 bytes
	//Note that, the data is read of SECTOR_SIZE but it is returned into a PAGE
	read_page = dz_read_page_from_raw_disk_sync(avm_metadata_bdev, sector, io_size);
	if (!read_page) {
		//TODO: Failure scenario
		spin_unlock(&metadata_lba_lock_table[sector].md_lock);
		LOG("Metadata updation Read failed for lba %lli at offset %u\n", lba_index, offset);
		RETURNF;
	} 
	pagebuf = kmap(read_page);
	pagebuf += offset;
	memcpy(pagebuf, pmdd, METADATA_BLOCK_SIZE);
	kunmap(read_page);


	ret = dz_write_page_to_raw_disk_sync(avm_metadata_bdev, sector, io_size, read_page);
	if (ret != SUCCESS) {
		//TODO: Failure scenario
		spin_unlock(&metadata_lba_lock_table[sector].md_lock);
		LOG("Metadata updation Write failed for lba %lli at offset %u\n", lba_index, offset);
		BUG_ON(1);
		RETURNF;

	} else {
		spin_unlock(&metadata_lba_lock_table[sector].md_lock);
		LOG("Metadata updated Write successfully for lba %lli at offset %u\n", lba_index, offset);
	}
	//print_metadata_block((PMDD_BLOCK)pagebuf);

	dz_read_page_free(read_page);

	dz_read_and_print_metadata_block(offset, sector , false);
	//dz_read_and_print_metadata_block(offset, sector , true);

	RETURNS;
}
EXPORT_SYMBOL(dz_metadata_write);

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _MD_ENGINE_H_
#define _MD_ENGINE_H_

#include "types.h"
#include "macros.h"
#include "iorequest.h"

// Hash Related Data Structures
typedef struct dz_hash_s {
	union {
		U32 checksum  [4];
		U32 cksum 	  [4];
		U32 hash 	  [4];
		U32 hash32 	  [4];
		U64 hash64	  [2];
	};
}HASH, *PHASH, HASH_T, *PHASH_T;

typedef struct dz_superblock_s {
	NAME	version;

	MAGICNO	metadata_dev_magicno; 	// MagicNo of metadata disk
	NAME	metadata_dev_name;		// Name of metadata disk
	UUID	metadata_dev_uuid;		// Unique Id of metadata disk

	MAGICNO	data_dev_magicno;		// Datadisk MagicNo
	NAME	data_dev_name;			// Datadisk name with path
	UUID	data_dev_uuid;			// Datadisk Uniquie Id

	U64 	lba_table_size;			// Size of LBA table. It will be in 4K size Pages
	USHORT	lba_block_size;
	U64		pba_table_size;			// Size of PBA table. It will be in 4K size  Pages
	USHORT	pba_block_size;
	U64		metadata_lba_table_size;
	USHORT	metadata_lba_block_size;
	U64		metadata_lba_lock_table_size;
	USHORT	metadata_lba_lock_block_size;
	U64		hash_table_size;
	INT		hash_size;				// Size of HASH (hash_s) data structure

	U64		num_unused_pbas;
	U64		num_unused_lbas;
	U64		num_unused_hash;
	U64		num_used_pbas;
	U64		num_used_lbas;
	U64		num_used_hash;
	U64		data_offset;			// Location on datadisk where the real data gets stored
	U64		metadata_offset;		// Location on metadisk where the metadata of real data gets stored
	BOOL	fresh;			// Is it being used for the first time
	HASH	checksum;				//Hash Calculated for SUPERBLOCK (this) datastructure
	PPAGE	page;

} SUPERBLOCK, *PSUPERBLOCK;

//Disk Structure for storing Metadata on disk. This disk typically will be different than
//DATA DISK. 
//It should not be greater than METADATA_BLOCK_SIZE
typedef struct dz_metadata_disk_block_s {
	U64     mdd_lba;                    //LBA index refers to location in lba_table     
	HASH    mdd_hash;                   //Actual generated hash value of data page for the given lba
	U32     mdd_hash_index:31;          //This is used to get the pba_index from hash_table
	U64     mdd_hash_collision_tag:33;  //Used in case of dedupe or hash collision. 
	U64     mdd_pba_index;              //Refers to pba in pba_table
	U64     mdd_dd_sector;              //DATA DISK sector on which this LBA's data is stored
	LIST_HEAD   list;                   //Except this field all the above fields will be stored on disk.
	BOOL    mdd_dirty:1;                //Indicates whether required flushing
                
}__attribute__((__packed__)) MDD_BLOCK, *PMDD_BLOCK, MD_BLOCK, *PMD_BLOCK;

//Data structure of locking metadata blocks.
//Note that we have kept it outside the MDD_BLOCK because we are going to create
//a table/array of locks where one lock will be used for 8 MDD_BLOCKs
typedef struct dz_metadata_lock_s {
	SPINLOCK    md_lock;
}__attribute__((__packed__)) MD_LOCK, *PMD_LOCK, MDD_LOCK, *PMDD_LOCK;



// Hash / Checksum Related Function Declarations
VOID    test_hash(PVOID data, UINT size);
VOID    test_hash2(char *, char *);
VOID    dz_generate_hash(VOID *data, SIZE size, PVOID output_hash);
BOOL    dz_same_hash(PHASH hash1, PHASH hash2);
U32     dz_get_hash_index(U32 hash);


PSUPERBLOCK dz_superblock_alloc(VOID);
RVOID dz_superblock_free(PSUPERBLOCK ptr);
RVOID print_superblock(PSUPERBLOCK sb);
RINT dz_superblock_init(VOID);
VOID dz_superblock_exit(VOID);
RINT dz_superblock_read(VOID);
RINT dz_superblock_write(PSUPERBLOCK superblock);

VOID dz_metadata_engine_exit(VOID);
RINT dz_metadata_engine_init(VOID);
VOID dz_metadata_block_exit(VOID);
RINT dz_metadata_block_init(VOID);
RINT dz_metadata_write(PIOREQUEST iorequest, U64 pba_index, SECTOR dd_sector);
RINT dz_metadata_update(PIOREQUEST iorequest, U64 pba_index, SECTOR dd_sector);

RPVOID dz_create_metadata_table(SIZE table_size);
RVOID  dz_destroy_metadata_table(VOID);
RPVOID dz_create_metadata_lock_table(SIZE table_size);
RVOID  dz_destroy_metadata_lock_table(VOID);

VOID  dz_md_init_counters(VOID);
RVOID dz_metadata_flush_start_from_timer_context(VOID);
RINT  dz_metadata_flush_timer(PVOID data);
RVOID dz_metadata_flush_timer_stop(VOID);
RINT  dz_metadata_create_flush_thread_pool(INT num_threads);
RVOID dz_destroy_flush_thread_pool(VOID);
RVOID dz_metadata_flush_io(PIOREQUEST parent_iorequest);

#endif // _MD_ENGINE_H_

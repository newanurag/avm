#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "types.h"
#include "macros.h"
#include "metadata_engine.h"
#include "dedupe_engine.h"


typedef struct dz_bvec_holder_s {
	union {
		PPAGE	page;
		PPAGE	bv_page;
	};
	SHORT	idx;
	union {
		USHORT 	bv_len;
		USHORT 	len;
	};
	union {
	USHORT 	bv_offset;
	USHORT 	offset;
	};
} BVEC_HOLDER, *PBVEC_HOLDER;

// Parent Child BIO

typedef struct child_info {
	INT  	tot_child_cnt;
	ATOMIC  child_cnt;
	INT	error;
} CHILD_INFO, *PCHILD_INFO;




typedef struct dz_object_list_s {
	union {
		SPINLOCK	objectq_lock;
	};
	union {
		LIST_HEAD 	objectq_head;
	};
	union {
		ATOMIC64		objectq_num_items;     
	};
} OBJECT_LIST, *POBJECT_LIST, AVM_OBJECT_LIST, FREE_PAGE_LIST, *PFREE_PAGE_LIST;


// Device Object main data structure
typedef struct dz_target_object_s {
	MAGICNO		obj_magicno;
	NAME		obj_name;
	UUID		obj_uuid;
	COUNT		obj_child_cnt;
	PBA     	obj_start_pba;
	LBA     	obj_start_lba;
	LBA     	obj_end_lba;
	PBA     	obj_end_pba;
	FLAGS		obj_flags;
	NAME		obj_dm_dev_name; // Underlying Device Name
	UUID		obj_dm_dev_uuid; // Underlying Device ID
	DM_DEV	   *obj_dm_dev;
	UCHAR		obj_type; 		
	CHAR		obj_date[DATE_LENGTH];
	CHAR		obj_time[TIME_LENGTH];
#define	OBJECT_TYPE_AISA						1
#define	OBJECT_TYPE_DATA_VOLUME					2
#define	OBJECT_TYPE_DATA_VOLUME_SNAPSHOT_READ	3
#define	OBJECT_TYPE_DATA_VOLUME_SNAPSHOT_WRITE	4
#define	OBJECT_TYPE_VM_VOLUME					5
#define	OBJECT_TYPE_VM_VOLUME_SNAPSHOT_READ		6
#define	OBJECT_TYPE_VM_VOLUME_SNAPSHOT_WRITE	7
	LIST_HEAD 	objectq;
} DEVICE_OBJECT, *PDEVICE_OBJECT;

// Flash Device. 
typedef struct dz_target_device_s {
	DEVICE_OBJECT	obj;			// Always First object. Denotes the type of avm device
	PPBA_BLOCK	pba_table;			// Array of Physical Address Blocks
	PHASH_BLOCK	hash_table;			// Pointer to Hash Blocks
	PLBA_BLOCK	lba_table;			// Array of Logical Address Blocks
	PMD_BLOCK	metadata_lba_table;	// Array of MetaData blocks
	PMD_LOCK	metadata_lba_lock_table;		// Array of spinlocks for MetaData Lock
	U64			pba_disk_offset;	//Offset on DataDisk for the pba
	SPINLOCK	pba_disk_offset_lock;
	PBA_LIST    free_pbas;
	PBA_LIST    used_pbas;
	PBA_LIST    pvt_pbas;
	PBA_LIST    all_pbas;
	PBA_LIST	tobe_erased_pbas;
	PBA_LIST	bad_pbas;

	COUNT		free_cnt;
	COUNT		used_cnt;
	COUNT		pvt_cnt;
	COUNT		tot_cnt;
	COUNT		tobe_erased_cnt;
	COUNT		tot_erased_cnt;

	LOCK		free_pba_lock;
	LOCK		used_pba_lock;
	LOCK		pvt_pba_lock;
	LOCK		tobe_erased_pba_lock;

	CHECKSUM	checksum;
	CRC			crc;

	NAME			data_dev_name; 		// Underlying data Device Name
	MAGICNO			data_dev_magicno;	// Unique No. passed from CLI
	PBLOCK_DEVICE	data_bdev;
	UUID			data_dev_uuid;
	
	NAME			metadata_dev_name; 	// Underlying Metadata Device Name
	MAGICNO			metadata_dev_magicno;
	PBLOCK_DEVICE	metadata_bdev;
	UUID			metadata_dev_uuid;
	union {
		PSUPERBLOCK	superblk;			// Starting Place where header info is placed
		PSUPERBLOCK	superblock;
	};

} AVMDEVICE, *PAVMDEVICE, AVM_DEVICE, *PAVM_DEVICE, AISA_DEVICE, *PDZ_TARGET_DEVICE, DZ_TARGET_DEVICE; 

// Flash Volume main data  structure
typedef struct dz_dedupe_volume_s {
	DEVICE_OBJECT		ddvol_obj;		// Always First object
	PVOID				ddvol_parent;	// For normal volumes, parent will be AISA Target.
										// For snapshot volumes, parent will be normal volumes
	PDDVOL_LBA_BLOCK	ddvol_lba_table;// Array of LBA Blocks
	U64					ddvol_table_size;
	LOCK 				ddvol_lba_lock;	// Spinlock for entire volume. Need skip list to avoid full locking all lbas
	BITMAP				ddvol_bitmap;	//Bitmap which holds mapping
	U32 				ddvol_longs;	//No. of longs for lba
	U64					ddvol_reads;	//Counters: No. of reads
	U64					ddvol_writes;
	U64					ddvol_errors;
	U64					ddvol_read_rate;	//Rate at which reads are completed
	U64					ddvol_write_rate;
	U64					ddvol_sleep;	//For throttling purpose
} DDVOL, *PDDVOL;





// Data Structure which has all config details

typedef struct config_details_s {
	PDM_TARGET			target;
	PVOID				tp_read;
	PVOID				tp_write;
	PVOID				tp_flush;
} CONFIG, *PCONFIG;


typedef struct size_buf_map_s {
	int size;
	char *data;
} BUF_MAP;


#endif // DATA_STRUCTURES_H

#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "types.h"
#include "macros.h"
#include "counters.h"
#include "dedupe_engine.h"
#include "data_structures.h"
#include "fec_engine.h"
#include "bio_engine.h"
#include "alignment_engine.h"
#include "infra_engine.h"
#include "thread_pool_engine.h"
#include "memory_pool_engine.h"
#include "target_engine.h"
#include "sysfs_engine.h"
#include "iorequest.h"
#include "bec_engine.h"
#include "volume_engine.h"
#include "lab_engine_binary_tree.h"
#include "lab_engine_heap.h"
#include "lab_engine_interval.h"
#include "lab_engine_locks.h"
#include "lab_engine_linked_list.h"
#include "lab_engine_bplus_tree.h"
#include "lab_engine_pipe.h"

// Flash Volume Manager Related Function Declarations
RINT	dz_create_dedupe_volume(struct dm_target *target, UINT	argc, CHAR ** argv);
RVOID	dz_destroy_dedupe_volume(PDEVICE_OBJECT obj);

RINT	dz_target_create_aisa(struct dm_target *target, UINT	argc, CHAR ** argv);
RVOID	dz_avm_aisa_destroy(PDEVICE_OBJECT obj);

RINT	dz_target_create(struct dm_target *target, UINT	argc, CHAR ** argv);
RVOID	dz_target_destroy(struct dm_target *target);
INT 	aisa_lfv_init(VOID);
INT 	aisa_vfv_init(VOID);
RINT 	dz_process_write_io(PBIO bio);
RINT 	dz_process_read_io(PBIO bio);

//Block Device Function Declarations
PBLOCK_DEVICE dz_bdev_open(char dev_path[]);
RVOID         dz_bdev_close(PBLOCK_DEVICE bdev);


// Sector related Function Declarations
#ifdef DZ_ARCH_X86
INLINE 	SECTOR dz_convert_lba_to_sector(LBA lba);
INLINE 	LBA dz_convert_sector_to_lba(SECTOR sector);
#endif
#ifdef DZ_ARCH_ARM64
SECTOR dz_convert_lba_to_sector(LBA lba);
LBA dz_convert_sector_to_lba(SECTOR sector);
#endif


//Print and other Utility Function Declarations
RVOID 	_print_bio(struct bio *bio);
RVOID 	print_bio(struct bio *bio);
RVOID 	print_biom(struct bio *bio, char *);
VOID  	print_cli_args(struct dm_target *target, UINT	argc, CHAR ** argv);
RVOID 	print_data_pages(PDATAPAGES_T data_pages);
RVOID 	print_avm_device(PAVM_DEVICE);
RVOID 	print_avm_volume(PDDVOL obj);
VOID 	print_bdev(PBLOCK_DEVICE bdev);
RVOID 	print_lba_block(PLBA_BLOCK lba_block);
RVOID 	print_metadata_block(PMDD_BLOCK pmdd);
RVOID   print_data(PVOID data);
RVOID 	print_bio_page(PBIO bio);
RVOID 	print_bio_page_lines(PBIO bio);
RVOID 	print_page(PPAGE page);
RINT	dz_get_device_size(CHAR *, ULONG *);

RINT 	dz_alloc_data_pages(PDATAPAGES_T data_pages, UINT pgcnt);
RVOID 	dz_free_data_pages(PDATAPAGES_T data_pages, BOOL);




//Object Related Function Declarations
RINT 		dz_object_init(VOID);
RVOID 		dz_q_object(OBJECT_LIST *ol, PDEVICE_OBJECT object);
PDEVICE_OBJECT dz_dq_object(OBJECT_LIST *ol, char *name);
PDEVICE_OBJECT dz_find_object(OBJECT_LIST *ol, char *name);
RVOID 		print_object_list(VOID);
POBJECT_LIST dz_get_object_list(VOID);

// Configfs
RVOID dz_configfs_init(VOID);
RVOID dz_configfs_exit(VOID);
CHAR *dz_get_io_data(INT size);

//RPVOID dz_create_ddvol_lba_table(SIZE table_size);
//RVOID dz_destroy_ddvol_lba_table(PVOID ptr, SIZE table_size );
RINT dz_demo_passthrough_io(PDM_TARGET target, struct bio *bio);
RINT dz_demo_passthrough_readonly(PDM_TARGET target, struct bio *bio);
RINT dz_demo_passthrough_writeonly(PDM_TARGET target, struct bio *bio);
#endif // PROTOTYPES_H

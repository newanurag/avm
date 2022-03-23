/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _BIO_ENGINE_H_
#define _BIO_ENGINE_H_

#include "types.h"
#include "macros.h"
#include "dedupe_engine.h"
#include "data_structures.h"
#include "fec_engine.h"
#include "prototypes.h"

#ifdef DZ_KERNEL_VERSION_4
RVOID 	dz_read_biodone_sync(PBIO bio);
RVOID 	dz_read_biodone(PBIO bio);
RVOID 	dz_write_biodone(PBIO bio);
RVOID 	dz_write_biodone_sync(PBIO bio);
RVOID 	dz_bec_read_biodone(PBIO bio);
#else
RVOID 	dz_read_biodone_sync(PBIO bio, int error);
RVOID 	dz_read_biodone(PBIO bio, int error);
RVOID 	dz_write_biodone(PBIO bio, INT error);
RVOID 	dz_write_biodone_sync(PBIO bio, INT error);
RVOID 	dz_bec_read_biodone(PBIO bio, INT error);
#endif

// Read Write Function Declarations
RINT  	dz_read_data_from_raw_disk(PBLOCK_DEVICE, SECTOR, PDATA_PAGES_T, UINT);
RINT  	dz_read_data_from_raw_disk_sync(PBLOCK_DEVICE, SECTOR, PDATA_PAGES_T, UINT);
PPAGE 	dz_read_page_from_raw_disk_sync(PBLOCK_DEVICE ,SECTOR , UINT);
PPAGE 	dz_read_page_sync(PBIO);
RINT  	dz_read_data_pages_from_raw_disk(PBLOCK_DEVICE, SECTOR, PDATA_PAGES_T, UINT);
RINT 	dz_read_split_bio_without_alignment_processing(PIOREQUEST piorequest);
RINT 	dz_read_data(PIOREQUEST iorequest, PDATA_PAGES data_pages);
RINT 	dz_read_data_from_block_device(PIOREQUEST , CHAR *, PDATA_PAGES );
RINT  	dz_generate_read_io(VOID);

RINT  	dz_write_data_to_raw_disk(PBLOCK_DEVICE , SECTOR , PVOID , UINT );
RINT 	dz_write_data_to_raw_disk_sync(PBLOCK_DEVICE , SECTOR , PVOID , UINT);
RINT 	dz_write_page_to_raw_disk_sync(PBLOCK_DEVICE , SECTOR , UINT, PPAGE);
RINT 	dz_write_data_with_double_page_to_raw_disk(PBLOCK_DEVICE , SECTOR , PVOID , UINT );
RINT 	dz_write_data_pages_to_raw_disk(PBLOCK_DEVICE , SECTOR , PDATA_PAGES_T, UINT);
RINT  	dz_write_data(PIOREQUEST );
RINT  	dz_write_data_to_block_device(PIOREQUEST, CHAR * );
RINT  	dz_generate_write_io(VOID);
RINT  	dz_generate_zero_io(VOID);
RINT  	dz_generate_write_verify_io(VOID);

RINT  	dz_read_modify_write(PBLOCK_DEVICE, SECTOR src, SECTOR dst, PVOID, UINT);

#endif //_BIO_ENGINE_H_

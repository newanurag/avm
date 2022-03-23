/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

//AISA Target

///////////////////////////////////////
//Enable / Disable Alignment Engine
///////////////////////////////////////
 
INT dz_switch_target_iopath = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_target_iopath);


/////////////////////////////////////////////
//Stats related Counters
/////////////////////////////////////////////
BASIC_COUNTERS  dz_basic_counters;
PBASIC_COUNTERS pdz_basic_counters;
EXPORT_SYMBOL(dz_basic_counters);
EXPORT_SYMBOL(pdz_basic_counters);

AVM_COUNTERS    dz_avm_counters;
PAVM_COUNTERS   pdz_avm_counters;
EXPORT_SYMBOL(dz_avm_counters);
EXPORT_SYMBOL(pdz_avm_counters);

/////////////////////////////////////////////
//Data Disk Name, MetaData Disk Name
/////////////////////////////////////////////
PBLOCK_DEVICE DATA_DISK_DEVICE 				= NULL;
EXPORT_SYMBOL(DATA_DISK_DEVICE);

NAME                DATA_DISK_DEVICE_NAME;
EXPORT_SYMBOL(DATA_DISK_DEVICE_NAME);

PBLOCK_DEVICE   METADATA_DISK_DEVICE        = NULL;
EXPORT_SYMBOL(METADATA_DISK_DEVICE);

NAME            METADATA_DISK_DEVICE_NAME;
EXPORT_SYMBOL(METADATA_DISK_DEVICE_NAME);

PBLOCK_DEVICE   TARGET_DEVICE               = NULL;
EXPORT_SYMBOL(TARGET_DEVICE);

NAME            TARGET_DEVICE_NAME;
EXPORT_SYMBOL(TARGET_DEVICE_NAME);

SIZE            TARGET_DEVICE_SIZE          = 0; // In terms of 4K blocks
EXPORT_SYMBOL(TARGET_DEVICE_SIZE);

PSUPERBLOCK superblock						= NULL;
EXPORT_SYMBOL(superblock);

PAVM_DEVICE aisa_device_object 				= NULL;
EXPORT_SYMBOL(aisa_device_object);


/////////////////////////////////////////////
//Total READ Application IOs received.
/////////////////////////////////////////////
ATOMIC64 tot_application_iocount_reads;
EXPORT_SYMBOL(tot_application_iocount_reads);

ATOMIC64 tot_application_iocount_reads_single_page;
EXPORT_SYMBOL(tot_application_iocount_reads_single_page);

ATOMIC64 tot_application_iocount_reads_partial_page;
EXPORT_SYMBOL(tot_application_iocount_reads_partial_page);

ATOMIC64 tot_application_iocount_reads_multi_page;
EXPORT_SYMBOL(tot_application_iocount_reads_multi_page);

/////////////////////////////////////////////
//Total WRITE Application IOs received.
/////////////////////////////////////////////
ATOMIC64 tot_application_iocount_writes;
EXPORT_SYMBOL(tot_application_iocount_writes);

ATOMIC64 tot_application_iocount_writes_single_page;
EXPORT_SYMBOL(tot_application_iocount_writes_single_page);

ATOMIC64 tot_application_iocount_writes_partial_page;
EXPORT_SYMBOL(tot_application_iocount_writes_partial_page);

ATOMIC64 tot_application_iocount_writes_multi_page;
EXPORT_SYMBOL(tot_application_iocount_writes_multi_page);

//Total Application IOs (including READs and WRITEs) received.
ATOMIC64 tot_application_iocount;
EXPORT_SYMBOL(tot_application_iocount);

/////////////////////////////////////////////
//Storage Space holders
/////////////////////////////////////////////
U64             tot_free_space;
EXPORT_SYMBOL(tot_free_space);

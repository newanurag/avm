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

extern USHORT 	num_bec_threads;
extern ULONG    bec_size;
extern INT		bec_cutoff_percent;

//extern ATOMIC64 dz_iorequest_alloc_cnt;
//extern ATOMIC64 dz_iorequest_free_cnt;
extern BASIC_COUNTERS  dz_basic_counters;
extern PBASIC_COUNTERS pdz_basic_counters;

//extern ATOMIC64 dz_page_alloc_cnt;
//extern ATOMIC64 dz_page_free_cnt;

extern SIZE 			TARGET_DEVICE_SIZE;

extern NAME             DATA_DISK_DEVICE_NAME;

extern ATOMIC64			bec_pba_page_count;
extern ATOMIC64			bec_cache_hit;
extern ATOMIC64			bec_cache_miss;
extern ATOMIC64			bec_cache_delete;
extern ULONG			bec_size;
extern INT				bec_cutoff_nodes;
extern INT				bec_cutoff_percent;
extern INT				dz_bec_node_mempool_count;

extern PSUPERBLOCK 	superblock;

extern CHAR avm_sysfs_entry_get_bec_counters[DZ_SYSFS_PAGE_SIZE];
INT avm_get_bec_counters(CHAR *buf)
{
	INT count = 0;
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_bec_counters;
	memset(avm_sysfs_entry_get_bec_counters, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;
	ptr += sprintf(ptr, "=================================\n");
	ptr += sprintf(ptr, "BEC ENGINE COUNTERS\n");
	ptr += sprintf(ptr, "=================================\n");
	if (!superblock) {
		ptr += sprintf(ptr, "Superblock is NULL");
		return 0;
	}

	if (dz_switch_bec_engine == DZ_DISABLED) {
		ptr += sprintf(ptr, "BEC ENGINE DISABLED");
		return 0;
	}

	//Note: Value is NOT lock safe
	count = sprintf(ptr,
	"BEC bec_pba_page_count..........= %lli\n"
	"BEC bec_cache_hit...............= %lli\n"
	"BEC bec_cache_miss..............= %lli\n"
	"BEC bec_cache_delete............= %lli\n"
	"BEC bec_size....................= %lli\n"
	"BEC bec_cutoff_nodes............= %lli\n"
	"BEC bec_cutoff_percent..........= %lli\n"
	"BEC bec_node_mempool_count......= %lli\n",
	(U64)READ_COUNTER(bec_pba_page_count),
	(U64)ATOMIC64_READ(bec_cache_hit),
	(U64)ATOMIC64_READ(bec_cache_miss),
	(U64)ATOMIC64_READ(bec_cache_delete),
	(U64)bec_size,
	(U64)bec_cutoff_nodes,
	(U64)bec_cutoff_percent,
	(U64)dz_bec_node_mempool_count);

	//LOGR("<------------ BEC Show ----------->\n");
	//LOGR("%s",buf);

	LOGD("Buffer Bytes used           = %d\n", count);
	//return count;
	return 0;
}


extern U64 avm_sysfs_entry_get_set_switch_bec;
INT avm_get_switch_bec(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_switch_bec, dz_switch_bec_engine);
}
INT avm_set_switch_bec(U64 buf)
{
	DZ_SYS_SET_SWITCH(dz_switch_bec_engine, buf);
}

extern U64 avm_sysfs_entry_get_set_tunable_bec_size;
INT avm_get_tunable_bec_size(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_tunable_bec_size, dz_switch_bec_engine);
}

INT avm_set_tunable_bec_size(U64 buf)
{
	DZ_SYS_SET_SWITCH(avm_sysfs_entry_get_set_tunable_bec_size, buf);
}

extern U64 avm_sysfs_entry_get_set_tunable_bec_threads;
INT avm_get_tunable_bec_threads(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_tunable_bec_threads, num_bec_threads);
}

INT avm_set_tunable_bec_threads(U64 buf)
{
	DZ_SYS_SET_SWITCH(num_bec_threads, buf);
}

extern U64 avm_sysfs_entry_get_set_tunable_bec_ep;
INT avm_get_tunable_bec_ep(CHAR *buf)
{
	DZ_SYS_GET_SWITCH(avm_sysfs_entry_get_set_tunable_bec_ep, bec_cutoff_percent);
}

INT avm_set_tunable_bec_ep(U64 buf)
{
	DZ_SYS_SET_SWITCH(bec_cutoff_percent, buf);
}

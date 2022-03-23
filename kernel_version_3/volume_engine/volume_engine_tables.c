/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern SIZE				dz_pba_page_mempool_count;
extern U64 				lba_table_size;
extern U64 				pba_table_size;
extern U64 				hash_table_size;
extern BITMAP 			gpba_bitmap;
extern BITMAP 			pba_bitmap;
extern BITMAP 			lba_bitmap;
extern PPBA_BLOCK		pba_table;
extern PHASH_BLOCK		hash_table;
extern PLBA_BLOCK		lba_table;

extern SPINLOCK 	lba_table_lock; 
extern SPINLOCK 	hash_table_lock; 
extern SPINLOCK 	pba_table_lock; 

RPVOID dz_volume_create_lba_table(SIZE table_size)
{

	#define	DZ_UNIT_SIZE		(sizeof(DDVOL_LBA_BLOCK))
	PVOID ptr 		= vmalloc_user(table_size * DZ_UNIT_SIZE);
	if (!ptr) {
		LOGE("Unable to allocate memory for DDVOL LBA Table\n");
		return NULL;
	} else {
		LOG("DDVOL LBA Table created of entries %zu, size %zu bytes\n",
		table_size, table_size * DZ_UNIT_SIZE);
	}
	memset(ptr, 0, table_size * DZ_UNIT_SIZE);
	log_memory_usage(table_size * DZ_UNIT_SIZE);
	
	return ptr;
	#undef DZ_UNIT_SIZE
}

RVOID dz_volume_destroy_lba_table(PDDVOL ddvol)
{
	#define	DZ_UNIT_SIZE		(sizeof(DDVOL_LBA_BLOCK))
	if (ddvol->ddvol_lba_table) {
		vfree(ddvol->ddvol_lba_table);
		ddvol->ddvol_lba_table = NULL;
		reduce_memory_usage (ddvol->ddvol_table_size * DZ_UNIT_SIZE);
		LOG("DDVOL LBA Table destroyed\n");
	}
	#undef DZ_UNIT_SIZE
}

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern U64  		metadata_lba_table_size;
extern PMDD_BLOCK  	metadata_lba_table;

extern U64 		metadata_lba_lock_table_size;
extern PMD_LOCK	metadata_lba_lock_table;	// Array of spinlocks for MetaData Lock

RPVOID dz_create_metadata_lock_table(SIZE table_size)
{
	INT i = 0;
	PVOID ptr = vmalloc_user(table_size * sizeof(MD_LOCK));
	metadata_lba_lock_table = NULL;
	if (!ptr) {
		LOGE("Unable to allocate memory for md_lock Table\n");
		return NULL;
	} else {
		LOG("md_lock Table created of entries %zu, size %zu bytes\n",
			table_size, table_size * sizeof(MD_LOCK));
	}

	log_memory_usage(table_size * sizeof(MD_LOCK));
	metadata_lba_lock_table = (PMD_LOCK) ptr;
	//Now initialize all the locks
	for (i=0; i < table_size; i++) {
		spin_lock_init(&metadata_lba_lock_table[i].md_lock);
	}
	return ptr;
}
EXPORT_SYMBOL(dz_create_metadata_lock_table);

RVOID dz_destroy_metadata_lock_table(VOID)
{
	if (metadata_lba_lock_table) {
		vfree(metadata_lba_lock_table);
		metadata_lba_lock_table = NULL;
		reduce_memory_usage(metadata_lba_lock_table_size * sizeof(MD_LOCK));
		LOG("MD Lock Table destroyed\n");
	}
}
EXPORT_SYMBOL(dz_destroy_metadata_lock_table);

RPVOID dz_create_metadata_table(SIZE table_size)
{

	PVOID ptr       = vmalloc_user(table_size * sizeof(MDD_BLOCK));
	if (!ptr) {
		LOGE("Unable to allocate memory for MetaData Table\n");
		return NULL;
	} else {
		LOG("MetaData Table created of entries %zu, size %zu bytes\n",
		table_size, table_size * sizeof(MDD_BLOCK));
    }   
	memset(ptr, 0, table_size * sizeof(MDD_BLOCK));
	log_memory_usage(table_size * sizeof(MDD_BLOCK));

	return ptr;
}
EXPORT_SYMBOL(dz_create_metadata_table);

RVOID dz_destroy_metadata_table(VOID)
{
	if (metadata_lba_table) {
		vfree(metadata_lba_table);
		metadata_lba_table = NULL;
		reduce_memory_usage(metadata_lba_table_size * sizeof(MDD_BLOCK));
		LOG("MetaData Block Table destroyed\n");
	}
}
EXPORT_SYMBOL(dz_destroy_metadata_table);

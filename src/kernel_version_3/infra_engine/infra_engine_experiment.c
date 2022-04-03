/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PBLOCK_DEVICE	DATA_DISK_DEVICE;
extern NAME				DATA_DISK_DEVICE_NAME;
extern OBJECT_LIST 	global_object_list;
extern U64 lba_table_size;
extern U64 pba_table_size;
extern U64 hash_table_size;
extern U64 sysfs_avm_free_page_count;

extern BUF_MAP buffer_map[];

#define TEST_PASSED   100
#define TEST_FAILED   -1
RVOID test_bitmap(VOID)
{
	unsigned long *bitmap;
	U32 number_of_longs;
	U64 pba_index;
	UINT count = 0;
	UINT	limit = 100;

	number_of_longs = BITS_TO_LONGS(limit);
	PRINT_UINT(number_of_longs);
	bitmap = kmalloc(number_of_longs * sizeof(long), GFP_KERNEL);
	memset(bitmap , 0, number_of_longs * sizeof(long));
	pba_index = find_first_zero_bit(bitmap, limit);
	PRINT_U64(pba_index);
	test_and_set_bit(pba_index, bitmap);

	LOG("LOOPING\n");

	for (count = 0; count < limit + 2; count ++) {
		PRINT_UINT(count);
		pba_index = find_first_zero_bit(bitmap, limit);
		PRINT_U64(pba_index);
		test_and_set_bit(pba_index, bitmap);
	}
	LOG("LOOPING DONE\n");

	LOG("Freeing 50th index\n");
	if (!test_and_clear_bit(50, bitmap)){
		LOGA("asked to put pba:%d which is not in use.", 50);
	}

	pba_index = find_first_zero_bit(bitmap, limit);
	PRINT_U64(pba_index);
	test_and_set_bit(pba_index, bitmap);

	pba_index = find_first_zero_bit(bitmap, limit);
	PRINT_U64(pba_index);
	test_and_set_bit(pba_index, bitmap);
}

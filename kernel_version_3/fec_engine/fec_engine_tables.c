/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "fec_engine.h"

extern ATOMIC64	fec_active_count_sequence;
extern ATOMIC64	fec_free_count_sequence;
BITMAP 			fec_buffer_bitmap;
U32 number_of_longs_for_fec_buffer;
// A map which has entries of all free pages required for FEC Write Buffer
RINT dz_bitmap_init(PBITMAP bitmap, U64 table_size)
{
	U32	num_longs;
	MEMSET(bitmap);
	atomic64_set(&bitmap->num_items, 0);
	spin_lock_init(&bitmap->bitmap_lock);

	num_longs = BITS_TO_LONGS(table_size);
	bitmap->bitmap = dz_kmalloc(num_longs * sizeof(long), GFP_KERNEL);
	if (!bitmap->bitmap) {
		LOGFECE("Unable to allocate memory for FEC Buffer bitmap\n");
		RETURNF;
	}
	memset(bitmap->bitmap , 0, num_longs * sizeof(long));
	bitmap->num_of_longs = num_longs;
	LOGR("Number of longs required for %lli is %u\n", table_size, num_longs);
	LOGFEC("FEC Buffer Bitmap Initialized\n");
	RETURNS;
}

//This table will be created from NVDIMM Memory.
//Currently we are creating it on RAM due to unavailability of
//NVDIMM cards
PVOID dz_create_fec_data_write_table(U64 table_size)
{
#define FEC_DATA_WRITE_TABLE_SIZE	(table_size * sizeof(FEC_WRITE))

	PVOID ptr = vmalloc_user(FEC_DATA_WRITE_TABLE_SIZE);
	if (!ptr) {
		LOGFECE("Unable to allocate memory for FEC Data table\n");
		return NULL;
	} else {
		LOGFEC("FEC Data Write Table created of size %zu bytes\n", (SIZE)FEC_DATA_WRITE_TABLE_SIZE);
	}
	log_memory_usage(FEC_DATA_WRITE_TABLE_SIZE);
	memset(ptr, 0, table_size * sizeof(FEC_WRITE));

	dz_bitmap_init(&fec_buffer_bitmap, table_size);

#undef FEC_DATA_WRITE_TABLE_SIZE
	return ptr;

}

RVOID dz_destroy_fec_data_write_table(PVOID ptr, U64 table_size)
{
#define FEC_DATA_WRITE_TABLE_SIZE	(table_size * sizeof(FEC_WRITE))
	if (ptr) {
		U32 num_longs = fec_buffer_bitmap.num_of_longs;
		dz_kfree(fec_buffer_bitmap.bitmap, (num_longs * sizeof(long)));
		vfree(ptr);
		ptr = NULL;
		reduce_memory_usage(FEC_DATA_WRITE_TABLE_SIZE);
		LOGFEC("FEC Data Write Table destroyed\n");
	}
#undef FEC_DATA_WRITE_TABLE_SIZE
}

S64 dz_fec_get_free_buffer_index(U64 table_size)
{
	U64 index;
	spin_lock(&fec_buffer_bitmap.bitmap_lock);
	index = find_first_zero_bit(fec_buffer_bitmap.bitmap, table_size);
	if (index >= table_size) {
		LOGFECE("All FEC Buffers are exausted\n");
		spin_unlock(&fec_buffer_bitmap.bitmap_lock);
		RETURNF;
	}
	test_and_set_bit(index, fec_buffer_bitmap.bitmap);
	ATOMIC64_INC(fec_active_count_sequence);
	spin_unlock(&fec_buffer_bitmap.bitmap_lock);
	//LOGFEC("index %lli acquired\n", index);

	return index;
}

RINT dz_fec_put_buffer_index(U64 index, U64 table_size)
{
	spin_lock(&fec_buffer_bitmap.bitmap_lock);
	if (index >= table_size) {
		LOGFECE("Invalid index received\n");
		BUG_ON(1);
	} else {
		if (!test_and_clear_bit(index, fec_buffer_bitmap.bitmap)){
			LOGFECE("test_and_clear_bit failed for index %lli\n", index);
		}
	}
	ATOMIC64_INC(fec_free_count_sequence);
	spin_unlock(&fec_buffer_bitmap.bitmap_lock);
	//LOGFEC("index %lli released\n", index);
	return index;
}

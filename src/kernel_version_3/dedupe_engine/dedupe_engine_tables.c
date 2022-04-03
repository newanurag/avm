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

extern SPINLOCK 	lba_table_lock; 
extern SPINLOCK 	hash_table_lock; 
extern SPINLOCK 	pba_table_lock; 

extern U32 number_of_longs_for_pba;
extern U32 number_of_longs_for_lba;

//No. of longs for pbas which are processed by Garbage Collector
extern U32 number_of_longs_for_gpba; 

U64 dz_disk_io_sector	= 0;

// A map which has entries of all free pages required for PBAs
RINT dz_pba_bitmap_init(VOID)
{
	MEMSET(&pba_bitmap);
	atomic64_set(&pba_bitmap.num_items, 0);
	spin_lock_init(&pba_bitmap.bitmap_lock);

	//Increment the table size by 1 because zeroth pba will not be used
	number_of_longs_for_pba = BITS_TO_LONGS(pba_table_size + 1);
	pba_bitmap.bitmap = dz_kmalloc(number_of_longs_for_pba * sizeof(long), GFP_KERNEL);
	if (!pba_bitmap.bitmap) {
		LOGDDEE("Unable to allocate memory for pba bitmap\n");
		RETURNF;
	}
	memset(pba_bitmap.bitmap , 0, number_of_longs_for_pba * sizeof(long));
	pba_bitmap.num_of_longs = number_of_longs_for_pba;
	//Setting zeroth bit set so that PBA 0 will not be used by anyone
	test_and_set_bit(0, pba_bitmap.bitmap);
	//test_and_set_bit(1, pba_bitmap.bitmap);
	//But now we have a dedicated metadata disk so there is no need
	LOGDDEF("PBA Bitmap: pba_table_size                   = %lli\n", pba_table_size);
	LOGDDEF("PBA Bitmap: number_of_longs_for_pba          = %u\n", number_of_longs_for_pba);
	LOGDDEF("PBA Bitmap: Bitmap Size                      = %lu\n", number_of_longs_for_pba * sizeof(long));

	RETURNS;

}

// A map which has entries of all garbage pbas
RINT dz_garbage_pba_bitmap_init(VOID)
{
	MEMSET(&gpba_bitmap);
	atomic64_set(&gpba_bitmap.num_items, 0);
	spin_lock_init(&gpba_bitmap.bitmap_lock);

	//Increment the table size by 1 because zeroth pba will not be used
	number_of_longs_for_gpba = BITS_TO_LONGS(pba_table_size + 1);
	gpba_bitmap.bitmap = dz_kmalloc(number_of_longs_for_gpba * sizeof(long), GFP_KERNEL);
	if (!gpba_bitmap.bitmap) {
		LOGDDEE("Unable to allocate memory for garbage pba bitmap\n");
		RETURNF;
	}
	memset(gpba_bitmap.bitmap , 0, number_of_longs_for_gpba * sizeof(long));
	gpba_bitmap.num_of_longs = number_of_longs_for_gpba;
	//Setting zeroth bit set so that PBA 0 will not be used by anyone
	test_and_set_bit(0, gpba_bitmap.bitmap);
	//test_and_set_bit(1, gpba_bitmap.bitmap);
	//But now we have a dedicated metadata disk so there is no need
	LOGDDEF("Garbage PBA Bitmap: pba_table_size           = %lli\n", pba_table_size);
	LOGDDEF("Garbage PBA Bitmap: number_of_longs_for_gpba = %u\n", number_of_longs_for_gpba);
	LOGDDEF("Garbage PBA Bitmap: Bitmap Size              = %lu\n", number_of_longs_for_gpba * sizeof(long));
	RETURNS;
}

RVOID dz_print_bitmap(BITMAP *bitmap)
{
	CHAR *buf = NULL;
	U64 *var = NULL;
	U64 i = 0;

	U32 number_of_longs = bitmap->num_of_longs;

	//For converting string to bitmap: use, bitmap_parse_user()

	buf = dz_kmalloc((number_of_longs * sizeof(long)) + 1, GFP_KERNEL); //Extra 1 for null char
	memset(buf , 0, (number_of_longs * sizeof(long)) + 1);
	//bitmap_scnprintf(buf, number_of_longs * sizeof(long), bitmap->bitmap, number_of_longs_for_lba * sizeof(long) * 8);
	var = (U64 *)bitmap->bitmap;
	for (i = 0; i < number_of_longs; i++) {
		if (*var != 0) {
			LOG("[%llu] Bitmap Buffer String = 0x%llx\n",i, *var++);
		}
	}

}

// A Queue/List which has entries of all free lbas 
RINT dz_lba_bitmap_init(VOID)
{
	U64 tmp_var;
	MEMSET(&lba_bitmap);
	atomic64_set(&lba_bitmap.num_items, 0);
	spin_lock_init(&lba_bitmap.bitmap_lock);

	number_of_longs_for_lba = BITS_TO_LONGS(lba_table_size);
	lba_bitmap.bitmap = dz_kmalloc(number_of_longs_for_lba * sizeof(long), GFP_KERNEL);
	if (!lba_bitmap.bitmap) {
		LOGDDEE("Unable to allocate memory for lba bitmap\n");
		RETURNF;
	}
	memset(lba_bitmap.bitmap , 0, number_of_longs_for_lba * sizeof(long));
	lba_bitmap.num_of_longs = number_of_longs_for_lba;
	LOGDDEF("LBA Bitmap: lba_table_size                   = %lli\n", lba_table_size);
	LOGDDEF("LBA Bitmap: number_of_longs_for_lba          = %u\n", number_of_longs_for_lba);
	LOGDDEF("LBA Bitmap: Bitmap Size                      = %lu\n", number_of_longs_for_lba * sizeof(long));

	//dz_print_bitmap(&lba_bitmap, number_of_longs_for_lba);
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();

	tmp_var = dz_get_free_lba_index();
	PRINT_U64(tmp_var);
	dz_print_bitmap(&lba_bitmap);

	tmp_var = dz_get_free_lba_index();
	PRINT_U64(tmp_var);
	dz_print_bitmap(&lba_bitmap);

	tmp_var = dz_get_free_lba_index();
	PRINT_U64(tmp_var);
	dz_print_bitmap(&lba_bitmap);

	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	PRINT_U64(tmp_var);
	dz_print_bitmap(&lba_bitmap);
	tmp_var = dz_get_free_lba_index();
	PRINT_U64(tmp_var);
	dz_print_bitmap(&lba_bitmap);
	tmp_var = dz_get_free_lba_index();
	PRINT_U64(tmp_var);
	dz_print_bitmap(&lba_bitmap);
	tmp_var = dz_get_free_lba_index();
	PRINT_U64(tmp_var);
	dz_print_bitmap(&lba_bitmap);

	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	tmp_var = dz_get_free_lba_index();
	PRINT_U64(tmp_var);
	dz_put_lba_index(0);
	dz_print_bitmap(&lba_bitmap);
	RETURNS;
}

U64 dz_get_free_pba_index(VOID)
{
	U64 pba_index;
	spin_lock(&pba_bitmap.bitmap_lock);
	pba_index = find_first_zero_bit(pba_bitmap.bitmap, pba_table_size);
	if (pba_index >= pba_table_size) {
		LOGDDEE("All PBAs are exausted\n");
		spin_unlock(&pba_bitmap.bitmap_lock);
		BUG_ON(1);
		RETURNF;
	}
	test_and_set_bit(pba_index, pba_bitmap.bitmap);
	spin_unlock(&pba_bitmap.bitmap_lock);
	LOGD("pba_index %lli acquired\n", pba_index);

	return pba_index;
}

RINT dz_put_pba_index(U64 pba_index)
{
	spin_lock(&pba_bitmap.bitmap_lock);
	if (pba_index >= pba_table_size) {
		LOGDDEE("Invalid pba_index received\n");
		BUG_ON(1);
	} else {
		if (!test_and_clear_bit(pba_index, pba_bitmap.bitmap)){
			LOGDDEE("test_and_clear_bit failed for pba_index %lli\n", pba_index);
		}
	}
	spin_unlock(&pba_bitmap.bitmap_lock);
	LOGD("pba_index %lli released\n", pba_index);
	return pba_index;
}

U64 dz_set_garbage_pba_index(U64 pba_index)
{
	spin_lock(&gpba_bitmap.bitmap_lock);
	if (pba_index >= pba_table_size) {
		LOGDDEE("Invalid pba_index received\n");
		BUG_ON(1);
	} else {
		test_and_set_bit(pba_index, gpba_bitmap.bitmap);
	}
	spin_unlock(&gpba_bitmap.bitmap_lock);
	LOGD("pba_index %lli set for garbage collection\n", pba_index);
	return pba_index;
}


S64 dz_get_free_lba_index(VOID)
{
	U64 lba_index;
	spin_lock(&lba_bitmap.bitmap_lock);
	lba_index = find_first_zero_bit(lba_bitmap.bitmap, lba_table_size);
	if (lba_index >= lba_table_size) {
		LOGDDEE("All LBAs are exausted\n");
		spin_unlock(&lba_bitmap.bitmap_lock);
		RETURNF;
	}
	test_and_set_bit(lba_index, lba_bitmap.bitmap);
	spin_unlock(&lba_bitmap.bitmap_lock);
	//LOG("lba_index %lli acquired\n", lba_index);

	return lba_index;
}
EXPORT_SYMBOL(dz_get_free_lba_index);

RINT dz_put_lba_index(U64 lba_index)
{
	spin_lock(&lba_bitmap.bitmap_lock);
	if (lba_index >= lba_table_size) {
		LOGDDEE("Invalid lba_index received\n");
		BUG_ON(1);
	} else {
		if (!test_and_clear_bit(lba_index, lba_bitmap.bitmap)){
			LOGDDEE("test_and_clear_bit failed for lba_index %lli\n", lba_index);
		}
	}
	spin_unlock(&lba_bitmap.bitmap_lock);
	//LOG("lba_index %lli released\n", lba_index);
	return lba_index;
}
EXPORT_SYMBOL(dz_put_lba_index);



RPVOID dz_create_hash_table(SIZE table_size)
{

	PVOID ptr = vmalloc_user(table_size * sizeof(HASH_BLOCK));
	if (!ptr) {
		LOGDDEE("Unable to allocate memory for Hash Table\n");
		return NULL;
	} else {
		LOGDDEF("HASH Table created of entries %zu, size %zu bytes\n",
		table_size, table_size * sizeof(HASH_BLOCK));
	}
	log_memory_usage(table_size * sizeof(HASH_BLOCK));
	memset(ptr, 0, table_size * sizeof(HASH_BLOCK));

	spin_lock_init(&hash_table_lock);

	return ptr;
}

RVOID dz_destroy_hash_table(PVOID ptr)
{
	if (ptr) {
		vfree(ptr);
		ptr = NULL;
		reduce_memory_usage(hash_table_size * sizeof(HASH_BLOCK));
		LOGDDEF("HASH Table destroyed\n");
	}
}

RPVOID dz_create_pba_table(SIZE table_size)
{
	PPBA_BLOCK ptr = (PPBA_BLOCK)vmalloc_user(table_size * sizeof(PBA_BLOCK));
	if (!ptr) {
		LOGDDEE("Unable to allocate memory for Pba Table\n");
		return NULL;
	} else {
		LOGDDEF("PBA Table created of entries %zu, size %zu bytes\n",
		table_size, table_size * sizeof(PBA_BLOCK));
	}
	memset(ptr, 0, table_size * sizeof(PBA_BLOCK));
	log_memory_usage(table_size * sizeof(PBA_BLOCK));
	
	spin_lock_init(&pba_table_lock);
	dz_pba_bitmap_init();
	dz_garbage_pba_bitmap_init();

	return ptr;
}

RVOID dz_destroy_pba_table(PPBA_BLOCK ptr)
{
	if (ptr) {
		dz_kfree(pba_bitmap.bitmap, (number_of_longs_for_pba * sizeof(long)));
		dz_kfree(gpba_bitmap.bitmap, (number_of_longs_for_gpba * sizeof(long)));
		vfree(ptr);
		ptr = NULL;
		reduce_memory_usage(pba_table_size * sizeof(PBA_BLOCK));
		LOGDDEF("PBA Table destroyed\n");
	}
}

RPVOID dz_create_lba_table(SIZE table_size)
{

	INT i = 0;
	PVOID ptr 		= vmalloc_user(table_size * sizeof(LBA_BLOCK));
	PLBA_BLOCK lb 	= NULL;
	if (!ptr) {
		LOGDDEE("Unable to allocate memory for LBA Table\n");
		return NULL;
	} else {
		LOGDDEF("LBA Table created of entries %zu, size %zu bytes\n",
		table_size, table_size * sizeof(LBA_BLOCK));
	}
	memset(ptr, 0, table_size * sizeof(LBA_BLOCK));
	log_memory_usage(table_size * sizeof(LBA_BLOCK));
	

	lb = (PLBA_BLOCK)ptr;
	spin_lock_init(&lba_table_lock);
	for (i=0; i < table_size; i++) {
		INIT_LIST_HEAD(&lb[i].ioq_head);
	}

	dz_lba_bitmap_init();
	return ptr;
}

RVOID dz_destroy_lba_table(PVOID ptr)
{
	if (ptr) {
		dz_kfree(lba_bitmap.bitmap, (number_of_longs_for_lba * sizeof(long)));
		vfree(ptr);
		ptr = NULL;
		reduce_memory_usage(lba_table_size * sizeof(LBA_BLOCK));
		LOGDDEF("LBA Table destroyed\n");
	}
}

VOID dz_update_pba_entry(PPBA_BLOCK pba_entry, U64 tag, U32 size, U64 ref_cnt, U64 next)
{
	pba_entry->hash_collision_tag			= tag;
	pba_entry->size 						= size;
	pba_entry->ref_cnt 						= ref_cnt;
	pba_entry->next_entry_in_collision_list = next;
	LOGD("pba entry updated for tag %lli\n", (U64) tag);
}

VOID dz_update_lba_entry(PLBA_BLOCK lba_entry, U32 hash_index, U64 tag)
{
	lba_entry->hash_index			= hash_index;
	lba_entry->hash_collision_tag	= tag;
	LOGD("lba entry updated\n");
}

VOID dz_update_hash_entry(PHASH_BLOCK hash_entry, U64 pba_index)
{
	hash_entry->pba_index = pba_index;
	LOGD("hash entry updated for pba_index %lli\n", (U64)pba_index);
}

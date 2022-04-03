/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"
typedef struct dz_block_device_s {
	struct block_device *bdev_raw;
} dz_block_device_t;


static dz_block_device_t dz_block_device;
extern OBJECT_LIST 	global_object_list;
extern U64 lba_table_size;
extern U64 pba_table_size;
extern U64 hash_table_size;
extern U64 sysfs_avm_free_page_count;
SPINLOCK dz_print_lock;

RVOID _print_bio(struct bio *bio)
{
	extern const char *bdevname(struct block_device *bdev, char *buffer);
	char b[BDEVNAME_SIZE];
	PVOID mem = NULL;
	if (!bio) {
		LOG("Bio is Null\n");
		return;
	}
	/* TODO:
	if (!bio->bi_bdev) {
		LOG("bio->bi_bdev is Null\n");
		return;
	}
	*/
	LOGR("| bio                    = %p\n", bio);
	LOGR("| bio direction          = %s\n", 
			bio_data_dir(bio) == WRITE ? "WRITE" : "READ");
	LOGR("| bio->device_name       = %s\n", bdevname(DZ_BIO_GET_DEV(bio), b));
	LOGR("| bio->device_name       = %p (Address)\n", DZ_BIO_GET_DEV(bio));
#ifdef DZ_KERNEL_VERSION_4
	LOGR("| bio->bi_flags          = %hu, 0x%x\n", bio->bi_flags, bio->bi_flags);
#else
	LOGR("| bio->bi_flags          = %lu, 0x%lx\n", bio->bi_flags, bio->bi_flags);

	if (bio_flagged(bio, BIO_OWNS_VEC)) { //TODO:
		LOGR("| bio has BIO_OWNS_VEC flag set\n");
	}
	if (bio_flagged(bio, BIO_UPTODATE)) {
		LOGR("| bio has BIO_UPTODATE flag set\n");
	}
#endif

#ifdef DZ_KERNEL_VERSION_4
	LOGR("| bio->bi_opf            = %u, 0x%x\n", bio->bi_opf, bio->bi_opf);
	LOGR("| bio->bi_bdev           = 0x%p\n", DZ_BIO_GET_DEV(bio));
#else
	LOGR("| bio->bi_rw             = %lu, 0x%lx\n", bio->bi_rw, bio->bi_rw);
	LOGR("| bio->bi_bdev           = 0x%p\n", bio->bi_bdev);
#endif
	LOGR("| bio->bi_vcnt           = %d\n", bio->bi_vcnt);
	LOGR("| bio->bi_cnt            = %d\n", atomic_read(& DZ_BIO_GET_CNT(bio)));
	LOGR("| bio->bi_max_vecs       = %d\n", bio->bi_max_vecs);
	LOGR("| bio->bi_sector         = %llu, %lld", (ULONGLONG)DZ_BIO_GET_SECTOR(bio), (S64)DZ_BIO_GET_SECTOR(bio)); 
	LOGR("| bio->bi_size           = %u", DZ_BIO_GET_SIZE(bio)); 
	LOGR("| bio->bi_idx            = %u", DZ_BIO_GET_IDX(bio)); 
	LOGR("| bio->bi_next           = %p", DZ_BIO_GET_NEXT(bio)); 
	LOGR("| bio->bi_private        = %p", bio->bi_private ); 
	LOGR("| bio->bi_pool           = %p", bio->bi_pool ); 
	LOGR("| bio->bi_phys_segments  = %u\n", bio->bi_phys_segments);
	LOGR("| bio->bi_seg_front_size = %u\n", bio->bi_seg_front_size);
	LOGR("| bio->bi_seg_back_size  = %u\n", bio->bi_seg_back_size);
	LOGR("| bio->bi_end_io         = %p\n", bio->bi_end_io);
	LOGR("| bio->bi_io_vec         = %p\n", bio->bi_io_vec);
	{
#ifdef DZ_KERNEL_VERSION_4
		struct bio_vec bvec;
		struct bvec_iter iter;
		LOGR("|                 Printing biovec Information          |\n");
		bio_for_each_segment(bvec, bio, iter) {
			LOGR("|	bio->bi_io_vec.bv_offset = %d\n", bvec.bv_offset);
			LOGR("|	bio->bi_io_vec.bv_len    = %d\n", bvec.bv_len);
			LOGR("|	bio->bi_io_vec.bv_page   = %p\n", bvec.bv_page);
			mem = kmap(bvec.bv_page);
			LOGR("|	bio->bi_io_vec->bv_page   = %p (kmap)\n", mem);
			kunmap(bvec.bv_page);
			LOGR("\n");
		}
#else
		int i = 0;
		struct bio_vec *bvec = NULL;
		LOGR("|                 Printing biovec Information          |\n");
		bio_for_each_segment(bvec, bio, i) {
			LOGR("|	bio->bi_io_vec->bv_offset = %d\n", bvec->bv_offset);
			LOGR("|	bio->bi_io_vec->bv_len    = %d\n", bvec->bv_len);
			LOGR("|	bio->bi_io_vec->bv_page   = %p\n", bvec->bv_page);
			mem = kmap(bvec->bv_page);
			LOGR("|	bio->bi_io_vec->bv_page   = %p (kmap)\n", mem);
			kunmap(bvec->bv_page);
			LOGR("\n");
		}
#endif
	}
//	LOGR("|	Alternatively bio->bi_io_vec->bv_offset = %d\n", bio_iovec(bio)->bv_offset);
//	LOGR("|	Alternatively bio->bi_io_vec->bv_len    = %d\n", bio_iovec(bio)->bv_len);
//	mem = kmap(bio_iovec(bio)->bv_page);
//	LOGR("|	Alternatively bio->bi_io_vec->bv_page   = %p\n", mem);
//	kunmap(bio_iovec(bio)->bv_page);
}

RVOID print_bio(struct bio *bio)
{
	LOGR(" ______________________________________________________ \n");
	LOGR("|                                                      |\n");
	LOGR("|                 Printing bio Information             |\n");
	LOGR("|______________________________________________________|\n");

	_print_bio(bio);
	LOGR(" ______________________________________________________ \n");
	LOGR("|______________________________________________________|\n");
}
EXPORT_SYMBOL(print_bio);

// Print bios with user supplied marker string
RVOID print_biom(struct bio *bio, char *mark)
{
	LOGR(" ______________________________________________________ \n");
	LOGR("|                                                      |\n");
	LOGR("|                 Printing bio Information             |\n");
	LOGR("|                 %s                                    \n", mark);
	LOGR("|______________________________________________________|\n");

	_print_bio(bio);
	LOGR(" ______________________________________________________ \n");
	LOGR("|______________________________________________________|\n");
}
EXPORT_SYMBOL(print_biom);

PBLOCK_DEVICE dz_bdev_open(char dev_path[])
{
    /* Open underlying device */
	PBLOCK_DEVICE bdev_raw = lookup_bdev(dev_path);
	PBLOCK_DEVICE bdev_raw1 = NULL;
	INT ret = 0;
	struct gendisk *disk = NULL;


	if (IS_ERR(bdev_raw)) {   
		LOGE("in opening raw device %s\n", dev_path);
		return NULL;
	}   
	if (!bdget(bdev_raw->bd_dev)) {   
		LOGE("in bdget()\n");
		return NULL;
	}   
	ret = (blkdev_get(bdev_raw, DZ_BLOCK_DEVICE_MODE, &dz_block_device));
	if (ret) {   
		LOGE("in blkdev_get(), ret=%d\n", ret);
		bdput(bdev_raw);
		return NULL;
	}   
	LOGINF("Block device %s opened successfully. Address is %p\n",dev_path,  bdev_raw);
	disk = bdev_raw->bd_disk;
	bdev_raw1 = bdget_disk(disk, 10);
	LOGINF("Disk Name = %s, New Address %p\n", disk->disk_name, bdev_raw1);
	return bdev_raw;
}
EXPORT_SYMBOL(dz_bdev_open);

RVOID dz_bdev_close(PBLOCK_DEVICE bdev)
{
	blkdev_put(bdev, DZ_BLOCK_DEVICE_MODE);
	bdput(bdev);
}
EXPORT_SYMBOL(dz_bdev_close);

INLINE LBA dz_convert_sector_to_lba(SECTOR sector)
{
	return ((sector * SECTOR_SIZE) / LBA_BLOCK_SIZE);
}
EXPORT_SYMBOL(dz_convert_sector_to_lba);

INLINE SECTOR dz_convert_lba_to_sector(LBA lba)
{
	if (0 == lba) {
		return 0;
	} else {
		return ((lba * LBA_BLOCK_SIZE)/ SECTOR_SIZE);
	}
}
EXPORT_SYMBOL(dz_convert_lba_to_sector);

RINT dz_bio_bvec_write_page_free(PBIO bio)
{
#ifdef DZ_KERNEL_VERSION_4
	struct bio_vec bvec;
	struct bvec_iter iter;
	bio_for_each_segment(bvec, bio, iter) {
		LOGINF("Freeing bvec.bv_page   = %p\n", bvec.bv_page);
		dz_write_page_free(bvec.bv_page);
		bvec.bv_page = NULL;
	}
#else
	struct bio_vec *bvec = NULL;
	int i = 0;
	bio_for_each_segment(bvec, bio, i) {
		LOGINF("Freeing bvec->bv_page   = %p\n", bvec->bv_page);
		dz_write_page_free(bvec->bv_page);
		bvec->bv_page = NULL;
	}
#endif
	RETURNS;
}

RINT dz_bio_bvec_read_page_free(PBIO bio)
{
#ifdef DZ_KERNEL_VERSION_4
	struct bio_vec bvec;
	struct bvec_iter iter;
	bio_for_each_segment(bvec, bio, iter) {
		LOGINF("Freeing bvec.bv_page   = %p\n", bvec.bv_page);
		dz_read_page_free(bvec.bv_page);
		bvec.bv_page = NULL;
	}
#else
	struct bio_vec *bvec = NULL;
	int i = 0;
	bio_for_each_segment(bvec, bio, i) {
		dz_read_page_free(bvec->bv_page);
		bvec->bv_page = NULL;
	}
#endif
	RETURNS;
}

INT dz_bio_vector_page_print(PBIO bio)
{
#ifdef DZ_KERNEL_VERSION_4
	struct bio_vec bvec;
	struct bvec_iter iter;
	PVOID data = NULL;
	bio_for_each_segment(bvec, bio, iter) {
		LOGINF("bvec.bv_page   = %p\n", bvec.bv_page);
		data = kmap(bvec.bv_page);
		//Data Print
		kunmap(bvec.bv_page);
	}
#else
	struct bio_vec *bvec = NULL;
	int i = 0;
	PVOID data = NULL;
	bio_for_each_segment(bvec, bio, i) {
		LOGINF("bvec->bv_page   = %p\n", bvec->bv_page);
		data = kmap(bvec->bv_page);
		//Data Print
		kunmap(bvec->bv_page);
	}
#endif
	RETURNS;
}

//Note : Allocation is done from read page pool
RINT dz_alloc_data_pages(PDATAPAGES_T data_pages, UINT pgcnt)
{
	PPAGE      page	= NULL;
	INT			  i = 0;

	data_pages->pgcnt = pgcnt;
	data_pages->pagevec = kmalloc(pgcnt * sizeof (BIOVEC), GFP_KERNEL | __GFP_ZERO);
	memset(data_pages->pagevec, 0, pgcnt * sizeof(BIOVEC)); 
	if (!data_pages->pagevec) {
		LOGE("Unable to create Data Pages Vector \n");
		goto exit_failure;
	}

	for (i=0; i < pgcnt; i++) {
		struct bio_vec *bvec = &(data_pages->pagevec[i]);
		page =  dz_read_page_alloc();
		if (!page)  {
			LOGE("Unable to get free read page\n");
			goto exit_failure;
		}
		MEMSET_PAGE(page);
		LOGINF("Page Allocated = %p\n", page);
		bvec->bv_page 	= page;
		bvec->bv_offset = 0;
		bvec->bv_len	= PAGE_SIZE; // Note init to PAGE_SIZE
	}

	RETURNS;

exit_failure:
	dz_free_data_pages(data_pages, false);
	RETURNF;
}

//Note : These pages are always freed to Read Page Pool
RVOID dz_free_data_pages(PDATAPAGES_T data_pages, BOOL self)
{	
	INT i 			= 0;

	if (!data_pages) {
		LOGINF("Data Pages are already NULL\n");
		return;
	}

	// Freeup individual pages
	
	for (i=0; i < data_pages->pgcnt; i++){
		struct bio_vec *bvec = &(data_pages->pagevec[i]);
		if (!bvec) {
			LOGINF(" bvec is NULL\n");
		} else {
			if(bvec->bv_page) {
				LOGINF("Page Freed     = %p\n", bvec->bv_page);
				dz_read_page_free(bvec->bv_page);
				bvec->bv_page = NULL;
			}
		}
	}

	//Finally freeup the pagevec vector
	if (data_pages->pagevec) {
		kfree(data_pages->pagevec);
	}
	if (self) {
		kfree(data_pages);
	}
	return;

}
EXPORT_SYMBOL(dz_free_data_pages);

RVOID print_page(PPAGE page)
{
	INT j				= 0;
	PVOID pagedata 		= NULL;
	CHAR	buf[513] = {'\0'};

	LOGINF(" ______________________________________________________ \n");
	LOGINF("|                                                      |\n");
	LOGINF("|          Printing     Page                          |\n");
	LOGINF("|______________________________________________________|\n");
	
		pagedata = kmap(page);
		for (j = 0; j < 8; j++) {
			printk(KERN_ERR"\n");
			memcpy(buf, pagedata, 512);
			printk(KERN_ERR"%s", buf);
			pagedata += 512;
			memset(buf, 0, 513);
		}
		kunmap(page);

	LOGINF(" ______________________________________________________ \n");
	LOGINF("|______________________________________________________|\n");
	return;
}

RVOID print_bio_page(PBIO bio)
{
	INT j				= 0;
	
#ifdef DZ_KERNEL_VERSION_4
	struct bio_vec bvec;
	struct bvec_iter iter;
	PVOID pagedata 		= NULL;
	CHAR	buf[513] = {'\0'};
	LOGINF(" ______________________________________________________ \n");
	LOGINF("|                                                      |\n");
	LOGINF("|          Printing Bio Page                          |\n");
	LOGINF("|______________________________________________________|\n");

	bio_for_each_segment(bvec, bio, iter) {
		pagedata = kmap(bvec.bv_page);
		for (j = 0; j < 8; j++) {
			printk(KERN_ERR"\n");
			memcpy(buf, pagedata, 512);
			printk(KERN_ERR"%s", buf);
			pagedata += 512;
			memset(buf, 0, 513);
		}
		kunmap(bvec.bv_page);
	}
#else
	INT	i				= 0;
	PVOID pagedata 		= NULL;
	CHAR	buf[513] = {'\0'};
	struct bio_vec *bvec = NULL;

	LOGINF(" ______________________________________________________ \n");
	LOGINF("|                                                      |\n");
	LOGINF("|          Printing Bio Page                          |\n");
	LOGINF("|______________________________________________________|\n");

	bio_for_each_segment(bvec, (bio), i) {
		pagedata = kmap(bvec->bv_page);
		for (j = 0; j < 8; j++) {
			printk(KERN_ERR"\n");
			memcpy(buf, pagedata, 512);
			printk(KERN_ERR"%s", buf);
			pagedata += 512;
			memset(buf, 0, 513);
		}
		kunmap(bvec->bv_page);
	}
#endif

	LOGINF(" ______________________________________________________ \n");
	LOGINF("|______________________________________________________|\n");
	return;
}

RVOID print_bio_page_lines(PBIO bio)
{
	INT j				= 0;
	//INT k				= 0;
	PVOID pagedata 		= NULL;
	CHAR	buf[65] = {'\0'};
#ifdef DZ_KERNEL_VERSION_4
	struct bio_vec bvec;
	struct bvec_iter iter;
#else
	INT	i				= 0;
	struct bio_vec *bvec = NULL;
#endif


	static int print_lock_idx = 0;

	if (print_lock_idx == 0) {
		spin_lock_init(&dz_print_lock);
		print_lock_idx = 1;
	}

	//spin_lock(&dz_print_lock);


	LOGINF(" ______________________________________________________ \n");
	LOGINF("|                                                      |\n");
	LOGINF("|          Printing Bio Page Line                      |\n");
	LOGINF("|______________________________________________________|\n");
	memset(buf, 0, 65);
#ifdef DZ_KERNEL_VERSION_4
	bio_for_each_segment(bvec, bio, iter) {
		pagedata = kmap(bvec.bv_page);
		pagedata += bvec.bv_offset;			\
		for (j = 0; j < 64; j++) {
			memcpy(buf, pagedata, 64);
			printk(KERN_ERR"<<[%s]>>", buf);

			/*
			for (k = 0; k < 64; k++) {
				printk(KERN_ERR"%c", buf[k]);
			}
			*/
			pagedata += 64;
			memset(buf, 0, 65);
		}
		kunmap(bvec.bv_page);
	}
#else
	
	bio_for_each_segment(bvec, (bio), i) {
		pagedata = kmap(bvec->bv_page);
		pagedata += bvec->bv_offset;			\
		for (j = 0; j < 64; j++) {
			memcpy(buf, pagedata, 64);
			printk(KERN_ERR"<<[%s]>>", buf);

			/*
			for (k = 0; k < 64; k++) {
				printk(KERN_ERR"%c", buf[k]);
			}
			*/
			pagedata += 64;
			memset(buf, 0, 65);
		}
		kunmap(bvec->bv_page);
	}
#endif

	LOGINF(" ______________________________________________________ \n");
	LOGINF("|______________________________________________________|\n");
	//spin_unlock(&dz_print_lock);
	return;
}

RVOID print_data(PVOID data)
{
	INT j				= 0;
	PVOID pagedata 		= data;
	CHAR	buf[513] = {'\0'};

	LOGINF(" ______________________________________________________ \n");
	LOGINF("|                                                      |\n");
	LOGINF("|          Printing Data Pages Information             |\n");
	LOGINF("|______________________________________________________|\n");
	
	for (j = 0; j < 8; j++) {
		printk(KERN_ERR"\n");
		memcpy(buf, pagedata, 512);
		printk(KERN_ERR"%s", buf);
		pagedata += 512;
		memset(buf, 0, 513);
	}

	LOGINF(" ______________________________________________________ \n");
	LOGINF("|______________________________________________________|\n");
	return;
}
EXPORT_SYMBOL(print_data);

RVOID print_data_pages(PDATAPAGES_T data_pages)
{
	INT i 				= 0;
	INT j				= 0;
	PVOID pagedata 		= NULL;
	UINT	pgcnt		= 0;
	PHASH	hash		= NULL;
	U32		*hash_array	= NULL;
	CHAR	buf[513] = {'\0'};

	if (!data_pages) {
		LOGINF("Data Pages are NULL\n");
		return;
	}
	pgcnt = data_pages->pgcnt;
	LOGINF(" ______________________________________________________ \n");
	LOGINF("|                                                      |\n");
	LOGINF("|          Printing Data Pages Information             |\n");
	LOGINF("|______________________________________________________|\n");
	LOGINF("| data_pages->pgcnt          = %d\n", pgcnt);
	
	for (i=0; i < pgcnt; i++){
		struct bio_vec *bvec = &(data_pages->pagevec[i]);
		if (!bvec) {
			LOGINF("| pagevec[%d/%d]->bv_offset = %d\n", i, pgcnt-1, 0);
			LOGINF("| pagevec[%d/%d]->bv_len    = %d\n", i, pgcnt-1, 0);
			LOGINF("| pagevec[%d/%d]->bv_page   = %p\n", i, pgcnt-1, NULL);
		} else {
			LOGINF("| pagevec[%d/%d]->bv_offset = %d\n", i, pgcnt-1, bvec->bv_offset);
			LOGINF("| pagevec[%d/%d]->bv_len    = %d\n", i, pgcnt-1, bvec->bv_len);
			LOGINF("| pagevec[%d/%d]->bv_page   = %p\n", i, pgcnt-1, bvec->bv_page);
			pagedata = kmap(bvec->bv_page);
			pagedata += bvec->bv_offset;
			printk(KERN_ERR"| ");
			//TODO: Move dz_hash_alloc function to infra engine
			//hash = dz_hash_alloc();
			if (!hash) {
				LOGE("Unable to allocate hash\n");
			} else {
				hash_array = hash->hash;
				//TODO:Move this function to infra engine or define a new one
				//dz_generate_hash(pagedata, bvec->bv_len, hash_array);
				PRINT_HASH(hash);
			}

			if (bvec->bv_len < PAGE_SIZE) {
				INT k = bvec->bv_len / 512;
				for(j = 0; j < k; j++) {
					printk(KERN_ERR"\n");
					memcpy(buf, pagedata, 512);
					printk(KERN_ERR"%s", buf);
					pagedata += 512;
					memset(buf, 0, 513);
				}
			} else {
				//printk is unable to write full 4K / PAGE_SIZE data on console. so writing into 512 bytes chunks
				for (j = 0; j < 8; j++) {
					printk(KERN_ERR"\n");
					memcpy(buf, pagedata, 512);
					printk(KERN_ERR"%s", buf);
					pagedata += 512;
					memset(buf, 0, 513);
				}
				/*
				for (j = 0; j < 4; j++) {
					memcpy(buf, pagedata, 1024);
					printk(KERN_ERR"%s", buf);
					pagedata += 1024;
					memset(buf, 0, 1024+1);
				}
				*/
			}
			kunmap(bvec->bv_page);
		}
	}

	LOGINF(" ______________________________________________________ \n");
	LOGINF("|______________________________________________________|\n");
	return;
}
EXPORT_SYMBOL(print_data_pages);

RVOID dz_panic(char *str)
{
	struct bio *bio = NULL;
	LOGINF("Forcing System Panic in 3 Seconds\n");
	SLEEP(3);
	LOGINF("%lu\n", bio->bi_flags); // Dereferencing Null pointer
}
EXPORT_SYMBOL(dz_panic);

RVOID print_avm_device(PAVM_DEVICE obj)
{

	PRINT_STRING(obj->obj.obj_magicno);
	PRINT_NAME(obj->obj.obj_name);
	PRINT_UUID(obj->obj.obj_uuid);
	PRINT_COUNT(obj->obj.obj_start_pba);
	PRINT_COUNT(obj->obj.obj_end_pba);
	PRINT_FLAGS(obj->obj.obj_flags);
	PRINT_NAME(obj->obj.obj_dm_dev_name);
	PRINT_UUID(obj->obj.obj_dm_dev_uuid);
	PRINT_COUNT(obj->obj.obj_child_cnt);

	PRINT_COUNT(obj->free_cnt);
	PRINT_COUNT(obj->used_cnt);
	PRINT_COUNT(obj->pvt_cnt);
	PRINT_COUNT(obj->tot_cnt);
	PRINT_COUNT(obj->tobe_erased_cnt);
	PRINT_COUNT(obj->tot_erased_cnt);

}

RVOID print_avm_volume(PDDVOL ddvol)
{
	PVOID lba_table = ddvol->ddvol_lba_table;
	PRINT_STRING	(ddvol->ddvol_obj.obj_magicno);
	PRINT_NAME		(ddvol->ddvol_obj.obj_name);
	PRINT_UUID		(ddvol->ddvol_obj.obj_uuid);
	PRINT_LBA		(ddvol->ddvol_obj.obj_start_lba);
	PRINT_LBA		(ddvol->ddvol_obj.obj_end_lba);
	PRINT_FLAGS		(ddvol->ddvol_obj.obj_flags);
	PRINT_NAME		(ddvol->ddvol_obj.obj_dm_dev_name);
	PRINT_UUID		(ddvol->ddvol_obj.obj_dm_dev_uuid);
	PRINT_POINTER	(ddvol->ddvol_parent);
	PRINT_POINTER	(lba_table);
}

VOID print_cli_args(struct dm_target *target, UINT argc, CHAR ** argv) 
{
	int i;
	LOGINF("argc = %u\n", argc);
	LOGINF("target->begin = %lu (Start Sector Passed from CLI)\n", target->begin);
	LOGINF("target->len   = %lu (Sector Size  Passed from CLI)\n", target->begin);
	//PRINT_ULONG	(target->begin); // Start Sector Passed from cli
	//PRINT_ULONG	(target->len); // Sector size passed from cli

	for (i=0; i < argc; i++) {
		LOGINF("argv[%d]  = %s",i, argv[i]);
	}
}
EXPORT_SYMBOL(print_cli_args);

VOID print_bdev(PBLOCK_DEVICE bdev)
{
	char b[BDEVNAME_SIZE];
	if (bdev) {
		LOGINF("bdev = %s\n", bdevname(bdev, b));
	} else {
		LOGINF("bdev is NULL\n");
	}
}

RVOID print_sizeof_types(VOID)
{
	LOGINF("\n%s(): Basic Data Structures\n", __func__);
	LOGINF("Size of int                      = %zu\n", sizeof(int)); 
	LOGINF("Size of long int                 = %zu\n", sizeof( long int)); 
	LOGINF("Size of long unsigned int        = %zu\n", sizeof( long unsigned int)); 
	LOGINF("Size of Long                     = %zu\n", sizeof(long)); 
	LOGINF("Size of U64                      = %zu\n", sizeof(U64)); 
	LOGINF("Size of unsigned long            = %zu\n", sizeof(unsigned long)); 
	LOGINF("Size of unsigned long long       = %zu\n", sizeof(unsigned long long)); 
	LOGINF("Size of signed long              = %zu\n", sizeof(signed long)); 
	LOGINF("Size of signed long long         = %zu\n", sizeof(signed long long)); 
	LOGINF("Size of long long unsigned int   = %zu\n", sizeof(long long unsigned int)); 
	LOGINF("Size of long long signed int     = %zu\n", sizeof(long long signed int)); 

	LOGINF("\n%s(): Kernel Data Structures\n", __func__);
	LOGINF("Size of struct bio               = %zu bytes\n", sizeof(struct bio)); 
	LOGINF("Size of struct bio_vec           = %zu bytes\n", sizeof(struct bio_vec)); 
	LOGINF("Size of struct bio_set           = %zu bytes\n", sizeof(struct bio_set)); 
	LOGINF("Size of struct block_device      = %zu bytes\n", sizeof(struct block_device)); 
	LOGINF("Size of struct list_head         = %zu bytes\n", sizeof(struct list_head)); 
	LOGINF("Size of struct dm_dev            = %zu bytes\n", sizeof(struct dm_dev)); 
	LOGINF("Size of struct dm_target         = %zu bytes\n", sizeof(struct dm_target)); 
	LOGINF("Size of struct page              = %zu bytes\n", sizeof(struct page)); 
	LOGINF("Size of struct completion        = %zu bytes\n", sizeof(struct completion)); 
	LOGINF("Size of struct radix_tree_root   = %zu bytes\n", sizeof(struct radix_tree_root)); 

	LOGINF("%s(): ZAIDSTOR AVM Data Structures\n", __func__);
	LOGINF("Size of struct dz_bitmap_s             = %zu bytes\n", sizeof(struct dz_bitmap_s)); 
	LOGINF("Size of struct dz_target_object_s      = %zu bytes\n", sizeof(struct dz_target_object_s)); 
	LOGINF("Size of struct dz_target_device_s      = %zu bytes\n", sizeof(struct dz_target_device_s)); 
	LOGINF("Size of struct dz_dedupe_volume_s      = %zu bytes\n", sizeof(struct dz_dedupe_volume_s)); 
	LOGINF("Size of struct dz_object_list_s        = %zu bytes\n", sizeof(struct dz_object_list_s)); 
	LOGINF("Size of struct dz_bvec_holder_s        = %zu bytes\n", sizeof(struct dz_bvec_holder_s)); 
	LOGINF("Size of struct dz_iorequest_s          = %zu bytes\n", sizeof(struct dz_iorequest_s)); 
	LOGINF("Size of struct dz_dedupe_info_s        = %zu bytes\n", sizeof(struct dz_dedupe_info_s)); 
	LOGINF("Size of struct dz_data_pages_s         = %zu bytes\n", sizeof(struct dz_data_pages_s)); 
	LOGINF("Size of union  dz_lba_block_s          = %zu bytes\n", sizeof(union dz_lba_block_s)); 
	LOGINF("Size of union  dz_hash_block_s         = %zu bytes\n", sizeof(union dz_hash_block_s)); 
	LOGINF("Size of union  dz_pba_block_s          = %zu bytes\n", sizeof(union dz_pba_block_s)); 
	LOGINF("Size of union  dz_fec_data_write_s     = %zu bytes\n", sizeof(union dz_fec_data_write_s)); 
	LOGINF("Size of struct dz_fec_table_s          = %zu bytes\n", sizeof(struct dz_fec_table_s)); 
	LOGINF("Size of struct dz_metadata_disk_block_s= %zu bytes\n", sizeof(struct dz_metadata_disk_block_s)); 
	LOGINF("Size of struct dz_metadata_lock_s      = %zu bytes\n", sizeof(struct dz_metadata_lock_s)); 
	LOGINF("Size of struct dz_completion_s         = %zu bytes\n", sizeof(struct dz_completion_s)); 
	LOGINF("Size of struct dz_ddvol_lba_block_s    = %zu bytes\n", sizeof(struct dz_ddvol_lba_block_s)); 
	LOGINF("Size of struct dz_bec_request_s        = %zu bytes\n", sizeof(struct dz_bec_request_s)); 
	LOGINF("Size of struct dz_bec_node_s           = %zu bytes\n", sizeof(struct dz_bec_node_s)); 
	LOGINF("Size of struct dz_hash_s               = %zu bytes\n", sizeof(struct dz_hash_s)); 
	LOGINF("Size of struct dz_superblock_s         = %zu bytes\n", sizeof(struct dz_superblock_s)); 
	LOGINF("Size of struct dz_job_s                = %zu bytes\n", sizeof(struct dz_job_s)); 
	LOGINF("Size of struct dz_thread_s             = %zu bytes\n", sizeof(struct dz_thread_s)); 
	LOGINF("Size of struct dz_thread_pool_s        = %zu bytes\n", sizeof(struct dz_thread_pool_s)); 
}

RVOID print_lba_block(PLBA_BLOCK lba_block)
{
	LOGINF("Printing LBA_BLOCK datastructure\n");
	LOGINF("LBA_BLOCK->lock                 = %d, 0x%x\n", lba_block->lba_lock, lba_block->lba_lock);
	LOGINF("LBA_BLOCK->in_use               = %d, 0x%x\n", lba_block->in_use, lba_block->in_use);
	LOGINF("LBA_BLOCK->allocated            = %d, 0x%x\n", lba_block->allocated, lba_block->allocated);
	LOGINF("LBA_BLOCK->hash_index           = %u, 0x%x\n", lba_block->hash_index, lba_block->hash_index);
	LOGINF("LBA_BLOCK->hash_collision_index = %lli, 0x%llx\n",
		(U64)lba_block->hash_collision_tag, (U64)lba_block->hash_collision_tag);
	LOGINF("LBA_BLOCK->donotchange          = %lu, 0x%lx\n",
		lba_block->donotchange, lba_block->donotchange);
}
EXPORT_SYMBOL(print_lba_block);

RVOID print_metadata_block(PMDD_BLOCK pmdd)
{
	LOGINF("Printing MD_BLOCK datastructure\n");
	LOGINF("mdd_lba           = %llu\n",   pmdd->mdd_lba);
	LOGINF("mdd_pba_index     = %llu\n", pmdd->mdd_pba_index);
	LOGINF("mdd_hash_index    = %llu\n", (U64)pmdd->mdd_hash_index);
	LOGINF("mdd_dd_sector     = %llu\n", pmdd->mdd_dd_sector);
	LOGINF("mdd_collision_tag = %llu\n", (U64)pmdd->mdd_hash_collision_tag);
	
	//64 bit Decimal
	LOGINF("mdd_hash    64[0] = %llu, 64[1] = %llu\n",
		(U64)pmdd->mdd_hash.hash64[0],
	 	(U64)pmdd->mdd_hash.hash64[1]);

	//64 bit Hex
	LOGINF("mdd_hash    64[0] = 0x%llx, 64[1] = 0x%llx\n",
		(U64)pmdd->mdd_hash.hash64[0],
		(U64)pmdd->mdd_hash.hash64[1]);

	//32 bit Decimal
	LOGINF("mdd_hash    32[0] = %u, 32[1] = %u 32[2] = %u 32[3] = %u\n", 
		pmdd->mdd_hash.hash32[0],
		pmdd->mdd_hash.hash32[1],
		pmdd->mdd_hash.hash32[2],
		pmdd->mdd_hash.hash32[3]);
	
	//32 bit Hex
	LOGINF("mdd_hash    32[0] = 0x%x, 32[1] = 0x%x 32[2] = 0x%x 32[3] = 0x%x\n", 
		pmdd->mdd_hash.hash32[0],
		pmdd->mdd_hash.hash32[1],
		pmdd->mdd_hash.hash32[2],
		pmdd->mdd_hash.hash32[3]);

}
EXPORT_SYMBOL(print_metadata_block);

RINT dz_get_device_size(char *device_name, ULONG *rd_blocks)
{
	INT 	ret;

	PBLOCK_DEVICE	blockdev = NULL;
	mm_segment_t old_fs;

    blockdev = dz_bdev_open(device_name);
    if (! blockdev) {
        LOGE("Unable to open device %s\n", device_name);
		return -1;
    } else {
        LOGINF("Device %s Opened successfully\n", device_name);
    }

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	ret = blkdev_ioctl(blockdev, FMODE_READ, BLKGETSIZE64, (ULONG)rd_blocks);
	set_fs(old_fs);
	dz_bdev_close(blockdev);
	return ret;
}
EXPORT_SYMBOL(dz_get_device_size);

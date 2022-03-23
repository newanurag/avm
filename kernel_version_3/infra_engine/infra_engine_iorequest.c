/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PPBA_BLOCK	pba_table;
extern PHASH_BLOCK	hash_table;
extern PLBA_BLOCK	lba_table;
extern CHAR sysfs_avm_get_target_name[DZ_SYSFS_PAGE_SIZE];
extern SIZE				dz_pba_page_mempool_count;
extern U64 				lba_table_size;
extern U64 				pba_table_size;
extern U64 				hash_table_size;
extern BITMAP 			pba_bitmap;
extern PDZ_THREAD_POOL read_thread_pool;
extern PDZ_THREAD_POOL write_thread_pool;
extern SPINLOCK			print_iorequest_lock;

RVOID dz_init_child_bio(PBIO parent_bio, PBIO child, SECTOR sector, BIO_CALLBACK done, PVOID private)
{
#ifdef DZ_KERNEL_VERSION_4
	child->bi_disk 		= parent_bio->bi_disk; //TODO:
	child->bi_iter.bi_sector= sector;
	child->bi_opf 		= parent_bio->bi_opf;
	//child->bi_flags   	= 1 << BIO_UPTODATE; //TODO:
	//child->bi_iter.bi_idx	= sector; //TODO:
	DZ_BIO_SET_ZIDX(child);
#else
	child->bi_bdev 		= parent_bio->bi_bdev;
	child->bi_sector 	= sector;
	child->bi_rw 		= parent_bio->bi_rw;
	child->bi_flags   	= 1 << BIO_UPTODATE;
	DZ_BIO_SET_ZIDX(child);
#endif
	child->bi_end_io 	= done;
	child->bi_next 		= NULL;
	//child->bi_next 		= parent_bio; //Saving parent bio in child bio next field
	child->bi_private  	= private;
}
EXPORT_SYMBOL(dz_init_child_bio);

RVOID dz_q_child_iorequest_to_parent_q(PDZIO parent_iorequest, PDZIO child_iorequest)
{
	// Since the queues are adding local, hence no locking required
	list_add_tail(&child_iorequest->ior_ioq, &parent_iorequest->ior_ioq_head_local);
}
EXPORT_SYMBOL(dz_q_child_iorequest_to_parent_q);

RVOID dz_merge_parent_local_q_to_threadpool_q(DZ_THREAD_POOL *pool, PIOREQUEST parent_iorequest)
		//struct list_head *parent_iorequest_list)
{
	if (pool->ioq_open) {
		
		//PRINT_INT(atomic_read(&pool->ioq_num_ios));
		//PRINT_INT(atomic_read(&parent_iorequest->child_cnt));
		atomic_add(atomic_read(&parent_iorequest->ior_child_cnt), &pool->ioq_num_ios);
		//PRINT_INT(atomic_read(&pool->ioq_num_ios));
		//atomic_inc(&pool->ioq_num_ios);
		//dz_merge_parent_local_q_to_threadpool_q(piorequest->ior_thread_pool, &piorequest->ioq_head_local);
		spin_lock(&pool->ioq_lock);
		//list_splice_tail(parent_iorequest_list, &pool->ioq_head);
		list_splice_tail(&parent_iorequest->ior_ioq_head_local, &pool->ioq_head);
		spin_unlock(&pool->ioq_lock);
		complete(&pool->ioq_addition);
	} else {
		LOG("IO Request Addition is disabled in pool %s\n", pool->name);
	}
}
EXPORT_SYMBOL(dz_merge_parent_local_q_to_threadpool_q);

UINT dz_outstanding_iorequests(DZ_THREAD_POOL *pool)
{
	return atomic_read(&pool->ioq_num_ios);
}

PDZIO dz_dq_iorequest(DZ_THREAD_POOL *pool)
{
	PDZIO iorequest = NULL;
	spin_lock(&pool->ioq_lock);
	if (list_empty(&pool->ioq_head)) {
		//TODO:reinit_completion(&pool->job_addition); 
		//Latest kernel is using reinit_completion instead of INIT_COMPLETION
#ifdef DZ_KERNEL_VERSION_4
		init_completion(&pool->ioq_addition);
#else
		INIT_COMPLETION(pool->ioq_addition);
#endif
		spin_unlock(&pool->ioq_lock);
		goto out; 
	} else {
		iorequest = list_first_entry(&pool->ioq_head, struct dz_iorequest_s, ior_ioq);
		list_del(&iorequest->ior_ioq);
	}
	spin_unlock(&pool->ioq_lock);
out:
	return iorequest; 
}
//
// Used for Queuing IOs into the  IO queue
RVOID dz_q_iorequest_list(DZ_THREAD_POOL *pool, struct list_head *newlist, UINT count)
{
	if (pool->ioq_open) {
		//atomic_inc(&pool->ioq_num_ios);
		atomic_add(count, &pool->ioq_num_ios);
		spin_lock(&pool->ioq_lock);
		list_splice_tail(newlist, &pool->ioq_head);
		spin_unlock(&pool->ioq_lock);
		complete(&pool->ioq_addition);
	} else {
		LOG("IO Request Addition is disabled in pool %s\n", pool->name);
	}
}

// Used for Queuing IOs into the  IO queue
RVOID dz_q_iorequest(DZ_THREAD_POOL *pool, PDZIO iorequest)
{
	if (pool->ioq_open) {
		atomic_inc(&pool->ioq_num_ios);
		spin_lock(&pool->ioq_lock);
		list_add_tail(&iorequest->ior_ioq, &pool->ioq_head);
		spin_unlock(&pool->ioq_lock);
		complete(&pool->ioq_addition);
	} else {
		LOG("IO Request Addition is disabled for pool %s\n", pool->name);
	}
}
EXPORT_SYMBOL(dz_q_iorequest);

RVOID dz_q_iorequest_thread_pool(PDZIO iorequest)
{
	DZ_THREAD_POOL *pool = iorequest->ior_thread_pool;
	if (pool->ioq_open) {
		atomic_inc(&pool->ioq_num_ios);
		spin_lock(&pool->ioq_lock);
		list_add_tail(&iorequest->ior_ioq, &pool->ioq_head);
		spin_unlock(&pool->ioq_lock);
		complete(&pool->ioq_addition);
	} else {
		LOG("IO Request Addition is disabled in pool %s\n", pool->name);
	}
}
EXPORT_SYMBOL(dz_q_iorequest_thread_pool);

RVOID print_iorequest_lite(PIOREQUEST p, char *str)
{
	CHAR	*marker = dz_kmalloc(NAME_LENGTH, GFP_KERNEL);
	if (!marker) {
		LOG("Unable to allocate memory for print_iorequest\n");
		return;
	}
	memset(marker, 0, NAME_LENGTH);
	spin_lock(&print_iorequest_lock);
	LOGINF("Printing iorequest info : %s\n", str);
	LOGINF("iorequest               = %p\n", p); 
	LOGINF("iorequest->ior_type     = %u\n", p->ior_type); 
	LOGINF("iorequest->ior_lba      = %lli\n", (U64)p->ior_lba); 
	LOGINF("iorequest->ior_size     = %lli\n", (U64)p->ior_size); 
	LOGINF("iorequest->ior_sector   = %lli\n", (U64)p->ior_sector); 
	LOGINF("iorequest->ior_child_cnt= %d\n",      atomic_read(&p->ior_child_cnt)); 
	LOGINF("iorequest->ior_bio      = %p\n",      p->ior_bio); 


	LOGINF("iorequest->parent       = %p\n",      p->ior_parent); 
	if (p->ior_parent) {
		if (p->ior_parent->ior_bio) {
			LOGINF("iorequest->parent->ior_bio   = %p\n",      p->ior_parent->ior_bio); 
		}
	}
	if (p->ior_thread_pool) {
		LOGINF("iorequest->thread_pool  = %s  \n", p->ior_thread_pool->name);
	}

	if (p->ior_bio) {
		if (p->ior_bio->bi_private == p) {
			//sprintf(marker,"%s: Internal bio", str);
		} else {
			//sprintf(marker,"%s: External bio", str);
		}
		print_biom(p->ior_bio, marker);
	}

	spin_unlock(&print_iorequest_lock);
	dz_kfree(marker, NAME_LENGTH);
	LOG("\n\n");
}




RVOID print_iorequest(PIOREQUEST p, char *str)
{
	CHAR	*marker = dz_kmalloc(NAME_LENGTH, GFP_KERNEL);
	if (!marker) {
		LOG("Unable to allocate memory for print_iorequest\n");
		return;
	}
	spin_lock(&print_iorequest_lock);
	LOGINF("Printing iorequest info : %s\n", str);
	LOGINF("iorequest               = %p\n", p); 
	LOGINF("iorequest->ior_lba      = %lli\n", (U64)p->ior_lba); 
	LOGINF("iorequest->ior_size     = %lli\n", (U64)p->ior_size); 
	LOGINF("iorequest->ior_sector   = %lli\n", (U64)p->ior_sector); 
	LOGINF("iorequest->ior_child_cnt= %d\n",      atomic_read(&p->ior_child_cnt)); 
	LOGINF("iorequest->ior_bio      = %p\n",      p->ior_bio); 
	if (p->ior_bio->bi_next) {
	LOGINF("iorequest->sibling_bio  = %p\n",      p->ior_bio->bi_next); 
	}
	LOGINF("iorequest->ior_parent   = %p\n",      p->ior_parent); 
	if (p->ior_parent) {
		if (p->ior_parent->ior_bio) {
			LOGINF("iorequest->parent_bio   = %p\n",      p->ior_parent->ior_bio); 
		}
	}
	if (p->ior_thread_pool) {
		LOGINF("iorequest->thread_pool  = %s  \n", p->ior_thread_pool->name);
	}

	memset(marker, 0, NAME_LENGTH);
	if (p->ior_bio) {
		if (p->ior_bio->bi_private == p) {
			sprintf(marker,"%s: Internal bio", str);
		} else {
			sprintf(marker,"%s: External bio", str);
		}
		print_biom(p->ior_bio, marker);
		if (p->ior_bio->bi_next) {
			memset(marker, 0, NAME_LENGTH);
			sprintf(marker,"%s: sibling bio", str);
			print_biom(p->ior_bio->bi_next, marker);
		}
	}

	if (!p->ior_parent) {
		goto exit;
	}
	p = p->ior_parent;

	LOGINF("Printing Parent iorequest info  : %s\n", str);
	LOGINF("parent_iorequest                = %p  \n", p); 
	LOGINF("parent_iorequest->ior_lba       = %lli\n", (U64)p->ior_lba); 
	LOGINF("parent_iorequest->ior_size      = %lli\n", (U64)p->ior_size); 
	LOGINF("parent_iorequest->ior_sector    = %lli\n", (U64)p->ior_sector); 
	LOGINF("parent_iorequest->ior_child_cnt = %d\n",      atomic_read(&p->ior_child_cnt)); 
	LOGINF("parent_iorequest->ior_bio       = %p\n",      p->ior_bio); 
	if (p->ior_thread_pool) {
		LOGINF("parent_iorequest->thread_pool   = %s  \n", p->ior_thread_pool->name);
	}

	memset(marker, 0, NAME_LENGTH);
	if (p->ior_bio) {
		if (p->ior_bio->bi_private == p) {
			sprintf(marker,"%s: Parent Internal bio", str);
		} else {
			sprintf(marker,"%s: Parent External bio", str);
		}
		print_biom(p->ior_bio, marker);
		if (p->ior_bio->bi_next) {
			memset(marker, 0, NAME_LENGTH);
			sprintf(marker,"%s: Parent sibling bio", str);
			print_biom(p->ior_bio->bi_next, marker);
		}
	}

exit:
	spin_unlock(&print_iorequest_lock);
	dz_kfree(marker, NAME_LENGTH);
	LOG("\n\n");
}
EXPORT_SYMBOL(print_iorequest);

RVOID dz_fake_iodone(PIOREQUEST iorequest)
{
	if (iorequest->ior_parent) {
		READ_IO_DONE(iorequest);
		//Just done the child bio.
		//Iorequest will be freed in the child biodone
		return;
	}
	READ_IO_DONE(iorequest);
	dz_io_free(iorequest);
}
EXPORT_SYMBOL(dz_fake_iodone);

RVOID dz_iodone(PIOREQUEST iorequest)
{
	if (iorequest->ior_parent) {
		IO_DONE(iorequest);
		//Just done the child bio.
		//Iorequest will be freed in the child biodone
		return;
	}
	IO_DONE(iorequest);
	iorequest_put(iorequest);
}
EXPORT_SYMBOL(dz_iodone);

RVOID dz_iodone_error(PIOREQUEST iorequest, INT error)
{
	if (iorequest->ior_parent) {
		IO_DONE_ERROR(iorequest, error);
		//Just done the child bio.
		//Iorequest will be freed in the child biodone
		return;
	}
	IO_DONE_ERROR(iorequest, error);
	iorequest_put(iorequest);
}
EXPORT_SYMBOL(dz_iodone_error);

//TODO: We should make it MACRO
RVOID iorequest_get(PIOREQUEST iorequest)
{
	ATOMIC_INC(iorequest->ior_ref_cnt);
}
EXPORT_SYMBOL(iorequest_get);

//TODO: We should make it MACRO
RVOID iorequest_put(PIOREQUEST iorequest)
{
	//BUG_ON(!ATOMIC_READ(iorequest->ref_cnt));
	if (ATOMIC_READ(iorequest->ior_ref_cnt) == 0) {
		dz_io_free(iorequest);
	} else {
	//LOG("REF cnt = %d, iorequest=%p\n",ATOMIC_READ(iorequest->ref_cnt), iorequest);
		if (atomic_dec_and_test(&iorequest->ior_ref_cnt)) {
			dz_io_free(iorequest);
			//LOG("Freeing iorequest\n");
			//dump_stack();
		}
	}
}
EXPORT_SYMBOL(iorequest_put);

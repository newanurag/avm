/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PBLOCK_DEVICE	DATA_DISK_DEVICE;
extern PAVM_DEVICE 		aisa_device_object;
extern SIZE				dz_pba_page_mempool_count;
extern BITMAP 			pba_bitmap;
extern PHASH_BLOCK		hash_table;
extern CHAR 			sysfs_avm_get_target_name[DZ_SYSFS_PAGE_SIZE];
extern SIZE				dz_pba_page_mempool_count;
extern U64 				lba_table_size;
extern U64 				pba_table_size;
extern U64 				hash_table_size;
extern BITMAP 			pba_bitmap;
extern PDZ_THREAD_POOL 	read_thread_pool;
extern PDZ_THREAD_POOL 	write_thread_pool;

extern SPINLOCK 		lba_table_lock; 
extern SPINLOCK 		hash_table_lock; 
extern SPINLOCK 		pba_table_lock; 

extern U64 dz_disk_io_sector;
extern PBLOCK_DEVICE 	avm_metadata_bdev;
extern PMD_LOCK	md_lock_table;	// Array of spinlocks for MetaData Lock

extern ATOMIC64	dedupe_disk_reads;
extern ATOMIC64	dedupe_disk_writes;
extern ATOMIC64	dedupe_cache_hits;
extern ATOMIC64	dedupe_deleted_entries;
extern ATOMIC64	dedupe_cur_logical_space;
extern ATOMIC64	dedupe_cur_physical_space;
extern ATOMIC64	dedupe_cur_duplicate_space;
extern U64		dedupe_tot_logical_space;
extern U64		dedupe_tot_physical_space;

//This is applicable for internally generated bios and 
//not for application bios.
//Currently its parent will be a flush bio
//Note : ACQUIRE_LBA_LOCK_WRITE is already taken
//Also this function will write only one/single block/Page only

RINT dz_write_block_to_raw_disk_sync_internal(PIOREQUEST iorequest, 
		SECTOR sector,
		PLBA_BLOCK lba_entry)
{
	PBIO 	bio 			= NULL;
	INT 	error			= SUCCESS;
	PVOID	buffer			= iorequest->ior_bv_buffer;
	PDD_INFO	ddinfo		= iorequest->ior_ddinfo;
	DZ_COMPLETION   		io_completion;
	//PFEC_WRITE		fecw 	= (PFEC_WRITE)iorequest->private;
	PPAGE	pageref			 = NULL;
	lba_entry				= ddinfo->lba_entry;

	
	io_completion.error = SUCCESS;

	//Allocate a fresh bio for writing onto Data Disk
	bio = dz_bio_page_alloc(DATA_DISK_DEVICE);

	pageref = bio_page(bio);
	LOGD("Before Page Count = %d\n", atomic_read(&((pageref)->_count)));
	
	DZ_BIO_SET_SECTOR(bio, sector);
	DZ_BIO_SET_END_IO(bio, dz_write_biodone_sync);
	DZ_BIO_SET_ZIDX(bio);
	DZ_BIO_SET_WRITE(bio);
	DZ_BIO_SET_NEXT(bio, NULL);
	bio->bi_private = (VOID *)&io_completion;
	//TODO:bio->bi_flags   = 1 << BIO_UPTODATE;

	COPY_BUFFER_TO_PAGE(bio_page(bio), buffer);

	init_completion(&io_completion.completion);

	DZ_OS_KERNEL_BIO_GET(bio);

	DZ_OS_KERNEL_GENERIC_MAKE_REQUEST(bio);

	LOGD("Waiting for completion for iorequest %p @ lba %lli\n",
	 	iorequest, (U64)iorequest->ior_lba);

	//TODO:Should we release the LBA lock here itself.
	//What is the harm in releasing the lock since the io is submitted to
	//lower layer. 
	//Will there be any data corruption.
	//Will there be any inconsistency if there is a parallel Read IO

	ATOMIC64_INC(dedupe_disk_writes);
	wait_for_completion(&io_completion.completion);


	//Get the Error returned (if any)
	//io_completion = bio->bi_private;
	error = io_completion.error;

	if (unlikely(error)){
		LOGE("Error in write to raw disk . Error Code : %d", error);
		print_biom(bio, "bio with error");
		BUG_ON(1);
	}

	if (!error) {
		//FEC_REMOVE_WRITE_BUFFER_FROM_LBA_ENTRY_LIST(fecw);
		_RELEASE_LBA_LOCK_WRITE(lba_entry);
		dz_dedupe_free(iorequest->ior_ddinfo);

		//We are not going to put this bio page data inside backend cache
		// Updation of bec will be taken during flushing of frontend cache
		// write buffers
		//dz_bec_proceed_pba_page_add_write(iorequest);

		//Mark this internally generated iorequest done. This will invoke
		// the marking of state of fecw to Disk Write Completed
		DZ_DDE_IOREQUEST_DONE(iorequest);
		//dz_bio_page_free(bio);
		//TODO:When invoked above function then it was found that bio_page(bio) returning NULL
		//Have not figured out why. Not sure if lower layer drivers are changing that bio
		//because bi_sector and bi_size fields were also changed inside the done function
		//So freeing the Page with the pageref variable (defined above)
		//if (bio_page(bio)) {
	//		dz_bio_page_free(bio); //TODO:Still feel this is buggy
	//	} else {
			dz_page_free(pageref);
			DZ_OS_KERNEL_BIO_PUT(bio);
	//	}
		RETURNS;
	}
	//Due to error, No need to remove buffer as of now
	//TODO:How to handle this case
	_RELEASE_LBA_LOCK_WRITE(lba_entry);
	dz_dedupe_free(iorequest->ior_ddinfo);
	DZ_DDE_IOREQUEST_DONE_ERROR(iorequest, error);

	//See the above description for why we are doing this way
	//if (bio_page(bio)) {
	//	dz_bio_page_free(bio); //TODO:Still feel this is buggy
	//} else {
		dz_page_free(pageref);
		DZ_OS_KERNEL_BIO_PUT(bio);
	//}
	RETURNF;
}

// Find the old overwritten_pba_index and the one previous to it.
// Previous is required to update the entry
VOID dz_dedupe_overwrite_update_old_entries(PIOREQUEST iorequest, U32 lba_hash_index, U64 lba_hash_collision_index)
{
	PPBA_BLOCK	overwrite_pba_entry	= NULL;
	PPBA_BLOCK	head_pba_list		= NULL;
	U64 		overwrite_pba_index	= 0;
	U64 	prev_to_overwrite_index	= 0;
	PHASH_BLOCK	old_hash_entry		= NULL;
	PIOREQUEST	iorequest_del		= NULL;
	U64			retry_cnt = 0;
	// Terminology : 
	// Current and New refers to the newly generated current IO hash, lba, pba
	// Old and Existing means whatever is already present at lba entry, i.e. the  hash and its
	// corresponding pba 

	// Get the Existing hash index and collision index, which are already stored in
	// lba_entry

	//Get the corresponding old hash_entry
	old_hash_entry = GET_HBA_ENTRY(lba_hash_index);
	_ACQUIRE_HBA_LOCK(old_hash_entry);

	// Now traverse the hash collision list to find out the pba_index of the old data
	overwrite_pba_index = old_hash_entry->pba_index;
	overwrite_pba_entry = GET_PBA_ENTRY(overwrite_pba_index);
	head_pba_list 		= overwrite_pba_entry;

	_ACQUIRE_PBA_LOCK(head_pba_list);

	prev_to_overwrite_index = 0; 

	// Get the pba_index which corresponds to this lba overwrite
	while(overwrite_pba_entry->hash_collision_tag != lba_hash_collision_index) {
		prev_to_overwrite_index = overwrite_pba_index;
		overwrite_pba_index = overwrite_pba_entry->next_entry_in_collision_list;
		overwrite_pba_entry = GET_PBA_ENTRY(overwrite_pba_index);
		if (retry_cnt++ > 16) {
			LOGR("retry_cnt = %lli @lba_hash_index = %lli\n", retry_cnt++, (U64)lba_hash_index);
			SL1;
		}
	}

	if (overwrite_pba_entry->ref_cnt == 0) {
		LOGE("overwrite_pba_entry->ref_cnt cannot be zero\n");
		//TODO: print_pba
		BUG_ON(1);
	} else {
		overwrite_pba_entry->ref_cnt-- ;
	}
	LOGD("Reduced ref count of pba_index %lli = %llu\n",
			(U64)overwrite_pba_index,
			 (U64)overwrite_pba_entry->ref_cnt);

	//Now check if this pba is still referred by others. If yes then 
	//dont do anything just return from here
	if (overwrite_pba_entry->ref_cnt) {
		//Yes, so no need to delete this pba
		_DZ_DDE_RELEASE_PBA_LOCK(head_pba_list);
		_DZ_DDE_RELEASE_HBA_LOCK(old_hash_entry);
		if (iorequest) {
			iorequest_put(iorequest);
		}
		return;
	}
	
	//Now set the next pointers
	if (prev_to_overwrite_index == 0) {
		//Update the pba_index inside the old_hash_entry, since it is the first pba
		DZ_DDE_UPDATE_HASH_TABLE(old_hash_entry, overwrite_pba_entry->next_entry_in_collision_list);
		
	} else {
		// Overwrite pba entry either comes in between or at the end/tail
		//Delete the entry of overwrite pba. Hash_entry update is not required because there are other entries
		//present for this index
		(GET_PBA_ENTRY(prev_to_overwrite_index))->next_entry_in_collision_list = overwrite_pba_entry->next_entry_in_collision_list;
	}
	_DZ_DDE_RELEASE_PBA_LOCK(head_pba_list);
	_DZ_DDE_RELEASE_HBA_LOCK(old_hash_entry);
	// Now at this point overwrite_pba_entry is free from all relations in this material world.
	// Mark this pba_entry for garbage collection
	//dz_set_garbage_pba_index(overwrite_pba_index);
	dz_put_pba_index(overwrite_pba_index);

	//Now delete this pba_index from BEC Cache
	if (iorequest) {
		iorequest->ior_sector = overwrite_pba_index;
		dz_bec_proceed_pba_page_delete(iorequest);
	} else {
		iorequest_del = dz_io_alloc();
		if(iorequest_del) {
			iorequest_del->ior_sector = overwrite_pba_index;
			iorequest_get(iorequest_del);
			dz_bec_proceed_pba_page_delete(iorequest_del);
		}
	}
	ATOMIC64_INC(dedupe_deleted_entries);
	return;
}
BOOL compare_page_and_buffer(PPAGE s1 , PVOID s2)
{
	PVOID	s1pagebuf				= NULL;
	PVOID	s2pagebuf				= s2;

	s1pagebuf = kmap(s1);
	if (memcmp(s1pagebuf, s2pagebuf, PAGE_SIZE) == 0) {
		kunmap(s1);
		return true;
	} else {
		kunmap(s1);
		return false;
	}
}

/*
 * ************************************************************
 * This function does these things:
 * 1. Allocate a DDINFO structure
 * 2. Generate Hash for the IO data which is present in iorequest
 * 3. Populate the structure with more details such as lba and 
 *    hba table entries
 *
 * ************************************************************
 */

PDD_INFO dz_create_dedupe_info(PIOREQUEST iorequest)
{
	U32		*hash_array			= NULL;
	PVOID	pagebuf				= iorequest->ior_bv_buffer;
	PDD_INFO	ddinfo			= NULL;

ddinfo_alloc:
	ddinfo = dz_dedupe_alloc();
	if (!ddinfo) {
		SLEEP_MILLI_SECONDS(1);
		goto ddinfo_alloc;
	}
	MEMSET(ddinfo);
	hash_array 	= ddinfo->hash;
	dz_generate_hash(pagebuf, PAGE_SIZE, hash_array);

	ddinfo->new_hash_index 	= dz_get_hash_index(hash_array[0]);

	LOGD("Hash generated for lba %lli New hash index = %lli\n",
		(U64)iorequest->ior_lba, ddinfo->new_hash_index);

	ddinfo->lba_entry		= GET_LBA_ENTRY(iorequest->ior_lba);
	ddinfo->hba_entry		= GET_HBA_ENTRY(ddinfo->new_hash_index);
	iorequest->ior_ddinfo 		= ddinfo;
	return ddinfo;
}

//This function traverses the list of all pbas in the collision list
//starting with hash_pba_index. Also for each pba_index it encounters,
//it does a read back of data from the bec cache or physical disk
//and compare with Inflight IO data buffer present in iorequest-bv_buffer.
//Once successful, it returns updated last_pba_index and the collision tag.

U64 dz_dedupe_get_pba_index(PIOREQUEST iorequest, U64 hash_pba_index,
		U64 *last_pba_index, U64 *next_hash_collision_tag )
{	
	U64 pba_index 			= 0;
	U64 prev_pba_index 		= 0;
	PPBA_BLOCK	pba_entry	= NULL;
	PPAGE		read_page	= NULL;
	PDD_INFO	ddinfo		= iorequest->ior_ddinfo;
	//PLBA_BLOCK	lba_entry	= ddinfo->lba_entry;
	PBIO 		bio			= dz_bio_page_alloc(DATA_DISK_DEVICE);
	INT			retry_cnt	= 0;
	INT			diskr_cnt	= 0;

	if (!bio) {
		//TODO: Handle it
		BUG_ON(1);
	}
	
	pba_index				= hash_pba_index;

	do {
		if(dz_bec_pba_page_lookup(pba_index, bio)) {
			read_page = bio_page(bio);
			ATOMIC64_INC(dedupe_cache_hits);
		} else {
			DZ_BIO_SET_SECTOR(bio, dz_convert_lba_to_sector(pba_index));
			read_page = dz_read_page_sync(bio);
			//TODO: We need to handle read_page is NULL.
			ATOMIC64_INC(dedupe_disk_reads);
			diskr_cnt++;
		}
		if (retry_cnt >= 0) {
			//LOGR("Inside get pba_index = %lli, retry_cnt=%d\n", pba_index, retry_cnt++);
		}

		pba_entry	= GET_PBA_ENTRY(pba_index);

		if (retry_cnt == 0) {
			//LOGR("gpif:pba_index = %lli, retry_cnt=%d, diskr_cnt = %d, Tag = %lli, Next pba_index = %lli\n",
			//pba_index, retry_cnt++, diskr_cnt, 
			//(U64)pba_entry->hash_collision_tag,
			//(U64)pba_entry->next_entry_in_collision_list);
		} else {
			//LOGR("gpin:pba_index = %lli, retry_cnt=%d, diskr_cnt = %d, Tag = %lli, Next pba_index = %lli\n",
			//pba_index, retry_cnt++, diskr_cnt, 
			//(U64)pba_entry->hash_collision_tag,
			//(U64)pba_entry->next_entry_in_collision_list);
		}

		// if data is same then its a duplication data which needs dedupe
		if (read_page) {
			if (compare_page_and_buffer(read_page , iorequest->ior_bv_buffer )) {
				// Data is same. Case of dedupe
				//Also if the existing destination pba_index of lba is same then its a 
				//case of overwrite with same data
				//dz_read_page_free(read_page);
				dz_bio_page_free(bio);
				
				//Set the pba_entry so that the caller should know which pba gives dedupe
				ddinfo->dedupe_pba_index = pba_index; 
				return DZ_DDE_WRITE_TYPE_DEDUPE ;
			} else {
				// Its a case of hash collision. So continue to next pba_entry
				// till the end
				//dz_read_page_free(read_page);
			}

		} else {
			LOGE("Read IO Failure during dedupe detection\n"); //TODO:
			//TODO:
			BUG_ON(1);
			RETURNF;
		}
		prev_pba_index 	= pba_index;
		pba_index 		= pba_entry->next_entry_in_collision_list;
		//PRINT_U64(pba_index);

	} while(pba_index != 0);

	//It will come here only in case of hash collision
	//*last_hash_collision_index = ((GET_PBA_ENTRY(prev_pba_index))->hash_collision_tag + 1);
	*last_pba_index 			= prev_pba_index; 
	*next_hash_collision_tag 	= (pba_entry->hash_collision_tag + 1);
	dz_bio_page_free(bio);
	return DZ_DDE_WRITE_TYPE_HASH_COLLISON;

}

RINT dz_dedupe_check_write_type(PIOREQUEST iorequest, U64 hash_pba_index,
		U64 *last_pba_index, U64 *next_hash_collision_tag )
{
	
	U64 pba_index 			= 0;
	U64 prev_pba_index 		= 0;
	PPBA_BLOCK	pba_entry	= NULL;
	//SECTOR			sector 	= 0;
	PPAGE		read_page	= NULL;
	PDD_INFO	ddinfo		= iorequest->ior_ddinfo;
	PLBA_BLOCK	lba_entry	= ddinfo->lba_entry;
	PBIO 		bio			= dz_bio_page_alloc(DATA_DISK_DEVICE);
	INT			retry_cnt	= 0;
	INT			diskr_cnt	= 0;

	if (!bio) {
		//TODO: Handle it
		BUG_ON(1);
	}
	
	pba_index				= hash_pba_index;

	do {
		if(dz_bec_pba_page_lookup(pba_index, bio)) {
			read_page = bio_page(bio);
			ATOMIC64_INC(dedupe_cache_hits);
		} else {
			DZ_BIO_SET_SECTOR(bio, dz_convert_lba_to_sector(pba_index));
			read_page = dz_read_page_sync(bio);
			ATOMIC64_INC(dedupe_disk_reads);
			diskr_cnt++;
		}
		if (retry_cnt >= 0) {
			//LOGR("Inside write type pba_index = %lli, retry_cnt=%d\n", pba_index, retry_cnt++);
		}

		pba_entry	= GET_PBA_ENTRY(pba_index);

		if (retry_cnt == 0) {
			//LOGR("cwtf:pba_index = %lli, retry_cnt=%d, diskr_cnt = %d, Tag = %lli, Next pba_index = %lli\n",
			//pba_index, retry_cnt++, diskr_cnt, 
			//(U64)pba_entry->hash_collision_tag,
			//(U64)pba_entry->next_entry_in_collision_list);
		} else {
			//LOGR("cwtn:pba_index = %lli, retry_cnt=%d, diskr_cnt = %d, Tag = %lli, Next pba_index = %lli\n",
			//pba_index, retry_cnt++, diskr_cnt, 
			//(U64)pba_entry->hash_collision_tag,
			//(U64)pba_entry->next_entry_in_collision_list);

		}

		// if data is same then dedupe
		if (read_page) {
			if (compare_page_and_buffer(read_page , iorequest->ior_bv_buffer )) {
				// Data is same. Case of dedupe
				//Also if the existing destination pba_index of lba is same then its a 
				//case of overwrite with same data
				//dz_read_page_free(read_page);
				dz_bio_page_free(bio);
				if (lba_entry->pba_index == pba_index) {
					if (pba_entry->hash_collision_tag != lba_entry->hash_collision_tag) {
						LOGR("Returned Inside write type pba_index = %lli, retry_cnt=%d\n", 
						pba_index, retry_cnt++);
						LOG("lba_tag = %lli, pba_tag = %lli\n", 
						(U64)lba_entry->hash_collision_tag,
						(U64)pba_entry->hash_collision_tag);
						SL1;
						BUG_ON(1);
					}
				}
				
				if (lba_entry->pba_index == pba_index) {
					return DZ_DDE_WRITE_TYPE_OVERWRITE ;
				} else {
					//Set the pba_entry so that the caller should know which pba gives dedupe
					ddinfo->dedupe_pba_index = pba_index; 
					return DZ_DDE_WRITE_TYPE_DEDUPE ;
				}
			} else {
				// Its a case of hash collision. So continue to next pba_entry
				// till the end
				//dz_read_page_free(read_page);
			}

		} else {
			LOGE("Read IO Failure during dedupe detection\n"); //TODO:
			//TODO:
			BUG_ON(1);
			RETURNF;
		}
		prev_pba_index 	= pba_index;
		pba_index 		= pba_entry->next_entry_in_collision_list;
		//PRINT_U64(pba_index);

	} while(pba_index != 0);

	//It will come here only in case of hash collision
	//*last_hash_collision_index = ((GET_PBA_ENTRY(prev_pba_index))->hash_collision_tag + 1);
	*last_pba_index 			= prev_pba_index; 
	*next_hash_collision_tag 	= (pba_entry->hash_collision_tag + 1);
	dz_bio_page_free(bio);
	return DZ_DDE_WRITE_TYPE_HASH_COLLISON;
}

/*
 * **********************************************************************************
 * This is the main function for WRITE IO in dedupe engine.
 * Every WRITE IO has to go through this function irrespective whether its duplicate
 * or NOT
 * It gets invoked through dedupe thread
 * It handles all possible cases typically in 3 stages. Note that pba actually holds
 * the exact location of data on DATA_DISK and it is entirely different than lba
 *
 * Stage_1:
 * First handle the case where hash of inflight IO is a first time write which 
 * means it does not point to any pba. Here first time means, the hash generated 
 * is a NEW one
 *
 * Stage_2:
 * Hash of inflight IO is already pointing to some pba i.e. having a write already.
 * Find out if inflight IO lba is having its WRITE for the first time or already
 *
 * Stage_3:
 * Comes here when lba_entry points to hash_index i.e lba
 * is also having existing writes and hash is also referring to existing pba_index
 * We need to find out the collision, overwrite and dedupe

************************************************************************************/

RVOID dz_dedupe_write_io(PIOREQUEST iorequest)
{
	INT 		ret 			= SUCCESS;
	PLBA_BLOCK	lba_entry		= NULL;
	PHBA_BLOCK	hba_entry		= NULL;
	PPBA_BLOCK	pba_entry_curr	= NULL;
	//PPBA_BLOCK	pba_entry_old	= NULL;
	PPBA_BLOCK	pba_entry_dedupe= NULL;
	PDD_INFO	ddinfo			= NULL;
	PPBA_BLOCK	free_pba_entry	= NULL;
	U64			free_pba_index	= 0;
	//U64			match_pba_index	= 0;
	SECTOR		sector			= 0;
	U64 old_lba_hash_index		= 0;
	U64 old_lba_hash_collision_tag	= 0;
	U64 last_pba_index			= 0;
	U64 next_hash_collision_tag	= 0;

	//First calculate hash of IO data. This is anyway required independent of any lock
	ddinfo 		= dz_create_dedupe_info(iorequest);
	lba_entry	= ddinfo->lba_entry;

	// Take a write lock for this LBA. This is to protect parallel writes on this lba
	_ACQUIRE_LBA_LOCK_WRITE(lba_entry);
	//print_lba_block(lba_entry);
	
	hba_entry = ddinfo->hba_entry;

	//Now acquire the HASH lock as well
	_ACQUIRE_HBA_LOCK(hba_entry);

	/********************************************************************************
 									CheckStage_1:
	//First handle the case where hash of inflight IO is a first time write which 
	//means it does not point to any pba. At this stage both lba entry lock and
	//hba entry lock are acquired. pba entry lock is not required here
	
	*********************************************************************************/
	if (hba_entry->hash_pba_index == 0) {	//A write for which there is no pba allocated for its hash
		if (lba_entry->hash_index == 0) {	//If there is no hash attached to this lba it means it is a first time write

			LOGD("STAGE_1:FRESH FIRST TIME WRITE @lba %lli\n", iorequest->ior_lba);

			// Proceed to New write with new pba allocated from list of free pbas
			DZ_DDE_FETCH_FREE_PBA_ENTRY(free_pba_entry, free_pba_index);
	
			//Set the Hash Collision Tag for this PBA
			DZ_DDE_UPDATE_PBA_TABLE_FOR_NEW_PBA(free_pba_entry);

			// Since it is a new pba so no locking is required for this pba,

			//Now Update HASH TAble
			//Set the new pba index to hba entry
			DZ_DDE_UPDATE_HASH_TABLE(hba_entry, free_pba_index);

			//Since the Hash is updated with its corresponding pba entry so now release the hash lock
			_DZ_DDE_RELEASE_HBA_LOCK(hba_entry);

			//Update LBA Table. This macro will automatically set Hash Collision Tag in that lba entry
			DZ_DDE_UPDATE_LBA_TABLE_FOR_NEW_PBA(lba_entry, ddinfo->new_hash_index);
		
			//free_pba_index is basically a physical block address(or rather physical disk lba) where the page data
			//needs to be written on the data disk.
			//Now get the corresponding place on the data disk by converting this free_pba_index
			//into sector because bio needs the sector. Page Data will go to this sector finally

			sector = dz_convert_lba_to_sector(free_pba_index);
			lba_entry->pba_index = free_pba_index;

			/*****************************************************************/
			//TODO: We need to make this atomic i.e. data and metadata write
			//should happen atomically
			//IO_METADATA_UPDATE(iorequest, free_pba_index, sector);
			/*****************************************************************/

			LOG("Case1:DDE:Phsyical Disk Write @ sector %lli for internal io \n", (U64)sector);

			//LBA Lock will be released inside this function
			//ddinfo will also be freed there

			IO_METADATA_UPDATE(iorequest, free_pba_index, sector);
			ret = dz_write_block_to_raw_disk_sync_internal(iorequest, sector, lba_entry);
			if (ret == SUCCESS) {
				//The above write is of PAGE_SIZE so increment the usage counters for 
				//physical and logical space
				ATOMIC64_INC(dedupe_cur_physical_space);
				ATOMIC64_INC(dedupe_cur_logical_space);
			} //TODO:What in case of failure

			//Now at this stage the top level application IO is completely written to the physical disk
			return;

		} else {  // Inner loop: When lba_entry->pba_index is NON zero
			//Its a overwrite case because pba_index is not zero. We need to find out two possibilities here
			//1. Overwrite with same data.
			//This can not happen because hash_pba_index is zero (first loop). It essentianly means a fresh
			//arrival of data whose hash was not existed. So this case is NOT applicable here
			//2. Overwrite with different data (and first time data)
			//This case is applicable here because it fits with hash_pba_index as zero.
			//This overwrite case will lead to removal of old entries associated with lba_entry 

			LOGD("Case2:DDE:OVERWRITE_WITH_DIFFERENT_DATA @lba %lli\n", iorequest->ior_lba);
			//Save the old entries because we might need to release/free up the corresponding pba entry
			old_lba_hash_index			= lba_entry->hash_index;
			old_lba_hash_collision_tag	= lba_entry->hash_collision_tag;

			//Allcoate a new free pba entry 
			DZ_DDE_FETCH_FREE_PBA_ENTRY(free_pba_entry, free_pba_index);

			//Update PBA Table.
			//Since it is a new pba so no pba locking is required for this pba.
			//Also the default starting Hash Collision Tag will be set for this pba
			DZ_DDE_UPDATE_PBA_TABLE_FOR_NEW_PBA(free_pba_entry);

			//Now update HASH table with the newly allocated pba entry index
			DZ_DDE_UPDATE_HASH_TABLE(hba_entry, free_pba_index);
			_DZ_DDE_RELEASE_HBA_LOCK(hba_entry);

			//Update LBA Table. Macro will automatically set Hash Collision Tag as well inside lba entry
			DZ_DDE_UPDATE_LBA_TABLE_FOR_NEW_PBA(lba_entry, ddinfo->new_hash_index);

			//Get the corresponding sector
			sector = dz_convert_lba_to_sector(free_pba_index);
			lba_entry->pba_index = free_pba_index;

			//TODO: We need to make this atomic i.e. data and metadata write
			//should happen atomically
			//IO_METADATA_UPDATE(iorequest, free_pba_index, sector);
			LOGD("Case2:DDE:Physical Disk Write @ sector %lli for internal io \n", (U64)sector);

			//////////////////////////////////////////////////////////////////////////////	
			//Note: LBA Lock is not yet released. It will be released inside this function
			//Note: ddinfo will also be freed there
			//////////////////////////////////////////////////////////////////////////////	
			ret = dz_write_block_to_raw_disk_sync_internal(iorequest, sector, lba_entry);
			if (ret == SUCCESS) {
				ATOMIC64_INC(dedupe_cur_physical_space);
				ATOMIC64_INC(dedupe_cur_logical_space);

				//Now we are done with overwriting with new data/hash to the phsycial disk.
				//Also note that the LBA lock is also released. It means we are done with releasing
				//all the locks.
				//Now process the old entries by using the old_lba_hash_index and tag 
				//Passing iorequest as NULL, because same iorequest can't be used
				//for cache addition and node deletion
				LOGD("Case2:DDE:Updating old entries\n");
				dz_dedupe_overwrite_update_old_entries(NULL, old_lba_hash_index, old_lba_hash_collision_tag);

			} else {
				//TODO:What happens when the disk write fails
				LOG("Case2:Disk Write Failed @ sector %lli for internal io \n", (U64)sector);
			}
			return;
		} //End of else part of When lba_entry->pba_index is NON zero
	}

	/*********************************************************************************
 									CheckStage_2:
	hash of inflight IO is already pointing to some pba i.e. having a write already.
	find out if inflight IO lba is having its WRITE for the first time or already
	*********************************************************************************/
	if (lba_entry->hash_index == 0) { 
		//Inflight IO lba is having its first time write because the lba entry is not
		//associated with any hash (hash_index/hash entry)

		//We can't blindly assign the current hash_pba_index (present in hash entry)
		//to lba because altough the hash is same for the current write, but it can be
		//happened due to collision as well. Note Two different writes can also generate
		//same hash. It essentially boils down to murmur hash function.
		//To really find out whether the same hash for the Inflight IO and the already
		//existing pba belongs to same data, we need to readback the existing pba entry
		//data from the physical disk and compare it with Inflight IO data byte by byte.
		//If comparison is same then we say hash is same because data is same
		//If comparison is different then we can say hash is same but ends in collision
	
		//If there is only entry in pba_entry_curr hash collision list
		//then no need to do a disk readback to compare the data
		//just assign that pba_index to lba, because lba is having
		//write (inflight IO) for first time
		pba_entry_curr = GET_PBA_ENTRY(hba_entry->hash_pba_index);
		_ACQUIRE_PBA_LOCK(pba_entry_curr);
		//Need to traverse all pbas in the collision list starts with hash_pba_index
		//Find that pba which matches the current data present in iorequest->bv_buffer
		//If matches then, it is a clear case of dedupe. Assign that pba to this lba
		//If NOT, then create a new pba and add it to the collision list, and assign
		//that pba to this lba
		//
		//match_pba_index = dz_dedupe_get_pba_index(iorequest, hba_entry->hash_pba_index);
		ret = dz_dedupe_get_pba_index(iorequest, hba_entry->hash_pba_index,
				&last_pba_index, &next_hash_collision_tag);

		switch (ret) {
			case DZ_DDE_WRITE_TYPE_DEDUPE: {

				LOGD("GET_PBA:DEDUPE:FRESH FIRST TIME WRITE EXT @lba %lli\n", iorequest->lba);
				pba_entry_dedupe = GET_PBA_ENTRY(ddinfo->dedupe_pba_index);

				//Update PBA Table
				//Since this pba entry is a duplicate one so just increment the reference count
				pba_entry_dedupe->ref_cnt++;

				//Update LBA Table, to use the hash collision tag
				//Update lba table to set the new hash index and collision tag
				DZ_DDE_UPDATE_LBA_TABLE(lba_entry, ddinfo->new_hash_index, pba_entry_dedupe->hash_collision_tag);

				_DZ_DDE_RELEASE_PBA_LOCK(pba_entry_curr);

				//Update HBA Table : None
				_DZ_DDE_RELEASE_HBA_LOCK(hba_entry);

				lba_entry->pba_index = ddinfo->dedupe_pba_index;

				_RELEASE_LBA_LOCK_WRITE(lba_entry);

				//Since this a dedupe case so just Done the IO request as there is no need of disk write here.
				DZ_DDE_IOREQUEST_DONE(iorequest);
				dz_dedupe_free(ddinfo);
				ATOMIC64_INC(dedupe_cur_logical_space);
				ATOMIC64_INC(dedupe_cur_duplicate_space);
				return;
			}

			case DZ_DDE_WRITE_TYPE_HASH_COLLISON: {
			//Comes here, because there is no entry in current hash collision list which
			//matches the current data. Need to create new pba for this data
			//

				LOGD("GET_PBA:HASH_COLLISON:OVERWRITE WITH DIFFERENT DATA @lba %lli\n", iorequest->lba);

				// Proceed to New write with new pba allocated
				DZ_DDE_FETCH_FREE_PBA_ENTRY(free_pba_entry, free_pba_index);
		
				//Update PBA Table
				//Add the new pba to the end of collision list
				DZ_DDE_ADD_NEW_PBA_TO_HASH_COLLISION_LIST(last_pba_index, free_pba_index);
				DZ_DDE_UPDATE_PBA_TABLE(free_pba_entry, next_hash_collision_tag );

				//Set the new hash index and the hash collision tag of the new 
				//pba entry for the lba
				//Update LBA Table
				DZ_DDE_UPDATE_LBA_TABLE(lba_entry, ddinfo->new_hash_index, next_hash_collision_tag);

				_DZ_DDE_RELEASE_PBA_LOCK(pba_entry_curr);

				//Update HASH Table
				//No need to update HASH Table as the new pba will be inserted
				//inside the collision of same hash
				_DZ_DDE_RELEASE_HBA_LOCK(hba_entry);


				sector = dz_convert_lba_to_sector(free_pba_index);
				lba_entry->pba_index = free_pba_index;
				//TODO: We need to make this atomic i.e. data and metadata write
				//should happen atomically
				//IO_METADATA_UPDATE(iorequest, free_pba_index, sector);
				LOGD("3:Disk Write @ sector %lli for internal io \n", (U64)sector);
				//LBA Lock will be released inside this function
				//ddinfo will also be freed there
				ret = dz_write_block_to_raw_disk_sync_internal(iorequest, sector, lba_entry);
				if (ret == SUCCESS) {
					ATOMIC64_INC(dedupe_cur_physical_space);
					ATOMIC64_INC(dedupe_cur_logical_space);
				}
				return;
			}//End of last case statement
		}//End of Switch statement
	} //End of lba_entry->hash_index == 0

	LOGD("lba->pba_index = %lli @lba %lli\n",(U64)lba_entry->pba_index, iorequest->ior_lba);
	LOGD("hba->hash_pba_index = %lli @lba %lli\n", (U64)hba_entry->hash_pba_index, iorequest->ior_lba);

	/********************************************************************************* 
 									CheckStage_3:
	* Comes here when lba_entry points to hash_index i.e lba
	* is also having existing writes and hash is also referring to existing pba_index
	* We need to find out the collision, overwrite and dedupe
	*
	*********************************************************************************/
	pba_entry_curr = GET_PBA_ENTRY(hba_entry->hash_pba_index);
	_ACQUIRE_PBA_LOCK(pba_entry_curr);

	//If old pba_index in lba refers to new pba then it is a overwrite with same
	//data, in that case no need to do a disk read back to compare the data
	if (lba_entry->pba_index == hba_entry->hash_pba_index) {

		//Update PBA Table : None
		_DZ_DDE_RELEASE_PBA_LOCK(pba_entry_curr);

		//Update HBA Table : None
		_DZ_DDE_RELEASE_HBA_LOCK(hba_entry);

		//Update LBA Table : None
		_RELEASE_LBA_LOCK_WRITE(lba_entry);
			
		DZ_DDE_IOREQUEST_DONE(iorequest);
		dz_dedupe_free(ddinfo);
		ATOMIC64_INC(dedupe_cur_logical_space);
		ATOMIC64_INC(dedupe_cur_duplicate_space);
		return;
	}

	/*
	//Trying to save one disk read because probability of collision is less
	//If the current entry is the only entry 
	if (pba_entry_curr->next_entry_in_collision_list == 0) {
		//pba_index will be different than stored in lba->pba_index so
		//there is no same data overwrite is ruled out
	}
	*/

	//Otherwise, we need to do the disk read for all pbas which starts with
	//hash_pba_index to find out overwrite, dedupe and collision

	ret = dz_dedupe_check_write_type(iorequest, hba_entry->hash_pba_index,
			&last_pba_index, &next_hash_collision_tag);

	switch(ret) {
	
		case DZ_DDE_WRITE_TYPE_OVERWRITE: {
			LOGD("DZ_DDE_WRITE_TYPE_OVERWRITE:OVERWRITE WITH SAME DATA @lba %lli\n", iorequest->ior_lba);
			//No need to do anything, i.e disk write, cache update etc.
			//just done the IOrequest

			//Update PBA Table : None
			_DZ_DDE_RELEASE_PBA_LOCK(pba_entry_curr);

			//Update HBA Table : None
			_DZ_DDE_RELEASE_HBA_LOCK(hba_entry);

			//Update LBA Table : None
			_RELEASE_LBA_LOCK_WRITE(lba_entry);
			
			DZ_DDE_IOREQUEST_DONE(iorequest);
			dz_dedupe_free(ddinfo);
			ATOMIC64_INC(dedupe_cur_logical_space);
			ATOMIC64_INC(dedupe_cur_duplicate_space);
			return;
		}

		case DZ_DDE_WRITE_TYPE_DEDUPE: {

			//No need to do anything, i.e disk write, cache update etc.
			LOGD("DDE_WRITE_TYPE_DEDUPE:OVERWRITE WITH DIFFERENT DATA @lba %lli\n", iorequest->ior_lba);
			// save the old entries for removal
			old_lba_hash_index			= lba_entry->hash_index;
			old_lba_hash_collision_tag	= lba_entry->hash_collision_tag;
			//Increment the iorequest so that it will not be freed after iorequest done.
			//This is needed because the same iorequest will be passed to bec cache
			//for deleting the node from cache
			iorequest_get(iorequest);		

			//Update PBA Table
			//Incrementing ref count of dedupe pba
			pba_entry_dedupe = GET_PBA_ENTRY(ddinfo->dedupe_pba_index);
			pba_entry_dedupe->ref_cnt++;

			DZ_DDE_UPDATE_LBA_TABLE(lba_entry, ddinfo->new_hash_index, pba_entry_dedupe->hash_collision_tag);
			_DZ_DDE_RELEASE_PBA_LOCK(pba_entry_curr);

			//Update HBA Table
			//No need to update in HBA table, because pba is already inserted in the
			//same hash
			_DZ_DDE_RELEASE_HBA_LOCK(hba_entry);

			//Update LBA Table
			//Update lba table to set the new hash index and collision tag
			lba_entry->pba_index = ddinfo->dedupe_pba_index;
			_RELEASE_LBA_LOCK_WRITE(lba_entry);
			
			//Done the IO request as there is no need of disk write here
			DZ_DDE_IOREQUEST_DONE(iorequest);
			LOGD("Updating old entries\n");
			dz_dedupe_overwrite_update_old_entries(iorequest, old_lba_hash_index, old_lba_hash_collision_tag);
			dz_dedupe_free(ddinfo);
			ATOMIC64_INC(dedupe_cur_logical_space);
			ATOMIC64_INC(dedupe_cur_duplicate_space);
			return;
		}
		case DZ_DDE_WRITE_TYPE_HASH_COLLISON: {

			LOGD("DZ_DDE_WRITE_TYPE_HASH_COLLISON:OVERWRITE WITH DIFFERENT DATA @lba %lli\n", iorequest->ior_lba);
			// save the old entries for removal
			old_lba_hash_index			= lba_entry->hash_index;
			old_lba_hash_collision_tag	= lba_entry->hash_collision_tag;

			// Proceed to New write with new pba allocated
			DZ_DDE_FETCH_FREE_PBA_ENTRY(free_pba_entry, free_pba_index);
		
			//Update PBA Table
			//Add the new pba to the end of collision list
			DZ_DDE_ADD_NEW_PBA_TO_HASH_COLLISION_LIST(last_pba_index, free_pba_index);
			DZ_DDE_UPDATE_PBA_TABLE(free_pba_entry, next_hash_collision_tag );

			//Set the new hash index and the hash collision tag of the new 
			//pba entry for the lba
			//Update LBA Table
			DZ_DDE_UPDATE_LBA_TABLE(lba_entry, ddinfo->new_hash_index, next_hash_collision_tag);

			_DZ_DDE_RELEASE_PBA_LOCK(pba_entry_curr);

			//Update HASH Table
			//No need to update HASH Table as the new pba will be inserted
			//inside the collision of same hash
			_DZ_DDE_RELEASE_HBA_LOCK(hba_entry);


			sector = dz_convert_lba_to_sector(free_pba_index);
			lba_entry->pba_index = free_pba_index;
			//TODO: We need to make this atomic i.e. data and metadata write
			//should happen atomically
			//IO_METADATA_UPDATE(iorequest, free_pba_index, sector);
			LOGD("4:Disk Write @ sector %lli for internal io \n", (U64)sector);
			//LBA Lock will be released inside this function
			//ddinfo will also be freed there
			ret = dz_write_block_to_raw_disk_sync_internal(iorequest, sector, lba_entry);
			if (ret == SUCCESS) {
				LOGD("Updating old entries\n");
				//Passing iorequest as NULL, because same iorequest can't be used
				//for cache addition and node deletion
				ATOMIC64_INC(dedupe_cur_physical_space);
				ATOMIC64_INC(dedupe_cur_logical_space);
				dz_dedupe_overwrite_update_old_entries(NULL, old_lba_hash_index, old_lba_hash_collision_tag);
			}
			return;
		}
	}

/*
fake_exit:
	_RELEASE_PBA_LOCK(pba_entry_curr);
	_RELEASE_HBA_LOCK(hba_entry);
	_RELEASE_LBA_LOCK_WRITE(lba_entry);
	DZ_DDE_IOREQUEST_DONE(iorequest);
	dz_dedupe_free(ddinfo);
	return;
*/
}

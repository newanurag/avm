/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "fec_engine.h"

extern PDZ_THREAD_POOL 	dedupe_thread_pool;
extern PDZ_THREAD_POOL 	read_thread_pool;

extern ATOMIC64			fec_memory_reads_count;
extern ATOMIC64			fec_forwarded_reads_count;
extern U64 				fec_table_size;
extern SPINLOCK			fec_active_lock;

//For operational purpose
extern PFEC_TABLE 		fec_tables_active;
extern PFEC_TABLE 		fec_tables_flush;
extern PLBA_BLOCK		lba_table;

RVOID dz_fec_align_read_io_direct(PIOREQUEST iorequest)
{
	PLBA_BLOCK	lba_entry	= NULL;
	PFEC_WRITE	fecw	= NULL;
	PBIO		bio 	= iorequest->ior_bio; // Parent or Original bio
	LBA			lba 	= iorequest->ior_lba;

	lba_entry = GET_LBA_ENTRY(lba);

	LOGD("Before:Read Request received at lba %lli\n", lba);
	_ACQUIRE_FEC_LOCK_READ(lba_entry);
	if (list_empty(&lba_entry->ioq_head)) {
		//Releasing lock will help new write IO to proceed
		//in addition to read, because write will be stored
		//in FEC engine, whereas Read will be served from dedupe engine
		//Data will still be consistent despite read and write on
		//same lba
		//TODO:More brainstorming required
		ATOMIC64_INC(fec_forwarded_reads_count);
		_RELEASE_FEC_LOCK_READ(lba_entry);
		//_RELEASE_LBA_LOCK_DEBUG(lba_entry, lba);
		dz_dedupe_read_io(iorequest);
		return;
	}
	BUG_ON(1);
start_processing:
	LOGD("After:Read Request received at lba %lli\n", lba);
	fecw = list_first_entry(&lba_entry->ioq_head, union dz_fec_data_write_s, ioq);
	//print_fecw(fecw);
	FEC_ENTRY_WRITE_LOCK(fecw);

	switch(fecw->state) {

		case FECBUF_STATE_IDLE:
		case FECBUF_STATE_ALLOCATED:
		case FECBUF_STATE_MEMORY_WRITE_ERROR:
		case FECBUF_STATE_FLUSH_COMPLETED:
			FEC_ENTRY_WRITE_UNLOCK(fecw);
			_RELEASE_FEC_LOCK_READ(lba_entry);
			ATOMIC64_INC(fec_forwarded_reads_count);
			dz_dedupe_read_io(iorequest);
		return;

		case FECBUF_STATE_MEMORY_WRITE_IN_PROGRESS:
			LOGFEC("Write in progress for lba %lli\n", fecw->lba);
			FEC_ENTRY_WRITE_UNLOCK(fecw);
			DELAY_MILLI_SECONDS(100);
		goto start_processing;
	}


	fecw->read_ref_cnt++;

	if (fecw->read_ref_cnt >= DZ_FEC_CONCURRENT_READS_LIMIT) {
		fecw->read_ref_cnt--;

		FEC_ENTRY_WRITE_UNLOCK(fecw);
		_RELEASE_FEC_LOCK_READ(lba_entry);

		DELAY_MILLI_SECONDS(100);

		IO_DONE_BUSY(iorequest);
		iorequest_put(iorequest);
		return;
	}

	COPY_TO_BIO(bio, fecw->bv_page);
	fecw->read_ref_cnt--;

	FEC_ENTRY_WRITE_UNLOCK(fecw);
	_RELEASE_FEC_LOCK_READ(lba_entry);

	ATOMIC64_INC(fec_memory_reads_count);

	//print_biom(iorequest->ior_bio, "direct bio");
	if (iorequest->ior_parent) {
		//print_biom(iorequest->parent->ior_bio, "parent bio");
		READ_IO_DONE(iorequest);
		//Just done the child bio.
		//Iorequest will be freed in the child biodone
		return;
	}
	READ_IO_DONE(iorequest);
	iorequest_put(iorequest);
	
	return;

	//This is the case where the read came when state was
	//write in progress, but after that it got flushed and 
	//read is now looking into dedupe engine. Its very very
	//rare but can be possible
	//TODO:Revisit later
	//_RELEASE_FEC_LOCK_READ(lba_entry);

	//Calling directly instead of queuing it.
	//Because it will add latency to read performance
	//_RELEASE_FEC_LOCK_READ(lba_entry);
	//dz_dedupe_read_io(iorequest);
}

RVOID dz_fec_read_io(PIOREQUEST iorequest)
{
	LOGD("Read Request received at lba %lli\n", lba);
	dz_fec_align_read_io(iorequest);
	return;
}

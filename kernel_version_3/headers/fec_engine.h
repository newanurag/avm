/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _FEC_ENGINE_H
#define _FEC_ENGINE_H

#include "prototypes.h"
#define	DZ_FEC_CONCURRENT_READS_LIMIT		100

#define	PMEMCPY		memcpy

#define ONE_MILLION_IOPS	1024*1024 
#define TWO_MILLION_IOPS	2*(ONE_MILLION_IOPS)
#define DZ_FEC_FLUSH_INTERVAL	15 //Flush interval in secs i.e. 15 seconds

#define	FEC_ENTRY_WRITE_LOCK(fecb)			bit_spin_lock(0, &(fecb)->donotchange)
#define	FEC_ENTRY_WRITE_UNLOCK(fecb)		bit_spin_unlock(0, &(fecb)->donotchange)

#define	FEC_ENTRY_READ_LOCK(fecb)			bit_spin_lock(0, &(fecb)->donotchange)
#define	FEC_ENTRY_READ_UNLOCK(fecb)			bit_spin_unlock(0, &(fecb)->donotchange)

//#define GET_FEC_READ_ENTRY(lba) 			&fec_table_read[((lba) % (fec_table_size - 1))]; // Take module
//#define GET_FEC_WRITE_ENTRY(index) 		&fec_table_write[index]

#define GET_FEC_READ_ENTRY(lba, table)		&table[(lba) % (fec_table_size - 1)]; // Take module
#define GET_FEC_WRITE_ENTRY(index, table)	&table[(index)]

#define GET_FEC_ACTIVE_WRITE_ENTRY(index)	&(fec_tables_active->table_write[(index)])
#define GET_FEC_ACTIVE_READ_ENTRY(index)	&(fec_tables_active->table_read[(index)])

#define GET_FEC_FLUSH_ENTRY(index)			&(fec_tables_flush->table_write[(index)])
#define GET_FEC_FLUSH_READ_ENTRY(index)		&(fec_tables_flush->table_read[(index)])

#define SET_FEC_ACTIVE_WRITE_ENTRY(index)	(fec_tables_active->table_write[(index)])

#define	FEC_ADD_WRITE_BUFFER_TO_LBA_ENTRY_LIST(new_entry, head_of_entry_list)	\
	list_add(&(new_entry)->ioq, &(head_of_entry_list)->ioq_head);

#define	FEC_ADD_WRITE_BUFFER_TO_LBA_TABLE(buffer, lba)	\
{												\
	PLBA_BLOCK lba_entry = GET_LBA_ENTRY((lba));	\
	_ACQUIRE_FEC_LOCK_WRITE(lba_entry);				\
	list_add_tail(&(buffer)->ioq, &lba_entry->ioq_head);	\
	_RELEASE_FEC_LOCK_WRITE(lba_entry);				\
}

#define	FEC_DELETE_WRITE_BUFFER_FROM_LBA_TABLE(buffer)	\
{												\
	PLBA_BLOCK lba_entry = GET_LBA_ENTRY((buffer)->lba);	\
	_ACQUIRE_FEC_LOCK_WRITE(lba_entry);				\
	list_del(&(buffer)->ioq);					\
	_RELEASE_FEC_LOCK_WRITE(lba_entry);				\
}

#define	FEC_DELETE_WRITE_BUFFER_FROM_LBA_TABLE_OLD(buffer)	\
{												\
	PLBA_BLOCK lba_entry = GET_LBA_ENTRY((buffer)->lba);	\
	_ACQUIRE_LBA_LOCK_WRITE(lba_entry);				\
	list_del(&(buffer)->ioq);					\
	_RELEASE_LBA_LOCK_WRITE(lba_entry);				\
}

#define	SET_FEC_BUFFER_STATE(buffer, state_val)	\
		FEC_ENTRY_WRITE_LOCK(buffer);			\
		(buffer)->state = (state_val);			\
		FEC_ENTRY_WRITE_UNLOCK(buffer);		


#ifdef DZ_KERNEL_VERSION_4


#define COPY_FROM_BIO(buffer, bio) \
{															\
	INT i, retry_cnt = 0;									\
	PBIOVEC	bvec		= NULL;								\
	PVOID   	spagebuf= NULL;								\
	PVOID   	dpagebuf= (buffer);							\
	INT			bv_len	= 0;								\
	bio_for_each_segment(bvec, (bio), i) {					\
		bv_len	= bvec->bv_len;								\
		spagebuf = kmap(bvec->bv_page);						\
		spagebuf += bvec->bv_offset;						\
		PMEMCPY(dpagebuf, spagebuf, bv_len);				\
		kunmap(bvec->bv_page);								\
		if (bv_len < PAGE_SIZE ) {							\
			dpagebuf += bv_len;								\
		}													\
		if (retry_cnt++ > 8) {								\
			LOGE("Error in bio vec: retry_cnt = %d\n", retry_cnt);\
			BUG_ON(1);										\
		}													\
	} 														\
}

#define COPY_TO_BIO(bio, buffer) \
{												\
	struct bvec_iter iter;						\
	BIOVEC	bvec;								\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= (buffer);				\
	INT			bv_len	= 0;					\
	bio_for_each_segment(bvec, (bio), iter) {	\
		bv_len	= bvec.bv_len;					\
		dpagebuf = kmap(bvec.bv_page);			\
		dpagebuf += bvec.bv_offset;				\
		/* PRINT_INT(bvec.bv_len);	 */ 		\
		/* PRINT_INT(bvec.bv_offset); */		\
		PMEMCPY(dpagebuf, spagebuf, bv_len);	\
		kunmap(bvec.bv_page);					\
		if (bv_len < PAGE_SIZE ) {				\
			spagebuf += bv_len;					\
		}										\
	} 											\
}

#define COPY_BUFFER_TO_PAGE(page, buffer) \
{												\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= (buffer);				\
	dpagebuf = kmap((page));					\
	PMEMCPY(dpagebuf, spagebuf, PAGE_SIZE);		\
	kunmap((page));								\
}
#define COPY_PAGE_TO_BUFFER(buffer, page) \
{												\
	PVOID   	spagebuf= NULL;					\
	spagebuf = kmap((page));					\
	PMEMCPY((buffer), spagebuf, PAGE_SIZE);		\
	kunmap((page));								\
}

#define COPY_PAGE_TO_BIO(bio, src_page) 		\
{												\
	struct bvec_iter iter;						\
	BIOVEC	bvec;								\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= NULL;    				\
	INT			bv_len	= 0;					\
	ULONG		flags	= 0;					\
	spagebuf = kmap(src_page);					\
	bio_for_each_segment(bvec, (bio), iter) {	\
		bv_len	= bvec.bv_len;					\
		dpagebuf = kmap(bvec.bv_page);			\
		dpagebuf += bvec.bv_offset;				\
		PMEMCPY(dpagebuf, spagebuf, bv_len);	\
		kunmap(bvec.bv_page);					\
		if (bv_len < PAGE_SIZE ) {				\
			spagebuf += bv_len;					\
		}										\
	} 											\
	kunmap(src_page);							\
}

#define COPY_PAGE_TO_BIO_ATOMIC(bio, src_page, status) 	\
{												\
	struct bvec_iter iter;						\
	BIOVEC	bvec;								\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= NULL;    				\
	INT			bv_len	= 0;					\
	ULONG		flags	= 0;					\
	spagebuf = kmap_atomic(src_page);			\
	bio_for_each_segment(bvec, (bio), iter) {	\
		bv_len	= bvec.bv_len;					\
	    dpagebuf = bvec_kmap_irq(&bvec, &flags);\
		if (!dpagebuf) {						\
			status = FALSE;						\
			LOGW("bvec_kmap_irq failed\n");		\
			/*TODO:*/							\
			BUG_ON(1);							\
			break;								\
		}										\
		PMEMCPY(dpagebuf, spagebuf, bv_len);	\
		bvec_kunmap_irq(dpagebuf, &flags);		\
		if (bv_len < PAGE_SIZE ) {				\
			spagebuf += bv_len;					\
		}										\
	} 											\
	kunmap_atomic(spagebuf);					\
}

#define COPY_SINGLE_PAGE_FROM_BIO(dst_page, bio) 		\
{												\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= NULL;    				\
	BIOVEC		 bvec	= bio_iovec((bio));		\
	dpagebuf = kmap((dst_page));					\
	spagebuf = kmap(bvec.bv_page);				\
	spagebuf += bvec.bv_offset;				\
	PMEMCPY(dpagebuf, spagebuf, PAGE_SIZE);		\
	kunmap(bvec.bv_page);						\
	kunmap(dst_page);							\
}

#define COPY_PAGE_FROM_BIO(dst_page, bio) 		\
{												\
	BIOVEC	bvec;								\
	struct bvec_iter iter;						\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= NULL;    				\
	INT			bv_len	= 0;					\
	dpagebuf = kmap(dst_page);					\
	bio_for_each_segment(bvec, (bio), iter) {		\
		bv_len	= bvec.bv_len;					\
		spagebuf = kmap(bvec.bv_page);			\
		spagebuf += bvec.bv_offset;			\
		PMEMCPY(dpagebuf, spagebuf, bv_len);	\
		kunmap(bvec.bv_page);					\
		if (bv_len < PAGE_SIZE ) {				\
			dpagebuf += bv_len;					\
		}										\
	} 											\
	kunmap(dst_page);							\
}

#define COPY_FROM_BIO_PARTIAL(fecw, bio) \
	{															\
		struct bvec_iter iter;									\
		BIOVEC	bvec;											\
		PVOID   	pagebuf	= NULL;								\
		bio_for_each_segment(bvec, (bio), iter) {				\
			(fecw)->bv_len = bvec.bv_len;						\
			(fecw)->bv_offset = bvec.bv_offset;					\
			pagebuf = kmap(bvec.bv_page);						\
			if (bvec.bv_offset == 0) {							\
				memcpy((fecw).bv_page, pagebuf, bvec.bv_len);	\
			} else {											\
				BUG_ON(1);										\
			}													\
			kunmap(bvec.bv_page);								\
		}														\
	}


#define COPY_TO_BIO_PARTIAL(bio, fecw) 							\
	{															\
		struct bvec_iter iter;									\
		BIOVEC	bvec;											\
		PVOID   	pagebuf	= NULL;								\
		bio_for_each_segment(bvec, (bio), iter) {				\
			pagebuf = kmap(bvec.bv_page);						\
			if (bvec.bv_offset == 0) {							\
				memcpy(pagebuf, (fecw).bv_page, (fecw).bv_len);\
			} else {											\
				BUG_ON(1);										\
			}													\
			kunmap(bvec.bv_page);								\
		}														\
	}

#else 


#define COPY_FROM_BIO(buffer, bio) \
{															\
	INT i, retry_cnt = 0;									\
	PBIOVEC	bvec		= NULL;								\
	PVOID   	spagebuf= NULL;								\
	PVOID   	dpagebuf= (buffer);							\
	INT			bv_len	= 0;								\
	bio_for_each_segment(bvec, (bio), i) {					\
		bv_len	= bvec->bv_len;								\
		spagebuf = kmap(bvec->bv_page);						\
		spagebuf += bvec->bv_offset;						\
		PMEMCPY(dpagebuf, spagebuf, bv_len);				\
		kunmap(bvec->bv_page);								\
		if (bv_len < PAGE_SIZE ) {							\
			dpagebuf += bv_len;								\
		}													\
		if (retry_cnt++ > 8) {								\
			LOGE("Error in bio vec: retry_cnt = %d\n", retry_cnt);\
			BUG_ON(1);										\
		}													\
	} 														\
}

#define COPY_TO_BIO(bio, buffer) \
{												\
	INT i;										\
	PBIOVEC	bvec		= NULL;					\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= (buffer);				\
	INT			bv_len	= 0;					\
	bio_for_each_segment(bvec, (bio), i) {		\
		bv_len	= bvec->bv_len;					\
		dpagebuf = kmap(bvec->bv_page);			\
		dpagebuf += bvec->bv_offset;			\
		/* PRINT_INT(bvec->bv_len);	 */ 		\
		/* PRINT_INT(bvec->bv_offset); */		\
		PMEMCPY(dpagebuf, spagebuf, bv_len);	\
		kunmap(bvec->bv_page);					\
		if (bv_len < PAGE_SIZE ) {				\
			spagebuf += bv_len;					\
		}										\
	} 											\
}

#define COPY_BUFFER_TO_PAGE(page, buffer) \
{												\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= (buffer);				\
	dpagebuf = kmap((page));					\
	PMEMCPY(dpagebuf, spagebuf, PAGE_SIZE);		\
	kunmap((page));								\
}
#define COPY_PAGE_TO_BUFFER(buffer, page) \
{												\
	PVOID   	spagebuf= NULL;					\
	spagebuf = kmap((page));					\
	PMEMCPY((buffer), spagebuf, PAGE_SIZE);		\
	kunmap((page));								\
}

#define COPY_PAGE_TO_BIO(bio, src_page) 		\
{												\
	INT i;										\
	PBIOVEC	bvec		= NULL;					\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= NULL;    				\
	INT			bv_len	= 0;					\
	ULONG		flags	= 0;					\
	spagebuf = kmap(src_page);					\
	bio_for_each_segment(bvec, (bio), i) {		\
		bv_len	= bvec->bv_len;					\
		dpagebuf = kmap(bvec->bv_page);			\
		dpagebuf += bvec->bv_offset;			\
		PMEMCPY(dpagebuf, spagebuf, bv_len);	\
		kunmap(bvec->bv_page);					\
		if (bv_len < PAGE_SIZE ) {				\
			spagebuf += bv_len;					\
		}										\
	} 											\
	kunmap(src_page);							\
}

#define COPY_PAGE_TO_BIO_ATOMIC(bio, src_page, status) 	\
{												\
	INT i;										\
	PBIOVEC	bvec		= NULL;					\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= NULL;    				\
	INT			bv_len	= 0;					\
	ULONG		flags	= 0;					\
	spagebuf = kmap_atomic(src_page);			\
	bio_for_each_segment(bvec, (bio), i) {		\
		bv_len	= bvec->bv_len;					\
	    dpagebuf = bvec_kmap_irq(bvec, &flags);	\
		if (!dpagebuf) {						\
			status = FALSE;						\
			LOGW("bvec_kmap_irq failed\n");		\
			/*TODO:*/							\
			BUG_ON(1);							\
			break;								\
		}										\
		PMEMCPY(dpagebuf, spagebuf, bv_len);	\
		bvec_kunmap_irq(dpagebuf, &flags);		\
		if (bv_len < PAGE_SIZE ) {				\
			spagebuf += bv_len;					\
		}										\
	} 											\
	kunmap_atomic(spagebuf);					\
}

#define COPY_SINGLE_PAGE_FROM_BIO(dst_page, bio) 		\
{												\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= NULL;    				\
	PBIOVEC		 bvec	= bio_iovec((bio));		\
	dpagebuf = kmap((dst_page));					\
	spagebuf = kmap(bvec->bv_page);				\
	spagebuf += bvec->bv_offset;				\
	PMEMCPY(dpagebuf, spagebuf, PAGE_SIZE);		\
	kunmap(bvec->bv_page);						\
	kunmap(dst_page);							\
}

#define COPY_PAGE_FROM_BIO(dst_page, bio) 		\
{												\
	INT i;										\
	PBIOVEC	bvec		= NULL;					\
	PVOID   	dpagebuf= NULL;					\
	PVOID   	spagebuf= NULL;    				\
	INT			bv_len	= 0;					\
	dpagebuf = kmap(dst_page);					\
	bio_for_each_segment(bvec, (bio), i) {		\
		bv_len	= bvec->bv_len;					\
		spagebuf = kmap(bvec->bv_page);			\
		spagebuf += bvec->bv_offset;			\
		PMEMCPY(dpagebuf, spagebuf, bv_len);	\
		kunmap(bvec->bv_page);					\
		if (bv_len < PAGE_SIZE ) {				\
			dpagebuf += bv_len;					\
		}										\
	} 											\
	kunmap(dst_page);							\
}

#define COPY_FROM_BIO_PARTIAL(fecw, bio) \
	{															\
		INT i;													\
		PBIOVEC	bvec		= NULL;								\
		PVOID   	pagebuf	= NULL;								\
		bio_for_each_segment(bvec, (bio), i) {					\
			(fecw)->bv_len = bvec->bv_len;						\
			(fecw)->bv_offset = bvec->bv_offset;				\
			pagebuf = kmap(bvec->bv_page);						\
			if (bvec->bv_offset == 0) {							\
				memcpy((fecw)->bv_page, pagebuf, bvec->bv_len);	\
			} else {											\
				BUG_ON(1);										\
			}													\
			kunmap(bvec->bv_page);								\
		}														\
	}


#define COPY_TO_BIO_PARTIAL(bio, fecw) 							\
	{															\
		INT i;													\
		PBIOVEC	bvec		= NULL;								\
		PVOID   	pagebuf	= NULL;								\
		bio_for_each_segment(bvec, (bio), i) {					\
			pagebuf = kmap(bvec->bv_page);						\
			if (bvec->bv_offset == 0) {							\
				memcpy(pagebuf, (fecw)->bv_page, (fecw)->bv_len);\
			} else {											\
				BUG_ON(1);										\
			}													\
			kunmap(bvec->bv_page);								\
		}														\
	}


#endif //End of ifdef DZ_KERNEL_VERSION_4

typedef union dz_fec_data_write_s {
	struct {
		UINT	write_lock:1;		
		UINT	state:7;		
#define	FECBUF_STATE_IDLE						0
#define	FECBUF_STATE_ALLOCATED					1
#define	FECBUF_STATE_MEMORY_WRITE_IN_PROGRESS	2
#define	FECBUF_STATE_MEMORY_WRITE_COMPLETED		3
#define	FECBUF_STATE_MEMORY_WRITE_ERROR			4
#define	FECBUF_STATE_DEDUPE_WRITE_IN_PROGRESS	5
#define	FECBUF_STATE_DEDUPE_WRITE_COMPLETED		6
#define	FECBUF_STATE_DEDUPE_WRITE_ERROR			7
#define	FECBUF_STATE_FLUSH_STARTED				8
#define	FECBUF_STATE_FLUSH_COMPLETED			9
#define	FECBUF_STATE_FLUSH_ERROR				10
#define	FECBUF_STATE_REPLAY_STARTED				11
#define	FECBUF_STATE_REPLAY_COMPLETED			12

		UINT	read_ref_cnt:24;		// Note that the above two and this variable occupies 32 bit only.
										// This restricts wastage of bits

		CHAR	bv_page[PAGE_SIZE];		// Page represents Application IO Data
		U64		lba;
		DZ_COMPLETION   		io_completion;
		U64		index;	//Index no. in Free buffer bitmap

		//A queue whose head owned by flush_table. Used for maintaing sequence while flushing 
		LIST_HEAD	flushq; 

		//A queue whose head owned by lba_table i.e. storing for same lba writes
		LIST_HEAD	ioq; 
	}__attribute__((__packed__));
	ULONG	donotchange; //Do not change this variable as it is used for bit_spin_lock
}__attribute__((__packed__)) FEC_WRITE, *PFEC_WRITE;


typedef struct dz_fec_table_s {

	union {
		PFEC_WRITE	write_table;
		PFEC_WRITE	table_write;
	};
	union {
		U64				total_active_cnt;
		U64				total_flush_cnt;
		U64				total_active_count;
		U64				total_flush_count;
	};
	BOOL				flush_in_progress;
	LIST_HEAD			flushq_head;

} FEC_TABLE, *PFEC_TABLE;

//Function Prototypes
//PFECDATA GET_FEC_FREE_WRITE_ENTRY(PIOREQUEST);
PFEC_WRITE 	GET_FEC_SINGLE_FREE_BUFFER(VOID);
PFEC_WRITE 	GET_FEC_MULTIPLE_FREE_BUFFERS(INT num_bufs);
INT 		dz_fec_engine_init(VOID);
RVOID 		dz_fec_engine_exit(VOID);
RVOID 		dz_fec_io(PIOREQUEST iorequest);
RVOID 		dz_fec_replay_io(PIOREQUEST iorequest);
RVOID 		dz_fec_table_reset(PFEC_TABLE table);

//Read Functions
RVOID 		dz_fec_read_io(PIOREQUEST iorequest);
RPVOID 		dz_fec_data_read_alloc(VOID);
RVOID 		dz_fec_data_read_free(PVOID ptr);
PMEMPOOL 	dz_create_fec_data_read_pool(UINT num_entries);
RVOID 		dz_destroy_fec_data_read_pool(PMEMPOOL pool, UINT num_entries);
PVOID 		dz_create_fec_data_read_table(U64 table_size);
RVOID 		dz_destroy_fec_data_read_table(PVOID ptr);

//Write Functions
RVOID 		dz_fec_write_io(PIOREQUEST iorequest);
PVOID 		dz_create_fec_data_write_table(U64 table_size);
RVOID 		dz_destroy_fec_data_write_table(PVOID ptr, U64);

//Flush functions
RVOID 		dz_fec_flush_io(PIOREQUEST iorequest);
RVOID 		dz_flush_biodone(PBIO bio, INT error);
PIOREQUEST 	dz_fec_create_flush_bio(PFEC_WRITE fec_data_entry);
PIOREQUEST 	dz_fec_create_flush_io(PFEC_WRITE fec_data_entry);
RINT 		dz_fec_flush_timer(PVOID data);
RVOID 		dz_fec_flush_timer_stop(VOID);
RVOID 		dz_fec_flush_switchover(VOID);

//Alignment Engine
RVOID dz_far_parent_biodone(PBIO bio, INT error);
RVOID dz_far_multi_block_head_unaligned_biodone(PBIO bio, INT error);
RVOID dz_far_multi_block_tail_unaligned_biodone(PBIO bio, INT error);
RVOID dz_far_multi_block_both_aligned_biodone(PBIO bio, INT error);
RVOID dz_far_for_single_block_biodone_tail(PBIO bio, INT error);
RVOID dz_far_for_single_block_biodone_head(PBIO bio, INT error);
RVOID dz_far_for_partial_block_biodone_tail(PBIO bio, INT error);
RVOID dz_far_for_partial_block_biodone_head(PBIO bio, INT error);
RVOID dz_far_for_partial_block_biodone(PBIO bio, INT error);
RVOID dz_fec_align_read_single_page_async(PIOREQUEST parent_iorequest, BIODONE done);
RVOID dz_fec_align_read_double_page_async(PIOREQUEST parent_iorequest,
		BIODONE done_head, BIODONE done_tail);

RVOID dz_faw_for_partial_block_biodone_head(PBIO bio, INT error);
RVOID dz_faw_for_partial_block_biodone_tail(PBIO bio, INT error);
RVOID dz_far_multi_block_both_unaligned(PIOREQUEST piorequest);
RVOID dz_far_multi_block_head_unaligned(PIOREQUEST piorequest);
RVOID dz_far_multi_block_tail_unaligned(PIOREQUEST piorequest);
RVOID dz_far_multi_block_both_aligned(PIOREQUEST piorequest);
RVOID dz_fec_align_read_for_partial_block(PIOREQUEST piorequest);
RVOID dz_fec_align_read_for_single_block(PIOREQUEST piorequest);
RINT  dz_fec_align_read_for_multi_block(PIOREQUEST piorequest);
RVOID dz_fec_align_read_io(PIOREQUEST piorequest);
RVOID dz_fec_align_read_io_direct(PIOREQUEST iorequest);

RVOID dz_faw_parent_biodone(PBIO bio, INT error);
RVOID dz_fec_align_write_double_page_async_biodone_head(PBIO bio, INT error);
RVOID dz_fec_align_read_double_page_async_biodone_tail(PBIO bio, INT error);
RVOID dz_fec_align_write_double_page_async(PIOREQUEST , SECTOR , BIODONE , BIODONE );
RVOID 		dz_fec_align_read_page_async_parent_biodone(PBIO , INT );
RVOID 		dz_fec_align_read_page_sync_biodone(PBIO , INT );
RVOID 		dz_fec_align_read_page_async_biodone(PBIO , INT );
RVOID 		dz_fec_align_read_page_async_biodone_head(PBIO , INT );
RVOID 		dz_fec_align_read_page_async_biodone_tail(PBIO , INT );
PPAGE  		dz_fec_align_read_page_sync(PIOREQUEST, PBIO, SECTOR);
PIOREQUEST	dz_fec_align_read_page_async(PIOREQUEST, PBIO, SECTOR, PBIOVEC, BIODONE );
RVOID 		dz_fec_align_read_page_async2(PIOREQUEST parent_iorequest, BIODONE done);

RVOID  		dz_fec_align_write_for_multi_block(PIOREQUEST);
RVOID 		dz_faw_multi_block_all_aligned(PIOREQUEST);
RVOID 		dz_faw_multi_block_tail_unaligned(PIOREQUEST);
RVOID 		dz_faw_multi_block_head_unaligned(PIOREQUEST);
RVOID 		dz_faw_multi_block_both_unaligned(PIOREQUEST);
PIOREQUEST 	dz_faw_multi_block_read_page_async(PIOREQUEST, PBIO, SECTOR, PBIOVEC, BIODONE);
RVOID 		dz_faw_multi_page_read_page_async_biodone_tail(PBIO, INT);
RVOID 		dz_faw_multi_page_read_page_async_biodone_head(PBIO, INT);
RVOID 		dz_faw_multi_block_read_page_async_parent_biodone(PBIO, INT);

RVOID 		dz_fec_align_write_for_partial_block(PIOREQUEST );
PPAGE 		dz_faw_partial_block_read_page_sync(PIOREQUEST, PBIO, SECTOR);
RVOID 		dz_faw_partial_block_read_page_sync_biodone(PBIO, INT);
RVOID 		dz_faw_partial_block_double_page_async(PIOREQUEST, SECTOR, BIODONE, BIODONE);
RVOID 		dz_faw_partial_block_double_page_async_biodone_head(PBIO, INT);
RVOID 		dz_faw_partial_block_double_page_async_biodone_tail(PBIO, INT);
RVOID 		dz_faw_partial_block_parent_biodone(PBIO, INT);

RVOID 		dz_fec_align_write_for_single_block(PIOREQUEST );
RVOID 		dz_faw_single_block_double_page_async(PIOREQUEST, SECTOR, BIODONE, BIODONE);
RVOID 		dz_faw_single_block_double_page_async_biodone_head(PBIO, INT);
RVOID 		dz_faw_single_block_double_page_async_biodone_tail(PBIO bio, INT error);
RVOID 		dz_faw_single_block_iorequest_done(PVOID , INT );
RVOID 		dz_faw_single_block_parent_biodone(PBIO , INT );
RVOID 		dz_fec_align_write_io(PIOREQUEST );
RVOID 		dz_fec_align_write_both_aligned(PIOREQUEST );
RVOID 		dz_fec_align_write_tail_unaligned(PIOREQUEST );
RVOID 		dz_fec_align_write_head_unaligned(PIOREQUEST );
RVOID 		dz_fec_align_write_both_unaligned(PIOREQUEST );

//Print functions
#define print_fecw(fecw)	print_fecws((fecw), NULL)
RVOID print_fecws(PFEC_WRITE fecw, CHAR *str);

RINT dz_bitmap_init(PBITMAP bitmap, U64 table_size);
S64 dz_fec_get_free_buffer_index(U64 table_size);
RINT dz_fec_put_buffer_index(U64 index, U64 table_size);
RVOID dz_fec_add_write_buffer_to_flushq(PFEC_WRITE fecw);
RVOID dz_fec_create_flush_iorequest(VOID);
RVOID dz_fec_init_counters(VOID);
#endif //_FEC_ENGINE_H

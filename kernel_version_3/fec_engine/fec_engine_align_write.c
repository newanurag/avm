/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "fec_engine.h"
//For operational purpose
extern ATOMIC64 fec_iocount_writes_partial_page;
extern ATOMIC64 fec_iocount_writes_single_page;
extern ATOMIC64 fec_iocount_writes_single_aligned_page;
extern ATOMIC64 fec_iocount_writes_single_unaligned_page;
extern ATOMIC64 fec_iocount_writes_multi_page;
extern U64 				fec_table_size;
extern U64				fec_active_count;
extern SPINLOCK			fec_active_lock;
extern SPINLOCK			fec_flush_lock;
extern BITMAP 			fec_buffer_bitmap;
extern PFEC_TABLE 		fec_tables_active;
extern PFEC_TABLE 		fec_tables_flush;
extern PIOREQUEST		fec_flush_iorequest;

extern U64 		time_fec_write_start;
extern U64 		time_fec_write_switch;
extern U64		fec_active_threshold;

//Need to return Retry/Busy status when previous flush is not yet completed
//Here we are initiating the flush process if the active count reaches
//to its max capacity.
//If a flush operation is started from timer context, then it may so happen 
//that that the active count is still less than max capacity. 
//However, the timer will reset the active count and switchover the tables.
//But that change will not impact this functionality because here it will get the
//modifed counter and tables.

inline PFEC_WRITE GET_FEC_SINGLE_FREE_BUFFER(VOID)
{
		U64 		tmp_count		= 0;
		PFEC_WRITE	write_entry		= NULL;
		U64			index			= 0;


		spin_lock(&fec_active_lock);

		tmp_count = fec_active_count++;
		//LOGFEC("Tmp_count =%lli\n", tmp_count);
		index = dz_fec_get_free_buffer_index(fec_table_size);

		//TODO: We need a good logic here of dynamically sleeping
		//for durations based on the current flush rate

		if ((index == FAILURE) || (fec_active_count > fec_active_threshold)) {
			//dz_fec_flush_switchover();
			GET_TIME_STAMP(time_fec_write_switch);
			fec_tables_flush->flush_in_progress = TRUE;
			fec_active_count = 0;
			spin_unlock(&fec_active_lock);			
			LOGFEC("Time To start flush thread. Index = %lli, Active Count = %lli, Threshold = %lli\n",
				index, fec_active_count, fec_active_threshold);
			//dz_q_iorequest(fec_flush_iorequest->ior_thread_pool, fec_flush_iorequest);
			dz_fec_create_flush_iorequest();
			//Sleep for more seconds relatively if no free bitmap bits
			if (index == FAILURE) {
				SLEEP_MILLI_SECONDS(3000);
			} else {
				SLEEP_MILLI_SECONDS(1000);
			}
			RETURNN;
		}
		write_entry 		= GET_FEC_ACTIVE_WRITE_ENTRY(index);
		if (!write_entry) {
			LOGFECE("Write Entry NULL. Index = %lli, Active Count = %lli, Threshold = %lli\n",
				index, fec_active_count, fec_active_threshold);
			SLEEP_MILLI_SECONDS(1000);
			RETURNN;
		}
		MEMSET(write_entry);
		write_entry->state 	= FECBUF_STATE_ALLOCATED;
		write_entry->index	= index;

		if (fec_tables_flush->flush_in_progress) {
			DELAY_MILLI_SECONDS(2);	
			//DELAY_MILLI_SECONDS(7);	
			// 7 is based on this calculation
			// Disk write rate 640KB/sec
			// i.e. 160 no. of 4K size ios takes 1000 milli seconds roughly
			// i.e. 1 IO takes 6+ milli seconds
			// hence rounded off to 7
		}

		if (fec_active_count == 1) {
			GET_TIME_STAMP(time_fec_write_start);
		}
		
		spin_unlock(&fec_active_lock);			

		return 	write_entry;
}

//This function will return the starting address of first
//write buffer from the list of free buffers.
//If there are insufficient buffers in the list then it will flush
//the current active buffers and allocate from new active buffers.
//
inline PFEC_WRITE GET_FEC_MULTIPLE_FREE_BUFFERS(INT num_bufs)
{
		//Initial count i.e. just before incrementing
		U64 		final_count		= 0;
		PFEC_WRITE	write_entry		= NULL;

		spin_lock(&fec_active_lock);
		final_count = fec_active_count + num_bufs;
		//LOGFEC("final_count =%lli\n", final_count);

		if (final_count > fec_table_size) {
			//LOGFEC("final_count =%lli\n", final_count);
			
			//Although its very rare that by the time active buffer is filled
			//there may be any reads pending on previously active buffer which became
			//flush buffer.
			if (fec_tables_flush->flush_in_progress) {
				//We should return the IOS with retry status. So that application should
				//retry them instead of direct failing. Similar to SCSI_BUSY
				spin_unlock(&fec_active_lock);			
				//LOGFECE("Previous Flush operation to be completed by now. Can't restart flushing\n");
				SLEEP_MILLI_SECONDS(1);
				RETURNN;

			}
			//LOGFEC("Time To switchover to Flush buffer\n");

			dz_fec_flush_switchover();
			fec_active_count += num_bufs;
			fec_tables_flush->flush_in_progress = TRUE;

			//Return the zeroth entry
			write_entry 		= GET_FEC_ACTIVE_WRITE_ENTRY(0);
			write_entry->state 	= FECBUF_STATE_ALLOCATED;
			spin_unlock(&fec_active_lock);			

			LOGD("Time To start flush thread\n");
			dz_q_iorequest(fec_flush_iorequest->ior_thread_pool, fec_flush_iorequest);
			return 	write_entry;
		}
		write_entry  		= GET_FEC_ACTIVE_WRITE_ENTRY(fec_active_count);
		write_entry->state 	= FECBUF_STATE_ALLOCATED;
		fec_active_count 	= final_count;
		spin_unlock(&fec_active_lock);			

		return 	write_entry;
}

RVOID dz_fec_align_write_io(PIOREQUEST iorequest)
{
	PBIO		bio 		= iorequest->ior_bio; // Parent or Original bio
	SIZE  		io_size		= bio->bi_size; // Parent or Original bio size


	if (io_size < LBA_BLOCK_SIZE) {
		LOGFEC("Case1: Write Processing io_size for partial block at lba %lli\n",
		dz_convert_sector_to_lba(DZ_BIO_GET_SECTOR(bio));

		INC_COUNTER(fec_iocount_writes_partial_page);
		dz_fec_align_write_for_partial_block(iorequest);
		return;

	} else if (io_size == LBA_BLOCK_SIZE) {

		LOGFEC("Case2: Write Processing io_size for single block at lba %lli\n",
		dz_convert_sector_to_lba(DZ_BIO_GET_SECTOR(bio));

		INC_COUNTER(fec_iocount_writes_single_page);

		dz_fec_align_write_for_single_block(iorequest);
		return;

	} else if (io_size > LBA_BLOCK_SIZE) {
		LOGFEC("Case3: Write Processing io_size for multiple blocks at lba %lli\n",
		dz_convert_sector_to_lba(DZ_BIO_GET_SECTOR(bio));

		INC_COUNTER(fec_iocount_writes_multi_page);
		dz_fec_align_write_for_multi_block(iorequest);
		return;

	} else {
		//Should not come here
		BUG_ON(1);
	}
	return;
}

RVOID dz_fec_write_io(PIOREQUEST iorequest)
{
	//LOGFEC("Write Request received at lba %lli\n", lba);
	dz_fec_align_write_io(iorequest);
	return;
}

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "fec_engine.h"

extern PDZ_THREAD_POOL 	dedupe_thread_pool;

extern U64 				fec_table_size;
extern SPINLOCK			fec_active_lock;

//For operational purpose
extern PFEC_TABLE 		fec_tables_active;
extern PFEC_TABLE 		fec_tables_flush;

RVOID dz_fec_replay_active_buffer(PIOREQUEST iorequest)
{

}
RVOID dz_fec_replay_flush_buffer(PIOREQUEST iorequest)
{
	PFEC_WRITE	fecw 		= NULL;
	PIOREQUEST	fiorequest 			= NULL;
	LIST_HEAD 	ioq_head_local;
	U64 count;

	INIT_LIST_HEAD(&ioq_head_local);
	for (count = 0; count < fec_tables_flush->total_flush_cnt; count++) {
		fecw	= GET_FEC_FLUSH_ENTRY(count);
		fecw->state 	= FECBUF_STATE_REPLAY_STARTED;
		fiorequest = dz_fec_create_flush_io(fecw);
		if (!fiorequest) {
			//This should not have happened, because we are after reboot so
			//there is no scarcity of memory
			BUG_ON(1);
		}

	}

}

RVOID dz_fec_replay_io(PIOREQUEST iorequest)
{
	//We need to replay flush buffers first
	dz_fec_replay_flush_buffer(iorequest);
	dz_fec_replay_active_buffer(iorequest);
}

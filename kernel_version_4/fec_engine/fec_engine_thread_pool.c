/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "fec_engine.h"

extern PCHAR	FLUSH_THREAD_POOL_NAME;
extern PCHAR	FLUSH_THREAD_POOL_SHORT_NAME;

extern PCHAR	WRITE_THREAD_POOL_NAME;
extern PCHAR	WRITE_THREAD_POOL_SHORT_NAME;

extern PCHAR	READ_THREAD_POOL_NAME;
extern PCHAR	READ_THREAD_POOL_SHORT_NAME;

extern USHORT num_read_threads;
extern USHORT num_write_threads;
extern USHORT num_flush_threads;

extern PDZ_THREAD_POOL read_thread_pool;
extern PDZ_THREAD_POOL write_thread_pool;
extern PDZ_THREAD_POOL flush_thread_pool;

extern PDZ_THREAD_POOL fec_read_thread_pool;
extern PDZ_THREAD_POOL fec_write_thread_pool;
extern PDZ_THREAD_POOL fec_flush_thread_pool;

RINT dz_create_read_thread_pool(INT num_threads)
{
	PDZ_THREAD_POOL tp = NULL;
	tp = dz_create_thread_pool(
		READ_THREAD_POOL_NAME, READ_THREAD_POOL_SHORT_NAME,
		 num_threads, NULL, NULL, dz_fec_align_read_io);

	if (tp) {
		LOGFEC("%s created successfully for %d threads \n", 
			READ_THREAD_POOL_NAME, num_threads);
		fec_read_thread_pool = tp;
		RETURNS;
	} else {
		LOGE("%s creation failed \n", READ_THREAD_POOL_NAME);
		RETURNF;
	}
}

RVOID dz_destroy_read_thread_pool(VOID)
{
	dz_destroy_thread_pool(fec_read_thread_pool);
}

RINT dz_create_write_thread_pool(INT num_threads)
{
	PDZ_THREAD_POOL tp = NULL;
	tp = dz_create_thread_pool(
		WRITE_THREAD_POOL_NAME, WRITE_THREAD_POOL_SHORT_NAME, 
		num_threads, NULL, NULL, dz_fec_align_write_io);
//		num_threads, NULL, NULL, dz_process_write_io);

	if (tp) {
		LOGFEC("%s created successfully for %d threads \n", 
			WRITE_THREAD_POOL_NAME, num_threads);
		fec_write_thread_pool = tp;
		RETURNS;
	} else {
		LOGE("%s creation failed \n", WRITE_THREAD_POOL_NAME);
		RETURNF;
	}
}

RVOID dz_destroy_write_thread_pool(VOID)
{
	dz_destroy_thread_pool(fec_write_thread_pool);
}

RINT dz_create_flush_thread_pool(INT num_threads)
{
	PDZ_THREAD_POOL tp = NULL;
	tp = dz_create_thread_pool(
		FLUSH_THREAD_POOL_NAME, FLUSH_THREAD_POOL_SHORT_NAME,
		num_threads, NULL, NULL, dz_fec_flush_io);
	if (tp) {
		LOGFEC("%s created successfully for %d threads \n", 
			FLUSH_THREAD_POOL_NAME, num_threads);
		fec_flush_thread_pool = tp;
		RETURNS;
	} else {
		LOGFECE("%s creation failed \n", FLUSH_THREAD_POOL_NAME);
		RETURNF;
	}
	return SUCCESS;
}

RVOID dz_destroy_flush_thread_pool(VOID)
{
	dz_destroy_thread_pool(fec_flush_thread_pool);
}

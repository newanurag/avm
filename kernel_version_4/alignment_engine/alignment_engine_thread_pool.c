/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern PCHAR	ALIGN_WRITE_THREAD_POOL_NAME;
extern PCHAR	ALIGN_WRITE_THREAD_POOL_SHORT_NAME;

extern PCHAR	ALIGN_READ_THREAD_POOL_NAME;
extern PCHAR	ALIGN_READ_THREAD_POOL_SHORT_NAME;

extern USHORT num_align_read_threads;
extern USHORT num_align_write_threads;

extern PDZ_THREAD_POOL align_read_thread_pool;
extern PDZ_THREAD_POOL align_write_thread_pool;

RINT dz_create_align_read_thread_pool(INT num_threads)
{
	PDZ_THREAD_POOL tp = NULL;
	tp = dz_create_thread_pool(
		ALIGN_READ_THREAD_POOL_NAME, ALIGN_READ_THREAD_POOL_SHORT_NAME,
		 num_threads, NULL, NULL, dz_align_read_io);

	if (tp) {
		LOGALIGN("%s created successfully for %d threads \n", 
			ALIGN_READ_THREAD_POOL_NAME, num_threads);
		align_read_thread_pool = tp;
		RETURNS;
	} else {
		LOGE("%s creation failed \n", ALIGN_READ_THREAD_POOL_NAME);
		RETURNF;
	}
}

RVOID dz_destroy_align_read_thread_pool(VOID)
{
	dz_destroy_thread_pool(align_read_thread_pool);
}

RINT dz_create_align_write_thread_pool(INT num_threads)
{
	PDZ_THREAD_POOL tp = NULL;
	tp = dz_create_thread_pool(
		ALIGN_WRITE_THREAD_POOL_NAME, ALIGN_WRITE_THREAD_POOL_SHORT_NAME, 
		num_threads, NULL, NULL, dz_align_write_io);
//		num_threads, NULL, NULL, dz_process_write_io);

	if (tp) {
		LOGALIGN("%s created successfully for %d threads \n", 
			ALIGN_WRITE_THREAD_POOL_NAME, num_threads);
		align_write_thread_pool = tp;
		RETURNS;
	} else {
		LOGE("%s creation failed \n", ALIGN_WRITE_THREAD_POOL_NAME);
		RETURNF;
	}
}

RVOID dz_destroy_align_write_thread_pool(VOID)
{
	dz_destroy_thread_pool(align_write_thread_pool);
}

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _LBA_ENGINE_PIPE_H_
#define _LBA_ENGINE_PIPE_H_

typedef struct dz_waitq_item_s {
	LIST_HEAD			wait_item;
	struct task_struct *wait_task;
} DZ_WAITQ_ITEM;

typedef struct dz_waitq_s {
	SPINLOCK		waitq_lock;
	LIST_HEAD		waitq_head;
} DZ_WAITQ;

BOOL dz_process_lab_engine_pipe(LBA offset, UINT iosize);

#endif //_LBA_ENGINE_PIPE_H_

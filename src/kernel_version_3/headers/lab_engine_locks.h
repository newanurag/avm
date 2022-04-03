/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _LBA_ENGINE_LOCKS_H_
#define _LBA_ENGINE_LOCKS_H_

#define	DZ_RWSPINLOCK_INIT	0x10000000
#define	DZ_RWSEMAPHORE_INIT	0x10000000

typedef struct dz_spinlock_s {
	union {
		ATOMIC64 lock_atomic;
		LONG lock_bit;
	};
	struct dz_spinlock_s *next;
	struct dz_spinlock_s *prev;
	struct dz_spinlock_s *tail;
} DZ_SPINLOCK, *PDZ_SPINLOCK;

typedef struct dz_rwspinlock_s {
	union {
		ATOMIC64 lock_atomic;
		LONG lock_bit;
	};
} DZ_RWSPINLOCK, *PDZ_RWSPINLOCK;

typedef struct dz_semaphore_s {
	INT count;
	PDZ_SPINLOCK	wait_lock;
	LIST_HEAD		wait_list;

}DZ_SEMAPHORE, *PDZ_SEMAPHORE;

typedef struct dz_rwsemaphore_s {
	LONG count;
	PDZ_SPINLOCK	wait_lock;
	LIST_HEAD		wait_list;

} DZ_RWSEMAPHORE, *PDZ_RWSEMAPHORE;

//Normal Spinlocks
VOID dz_spinlock_init_atomic(PDZ_SPINLOCK dzlock);
VOID dz_spinlock_init_bit(PDZ_SPINLOCK dzlock);
VOID dz_spinlock_acquire_atomic(PDZ_SPINLOCK dzlock);
VOID dz_spinlock_acquire_bit(PDZ_SPINLOCK dzlock);
VOID dz_spinlock_release_atomic(PDZ_SPINLOCK dzlock);
VOID dz_spinlock_release_bit(PDZ_SPINLOCK dzlock);

//RW Spinlocks
VOID dz_rwspinlock_init_bit(PDZ_RWSPINLOCK dzlock);
VOID dz_rwspinlock_acquire_bit_read(PDZ_RWSPINLOCK dzrwlock);
VOID dz_rwspinlock_release_bit_read(PDZ_RWSPINLOCK dzrwlock);
VOID dz_rwspinlock_acquire_bit_write(PDZ_RWSPINLOCK dzrwlock);
VOID dz_rwspinlock_release_bit_write(PDZ_RWSPINLOCK dzrwlock);

VOID dz_rwspinlock_init_atomic(PDZ_RWSPINLOCK dzrwlock);
VOID dz_rwspinlock_acquire_atomic_read(PDZ_RWSPINLOCK dzrwlock);
VOID dz_rwspinlock_release_atomic_read(PDZ_RWSPINLOCK dzrwlock);
VOID dz_rwspinlock_acquire_atomic_write(PDZ_RWSPINLOCK dzrwlock);
VOID dz_rwspinlock_release_atomic_write(PDZ_RWSPINLOCK dzrwlock);

//Normal Semaphores
VOID dz_semaphore_init_atomic(PDZ_SEMAPHORE dzsema);
VOID dz_semaphore_acquire_atomic(PDZ_SEMAPHORE dzsema);
VOID dz_semaphore_release_atomic(PDZ_SEMAPHORE dzsema);

//READ WRITE Semaphores
enum rwsem_waiter_type {
    RWSEM_WAITING_FOR_WRITE,
    RWSEM_WAITING_FOR_READ
};

struct rwsem_waiter {
    struct list_head list;
    struct task_struct *task;
    enum rwsem_waiter_type type;
};

VOID dz_rwsemaphore_init(PDZ_RWSEMAPHORE dzrwsema);
VOID dz_rwsemaphore_acquire_read(PDZ_RWSEMAPHORE dzrwsema);
VOID dz_rwsemaphore_release_read(PDZ_RWSEMAPHORE dzrwsema);
VOID dz_rwsemaphore_acquire_write(PDZ_RWSEMAPHORE dzrwsema);
VOID dz_rwsemaphore_release_write(PDZ_RWSEMAPHORE dzrwsema);


BOOL dz_process_lab_engine_locks(LBA offset, UINT iosize);
#endif //_LBA_ENGINE_LOCKS_H_

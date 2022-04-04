/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

spinlock_t dz_spinlock;
rwlock_t dz_rwlock;
struct rw_semaphore dz_rwsemaphore;
struct semaphore dz_semaphore;

//Reader Writer Semaphore Init
/*
 * the rw-semaphore definition
 - if count is 0 then there are no active readers or writers 
 - if count is +ve then that is the number of active readers 
 - if count is -1 then there is one active writer
 - if wait_list is not empty, then there are processes waiting for the semaphore
 */

VOID dz_rwsemaphore_init(PDZ_RWSEMAPHORE dzrwsema)
{
	ATOMIC64_SET(dzrwsema->wait_lock->lock_atomic, 0);
	dzrwsema->count = 0;
}

#ifdef DZ_KERNEL_VERSION_4

VOID dz_rwsemaphore_acquire_write(PDZ_RWSEMAPHORE dzrwsema)
{
	struct rwsem_waiter waiter;
	//unsigned long flags;

	long ret = cmpxchg(&dzrwsema->count, RWSEM_UNLOCKED_VALUE, RWSEM_ACTIVE_WRITE_BIAS);

    if (ret == RWSEM_UNLOCKED_VALUE) {
        return ; //Semaphore is acquired by Writer Thread
	}

	//Comes here when semaphore is busy
	waiter.task = current;
	waiter.type = RWSEM_WAITING_FOR_WRITE;
	list_add_tail(&waiter.list, &dzrwsema->wait_list);
	//raw_spin_lock_irqsave(&sem->wait_lock, flags);
	//dz_spinlock_acquire_atomic(&dzrwsema->wait_lock);

	for(;;) {
		//dz_spinlock_acquire_atomic(&dzrwsema->wait_lock);
		if (dzrwsema->count == RWSEM_UNLOCKED_VALUE) {
			break;
		}
		//dz_spinlock_release_atomic(&dzrwsema->wait_lock);
		set_current_state(TASK_UNINTERRUPTIBLE);
		schedule();
	}
}

#else
VOID dz_rwsemaphore_acquire_write(PDZ_RWSEMAPHORE dzrwsema)
{
	struct rwsem_waiter waiter;
	struct task_struct *tsk;
	//unsigned long flags;

	long ret = cmpxchg(&dzrwsema->count, RWSEM_UNLOCKED_VALUE, RWSEM_ACTIVE_WRITE_BIAS);

    if (ret == RWSEM_UNLOCKED_VALUE) {
        return ; //Semaphore is acquired by Writer Thread
	}

	//Comes here when semaphore is busy
	tsk = current;
	waiter.task = tsk;
	waiter.type = RWSEM_WAITING_FOR_WRITE;
	list_add_tail(&waiter.list, &dzrwsema->wait_list);
	//raw_spin_lock_irqsave(&sem->wait_lock, flags);
	//dz_spinlock_acquire_atomic(&dzrwsema->wait_lock);

	for(;;) {
		//dz_spinlock_acquire_atomic(&dzrwsema->wait_lock);
		if (dzrwsema->count == RWSEM_UNLOCKED_VALUE) {
			break;
		}
		//dz_spinlock_release_atomic(&dzrwsema->wait_lock);
		set_task_state(tsk, TASK_UNINTERRUPTIBLE);
		schedule();
	}
}

#endif

VOID dz_rwsemaphore_release_write(PDZ_RWSEMAPHORE dzrwsema)
{

}

//Reader Writer Spinlock Init
VOID dz_rwspinlock_init_atomic(PDZ_RWSPINLOCK dzlock)
{
	ATOMIC64_SET(dzlock->lock_atomic, DZ_RWSPINLOCK_INIT);
}

//Reader Writer Spinlock Read
VOID dz_rwspinlock_acquire_atomic_read(PDZ_RWSPINLOCK dzlock)
{
	INT ret = 0;
	for (;;) {
		preempt_disable();
		ret = atomic64_dec_return(&dzlock->lock_atomic);

		if (ret >=0) {
			return ; // Lock Acquired
		} else {
			atomic64_inc_return(&dzlock->lock_atomic);
			preempt_enable();
		}

		while(1) {
			if (ATOMIC64_READ(dzlock->lock_atomic) == 0) {
				cpu_relax();
			} else {
				break;
			}
		} //End of while loop
	} //End of for loop
}

//Reader Writer SpinUnlock Read
VOID dz_rwspinlock_release_atomic_read(PDZ_RWSPINLOCK dzlock)
{
	atomic64_inc_return(&dzlock->lock_atomic);
	preempt_enable();
}

//Reader Writer Spinlock Write
VOID dz_rwspinlock_acquire_atomic_write(PDZ_RWSPINLOCK dzlock)
{
	LONG ret = 0;
	for (;;) {
		preempt_disable();
		ret = atomic64_sub_return(DZ_RWSPINLOCK_INIT, &dzlock->lock_atomic);

		if (ret == 0) {
			return ; // Lock Acquired
		} else {
			atomic64_add_return(DZ_RWSPINLOCK_INIT, &dzlock->lock_atomic);
			preempt_enable();
		}

		while(1) {
			if (ATOMIC64_READ(dzlock->lock_atomic) != DZ_RWSPINLOCK_INIT ) {
				cpu_relax();
			} else {
				break;
			}
		} //End of while loop
	} //End of for loop
}
//Reader Writer SpinUnlock Write
VOID dz_rwspinlock_release_atomic_write(PDZ_RWSPINLOCK dzlock)
{
	ATOMIC64_SET(dzlock->lock_atomic, DZ_RWSPINLOCK_INIT);
	preempt_enable();
}

VOID dz_spinlock_init_atomic(PDZ_SPINLOCK dzlock)
{
	ATOMIC64_SET(dzlock->lock_atomic, 0);
}

VOID dz_spinlock_init_bit(PDZ_SPINLOCK dzlock)
{
	dzlock->lock_bit = 0;
}

VOID dz_spinlock_acquire_atomic(PDZ_SPINLOCK dzlock)
{
	for (;;) {
		preempt_disable();
		if (test_and_set_bit(0, &dzlock->lock_bit)) {

			PRINT_ATOMIC64(dzlock->lock_atomic);
			return; //Lock is acquired
		} else {
			preempt_enable();
		}

		while(1) {
			if (ATOMIC64_READ(dzlock->lock_atomic) != 0) {
				cpu_relax();
			} else {
				break;
			}
		} //End of while loop
	} //End of for loop
}


VOID dz_spinlock_acquire_bit(PDZ_SPINLOCK dzlock)
{
	for (;;) {
		preempt_disable();
		if (test_and_set_bit(0, &dzlock->lock_bit)) {

			PRINT_LONG(dzlock->lock_bit);
			return; //Lock is acquired
		} else {
			preempt_enable();
		}

		while(1) {
			if (test_bit(0, &dzlock->lock_bit) != 0) {
				cpu_relax();
			} else {
				break;
			}
		} //End of while loop
	} //End of for loop
}

VOID dz_spinlock_release_atomic(PDZ_SPINLOCK dzlock)
{
	ATOMIC64_SET(dzlock->lock_atomic, 0);
	preempt_enable();
}

VOID dz_spinlock_release_bit(PDZ_SPINLOCK dzlock)
{
	clear_bit(0, &dzlock->lock_bit);
	preempt_enable();
}


VOID dz_semaphore_test(VOID)
{
	sema_init(&dz_semaphore, 1);
	//LOG("RWSEM_WAITING_BIAS      = %ld, 0x%lx\n",RWSEM_WAITING_BIAS,RWSEM_WAITING_BIAS);
	//LOG("RWSEM_ACTIVE_READ_BIAS  = %ld, 0x%lx\n",RWSEM_ACTIVE_READ_BIAS,RWSEM_ACTIVE_READ_BIAS);
	//LOG("RWSEM_ACTIVE_WRITE_BIAS = %ld, 0x%lx\n",RWSEM_ACTIVE_WRITE_BIAS,RWSEM_ACTIVE_WRITE_BIAS);
	LOG("Initial Semaphore           count = %u, count = 0x%x\n", dz_semaphore.count, dz_semaphore.count);
	down(&dz_semaphore);
	LOG("After down() Semaphore      count = %u, count = 0x%x\n", dz_semaphore.count, dz_semaphore.count);
	up(&dz_semaphore);
	LOG("After up() Semaphore        count = %u, count = 0x%x\n", dz_semaphore.count, dz_semaphore.count);
}

VOID dz_spin_lock_test(VOID)
{
	spin_lock_init(&dz_spinlock);
#ifdef DZ_ARCH_X86
	LOG("SPINLOCK =%u, = 0x%x\n", (UINT)dz_spinlock.rlock.raw_lock.locked, (UINT)dz_spinlock.rlock.raw_lock.locked);
#else
	//TODO:LOG("SPINLOCK =%u, = 0x%x\n", (UINT)dz_spinlock.rlock.raw_lock.head_tail, (UINT)dz_spinlock.rlock.raw_lock.head_tail);
#endif
}

VOID dz_read_write_spin_lock_test(VOID)
{
	rwlock_init(&dz_rwlock);

#ifdef DZ_KERNEL_VERSION_4
	/* TODO:
	LOG("Initial RW raw_lock.lock             = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("Initial RW raw_lock.read             = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("Initial RW raw_lock.write            = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
	*/
#else
	LOG("Initial RW raw_lock.lock             = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("Initial RW raw_lock.read             = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("Initial RW raw_lock.write            = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
#endif

	read_lock(&dz_rwlock);

#ifdef DZ_KERNEL_VERSION_4
	/* TODO:
	LOG("After READ_LOCK RW raw_lock.lock     = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("After READ_LOCK RW raw_lock.read     = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("After READ_LOCK RW raw_lock.write    = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
	*/
#else
	LOG("After READ_LOCK RW raw_lock.lock     = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("After READ_LOCK RW raw_lock.read     = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("After READ_LOCK RW raw_lock.write    = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
#endif

	read_unlock(&dz_rwlock);

#ifdef DZ_KERNEL_VERSION_4
	/* TODO:
	LOG("After READ_UNLOCK RW raw_lock.lock   = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("After READ_UNLOCK RW raw_lock.read   = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("After READ_UNLOCK RW raw_lock.write  = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
	*/
#else
	LOG("After READ_UNLOCK RW raw_lock.lock   = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("After READ_UNLOCK RW raw_lock.read   = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("After READ_UNLOCK RW raw_lock.write  = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
#endif

	write_lock(&dz_rwlock);

#ifdef DZ_KERNEL_VERSION_4
	/* TODO:
	LOG("After WRITE_LOCK RW raw_lock.lock    = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("After WRITE_LOCK RW raw_lock.read    = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("After WRITE_LOCK RW raw_lock.write   = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
	*/
#else
	LOG("After WRITE_LOCK RW raw_lock.lock    = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("After WRITE_LOCK RW raw_lock.read    = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("After WRITE_LOCK RW raw_lock.write   = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
#endif

	write_unlock(&dz_rwlock);

#ifdef DZ_KERNEL_VERSION_4
	/* TODO:
	LOG("After WRITE_UNLOCK RW raw_lock.lock  = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("After WRITE_UNLOCK RW raw_lock.read  = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("After WRITE_UNLOCK RW raw_lock.write = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
	*/
#else
	LOG("After WRITE_UNLOCK RW raw_lock.lock  = %lld, raw_lock.lock = 0x%llx\n", dz_rwlock.raw_lock.lock, dz_rwlock.raw_lock.lock);
	LOG("After WRITE_UNLOCK RW raw_lock.read  = %u, raw_lock.read = 0x%x\n", dz_rwlock.raw_lock.read, dz_rwlock.raw_lock.read);
	LOG("After WRITE_UNLOCK RW raw_lock.write = %d, raw_lock.write  = 0x%x\n", dz_rwlock.raw_lock.write, dz_rwlock.raw_lock.write);
#endif
}

VOID dz_read_write_semaphore_test(VOID)
{
	init_rwsem(&dz_rwsemaphore);
	LOG("RWSEM_WAITING_BIAS      = %ld, 0x%lx\n",RWSEM_WAITING_BIAS,RWSEM_WAITING_BIAS);
	LOG("RWSEM_ACTIVE_READ_BIAS  = %ld, 0x%lx\n",RWSEM_ACTIVE_READ_BIAS,RWSEM_ACTIVE_READ_BIAS);
	LOG("RWSEM_ACTIVE_WRITE_BIAS = %ld, 0x%lx\n",RWSEM_ACTIVE_WRITE_BIAS,RWSEM_ACTIVE_WRITE_BIAS);
#ifdef DZ_KERNEL_VERSION_4
	//TODO:LOG("Initial RWSemaphore           count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#else
	LOG("Initial RWSemaphore           count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#endif

	down_read(&dz_rwsemaphore);

#ifdef DZ_KERNEL_VERSION_4
	//TODO:LOG("After down_read  RWSemaphore  count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#else
	LOG("After down_read  RWSemaphore  count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#endif

	up_read(&dz_rwsemaphore);

#ifdef DZ_KERNEL_VERSION_4
	//TODO:LOG("After up_read    RWSemaphore  count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#else
	LOG("After up_read    RWSemaphore  count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#endif

	down_write(&dz_rwsemaphore);

#ifdef DZ_KERNEL_VERSION_4
	//TODO:LOG("After down_write RWSemaphore  count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#else
	LOG("After down_write RWSemaphore  count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#endif

	up_write(&dz_rwsemaphore);

#ifdef DZ_KERNEL_VERSION_4
	//TODO:LOG("After up_write   RWSemaphore  count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#else
	LOG("After up_write   RWSemaphore  count = %ld, count = 0x%lx\n", dz_rwsemaphore.count, dz_rwsemaphore.count);
#endif

}


VOID dz_lock_test(VOID)
{

/*
typedef union {
        s64 lock;
        struct {
                u32 read;
                s32 write;
        };
} arch_rwlock_t;
*/


	//LOG("RW_LOCK_BIAS = 0x%lx\n", RW_LOCK_BIAS);
	//LOG("WRITE_LOCK_CMP = 0x%lx\n", WRITE_LOCK_CMP);

	LOG("CONFIG_NR_CPUS = %d, 0x%x\n", CONFIG_NR_CPUS, CONFIG_NR_CPUS);
	dz_spin_lock_test();
	//dz_read_write_spin_lock_test();
	//dz_semaphore_test();
	//dz_read_write_semaphore_test();

}

VOID dz_spinlock_dev(VOID)
{
	INT ret = 0;
	DZ_SPINLOCK local_lock;
	dz_spinlock_init_bit(&local_lock);

	PRINT_LONG(local_lock.lock_bit);
	ret = test_and_set_bit(0, &local_lock.lock_bit);
	PRINT_INT(ret);
	PRINT_LONG(local_lock.lock_bit);

	LOG("After Setting");
	ret = 0;
	ret = test_and_set_bit(0, &local_lock.lock_bit);
	PRINT_INT(ret);
	PRINT_LONG(local_lock.lock_bit);
	if (test_and_set_bit(0, &local_lock.lock_bit)) {
		LOG("It should be Failed\n");
	} else {
		LOG("Should NOt come here\n");
	}
	clear_bit(0, &local_lock.lock_bit);
	if (test_and_set_bit(0, &local_lock.lock_bit)) {
		LOG("Should Not come here\n");
	} else {
		LOG("It should be passed\n");
	}

}

BOOL dz_process_lab_engine_locks(LBA offset, UINT iosize)
{
	dz_lock_test();
	dz_spinlock_dev();

	RETURNT;
}

EXPORT_SYMBOL(dz_process_lab_engine_locks);

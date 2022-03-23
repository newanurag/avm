/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

extern struct rw_semaphore bec_lock;

RVOID _ACQUIRE_TRANSACTION_LOCK()
{
	//Use Linux Kernel Mutex aka sleeping lock
}
EXPORT_SYMBOL(_ACQUIRE_TRANSACTION_LOCK);

RVOID _RELEASE_TRANSACTION_LOCK()
{
	//Use Linux Kernel Mutex aka sleeping lock
}
EXPORT_SYMBOL(_RELEASE_TRANSACTION_LOCK);

RVOID _ACQUIRE_FEC_LOCK_READ(PLBA_BLOCK lba_entry)
{
	bit_spin_lock(FEC_LBA_LOCK_BIT, &(lba_entry)->donotchange);
}
EXPORT_SYMBOL(_ACQUIRE_FEC_LOCK_READ);

RVOID	_RELEASE_FEC_LOCK_READ(PLBA_BLOCK lba_entry)
{
	bit_spin_unlock(FEC_LBA_LOCK_BIT, &(lba_entry)->donotchange);
}
EXPORT_SYMBOL(_RELEASE_FEC_LOCK_READ);

RVOID _ACQUIRE_FEC_LOCK_WRITE(PLBA_BLOCK lba_entry)
{
	bit_spin_lock(FEC_LBA_LOCK_BIT, &(lba_entry)->donotchange);
}
EXPORT_SYMBOL(_ACQUIRE_FEC_LOCK_WRITE);

RVOID	_RELEASE_FEC_LOCK_WRITE(PLBA_BLOCK lba_entry)
{
	bit_spin_unlock(FEC_LBA_LOCK_BIT, &(lba_entry)->donotchange);
}
EXPORT_SYMBOL(_RELEASE_FEC_LOCK_WRITE);

RVOID _ACQUIRE_LBA_LOCK_READ(PLBA_BLOCK lba_entry)
{
	bit_spin_lock(DEDUPE_LBA_LOCK_BIT, &(lba_entry)->donotchange);
	LOGD("Acquired LBA LOCK Read for lba %p\n", lba_entry);
}
EXPORT_SYMBOL(_ACQUIRE_LBA_LOCK_READ);

RVOID	_RELEASE_LBA_LOCK_READ(PLBA_BLOCK lba_entry)
{
	bit_spin_unlock(DEDUPE_LBA_LOCK_BIT, &(lba_entry)->donotchange);
	LOGD("Release LBA LOCK Read for lba %p\n", lba_entry);
}
EXPORT_SYMBOL(_RELEASE_LBA_LOCK_READ);

RVOID _ACQUIRE_LBA_LOCK_WRITE(PLBA_BLOCK lba_entry)
{
retryall:
	bit_spin_lock(DEDUPE_LBA_LOCK_BIT, &(lba_entry)->donotchange);
	LOGD("Acquired LBA LOCK Write for lba %p\n", lba_entry);
	if ((lba_entry)->in_use) {
		bit_spin_unlock(DEDUPE_LBA_LOCK_BIT, &(lba_entry)->donotchange);
		SLEEP_MILLI_SECONDS(1);
		cpu_relax();
		goto retryall;
	} else {
		(lba_entry)->in_use = true;
		bit_spin_unlock(DEDUPE_LBA_LOCK_BIT, &(lba_entry)->donotchange);
	}
}
EXPORT_SYMBOL(_ACQUIRE_LBA_LOCK_WRITE);

RVOID	_RELEASE_LBA_LOCK_WRITE(PLBA_BLOCK lba_entry)
{
	if ((lba_entry)->in_use) {
		(lba_entry)->in_use = false;
		bit_spin_unlock(DEDUPE_LBA_LOCK_BIT, &(lba_entry)->donotchange);
	} else {
		bit_spin_unlock(DEDUPE_LBA_LOCK_BIT, &(lba_entry)->donotchange);
		LOGE("wrong lba release\n");
		BUG_ON(1);
	}
	LOGD("Release LBA LOCK Write for lba %p\n", lba_entry);
}
EXPORT_SYMBOL(_RELEASE_LBA_LOCK_WRITE);

RVOID _ACQUIRE_LBA_LOCK(PLBA_BLOCK lba_entry)
{
	LBA_LOCK((lba_entry));
	//_ACQUIRE_LBA_LOCK_DEBUG((lba_entry));
}
EXPORT_SYMBOL(_ACQUIRE_LBA_LOCK);

RVOID	_RELEASE_LBA_LOCK(PLBA_BLOCK lba_entry)
{
	LBA_UNLOCK((lba_entry));
	//_RELEASE_LBA_LOCK_DEBUG((lba_entry));
}
EXPORT_SYMBOL(_RELEASE_LBA_LOCK);

RVOID _ACQUIRE_LBA_LOCK_DEBUG(PLBA_BLOCK lba_entry)
{
	retryall:
	LBA_LOCK((lba_entry));
	if ((lba_entry)->in_use) {
		LBA_UNLOCK((lba_entry));
		//SLEEP_MILLI_SECONDS(1);
		cpu_relax();
		goto retryall;
	} else {
		(lba_entry)->in_use = true;
		LBA_UNLOCK((lba_entry));
	}
}
EXPORT_SYMBOL(_ACQUIRE_LBA_LOCK_DEBUG);

RVOID	_RELEASE_LBA_LOCK_DEBUG(PLBA_BLOCK lba_entry)
{
	LBA_LOCK((lba_entry));
	if ((lba_entry)->in_use) {
		(lba_entry)->in_use = false;
		LBA_UNLOCK((lba_entry));
	} else {
		LBA_UNLOCK((lba_entry));
		LOGE("wrong lba release\n");
		BUG_ON(1);
	}
}

RVOID	_ACQUIRE_PBA_LOCK(PPBA_BLOCK pba_entry)
{
	//PBA_LOCK((pba_entry));
}
EXPORT_SYMBOL(_ACQUIRE_PBA_LOCK);

RVOID	_DZ_DDE_RELEASE_PBA_LOCK(PPBA_BLOCK pba_entry)
{
	//PBA_UNLOCK((pba_entry));
}
EXPORT_SYMBOL(_DZ_DDE_RELEASE_PBA_LOCK);

RVOID	_ACQUIRE_PBA_LOCK_DEBUG(PPBA_BLOCK pba_entry)
{
retryapl:
	PBA_LOCK((pba_entry));
	if ((pba_entry)->in_use) {
		PBA_UNLOCK((pba_entry));
		goto retryapl;
	} else {
		(pba_entry)->in_use = true;
		PBA_UNLOCK((pba_entry));
	}
}
EXPORT_SYMBOL(_ACQUIRE_PBA_LOCK_DEBUG);

RVOID	_RELEASE_PBA_LOCK_DEBUG(PPBA_BLOCK pba_entry)
{
	PBA_LOCK((pba_entry));
	if ((pba_entry)->in_use) {
		(pba_entry)->in_use = false;
		PBA_UNLOCK((pba_entry));
	} else {
		PBA_UNLOCK((pba_entry));
		LOGE("wrong pba release\n");
		BUG_ON(1);
	}
}
EXPORT_SYMBOL(_RELEASE_PBA_LOCK_DEBUG);
		
RVOID	_ACQUIRE_HBA_LOCK(PHBA_BLOCK hash_entry)
{
	HASH_LOCK((hash_entry));
}
EXPORT_SYMBOL(_ACQUIRE_HBA_LOCK);

RVOID	_DZ_DDE_RELEASE_HBA_LOCK(PHBA_BLOCK hash_entry)
{
	HASH_UNLOCK((hash_entry));
}
EXPORT_SYMBOL(_DZ_DDE_RELEASE_HBA_LOCK);

RVOID	_ACQUIRE_HBA_LOCK_DEBUG(PHBA_BLOCK hash_entry)
{
retryahl:
	HASH_LOCK((hash_entry));
	if ((hash_entry)->in_use) {
		HASH_UNLOCK((hash_entry));
		goto retryahl;
	} else {
		(hash_entry)->in_use = true;
		HASH_UNLOCK((hash_entry));
	}
}
EXPORT_SYMBOL(_ACQUIRE_HBA_LOCK_DEBUG);

RVOID	_RELEASE_HBA_LOCK_DEBUG(PHBA_BLOCK hash_entry)
{
	HASH_LOCK((hash_entry));
	if ((hash_entry)->in_use) {
		(hash_entry)->in_use = false;
		HASH_UNLOCK((hash_entry));
	} else {
		HASH_UNLOCK((hash_entry));
		LOGE("wrong hba release\n");
		BUG_ON(1);
	}
}
EXPORT_SYMBOL(_RELEASE_HBA_LOCK_DEBUG);

RVOID _ACQUIRE_BEC_LOCK_READ()
{
	down_read(&bec_lock);
}
EXPORT_SYMBOL(_ACQUIRE_BEC_LOCK_READ);

RVOID _RELEASE_BEC_LOCK_READ()
{
	up_read(&bec_lock);
}
EXPORT_SYMBOL(_RELEASE_BEC_LOCK_READ);

RVOID _ACQUIRE_BEC_LOCK_WRITE()
//RINT _ACQUIRE_BEC_LOCK_WRITE()
{
	down_write(&bec_lock);
	//return down_write_trylock(&bec_lock);
}
EXPORT_SYMBOL(_ACQUIRE_BEC_LOCK_WRITE);

RVOID _RELEASE_BEC_LOCK_WRITE()
{
	up_write(&bec_lock);
}
EXPORT_SYMBOL(_RELEASE_BEC_LOCK_WRITE);



#define DZ_RW_LOCK_BIAS		0x00100000
#define DZ_WRITE_LOCK_CMP	DZ_RW_LOCK_BIAS
typedef struct dz_rwlock_s {
	union { 
	s32 lock;
	s32 write;
	};
	unsigned int break_lock;
} dz_rwlock_t, DZ_RWLOCK, DZ_READ_WRITE_LOCK, *PDZ_RWLOCK;


RVOID dz_write_lock_fast(PDZ_RWLOCK lock)
{
	atomic_t *count = NULL;
	for (;;) {
		preempt_disable();
		count = (atomic_t *)&lock->write;
		if (atomic_sub_and_test(DZ_WRITE_LOCK_CMP, count)) {
			break; //Lock is Acquired. Also preemption is disabled

		} else { // Result is false it means, lock is NOT free
			atomic_add(DZ_WRITE_LOCK_CMP, count);
			preempt_enable();
		}

		while(1) {
			if(lock->write != DZ_WRITE_LOCK_CMP) {
				cpu_relax();
			} else {
				break;
			}
		}// End of While loop
	} //End of for loop
}

RVOID dz_write_unlock_fast(PDZ_RWLOCK lock)
{
	lock->write = DZ_WRITE_LOCK_CMP;
	preempt_enable();
}


RVOID dz_read_lock_fast(PDZ_RWLOCK lock)
{

	atomic_t *count = NULL;
	for (;;) {
		preempt_disable(); 
		count = (atomic_t *)&lock->lock; 

		if (atomic_dec_return(count) >= 0) {
			break; // Lock is Acquired. Preemption is disabled;

		} else { //Result is Negative. i.e. Lock is NOT Free. Go for BusyWaiting
			atomic_inc(count);
			preempt_enable(); 
		}

		while(1) {
			if (lock->lock == 0) {
				cpu_relax();
			} else {
				break;
			}
		} //End of While loop
	} //End of for loop
}

RVOID dz_read_unlock_fast(PDZ_RWLOCK lock)
{
	atomic_t *count = (atomic_t *)&lock->lock;
	atomic_inc(count);
	preempt_enable();
}

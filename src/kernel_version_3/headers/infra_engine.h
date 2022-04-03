/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _INFRA_ENGINE_H_
#define _INFRA_ENGINE_H_

INT 	dz_infra_engine_init(VOID);
RVOID 	dz_infra_engine_exit(VOID);
RVOID 	dz_stop_all_threads(VOID);

RVOID 	_ACQUIRE_TRANSACTION_LOCK(VOID);
RVOID 	_RELEASE_TRANSACTION_LOCK(VOID);

RVOID 	_ACQUIRE_FEC_LOCK_READ(PLBA_BLOCK);
RVOID 	_RELEASE_FEC_LOCK_READ(PLBA_BLOCK);

RVOID 	_ACQUIRE_FEC_LOCK_WRITE(PLBA_BLOCK);
RVOID 	_RELEASE_FEC_LOCK_WRITE(PLBA_BLOCK);

RVOID 	_ACQUIRE_LBA_LOCK_WRITE(PLBA_BLOCK);
RVOID 	_RELEASE_LBA_LOCK_WRITE(PLBA_BLOCK);

RVOID 	_ACQUIRE_BEC_LOCK_READ(VOID);
RVOID 	_RELEASE_BEC_LOCK_READ(VOID);

RVOID 	_ACQUIRE_BEC_LOCK_WRITE(VOID);
//RINT 	_ACQUIRE_BEC_LOCK_WRITE(VOID);
RVOID 	_RELEASE_BEC_LOCK_WRITE(VOID);

#ifdef DZ_KERNEL_VERSION_4
struct file *dz_file_open(const char *path, INT flags, INT rights);
ssize_t dz_file_read(struct file *file, void *buf, size_t count, loff_t *pos);
ssize_t dz_file_write(struct file *file, const void *buf, size_t count, loff_t *pos);
RVOID dz_file_close(struct file *file);
RINT dz_file_sync(struct file *file);
#else
struct file *dz_file_open(const char *path, INT flags, INT rights);
ssize_t dz_file_read(struct file *file, char __user *data, size_t size, loff_t *offset);
ssize_t dz_file_write(struct file *file, const char __user *data, size_t size, loff_t offset);
RVOID dz_file_close(struct file *file);
RINT dz_file_sync(struct file *file);
#endif

RINT dz_avm_log_file_open(VOID);
RINT dz_avm_log_file_write_raw(CHAR *data, UINT size );
RINT dz_avm_log_file_write(const char *format, ...);
RVOID dz_avm_log_file_close(VOID);

BOOL dz_process_interval_tree(LBA offset, UINT iosize);

#define DZ_ALLOC_NODE(size)	dz_kmalloc((size), GFP_KERNEL);

#endif //_INFRA_ENGINE_H_

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

struct file *dz_file_open(const char *path, INT flags, INT rights) 
{
	struct file *filp = NULL;
	//mm_segment_t oldfs;
	INT err = 0;

	//oldfs = get_fs();
	//set_fs(get_ds());
	filp = filp_open(path, flags, rights);
	//set_fs(oldfs);
	if (IS_ERR(filp)) {
		err = PTR_ERR(filp);
		RETURNN;
	}
	return filp;
}
EXPORT_SYMBOL(dz_file_open);

RVOID dz_file_close(struct file *file) 
{
	filp_close(file, NULL);
}
EXPORT_SYMBOL(dz_file_close);

ssize_t dz_file_read(struct file *file, void *buf, size_t count, loff_t *pos) 
{
	return kernel_read(file, buf, count, pos);
}
EXPORT_SYMBOL(dz_file_read);

ssize_t dz_file_write(struct file *file, const void *buf, size_t count, loff_t *pos)
{
	return 0;
	//return kernel_write(file, buf, count, pos);
}
EXPORT_SYMBOL(dz_file_write);

RINT dz_file_sync(struct file *file) 
{
	return vfs_fsync(file, 0);
}
EXPORT_SYMBOL(dz_file_sync);

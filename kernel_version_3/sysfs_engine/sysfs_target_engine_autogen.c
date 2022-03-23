/* NOTE : This is a System generated file. Changes to this file will be overwritten */
#include "prototypes.h"

static struct kobject *dz_kobj = NULL;

extern struct kobject *sysfs_avm_kernel_kobj;

CHAR avm_sysfs_entry_get_target_name[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_target_name_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_target_name_attr =
	__ATTR(get_target_name, 0644, avm_sysfs_entry_get_target_name_show, NULL);

CHAR avm_sysfs_entry_get_target_backstore_name[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_target_backstore_name_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_target_backstore_name_attr =
	__ATTR(get_target_backstore_name, 0644, avm_sysfs_entry_get_target_backstore_name_show, NULL);

CHAR avm_sysfs_entry_get_target_device_size_bytes[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_target_device_size_bytes_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_target_device_size_bytes_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_target_device_size_bytes_attr =
	__ATTR(get_target_device_size_bytes, 0644, avm_sysfs_entry_get_target_device_size_bytes_show, avm_sysfs_entry_get_target_device_size_bytes_store);

CHAR avm_sysfs_entry_get_target_device_size_blocks[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_target_device_size_blocks_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_target_device_size_blocks_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_target_device_size_blocks_attr =
	__ATTR(get_target_device_size_blocks, 0644, avm_sysfs_entry_get_target_device_size_blocks_show, avm_sysfs_entry_get_target_device_size_blocks_store);

U64 avm_sysfs_entry_get_set_target_passthrough_mode = 0;
static ssize_t avm_sysfs_entry_get_set_target_passthrough_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_target_passthrough_mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_target_passthrough_mode_attr =
	__ATTR(get_set_target_passthrough_mode, 0644, avm_sysfs_entry_get_set_target_passthrough_mode_show, avm_sysfs_entry_get_set_target_passthrough_mode_store);

U64 avm_sysfs_entry_get_set_target_passthrough_read = 0;
static ssize_t avm_sysfs_entry_get_set_target_passthrough_read_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_target_passthrough_read_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_target_passthrough_read_attr =
	__ATTR(get_set_target_passthrough_read, 0644, avm_sysfs_entry_get_set_target_passthrough_read_show, avm_sysfs_entry_get_set_target_passthrough_read_store);

U64 avm_sysfs_entry_get_set_target_passthrough_write = 0;
static ssize_t avm_sysfs_entry_get_set_target_passthrough_write_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_target_passthrough_write_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_target_passthrough_write_attr =
	__ATTR(get_set_target_passthrough_write, 0644, avm_sysfs_entry_get_set_target_passthrough_write_show, avm_sysfs_entry_get_set_target_passthrough_write_store);

U64 avm_sysfs_entry_get_set_target_lba_block_size = 0;
static ssize_t avm_sysfs_entry_get_set_target_lba_block_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_target_lba_block_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_target_lba_block_size_attr =
	__ATTR(get_set_target_lba_block_size, 0644, avm_sysfs_entry_get_set_target_lba_block_size_show, avm_sysfs_entry_get_set_target_lba_block_size_store);

U64 avm_sysfs_entry_get_set_target_switch_iopath = 0;
static ssize_t avm_sysfs_entry_get_set_target_switch_iopath_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_target_switch_iopath_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_target_switch_iopath_attr =
	__ATTR(get_set_target_switch_iopath, 0644, avm_sysfs_entry_get_set_target_switch_iopath_show, avm_sysfs_entry_get_set_target_switch_iopath_store);

CHAR avm_sysfs_entry_issue_write_io_on_device[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_issue_write_io_on_device_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_issue_write_io_on_device_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_issue_write_io_on_device_attr =
	__ATTR(issue_write_io_on_device, 0644, avm_sysfs_entry_issue_write_io_on_device_show, avm_sysfs_entry_issue_write_io_on_device_store);

U64 avm_sysfs_entry_issue_write_io_at_sector = 0;
static ssize_t avm_sysfs_entry_issue_write_io_at_sector_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_issue_write_io_at_sector_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_issue_write_io_at_sector_attr =
	__ATTR(issue_write_io_at_sector, 0644, avm_sysfs_entry_issue_write_io_at_sector_show, avm_sysfs_entry_issue_write_io_at_sector_store);

INT avm_sysfs_entry_issue_write_io_with_size = 0;
static ssize_t avm_sysfs_entry_issue_write_io_with_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_issue_write_io_with_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_issue_write_io_with_size_attr =
	__ATTR(issue_write_io_with_size, 0644, avm_sysfs_entry_issue_write_io_with_size_show, avm_sysfs_entry_issue_write_io_with_size_store);

INT avm_sysfs_entry_issue_write_io = 0;
static ssize_t avm_sysfs_entry_issue_write_io_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_issue_write_io_attr =
	__ATTR(issue_write_io, 0644, avm_sysfs_entry_issue_write_io_show, NULL);

CHAR avm_sysfs_entry_issue_read_io_on_device[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_issue_read_io_on_device_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_issue_read_io_on_device_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_issue_read_io_on_device_attr =
	__ATTR(issue_read_io_on_device, 0644, avm_sysfs_entry_issue_read_io_on_device_show, avm_sysfs_entry_issue_read_io_on_device_store);

U64 avm_sysfs_entry_issue_read_io_at_sector = 0;
static ssize_t avm_sysfs_entry_issue_read_io_at_sector_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_issue_read_io_at_sector_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_issue_read_io_at_sector_attr =
	__ATTR(issue_read_io_at_sector, 0644, avm_sysfs_entry_issue_read_io_at_sector_show, avm_sysfs_entry_issue_read_io_at_sector_store);

INT avm_sysfs_entry_issue_read_io_with_size = 0;
static ssize_t avm_sysfs_entry_issue_read_io_with_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_issue_read_io_with_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_issue_read_io_with_size_attr =
	__ATTR(issue_read_io_with_size, 0644, avm_sysfs_entry_issue_read_io_with_size_show, avm_sysfs_entry_issue_read_io_with_size_store);

INT avm_sysfs_entry_issue_read_io = 0;
static ssize_t avm_sysfs_entry_issue_read_io_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_issue_read_io_attr =
	__ATTR(issue_read_io, 0644, avm_sysfs_entry_issue_read_io_show, NULL);

CHAR avm_sysfs_entry_issue_zero_io_on_device[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_issue_zero_io_on_device_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_issue_zero_io_on_device_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_issue_zero_io_on_device_attr =
	__ATTR(issue_zero_io_on_device, 0644, avm_sysfs_entry_issue_zero_io_on_device_show, avm_sysfs_entry_issue_zero_io_on_device_store);

U64 avm_sysfs_entry_issue_zero_io_at_sector = 0;
static ssize_t avm_sysfs_entry_issue_zero_io_at_sector_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_issue_zero_io_at_sector_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_issue_zero_io_at_sector_attr =
	__ATTR(issue_zero_io_at_sector, 0644, avm_sysfs_entry_issue_zero_io_at_sector_show, avm_sysfs_entry_issue_zero_io_at_sector_store);

INT avm_sysfs_entry_issue_zero_io_with_size = 0;
static ssize_t avm_sysfs_entry_issue_zero_io_with_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_issue_zero_io_with_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_issue_zero_io_with_size_attr =
	__ATTR(issue_zero_io_with_size, 0644, avm_sysfs_entry_issue_zero_io_with_size_show, avm_sysfs_entry_issue_zero_io_with_size_store);

INT avm_sysfs_entry_issue_zero_io = 0;
static ssize_t avm_sysfs_entry_issue_zero_io_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_issue_zero_io_attr =
	__ATTR(issue_zero_io, 0644, avm_sysfs_entry_issue_zero_io_show, NULL);

INT avm_sysfs_entry_write_verify_io = 0;
static ssize_t avm_sysfs_entry_write_verify_io_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_write_verify_io_attr =
	__ATTR(write_verify_io, 0644, avm_sysfs_entry_write_verify_io_show, NULL);

static ssize_t avm_sysfs_entry_get_target_name_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_target_name(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_target_name : %s\n",avm_sysfs_entry_get_target_name);
}

static ssize_t avm_sysfs_entry_get_target_backstore_name_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_target_backstore_name(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_target_backstore_name : %s\n",avm_sysfs_entry_get_target_backstore_name);
}

static ssize_t avm_sysfs_entry_get_target_device_size_bytes_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_target_device_size_bytes(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_target_device_size_bytes : %s\n",avm_sysfs_entry_get_target_device_size_bytes);
}

static ssize_t avm_sysfs_entry_get_target_device_size_bytes_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%s", avm_sysfs_entry_get_target_device_size_bytes);
	return len;
}

static ssize_t avm_sysfs_entry_get_target_device_size_blocks_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_target_device_size_blocks(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_target_device_size_blocks : %s\n",avm_sysfs_entry_get_target_device_size_blocks);
}

static ssize_t avm_sysfs_entry_get_target_device_size_blocks_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%s", avm_sysfs_entry_get_target_device_size_blocks);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_target_passthrough_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_target_passthrough_mode(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_target_passthrough_mode : %lli\n",avm_sysfs_entry_get_set_target_passthrough_mode);
}

static ssize_t avm_sysfs_entry_get_set_target_passthrough_mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_target_passthrough_mode);
	avm_set_target_passthrough_mode(avm_sysfs_entry_get_set_target_passthrough_mode);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_target_passthrough_read_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_target_passthrough_read(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_target_passthrough_read : %lli\n",avm_sysfs_entry_get_set_target_passthrough_read);
}

static ssize_t avm_sysfs_entry_get_set_target_passthrough_read_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_target_passthrough_read);
	avm_set_target_passthrough_read(avm_sysfs_entry_get_set_target_passthrough_read);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_target_passthrough_write_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_target_passthrough_write(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_target_passthrough_write : %lli\n",avm_sysfs_entry_get_set_target_passthrough_write);
}

static ssize_t avm_sysfs_entry_get_set_target_passthrough_write_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_target_passthrough_write);
	avm_set_target_passthrough_write(avm_sysfs_entry_get_set_target_passthrough_write);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_target_lba_block_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_target_lba_block_size(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_target_lba_block_size : %lli\n",avm_sysfs_entry_get_set_target_lba_block_size);
}

static ssize_t avm_sysfs_entry_get_set_target_lba_block_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_target_lba_block_size);
	avm_set_target_lba_block_size(avm_sysfs_entry_get_set_target_lba_block_size);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_target_switch_iopath_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_target_switch_iopath(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_target_switch_iopath : %lli\n",avm_sysfs_entry_get_set_target_switch_iopath);
}

static ssize_t avm_sysfs_entry_get_set_target_switch_iopath_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_target_switch_iopath);
	avm_set_target_switch_iopath(avm_sysfs_entry_get_set_target_switch_iopath);
	return len;
}

static ssize_t avm_sysfs_entry_issue_write_io_on_device_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_write_io_on_device : %s\n",avm_sysfs_entry_issue_write_io_on_device);
}

static ssize_t avm_sysfs_entry_issue_write_io_on_device_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%s", avm_sysfs_entry_issue_write_io_on_device);
	return len;
}

static ssize_t avm_sysfs_entry_issue_write_io_at_sector_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_write_io_at_sector : %lli\n",avm_sysfs_entry_issue_write_io_at_sector);
}

static ssize_t avm_sysfs_entry_issue_write_io_at_sector_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_issue_write_io_at_sector);
	return len;
}

static ssize_t avm_sysfs_entry_issue_write_io_with_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_write_io_with_size : %d\n",avm_sysfs_entry_issue_write_io_with_size);
}

static ssize_t avm_sysfs_entry_issue_write_io_with_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%d", &avm_sysfs_entry_issue_write_io_with_size);
	return len;
}

static ssize_t avm_sysfs_entry_issue_write_io_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_do_write_io(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_write_io : %d\n",avm_sysfs_entry_issue_write_io);
}

static ssize_t avm_sysfs_entry_issue_read_io_on_device_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_read_io_on_device : %s\n",avm_sysfs_entry_issue_read_io_on_device);
}

static ssize_t avm_sysfs_entry_issue_read_io_on_device_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%s", avm_sysfs_entry_issue_read_io_on_device);
	return len;
}

static ssize_t avm_sysfs_entry_issue_read_io_at_sector_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_read_io_at_sector : %lli\n",avm_sysfs_entry_issue_read_io_at_sector);
}

static ssize_t avm_sysfs_entry_issue_read_io_at_sector_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_issue_read_io_at_sector);
	return len;
}

static ssize_t avm_sysfs_entry_issue_read_io_with_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_read_io_with_size : %d\n",avm_sysfs_entry_issue_read_io_with_size);
}

static ssize_t avm_sysfs_entry_issue_read_io_with_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%d", &avm_sysfs_entry_issue_read_io_with_size);
	return len;
}

static ssize_t avm_sysfs_entry_issue_read_io_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_do_read_io(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_read_io : %d\n",avm_sysfs_entry_issue_read_io);
}

static ssize_t avm_sysfs_entry_issue_zero_io_on_device_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_zero_io_on_device : %s\n",avm_sysfs_entry_issue_zero_io_on_device);
}

static ssize_t avm_sysfs_entry_issue_zero_io_on_device_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%s", avm_sysfs_entry_issue_zero_io_on_device);
	return len;
}

static ssize_t avm_sysfs_entry_issue_zero_io_at_sector_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_zero_io_at_sector : %lli\n",avm_sysfs_entry_issue_zero_io_at_sector);
}

static ssize_t avm_sysfs_entry_issue_zero_io_at_sector_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_issue_zero_io_at_sector);
	return len;
}

static ssize_t avm_sysfs_entry_issue_zero_io_with_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_zero_io_with_size : %d\n",avm_sysfs_entry_issue_zero_io_with_size);
}

static ssize_t avm_sysfs_entry_issue_zero_io_with_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%d", &avm_sysfs_entry_issue_zero_io_with_size);
	return len;
}

static ssize_t avm_sysfs_entry_issue_zero_io_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_do_zero_io(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_issue_zero_io : %d\n",avm_sysfs_entry_issue_zero_io);
}

static ssize_t avm_sysfs_entry_write_verify_io_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_do_write_verify_io(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_write_verify_io : %d\n",avm_sysfs_entry_write_verify_io);
}

INT avm_sysfs_target_engine_init(VOID)
{
	INT ret = SUCCESS;
	dz_kobj = kobject_create_and_add("target_engine", sysfs_avm_kernel_kobj);
	if (!dz_kobj)
		return -ENOMEM;
	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_target_name_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/get_target_name created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/get_target_name creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_target_backstore_name_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/get_target_backstore_name created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/get_target_backstore_name creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_target_device_size_bytes_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/get_target_device_size_bytes created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/get_target_device_size_bytes creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_target_device_size_blocks_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/get_target_device_size_blocks created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/get_target_device_size_blocks creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_target_passthrough_mode_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/get_set_target_passthrough_mode created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/get_set_target_passthrough_mode creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_target_passthrough_read_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/get_set_target_passthrough_read created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/get_set_target_passthrough_read creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_target_passthrough_write_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/get_set_target_passthrough_write created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/get_set_target_passthrough_write creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_target_lba_block_size_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/get_set_target_lba_block_size created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/get_set_target_lba_block_size creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_target_switch_iopath_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/get_set_target_switch_iopath created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/get_set_target_switch_iopath creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_write_io_on_device_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_write_io_on_device created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_write_io_on_device creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_write_io_at_sector_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_write_io_at_sector created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_write_io_at_sector creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_write_io_with_size_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_write_io_with_size created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_write_io_with_size creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_write_io_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_write_io created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_write_io creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_read_io_on_device_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_read_io_on_device created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_read_io_on_device creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_read_io_at_sector_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_read_io_at_sector created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_read_io_at_sector creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_read_io_with_size_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_read_io_with_size created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_read_io_with_size creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_read_io_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_read_io created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_read_io creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_zero_io_on_device_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_zero_io_on_device created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_zero_io_on_device creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_zero_io_at_sector_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_zero_io_at_sector created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_zero_io_at_sector creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_zero_io_with_size_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_zero_io_with_size created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_zero_io_with_size creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_issue_zero_io_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/issue_zero_io created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/issue_zero_io creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_write_verify_io_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/target_engine/write_verify_io created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/target_engine/write_verify_io creation failed \n");
		RETURNF;
	}

	RETURNS;

}
RVOID avm_sysfs_target_engine_exit(VOID)
{
	kobject_put(dz_kobj);
	LOGSYS("dz_sysfs freed\n");
}
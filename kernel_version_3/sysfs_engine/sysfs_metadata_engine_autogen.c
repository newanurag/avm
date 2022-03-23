/* NOTE : This is a System generated file. Changes to this file will be overwritten */
#include "prototypes.h"

static struct kobject *dz_kobj = NULL;

extern struct kobject *sysfs_avm_kernel_kobj;

CHAR avm_sysfs_entry_get_metadata_counters[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_metadata_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_metadata_counters_attr =
	__ATTR(get_metadata_counters, 0644, avm_sysfs_entry_get_metadata_counters_show, NULL);

CHAR avm_sysfs_entry_get_superblock[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_superblock_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_superblock_attr =
	__ATTR(get_superblock, 0644, avm_sysfs_entry_get_superblock_show, NULL);

static ssize_t avm_sysfs_entry_get_metadata_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_metadata_counters(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_metadata_counters : %s\n",avm_sysfs_entry_get_metadata_counters);
}

static ssize_t avm_sysfs_entry_get_superblock_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_superblock(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_superblock : %s\n",avm_sysfs_entry_get_superblock);
}

INT avm_sysfs_metadata_engine_init(VOID)
{
	INT ret = SUCCESS;
	dz_kobj = kobject_create_and_add("metadata_engine", sysfs_avm_kernel_kobj);
	if (!dz_kobj)
		return -ENOMEM;
	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_metadata_counters_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/metadata_engine/get_metadata_counters created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/metadata_engine/get_metadata_counters creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_superblock_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/metadata_engine/get_superblock created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/metadata_engine/get_superblock creation failed \n");
		RETURNF;
	}

	RETURNS;

}
RVOID avm_sysfs_metadata_engine_exit(VOID)
{
	kobject_put(dz_kobj);
	LOGSYS("dz_sysfs freed\n");
}
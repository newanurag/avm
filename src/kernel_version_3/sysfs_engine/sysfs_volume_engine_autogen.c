/* NOTE : This is a System generated file. Changes to this file will be overwritten */
#include "prototypes.h"

static struct kobject *dz_kobj = NULL;

extern struct kobject *sysfs_avm_kernel_kobj;

CHAR avm_sysfs_entry_get_set_volume_info[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_set_volume_info_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_volume_info_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_volume_info_attr =
	__ATTR(get_set_volume_info, 0644, avm_sysfs_entry_get_set_volume_info_show, avm_sysfs_entry_get_set_volume_info_store);

CHAR avm_sysfs_entry_get_list_of_volumes[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_list_of_volumes_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_list_of_volumes_attr =
	__ATTR(get_list_of_volumes, 0644, avm_sysfs_entry_get_list_of_volumes_show, NULL);

CHAR avm_sysfs_entry_get_list_of_volumes_short[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_list_of_volumes_short_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_list_of_volumes_short_attr =
	__ATTR(get_list_of_volumes_short, 0644, avm_sysfs_entry_get_list_of_volumes_short_show, NULL);

static ssize_t avm_sysfs_entry_get_set_volume_info_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_set_volume_info(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_volume_info : %s\n",avm_sysfs_entry_get_set_volume_info);
}

static ssize_t avm_sysfs_entry_get_set_volume_info_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%s", avm_sysfs_entry_get_set_volume_info);
	return len;
}

static ssize_t avm_sysfs_entry_get_list_of_volumes_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_list_of_volumes(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_list_of_volumes : %s\n",avm_sysfs_entry_get_list_of_volumes);
}

static ssize_t avm_sysfs_entry_get_list_of_volumes_short_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_list_of_volumes_short(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_list_of_volumes_short : %s\n",avm_sysfs_entry_get_list_of_volumes_short);
}

INT avm_sysfs_volume_engine_init(VOID)
{
	INT ret = SUCCESS;
	dz_kobj = kobject_create_and_add("volume_engine", sysfs_avm_kernel_kobj);
	if (!dz_kobj)
		return -ENOMEM;
	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_volume_info_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/volume_engine/get_set_volume_info created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/volume_engine/get_set_volume_info creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_list_of_volumes_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/volume_engine/get_list_of_volumes created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/volume_engine/get_list_of_volumes creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_list_of_volumes_short_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/volume_engine/get_list_of_volumes_short created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/volume_engine/get_list_of_volumes_short creation failed \n");
		RETURNF;
	}

	RETURNS;

}
RVOID avm_sysfs_volume_engine_exit(VOID)
{
	kobject_put(dz_kobj);
	LOGSYS("dz_sysfs freed\n");
}
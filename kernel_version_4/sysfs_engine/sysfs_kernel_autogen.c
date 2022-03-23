/* NOTE : This is a System generated file. Changes to this file will be overwritten */
#include "prototypes.h"

static struct kobject *dz_kobj = NULL;

extern struct kobject *sysfs_avm_kernel_kobj;

U64 avm_sysfs_entry_get_free_page_count = 0;
static ssize_t avm_sysfs_entry_get_free_page_count_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_free_page_count_attr =
	__ATTR(get_free_page_count, 0644, avm_sysfs_entry_get_free_page_count_show, NULL);

CHAR avm_sysfs_entry_get_list_of_objects[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_list_of_objects_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_list_of_objects_attr =
	__ATTR(get_list_of_objects, 0644, avm_sysfs_entry_get_list_of_objects_show, NULL);

CHAR avm_sysfs_entry_get_version[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_version_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_version_attr =
	__ATTR(get_version, 0644, avm_sysfs_entry_get_version_show, NULL);

CHAR avm_sysfs_entry_get_memory_info[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_memory_info_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_memory_info_attr =
	__ATTR(get_memory_info, 0644, avm_sysfs_entry_get_memory_info_show, NULL);

CHAR avm_sysfs_entry_get_all_counters[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_all_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_all_counters_attr =
	__ATTR(get_all_counters, 0644, avm_sysfs_entry_get_all_counters_show, NULL);

CHAR avm_sysfs_entry_get_sizeof_datastructures[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_sizeof_datastructures_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_sizeof_datastructures_attr =
	__ATTR(get_sizeof_datastructures, 0644, avm_sysfs_entry_get_sizeof_datastructures_show, NULL);

INT avm_sysfs_entry_write = 0;
static ssize_t avm_sysfs_entry_write_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_write_attr =
	__ATTR(write, 0644, NULL, avm_sysfs_entry_write_store);

INT avm_sysfs_entry_read = 0;
static ssize_t avm_sysfs_entry_read_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_read_attr =
	__ATTR(read, 0644, avm_sysfs_entry_read_show, NULL);

static ssize_t avm_sysfs_entry_get_free_page_count_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_free_page_count(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_free_page_count : %lli\n",avm_sysfs_entry_get_free_page_count);
}

static ssize_t avm_sysfs_entry_get_list_of_objects_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_list_of_objects(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_list_of_objects : %s\n",avm_sysfs_entry_get_list_of_objects);
}

static ssize_t avm_sysfs_entry_get_version_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_version(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_version : %s\n",avm_sysfs_entry_get_version);
}

static ssize_t avm_sysfs_entry_get_memory_info_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_memory_info(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_memory_info : %s\n",avm_sysfs_entry_get_memory_info);
}

static ssize_t avm_sysfs_entry_get_all_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_all_counters(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_all_counters : %s\n",avm_sysfs_entry_get_all_counters);
}

static ssize_t avm_sysfs_entry_get_sizeof_datastructures_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_sizeof_datastructures(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_sizeof_datastructures : %s\n",avm_sysfs_entry_get_sizeof_datastructures);
}

static ssize_t avm_sysfs_entry_write_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%d", &avm_sysfs_entry_write);
	return len;
}

static ssize_t avm_sysfs_entry_read_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	return sprintf((ptr += count), "#avm_sysfs_entry_read : %d\n",avm_sysfs_entry_read);
}

INT avm_sysfs_kernel_init(VOID)
{
	INT ret = SUCCESS;
	dz_kobj = kobject_create_and_add("kernel", sysfs_avm_kernel_kobj);
	if (!dz_kobj)
		return -ENOMEM;
	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_free_page_count_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/kernel/get_free_page_count created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/kernel/get_free_page_count creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_list_of_objects_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/kernel/get_list_of_objects created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/kernel/get_list_of_objects creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_version_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/kernel/get_version created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/kernel/get_version creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_memory_info_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/kernel/get_memory_info created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/kernel/get_memory_info creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_all_counters_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/kernel/get_all_counters created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/kernel/get_all_counters creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_sizeof_datastructures_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/kernel/get_sizeof_datastructures created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/kernel/get_sizeof_datastructures creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_write_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/kernel/write created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/kernel/write creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_read_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/kernel/read created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/kernel/read creation failed \n");
		RETURNF;
	}

	RETURNS;

}
RVOID avm_sysfs_kernel_exit(VOID)
{
	kobject_put(dz_kobj);
	LOGSYS("dz_sysfs freed\n");
}
/* NOTE : This is a System generated file. Changes to this file will be overwritten */
#include "prototypes.h"

static struct kobject *dz_kobj = NULL;

extern struct kobject *sysfs_avm_kernel_kobj;

U64 avm_sysfs_entry_get_set_switch_fec = 0;
static ssize_t avm_sysfs_entry_get_set_switch_fec_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_switch_fec_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_switch_fec_attr =
	__ATTR(get_set_switch_fec, 0644, avm_sysfs_entry_get_set_switch_fec_show, avm_sysfs_entry_get_set_switch_fec_store);

CHAR avm_sysfs_entry_get_fec_counters[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_fec_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_fec_counters_attr =
	__ATTR(get_fec_counters, 0644, avm_sysfs_entry_get_fec_counters_show, NULL);

static ssize_t avm_sysfs_entry_get_set_switch_fec_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_switch_fec(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_switch_fec : %lli\n",avm_sysfs_entry_get_set_switch_fec);
}

static ssize_t avm_sysfs_entry_get_set_switch_fec_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_switch_fec);
	avm_set_switch_fec(avm_sysfs_entry_get_set_switch_fec);
	return len;
}

static ssize_t avm_sysfs_entry_get_fec_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_fec_counters(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_fec_counters : %s\n",avm_sysfs_entry_get_fec_counters);
}

INT avm_sysfs_fec_engine_init(VOID)
{
	INT ret = SUCCESS;
	dz_kobj = kobject_create_and_add("front_end_cache_engine", sysfs_avm_kernel_kobj);
	if (!dz_kobj)
		return -ENOMEM;
	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_switch_fec_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/front_end_cache_engine/get_set_switch_fec created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/front_end_cache_engine/get_set_switch_fec creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_fec_counters_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/front_end_cache_engine/get_fec_counters created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/front_end_cache_engine/get_fec_counters creation failed \n");
		RETURNF;
	}

	RETURNS;

}
RVOID avm_sysfs_fec_engine_exit(VOID)
{
	kobject_put(dz_kobj);
	LOGSYS("dz_sysfs freed\n");
}
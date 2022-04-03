/* NOTE : This is a System generated file. Changes to this file will be overwritten */
#include "prototypes.h"

static struct kobject *dz_kobj = NULL;

extern struct kobject *sysfs_avm_kernel_kobj;

U64 avm_sysfs_entry_get_set_switch_bec = 0;
static ssize_t avm_sysfs_entry_get_set_switch_bec_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_switch_bec_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_switch_bec_attr =
	__ATTR(get_set_switch_bec, 0644, avm_sysfs_entry_get_set_switch_bec_show, avm_sysfs_entry_get_set_switch_bec_store);

U64 avm_sysfs_entry_get_set_tunable_bec_size = 0;
static ssize_t avm_sysfs_entry_get_set_tunable_bec_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_tunable_bec_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_tunable_bec_size_attr =
	__ATTR(get_set_tunable_bec_size, 0644, avm_sysfs_entry_get_set_tunable_bec_size_show, avm_sysfs_entry_get_set_tunable_bec_size_store);

U64 avm_sysfs_entry_get_set_tunable_bec_threads = 0;
static ssize_t avm_sysfs_entry_get_set_tunable_bec_threads_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_tunable_bec_threads_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_tunable_bec_threads_attr =
	__ATTR(get_set_tunable_bec_threads, 0644, avm_sysfs_entry_get_set_tunable_bec_threads_show, avm_sysfs_entry_get_set_tunable_bec_threads_store);

U64 avm_sysfs_entry_get_set_tunable_bec_ep = 0;
static ssize_t avm_sysfs_entry_get_set_tunable_bec_ep_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_tunable_bec_ep_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_tunable_bec_ep_attr =
	__ATTR(get_set_tunable_bec_ep, 0644, avm_sysfs_entry_get_set_tunable_bec_ep_show, avm_sysfs_entry_get_set_tunable_bec_ep_store);

CHAR avm_sysfs_entry_get_bec_counters[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_bec_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_bec_counters_attr =
	__ATTR(get_bec_counters, 0644, avm_sysfs_entry_get_bec_counters_show, NULL);

static ssize_t avm_sysfs_entry_get_set_switch_bec_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_switch_bec(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_switch_bec : %lli\n",avm_sysfs_entry_get_set_switch_bec);
}

static ssize_t avm_sysfs_entry_get_set_switch_bec_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_switch_bec);
	avm_set_switch_bec(avm_sysfs_entry_get_set_switch_bec);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_tunable_bec_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_tunable_bec_size(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_tunable_bec_size : %lli\n",avm_sysfs_entry_get_set_tunable_bec_size);
}

static ssize_t avm_sysfs_entry_get_set_tunable_bec_size_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_tunable_bec_size);
	avm_set_tunable_bec_size(avm_sysfs_entry_get_set_tunable_bec_size);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_tunable_bec_threads_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_tunable_bec_threads(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_tunable_bec_threads : %lli\n",avm_sysfs_entry_get_set_tunable_bec_threads);
}

static ssize_t avm_sysfs_entry_get_set_tunable_bec_threads_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_tunable_bec_threads);
	avm_set_tunable_bec_threads(avm_sysfs_entry_get_set_tunable_bec_threads);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_tunable_bec_ep_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_tunable_bec_ep(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_tunable_bec_ep : %lli\n",avm_sysfs_entry_get_set_tunable_bec_ep);
}

static ssize_t avm_sysfs_entry_get_set_tunable_bec_ep_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_tunable_bec_ep);
	avm_set_tunable_bec_ep(avm_sysfs_entry_get_set_tunable_bec_ep);
	return len;
}

static ssize_t avm_sysfs_entry_get_bec_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_bec_counters(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_bec_counters : %s\n",avm_sysfs_entry_get_bec_counters);
}

INT avm_sysfs_bec_engine_init(VOID)
{
	INT ret = SUCCESS;
	dz_kobj = kobject_create_and_add("back_end_cache_engine", sysfs_avm_kernel_kobj);
	if (!dz_kobj)
		return -ENOMEM;
	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_switch_bec_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/back_end_cache_engine/get_set_switch_bec created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/back_end_cache_engine/get_set_switch_bec creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_tunable_bec_size_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/back_end_cache_engine/get_set_tunable_bec_size created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/back_end_cache_engine/get_set_tunable_bec_size creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_tunable_bec_threads_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/back_end_cache_engine/get_set_tunable_bec_threads created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/back_end_cache_engine/get_set_tunable_bec_threads creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_tunable_bec_ep_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/back_end_cache_engine/get_set_tunable_bec_ep created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/back_end_cache_engine/get_set_tunable_bec_ep creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_bec_counters_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/back_end_cache_engine/get_bec_counters created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/back_end_cache_engine/get_bec_counters creation failed \n");
		RETURNF;
	}

	RETURNS;

}
RVOID avm_sysfs_bec_engine_exit(VOID)
{
	kobject_put(dz_kobj);
	LOGSYS("dz_sysfs freed\n");
}
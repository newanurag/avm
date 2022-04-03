/* NOTE : This is a System generated file. Changes to this file will be overwritten */
#include "prototypes.h"

static struct kobject *dz_kobj = NULL;

extern struct kobject *sysfs_avm_kernel_kobj;

U64 avm_sysfs_entry_get_set_switch_dae = 0;
static ssize_t avm_sysfs_entry_get_set_switch_dae_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_switch_dae_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_switch_dae_attr =
	__ATTR(get_set_switch_dae, 0644, avm_sysfs_entry_get_set_switch_dae_show, avm_sysfs_entry_get_set_switch_dae_store);

U64 avm_sysfs_entry_get_set_switch_dae_mbr = 0;
static ssize_t avm_sysfs_entry_get_set_switch_dae_mbr_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_switch_dae_mbr_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_switch_dae_mbr_attr =
	__ATTR(get_set_switch_dae_mbr, 0644, avm_sysfs_entry_get_set_switch_dae_mbr_show, avm_sysfs_entry_get_set_switch_dae_mbr_store);

U64 avm_sysfs_entry_get_set_switch_dae_mbw = 0;
static ssize_t avm_sysfs_entry_get_set_switch_dae_mbw_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_switch_dae_mbw_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_switch_dae_mbw_attr =
	__ATTR(get_set_switch_dae_mbw, 0644, avm_sysfs_entry_get_set_switch_dae_mbw_show, avm_sysfs_entry_get_set_switch_dae_mbw_store);

U64 avm_sysfs_entry_get_set_switch_dae_pbr = 0;
static ssize_t avm_sysfs_entry_get_set_switch_dae_pbr_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_switch_dae_pbr_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_switch_dae_pbr_attr =
	__ATTR(get_set_switch_dae_pbr, 0644, avm_sysfs_entry_get_set_switch_dae_pbr_show, avm_sysfs_entry_get_set_switch_dae_pbr_store);

U64 avm_sysfs_entry_get_set_switch_dae_pbw = 0;
static ssize_t avm_sysfs_entry_get_set_switch_dae_pbw_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_switch_dae_pbw_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_switch_dae_pbw_attr =
	__ATTR(get_set_switch_dae_pbw, 0644, avm_sysfs_entry_get_set_switch_dae_pbw_show, avm_sysfs_entry_get_set_switch_dae_pbw_store);

U64 avm_sysfs_entry_get_set_tunable_dae_rthreads = 0;
static ssize_t avm_sysfs_entry_get_set_tunable_dae_rthreads_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_tunable_dae_rthreads_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_tunable_dae_rthreads_attr =
	__ATTR(get_set_tunable_dae_rthreads, 0644, avm_sysfs_entry_get_set_tunable_dae_rthreads_show, avm_sysfs_entry_get_set_tunable_dae_rthreads_store);

U64 avm_sysfs_entry_get_set_tunable_dae_wthreads = 0;
static ssize_t avm_sysfs_entry_get_set_tunable_dae_wthreads_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_tunable_dae_wthreads_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_tunable_dae_wthreads_attr =
	__ATTR(get_set_tunable_dae_wthreads, 0644, avm_sysfs_entry_get_set_tunable_dae_wthreads_show, avm_sysfs_entry_get_set_tunable_dae_wthreads_store);

CHAR avm_sysfs_entry_get_align_counters[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_align_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_align_counters_attr =
	__ATTR(get_align_counters, 0644, avm_sysfs_entry_get_align_counters_show, NULL);

static ssize_t avm_sysfs_entry_get_set_switch_dae_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_switch_dae(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_switch_dae : %lli\n",avm_sysfs_entry_get_set_switch_dae);
}

static ssize_t avm_sysfs_entry_get_set_switch_dae_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_switch_dae);
	avm_set_switch_dae(avm_sysfs_entry_get_set_switch_dae);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_switch_dae_mbr_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_switch_dae_mbr(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_switch_dae_mbr : %lli\n",avm_sysfs_entry_get_set_switch_dae_mbr);
}

static ssize_t avm_sysfs_entry_get_set_switch_dae_mbr_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_switch_dae_mbr);
	avm_set_switch_dae_mbr(avm_sysfs_entry_get_set_switch_dae_mbr);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_switch_dae_mbw_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_switch_dae_mbw(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_switch_dae_mbw : %lli\n",avm_sysfs_entry_get_set_switch_dae_mbw);
}

static ssize_t avm_sysfs_entry_get_set_switch_dae_mbw_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_switch_dae_mbw);
	avm_set_switch_dae_mbw(avm_sysfs_entry_get_set_switch_dae_mbw);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_switch_dae_pbr_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_switch_dae_pbr(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_switch_dae_pbr : %lli\n",avm_sysfs_entry_get_set_switch_dae_pbr);
}

static ssize_t avm_sysfs_entry_get_set_switch_dae_pbr_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_switch_dae_pbr);
	avm_set_switch_dae_pbr(avm_sysfs_entry_get_set_switch_dae_pbr);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_switch_dae_pbw_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_switch_dae_pbw(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_switch_dae_pbw : %lli\n",avm_sysfs_entry_get_set_switch_dae_pbw);
}

static ssize_t avm_sysfs_entry_get_set_switch_dae_pbw_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_switch_dae_pbw);
	avm_set_switch_dae_pbw(avm_sysfs_entry_get_set_switch_dae_pbw);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_tunable_dae_rthreads_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_tunable_dae_read_threads(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_tunable_dae_rthreads : %lli\n",avm_sysfs_entry_get_set_tunable_dae_rthreads);
}

static ssize_t avm_sysfs_entry_get_set_tunable_dae_rthreads_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_tunable_dae_rthreads);
	avm_set_tunable_dae_read_threads(avm_sysfs_entry_get_set_tunable_dae_rthreads);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_tunable_dae_wthreads_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_tunable_dae_write_threads(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_tunable_dae_wthreads : %lli\n",avm_sysfs_entry_get_set_tunable_dae_wthreads);
}

static ssize_t avm_sysfs_entry_get_set_tunable_dae_wthreads_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_tunable_dae_wthreads);
	avm_set_tunable_dae_write_threads(avm_sysfs_entry_get_set_tunable_dae_wthreads);
	return len;
}

static ssize_t avm_sysfs_entry_get_align_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_align_counters(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_align_counters : %s\n",avm_sysfs_entry_get_align_counters);
}

INT avm_sysfs_alignment_engine_init(VOID)
{
	INT ret = SUCCESS;
	dz_kobj = kobject_create_and_add("alignment_engine", sysfs_avm_kernel_kobj);
	if (!dz_kobj)
		return -ENOMEM;
	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_switch_dae_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_switch_dae_mbr_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae_mbr created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae_mbr creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_switch_dae_mbw_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae_mbw created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae_mbw creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_switch_dae_pbr_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae_pbr created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae_pbr creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_switch_dae_pbw_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae_pbw created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/alignment_engine/get_set_switch_dae_pbw creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_tunable_dae_rthreads_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/alignment_engine/get_set_tunable_dae_rthreads created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/alignment_engine/get_set_tunable_dae_rthreads creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_tunable_dae_wthreads_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/alignment_engine/get_set_tunable_dae_wthreads created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/alignment_engine/get_set_tunable_dae_wthreads creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_align_counters_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/alignment_engine/get_align_counters created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/alignment_engine/get_align_counters creation failed \n");
		RETURNF;
	}

	RETURNS;

}
RVOID avm_sysfs_alignment_engine_exit(VOID)
{
	kobject_put(dz_kobj);
	LOGSYS("dz_sysfs freed\n");
}
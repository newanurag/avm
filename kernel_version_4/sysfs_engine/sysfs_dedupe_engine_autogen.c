/* NOTE : This is a System generated file. Changes to this file will be overwritten */
#include "prototypes.h"

static struct kobject *dz_kobj = NULL;

extern struct kobject *sysfs_avm_kernel_kobj;

U64 avm_sysfs_entry_get_set_switch_dde = 0;
static ssize_t avm_sysfs_entry_get_set_switch_dde_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_switch_dde_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_switch_dde_attr =
	__ATTR(get_set_switch_dde, 0644, avm_sysfs_entry_get_set_switch_dde_show, avm_sysfs_entry_get_set_switch_dde_store);

CHAR avm_sysfs_entry_get_dedupe_counters[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_dedupe_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_dedupe_counters_attr =
	__ATTR(get_dedupe_counters, 0644, avm_sysfs_entry_get_dedupe_counters_show, NULL);

CHAR avm_sysfs_entry_get_set_lba_table_index[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_set_lba_table_index_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_lba_table_index_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_lba_table_index_attr =
	__ATTR(get_set_lba_table_index, 0644, avm_sysfs_entry_get_set_lba_table_index_show, avm_sysfs_entry_get_set_lba_table_index_store);

CHAR avm_sysfs_entry_get_set_pba_table_index[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_set_pba_table_index_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_pba_table_index_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_pba_table_index_attr =
	__ATTR(get_set_pba_table_index, 0644, avm_sysfs_entry_get_set_pba_table_index_show, avm_sysfs_entry_get_set_pba_table_index_store);

U64 avm_sysfs_entry_get_set_lba_entry = 0;
static ssize_t avm_sysfs_entry_get_set_lba_entry_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_lba_entry_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_lba_entry_attr =
	__ATTR(get_set_lba_entry, 0644, avm_sysfs_entry_get_set_lba_entry_show, avm_sysfs_entry_get_set_lba_entry_store);

U64 avm_sysfs_entry_get_set_pba_entry = 0;
static ssize_t avm_sysfs_entry_get_set_pba_entry_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static ssize_t avm_sysfs_entry_get_set_pba_entry_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); 
static struct kobj_attribute avm_sysfs_entry_get_set_pba_entry_attr =
	__ATTR(get_set_pba_entry, 0644, avm_sysfs_entry_get_set_pba_entry_show, avm_sysfs_entry_get_set_pba_entry_store);

U64 avm_sysfs_entry_get_pba_table_size = 0;
static ssize_t avm_sysfs_entry_get_pba_table_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_pba_table_size_attr =
	__ATTR(get_pba_table_size, 0644, avm_sysfs_entry_get_pba_table_size_show, NULL);

U64 avm_sysfs_entry_get_hash_table_size = 0;
static ssize_t avm_sysfs_entry_get_hash_table_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_hash_table_size_attr =
	__ATTR(get_hash_table_size, 0644, avm_sysfs_entry_get_hash_table_size_show, NULL);

CHAR avm_sysfs_entry_get_table_size[DZ_SYSFS_PAGE_SIZE] = {'\0'};
static ssize_t avm_sysfs_entry_get_table_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf); 
static struct kobj_attribute avm_sysfs_entry_get_table_size_attr =
	__ATTR(get_table_size, 0644, avm_sysfs_entry_get_table_size_show, NULL);

static ssize_t avm_sysfs_entry_get_set_switch_dde_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_switch_dde(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_switch_dde : %lli\n",avm_sysfs_entry_get_set_switch_dde);
}

static ssize_t avm_sysfs_entry_get_set_switch_dde_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_switch_dde);
	avm_set_switch_dde(avm_sysfs_entry_get_set_switch_dde);
	return len;
}

static ssize_t avm_sysfs_entry_get_dedupe_counters_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_dedupe_counters(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_dedupe_counters : %s\n",avm_sysfs_entry_get_dedupe_counters);
}

static ssize_t avm_sysfs_entry_get_set_lba_table_index_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_set_lba_table_index(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_lba_table_index : %s\n",avm_sysfs_entry_get_set_lba_table_index);
}

static ssize_t avm_sysfs_entry_get_set_lba_table_index_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%s", avm_sysfs_entry_get_set_lba_table_index);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_pba_table_index_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_set_pba_table_index(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_pba_table_index : %s\n",avm_sysfs_entry_get_set_pba_table_index);
}

static ssize_t avm_sysfs_entry_get_set_pba_table_index_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%s", avm_sysfs_entry_get_set_pba_table_index);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_lba_entry_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_lba_entry(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_lba_entry : %lli\n",avm_sysfs_entry_get_set_lba_entry);
}

static ssize_t avm_sysfs_entry_get_set_lba_entry_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_lba_entry);
	return len;
}

static ssize_t avm_sysfs_entry_get_set_pba_entry_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_pba_entry(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_set_pba_entry : %lli\n",avm_sysfs_entry_get_set_pba_entry);
}

static ssize_t avm_sysfs_entry_get_set_pba_entry_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) 
{
	INT ret = SUCCESS ;
	ret = sscanf(buf, "%lli", &avm_sysfs_entry_get_set_pba_entry);
	return len;
}

static ssize_t avm_sysfs_entry_get_pba_table_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_pba_table_size_show(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_pba_table_size : %lli\n",avm_sysfs_entry_get_pba_table_size);
}

static ssize_t avm_sysfs_entry_get_hash_table_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_hash_table_size(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_hash_table_size : %lli\n",avm_sysfs_entry_get_hash_table_size);
}

static ssize_t avm_sysfs_entry_get_table_size_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	INT count = 0;
	CHAR *ptr = buf;
	count = avm_get_table_size(buf);
	return sprintf((ptr += count), "#avm_sysfs_entry_get_table_size : %s\n",avm_sysfs_entry_get_table_size);
}

INT avm_sysfs_dedupe_engine_init(VOID)
{
	INT ret = SUCCESS;
	dz_kobj = kobject_create_and_add("dedupe_engine", sysfs_avm_kernel_kobj);
	if (!dz_kobj)
		return -ENOMEM;
	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_switch_dde_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/dedupe_engine/get_set_switch_dde created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/dedupe_engine/get_set_switch_dde creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_dedupe_counters_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/dedupe_engine/get_dedupe_counters created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/dedupe_engine/get_dedupe_counters creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_lba_table_index_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/dedupe_engine/get_set_lba_table_index created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/dedupe_engine/get_set_lba_table_index creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_pba_table_index_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/dedupe_engine/get_set_pba_table_index created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/dedupe_engine/get_set_pba_table_index creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_lba_entry_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/dedupe_engine/get_set_lba_entry created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/dedupe_engine/get_set_lba_entry creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_set_pba_entry_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/dedupe_engine/get_set_pba_entry created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/dedupe_engine/get_set_pba_entry creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_pba_table_size_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/dedupe_engine/get_pba_table_size created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/dedupe_engine/get_pba_table_size creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_hash_table_size_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/dedupe_engine/get_hash_table_size created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/dedupe_engine/get_hash_table_size creation failed \n");
		RETURNF;
	}

	ret = sysfs_create_file(dz_kobj,&avm_sysfs_entry_get_table_size_attr.attr);
	if (ret == SUCCESS) {
		LOGSYS("sysfs entry /sys/avm/dedupe_engine/get_table_size created \n");
	} else {
		LOGSYSE("sysfs entry /sys/avm/dedupe_engine/get_table_size creation failed \n");
		RETURNF;
	}

	RETURNS;

}
RVOID avm_sysfs_dedupe_engine_exit(VOID)
{
	kobject_put(dz_kobj);
	LOGSYS("dz_sysfs freed\n");
}
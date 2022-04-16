/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

OBJECT_LIST 	global_object_list;
EXPORT_SYMBOL(global_object_list);

extern U64 lba_table_size;
extern U64 pba_table_size;
extern U64 hash_table_size;
extern U64 sysfs_avm_free_page_count;

U64			obj_count;
SPINLOCK	obj_trans_lock;

RINT dz_object_init()
{
	obj_count	= 0;
	spin_lock_init(&obj_trans_lock);
	MEMSET(&global_object_list);
	INIT_LIST_HEAD(&global_object_list.objectq_head);
	atomic64_set(&global_object_list.objectq_num_items, 0);
	spin_lock_init(&global_object_list.objectq_lock);
	LOGINF("Global Object List Initialized\n");

	RETURNS;
}

POBJECT_LIST dz_get_object_list()
{
	return &global_object_list;
}
EXPORT_SYMBOL(dz_get_object_list);

// Used for Queuing object into the Object list
RVOID dz_q_object(OBJECT_LIST *ol, PDEVICE_OBJECT object)
{
	atomic64_inc(&ol->objectq_num_items);
	spin_lock(&ol->objectq_lock);
	list_add_tail(&object->objectq, &ol->objectq_head);
	spin_unlock(&ol->objectq_lock);
}
EXPORT_SYMBOL(dz_q_object);

PDEVICE_OBJECT dz_dq_object(OBJECT_LIST *ol, char *name)
{
	PDEVICE_OBJECT object = NULL;
	object = dz_find_object(ol, name);
	if (object) {
		list_del(&object->objectq);
	}
	return object; 
}
EXPORT_SYMBOL(dz_dq_object);

PDEVICE_OBJECT dz_find_object(OBJECT_LIST *ol, char *name)
{
	PDEVICE_OBJECT object = NULL;
	spin_lock(&ol->objectq_lock);
	if (list_empty(&ol->objectq_head)) {
		spin_unlock(&ol->objectq_lock);
		goto out; 
	} else {
		list_for_each_entry(object, &ol->objectq_head, objectq) {
			if (strcmp(object->obj_name, name)==0) {
				break;
			} 
		}
		//object = list_first_entry(&ol->objectq_head, struct avm_object_s, objectq);
	}
	spin_unlock(&ol->objectq_lock);
out:
	return object; 
}
EXPORT_SYMBOL(dz_find_object);

// A Queue/List which has entries of all volumes and device. Any volume created through 
// dmsetup command will be enqueued here
/*
RVOID dz_avm_object_list_init(VOID)
{
	MEMSET(&global_object_list);
	INIT_LIST_HEAD(&global_object_list.objectq_head);
	atomic64_set(&global_object_list.objectq_num_items, 0);
	spin_lock_init(&global_object_list.objectq_lock);
	LOGINF("Object List Initialized\n");
}
*/

RVOID print_object_list(VOID)
{
	PDEVICE_OBJECT object = NULL;

	spin_lock(&global_object_list.objectq_lock);
	list_for_each_entry(object, &global_object_list.objectq_head, objectq) {
		if (object->obj_type == true) {
			LOGINF(" ______________________________________________________ \n");
			LOGINF("|                                                      |\n");
			LOGINF("|                 Printing Object List                 |\n");
			LOGINF("|______________________________________________________|\n");
			print_avm_device((PAVM_DEVICE)object);
			LOGINF(" ______________________________________________________ \n");
			LOGINF("|______________________________________________________|\n");
		} else {
			LOGINF(" ______________________________________________________ \n");
			LOGINF("|                                                      |\n");
			LOGINF("|                 Printing Object List                 |\n");
			LOGINF("|______________________________________________________|\n");
			print_avm_volume((PDDVOL)object);
			LOGINF(" ______________________________________________________ \n");
			LOGINF("|______________________________________________________|\n");
		}
	}
	spin_unlock(&global_object_list.objectq_lock);
}
EXPORT_SYMBOL(print_object_list);

RVOID print_object_list_V2(VOID)
{
	PDEVICE_OBJECT object = NULL;
	LIST_HEAD	*entry;

	LOGINF(" ______________________________________________________ \n");
	LOGINF("|                                                      |\n");
	LOGINF("|                 Printing Object List V2              |\n");
	LOGINF("|______________________________________________________|\n");

	spin_lock(&global_object_list.objectq_lock);
	list_for_each(entry, &global_object_list.objectq_head) {
		object = list_entry(entry, struct dz_target_object_s, objectq);
		if (object->obj_type == true) {
			print_avm_device((PAVM_DEVICE)object);
		} else {
			print_avm_volume((PDDVOL)object);
		}
	}
	spin_unlock(&global_object_list.objectq_lock);
	LOGINF(" ______________________________________________________ \n");
	LOGINF("|______________________________________________________|\n");
}
EXPORT_SYMBOL(print_object_list_V2);

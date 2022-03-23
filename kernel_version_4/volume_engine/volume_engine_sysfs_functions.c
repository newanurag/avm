/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

#define TEST_PASSED   100
#define TEST_FAILED   -1

extern int dz_switch_passthrough_mode;
extern int dz_switch_passthrough_read;
extern int dz_switch_passthrough_write;

extern INT dz_switch_fec_engine;
extern INT dz_switch_bec_engine;
extern INT dz_switch_dedupe_engine;
extern INT dz_switch_alignment_engine;

//extern ATOMIC64 dz_iorequest_alloc_cnt;
//extern ATOMIC64 dz_iorequest_free_cnt;
extern BASIC_COUNTERS  dz_basic_counters;
extern PBASIC_COUNTERS pdz_basic_counters;

//extern ATOMIC64 dz_page_alloc_cnt;
//extern ATOMIC64 dz_page_free_cnt;

extern SIZE 			TARGET_DEVICE_SIZE;

extern PAVM_DEVICE      aisa_device_object;
extern NAME             DATA_DISK_DEVICE_NAME;
extern OBJECT_LIST 	global_object_list;
extern U64 lba_table_size;
extern U64 pba_table_size;
extern U64 hash_table_size;
extern U32 number_of_longs_for_pba;
extern U32 number_of_longs_for_lba;
extern U32 number_of_longs_for_gpba;
extern ULONG            metadata_flush_interval;
extern U64 metadata_lba_table_size;
extern U64 metadata_lba_lock_table_size;
extern U64 avm_sysfs_entry_free_page_count;
extern CHAR avm_sysfs_entry_get_list_of_volumes[DZ_SYSFS_PAGE_SIZE];
extern CHAR avm_sysfs_entry_get_list_of_volumes_short[DZ_SYSFS_PAGE_SIZE];
extern PPBA_BLOCK		pba_table;
extern PHASH_BLOCK		hash_table;
extern PLBA_BLOCK		lba_table;

extern ATOMIC64			bec_pba_page_count;
extern ATOMIC64			bec_cache_hit;
extern ATOMIC64			bec_cache_miss;
extern ATOMIC64			bec_cache_delete;
extern ULONG			bec_size;
extern INT				bec_cutoff_nodes;
extern INT				bec_cutoff_percent;
extern INT				dz_bec_node_mempool_count;
extern U64			time_flush_prepare_start;
extern U64			time_flush_start;
extern U64			time_flush_end;
extern U64			time_fec_write_start;
extern U64			time_fec_write_switch;
extern PFEC_TABLE	fec_tables_flush;
extern ATOMIC64		fec_active_count_sequence;
extern ATOMIC64		fec_free_count_sequence;
extern ATOMIC64		fec_cur_flush_count;
extern ATOMIC64		fec_flush_ops_count;
extern ATOMIC64		fec_cur_flush_ops_count;
extern ATOMIC64		fec_memory_reads_count;
extern ATOMIC64		fec_forwarded_reads_count;
extern U64			fec_active_count;
extern ATOMIC64	dedupe_disk_reads;
extern ATOMIC64	dedupe_disk_writes;
extern ATOMIC64	dedupe_zeroed_out_page_reads;
extern ATOMIC64	dedupe_cache_hits;
extern ATOMIC64	dedupe_deleted_entries;
extern ATOMIC64	dedupe_cur_logical_space;
extern ATOMIC64	dedupe_cur_physical_space;
extern ATOMIC64	dedupe_cur_duplicate_space;
extern U64		dedupe_tot_logical_space;
extern U64		dedupe_tot_physical_space;
extern USHORT   dedupe_num_threads;
extern PDZ_THREAD_POOL dedupe_thread_pool;
extern PDZ_THREAD_POOL rdedupe_thread_pool;
extern PDZ_THREAD_POOL wdedupe_thread_pool;
extern PDZ_THREAD_POOL read_thread_pool;
extern PDZ_THREAD_POOL align_read_thread_pool;
extern PDZ_THREAD_POOL write_thread_pool; 
extern PDZ_THREAD_POOL align_write_thread_pool; 
extern PDZ_THREAD_POOL flush_thread_pool;
extern PDZ_THREAD_POOL metadata_flush_thread_pool;
extern ATOMIC64		tot_application_iocount;
extern ATOMIC64		tot_application_iocount_reads;
extern ATOMIC64 	tot_application_iocount_reads_partial_page;
extern ATOMIC64 	tot_application_iocount_reads_single_page;
extern ATOMIC64 	tot_application_iocount_reads_multi_page;
extern ATOMIC64 	fec_iocount_reads_partial_page;
extern ATOMIC64 	fec_iocount_reads_single_page;
extern ATOMIC64 	fec_iocount_reads_single_aligned_page;
extern ATOMIC64 	fec_iocount_reads_single_unaligned_page;
extern ATOMIC64 	fec_iocount_reads_multi_page;
extern ATOMIC64 	fec_iocount_reads_multi_aligned_page;
extern ATOMIC64 	fec_iocount_reads_multi_unaligned_page;

extern ATOMIC64 	align_iocount_disk_read;
extern ATOMIC64 	align_iocount_disk_read_internal;
extern ATOMIC64 	align_iocount_reads_partial_page;
extern ATOMIC64 	align_iocount_reads_single_page;
extern ATOMIC64 	align_iocount_reads_single_aligned_page;
extern ATOMIC64 	align_iocount_reads_single_unaligned_page;
extern ATOMIC64 	align_iocount_reads_multi_page;
extern ATOMIC64 	align_iocount_reads_multi_aligned_page;
extern ATOMIC64 	align_iocount_reads_multi_unaligned_page;

extern ATOMIC64		tot_application_iocount_writes;
extern ATOMIC64 	tot_application_iocount_writes_partial_page;
extern ATOMIC64 	tot_application_iocount_writes_single_page;
extern ATOMIC64 	tot_application_iocount_writes_multi_page;
extern ATOMIC64 	fec_iocount_writes_partial_page;
extern ATOMIC64 	fec_iocount_writes_single_page;
extern ATOMIC64 	fec_iocount_writes_single_aligned_page;
extern ATOMIC64 	fec_iocount_writes_single_unaligned_page;
extern ATOMIC64 	fec_iocount_writes_multi_page;
extern ATOMIC64 	fec_iocount_writes_multi_aligned_page;
extern ATOMIC64 	fec_iocount_writes_multi_unaligned_page;

extern ATOMIC64 	align_iocount_disk_write;
extern ATOMIC64 	align_iocount_disk_write_internal;
extern ATOMIC64 	align_iocount_writes_partial_page;
extern ATOMIC64 	align_iocount_writes_single_page;
extern ATOMIC64 	align_iocount_writes_single_aligned_page;
extern ATOMIC64 	align_iocount_writes_single_unaligned_page;
extern ATOMIC64 	align_iocount_writes_multi_page;
extern ATOMIC64 	align_iocount_writes_multi_aligned_page;
extern ATOMIC64 	align_iocount_writes_multi_unaligned_page;

extern PSUPERBLOCK 	superblock;

INT avm_get_list_of_volumes_short(CHAR *buf)
{
	PDEVICE_OBJECT object = NULL;
	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_list_of_volumes_short;


	memset(avm_sysfs_entry_get_list_of_volumes_short, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;

	if (!aisa_device_object) {
		return 0;
	}

	spin_lock(&global_object_list.objectq_lock);

	list_for_each_entry(object, &global_object_list.objectq_head, objectq) {
		if (!object) {
			continue;
		}
		if (object->obj_type == true) { // Just return only volume names
			continue;
		}

		ptr += sprintf(ptr, "%s\n", object->obj_name);

	}
	spin_unlock(&global_object_list.objectq_lock);
	return 0;
}


INT avm_get_list_of_volumes(CHAR *buf)
{
	PDEVICE_OBJECT object = NULL;
	UINT found 	= 0;
	UINT count	= 0;

	CHAR *ptr = (CHAR *)avm_sysfs_entry_get_list_of_volumes;


	memset(avm_sysfs_entry_get_list_of_volumes, 0, DZ_SYSFS_PAGE_SIZE);
	memcpy(ptr, "\n", 1);
	ptr += 1;

	ptr += sprintf(ptr, "\nListing Volumes Begin\n");
	ptr += sprintf(ptr, "\n---------------------\n");

	if (!aisa_device_object) {
		ptr += sprintf(ptr, "\nAISA Target Not Found\n");
		ptr += sprintf(ptr, "\n---------------------\n");
		ptr += sprintf(ptr, "\nListing Volumes End\n");
		return 0;
	}

	spin_lock(&global_object_list.objectq_lock);

	list_for_each_entry(object, &global_object_list.objectq_head, objectq) {
		if (!object) {
			continue;
		}
		if (object->obj_type == true) { // Just return only volume names
			continue;
		}

		ptr += sprintf(ptr,
		"\n[%d]\n"
		"Volume Name       = %s\n"
		"Volume Path       = /dev/mapper/%s\n"
		"Volume Unique Id  = %s\n",
		++count,
		object->obj_name,
		object->obj_name,
		object->obj_uuid);

		found = 1;


	}
	spin_unlock(&global_object_list.objectq_lock);
	if (found == 0) {
		ptr += sprintf(ptr, "\nVolume Not Found\n");
	}
	ptr += sprintf(ptr, "\n-------------------\n");
	ptr += sprintf(ptr, "\nTotal %d Volumes Found\n", count);
	ptr += sprintf(ptr, "\nListing Volumes End\n");
	return 0;
}

extern CHAR avm_sysfs_entry_get_set_volume_info[DZ_SYSFS_PAGE_SIZE];
INT avm_get_set_volume_info(CHAR *buf)
{
	UINT len 	= 0;
	PDEVICE_OBJECT object = NULL;
	//PDDVOL		parent = NULL;
	CHAR *name 	= avm_sysfs_entry_get_set_volume_info;
	CHAR *parent_name 	= NULL;
	CHAR obj_type_str[64] = "\0";
	INT found = 0;
	BOOL target_found = FALSE;

	len = strlen(name);
	LOGVOL("len=%d\n", len);
	LOGVOL("name=<%s>\n", name);
	if (!aisa_device_object) {
		sprintf(avm_sysfs_entry_get_set_volume_info, "\nAISA Target Not Found\n");
		return 0;
	}
	
	spin_lock(&global_object_list.objectq_lock);

	list_for_each_entry(object, &global_object_list.objectq_head, objectq) {
		if (!object) {
			continue;
		}
		if (strcmp(name, object->obj_name) == 0 ) {

			switch(object->obj_type) {
				case OBJECT_TYPE_AISA:
				parent_name = NULL;
				target_found = TRUE;
				break;

				case OBJECT_TYPE_DATA_VOLUME:
				parent_name = object->obj_dm_dev_name;
				strcpy(obj_type_str, "DATA_VOLUME");
				break;

				case OBJECT_TYPE_DATA_VOLUME_SNAPSHOT_READ:
				parent_name = ((PDEVICE_OBJECT)(((PDDVOL)object)->ddvol_parent))->obj_name;
				strcpy(obj_type_str, "DATA_VOLUME_SNAPSHOT_READ");
				break;

				case OBJECT_TYPE_DATA_VOLUME_SNAPSHOT_WRITE:
				parent_name = ((PDEVICE_OBJECT)(((PDDVOL)object)->ddvol_parent))->obj_name;
				strcpy(obj_type_str, "DATA_VOLUME_SNAPSHOT_WRITE");
				break;

				case OBJECT_TYPE_VM_VOLUME:
				parent_name = object->obj_dm_dev_name;
				strcpy(obj_type_str, "VIRTUAL_MACHINE_VOLUME");
				break;

				case OBJECT_TYPE_VM_VOLUME_SNAPSHOT_READ:
				parent_name = ((PDEVICE_OBJECT)(((PDDVOL)object)->ddvol_parent))->obj_name;
				strcpy(obj_type_str, "VIRTUAL_MACHINE_VOLUME_SNAPSHOT_READ");
				break;

				case OBJECT_TYPE_VM_VOLUME_SNAPSHOT_WRITE:
				parent_name = ((PDEVICE_OBJECT)(((PDDVOL)object)->ddvol_parent))->obj_name;
				strcpy(obj_type_str, "VIRTUAL_MACHINE_VOLUME_SNAPSHOT_WRITE");
				break;
			}

			if (target_found) {
				sprintf(avm_sysfs_entry_get_set_volume_info,
				"\n"
				"Target Info Begin\n" 
				"--------------------------\n" 
				"Target Name..............= %s\n"
				"Target Path..............= /dev/mapper/%s\n"
				"Target Type..............= %u : %s\n"
				"Target MagicNo...........= %s\n"
				"Target Unique Id(UUID)...= %s\n"
				"Target StartLBA..........= %lli (512 Bytes Block)\n"
				"Target EndLBA............= %lli (512 Bytes Block)\n"
				"Target Size in Bytes.....= %lli (Bytes)\n"
				"Target Size in Blocks....= %lli (512 Bytes Block)\n"
				"Target Size in KB........= %lli (KB)\n"
				"Target Size in MB........= %lli (MB)\n"
				"Target Size in GB........= %lli (GB)\n"
				"Target Extents...........= %llu\n"
				"Target Extent Size.......= %lu bytes\n"
				"Target Status............= %u\n"
				"Target Flags.............= %u\n"
				"Target Creation Date.....= %c%c/%c%c/%c%c%c%c (dd/mm/yyyy)\n"
				"Target Creation Time.....= %c%c:%c%c:%c%c   (HH:MM:SS)\n"
				"Target Backend Storage...= %s\n"
				"--------------------------\n" 
				"Target Info End",
				object->obj_name,
				object->obj_name,
				object->obj_type,
				obj_type_str,
				object->obj_magicno,
				object->obj_uuid,
				object->obj_start_lba,
				object->obj_end_lba,
				((object->obj_end_lba - object->obj_start_lba + 1) * SECTOR_SIZE),
				(object->obj_end_lba - object->obj_start_lba + 1),
				(((object->obj_end_lba - object->obj_start_lba + 1) * SECTOR_SIZE) / KB),
				(((object->obj_end_lba - object->obj_start_lba + 1) * SECTOR_SIZE) / MB),
				(((object->obj_end_lba - object->obj_start_lba + 1) * SECTOR_SIZE) / GB),
				((object->obj_end_lba-object->obj_start_lba + 1) * SECTOR_SIZE)/PAGE_SIZE,
				PAGE_SIZE,
				object->obj_flags,
				object->obj_flags,
				object->obj_date[0], object->obj_date[1], object->obj_date[2], object->obj_date[3],
				object->obj_date[4], object->obj_date[5], object->obj_date[6], object->obj_date[7],
				object->obj_time[0], object->obj_time[1], object->obj_time[2],
				object->obj_time[3], object->obj_time[4], object->obj_time[5],
				object->obj_dm_dev_name);
				found = 1;

				target_found = FALSE;
			} else {

				sprintf(avm_sysfs_entry_get_set_volume_info,
				"\n"
				"Volume Info Begin\n" 
				"--------------------------\n" 
				"Volume Name..............= %s\n"
				"Volume Path..............= /dev/mapper/%s\n"
				"Volume Type..............= %u : %s\n"
				"Volume MagicNo...........= %s\n"
				"Volume Unique Id(UUID)...= %s\n"
				"Volume StartLBA..........= %lli (512 Bytes Block)\n"
				"Volume EndLBA............= %lli (512 Bytes Block)\n"
				"Volume Size in Bytes.....= %lli (Bytes)\n"
				"Volume Size in Blocks....= %lli (512 Bytes Block)\n"
				"Volume Size in KB........= %lli (KB)\n"
				"Volume Size in MB........= %lli (MB)\n"
				"Volume Size in GB........= %lli (GB)\n"
				"Volume Extents...........= %llu\n"
				"Volume Extent Size.......= %lu bytes\n"
				"Volume Status............= %u\n"
				"Volume Flags.............= %u\n"
				"Volume Creation Date.....= %c%c/%c%c/%c%c%c%c (dd/mm/yyyy)\n"
				"Volume Creation Time.....= %c%c:%c%c:%c%c   (HH:MM:SS)\n"
				"Volume Parent Name.......= %s\n" 
				"Volume Parent Name Path..= /dev/mapper/%s\n" 
				"Volume Target Name.......= %s\n"
				"--------------------------\n" 
				"Volume Info End",
				object->obj_name,
				object->obj_name,
				object->obj_type,
				obj_type_str,
				object->obj_magicno,
				object->obj_uuid,
				object->obj_start_lba,
				object->obj_end_lba,
				((object->obj_end_lba - object->obj_start_lba + 1) * SECTOR_SIZE),
				(object->obj_end_lba - object->obj_start_lba + 1),
				(((object->obj_end_lba - object->obj_start_lba + 1) * SECTOR_SIZE) / KB),
				(((object->obj_end_lba - object->obj_start_lba + 1) * SECTOR_SIZE) / MB),
				(((object->obj_end_lba - object->obj_start_lba + 1) * SECTOR_SIZE) / GB),
				((object->obj_end_lba-object->obj_start_lba + 1) * SECTOR_SIZE)/PAGE_SIZE,
				PAGE_SIZE,
				object->obj_flags,
				object->obj_flags,
				object->obj_date[0], object->obj_date[1], object->obj_date[2], object->obj_date[3],
				object->obj_date[4], object->obj_date[5], object->obj_date[6], object->obj_date[7],
				object->obj_time[0], object->obj_time[1], object->obj_time[2],
				object->obj_time[3], object->obj_time[4], object->obj_time[5],
				parent_name,
				parent_name,
				object->obj_dm_dev_name);
				found = 1;
			}
		}
		
	}
	spin_unlock(&global_object_list.objectq_lock);
	if (found == 0) {
			sprintf(avm_sysfs_entry_get_set_volume_info, "\nVolume NOT Found\n");
	}
	return 0;
}

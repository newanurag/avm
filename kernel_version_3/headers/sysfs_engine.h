/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _SYSFS_ENGINE_H_
#define _SYSFS_ENGINE_H_

#define DZ_SYS_GET_SWITCH(buf, switch_name) 	\
{                                               \
	LOGSYS("%s:GET\n",__func__);						\
	(buf) = (U64)(switch_name);					\
	return 0;									\
}

#define DZ_SYS_SET_SWITCH(switch_name, switch_value) 		\
{                                                           \
	LOGSYS("%s:SET: Input Value = %d\n",__func__, (INT)switch_value);									\
	LOGSYS("Before Modification : %d\n",(INT)(switch_name));	\
	(switch_name) = (switch_value);							\
	LOGSYS("After  Modification : %d\n",(INT)(switch_name));	\
	return 0;									\
}


//sysfs Related Function Declarations
RINT 	avm_sysfs_kernel_init(VOID);
RVOID 	avm_sysfs_kernel_exit(VOID);

RINT 	avm_sysfs_target_engine_init(VOID);
RVOID 	avm_sysfs_target_engine_exit(VOID);

RINT 	avm_sysfs_bec_engine_init(VOID);
RVOID 	avm_sysfs_bec_engine_exit(VOID);

RINT 	avm_sysfs_fec_engine_init(VOID);
RVOID 	avm_sysfs_fec_engine_exit(VOID);

RINT 	avm_sysfs_dedupe_engine_init(VOID);
RVOID 	avm_sysfs_dedupe_engine_exit(VOID);

RINT 	avm_sysfs_volume_engine_init(VOID);
RVOID 	avm_sysfs_volume_engine_exit(VOID);

RINT 	avm_sysfs_metadata_engine_init(VOID);
RVOID 	avm_sysfs_metadata_engine_exit(VOID);

RINT 	avm_sysfs_alignment_engine_init(VOID);
RVOID 	avm_sysfs_alignment_engine_exit(VOID);

INT 	avm_get_sizeof_datastructures(CHAR *buf);
INT 	avm_get_list_of_objects(CHAR *buf);
INT 	avm_get_list_of_volumes(CHAR *buf);
INT 	avm_get_list_of_volumes_short(CHAR *buf);
INT 	avm_get_version(CHAR *buf);
INT 	avm_get_table_size(CHAR *buf);
INT 	avm_get_memory_info(CHAR *buf);
INT 	avm_get_superblock(CHAR *buf);
INT 	avm_get_bec_counters(CHAR *buf);
INT 	avm_get_fec_counters(CHAR *buf);
INT 	avm_get_align_counters(CHAR *buf);
INT 	avm_get_metadata_counters(CHAR *buf);
INT 	avm_get_dedupe_counters(CHAR *buf);
INT 	avm_get_all_counters(CHAR *buf);
INT 	avm_get_set_lba_table_index(CHAR *buf);
INT 	avm_get_set_pba_table_index(CHAR *buf);
INT 	avm_get_target_name(CHAR *buf);
INT 	avm_get_target_backstore_name(CHAR *buf);
INT 	avm_pba_table_size_show(CHAR *buf);
INT 	avm_get_hash_table_size(CHAR *buf);
INT 	avm_get_set_volume_info(CHAR *buf);
INT 	avm_get_target_device_size_bytes(CHAR *buf);
INT 	avm_get_target_device_size_blocks(CHAR *buf);


INT 	avm_get_target_passthrough_mode(CHAR *buf);
INT 	avm_get_target_passthrough_read(CHAR *buf);
INT 	avm_get_target_passthrough_write(CHAR *buf);

INT 	avm_get_target_lba_block_size(CHAR *buf);
INT 	avm_set_target_lba_block_size(U64 buf);

INT		avm_get_switch_fec(CHAR *buf);
INT		avm_get_switch_bec(CHAR *buf);
INT		avm_get_switch_dde(CHAR *buf);
INT		avm_get_switch_dae(CHAR *buf);
INT		avm_get_switch_dae_mbr(CHAR *buf);
INT		avm_get_switch_dae_mbw(CHAR *buf);
INT		avm_get_switch_dae_pbr(CHAR *buf);
INT		avm_get_switch_dae_pbw(CHAR *buf);

INT 	avm_set_target_passthrough_mode(U64 buf);
INT 	avm_set_target_passthrough_read(U64 buf);
INT 	avm_set_target_passthrough_write(U64 buf);

INT		avm_set_switch_fec(U64 buf);
INT		avm_set_switch_bec(U64 buf);
INT		avm_set_switch_dde(U64 buf);
INT		avm_set_switch_dae(U64 buf);
INT		avm_set_switch_dae_mbr(U64 buf);
INT		avm_set_switch_dae_mbw(U64 buf);
INT		avm_set_switch_dae_pbr(U64 buf);
INT		avm_set_switch_dae_pbw(U64 buf);


INT 	avm_get_free_page_count(CHAR *buf);
INT 	avm_do_write_io(CHAR *buf);
INT 	avm_do_read_io(CHAR *buf);
INT 	avm_do_zero_io(CHAR *buf);
INT 	avm_do_write_verify_io(CHAR *buf);
INT 	avm_get_lba_entry(CHAR *buf);
INT 	avm_get_pba_entry(CHAR *buf);
INT 	avm_get_tasb(CHAR *buf);

//Sysfs Dedupe Engine 
INT 	avm_dedupe_show(CHAR *buf);

//Sysfs FEC Engine 
INT 	avm_fec_show(CHAR *buf);

//Sysfs BEC Engine 
INT 	avm_bec_show(CHAR *buf);


INT avm_get_tunable_dae_read_threads(CHAR *buf);
INT avm_set_tunable_dae_read_threads(U64 buf);
INT avm_get_tunable_dae_write_threads(CHAR *buf);
INT avm_set_tunable_dae_write_threads(U64 buf);

INT avm_get_tunable_bec_size(CHAR *buf);
INT avm_set_tunable_bec_size(U64 buf);

INT avm_get_tunable_bec_threads(CHAR *buf);
INT avm_set_tunable_bec_threads(U64 buf);

INT avm_get_tunable_bec_ep(CHAR *buf);
INT avm_set_tunable_bec_ep(U64 buf);

INT avm_get_target_switch_iopath(CHAR *buf);
INT avm_set_target_switch_iopath(U64 buf);

#endif //_SYSFS_ENGINE_H_

/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _VOLUME_ENGINE_H_
#define _VOLUME_ENGINE_H_

//Data Volumes
RINT 	dz_register_datavol_target(VOID);
RVOID 	dz_unregister_datavol_target(VOID);
RVOID	dz_volume_destroy_datavol(PDDVOL ddvol);
RVOID	dz_volume_destroy_datavol_sread(PDDVOL ddvol);
RVOID	dz_volume_destroy_datavol_swrite(PDDVOL ddvol);
RPVOID 	dz_volume_create_lba_table(SIZE table_size);
RVOID 	dz_volume_destroy_lba_table(PDDVOL ddvol);
U64 	dz_get_free_space(VOID);
RVOID 	dz_volume_decrease_free_space(U64 space);
RVOID 	dz_volume_increase_free_space(U64 space);
RINT 	dz_volume_set_used_space(PDM_TARGET target, PDDVOL ddvol);
RVOID 	dz_volume_freeup_space(PDDVOL ddvol);
RINT  	dz_volume_create_datavol(PDM_TARGET target, UINT argc, CHAR ** argv);
RPVOID 	dz_ddvol_create_snapshot(PDM_TARGET target, UINT argc, CHAR ** argv, INT *error);
RINT  	dz_volume_create_datavol_sread(PDM_TARGET target, UINT argc, CHAR ** argv);
RINT  	dz_volume_create_datavol_swrite(PDM_TARGET target, UINT argc, CHAR ** argv);

//Virtual Machine Volumes
RINT 	dz_register_vmvol_target(VOID);
RVOID 	dz_unregister_vmvol_target(VOID);
#endif // _VOLUME_ENGINE_H_

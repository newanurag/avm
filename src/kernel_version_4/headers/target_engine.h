/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#ifndef _TARGET_ENGINE_H_
#define _TARGET_ENGINE_H_

int      dz_avm_target_ctr(struct dm_target *target, unsigned int argc,char **argv);
void     dz_avm_target_dtr(struct dm_target *ti);
int      dz_avm_target_map(struct dm_target *target, struct bio *bio);
int      dz_avm_target_default_map(struct dm_target *target, struct bio *bio);
#endif // _TARGET_ENGINE_H_

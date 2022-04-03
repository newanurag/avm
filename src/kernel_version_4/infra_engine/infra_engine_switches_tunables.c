/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

//INFRA Engine

///////////////////////////////////////
//Enable / Disable BEC Engine
///////////////////////////////////////

INT dz_switch_fec_engine = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_fec_engine);
     
INT dz_switch_bec_engine = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_bec_engine);

INT dz_switch_dedupe_engine = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_dedupe_engine);

INT dz_switch_alignment_engine = DZ_ENABLED;
EXPORT_SYMBOL(dz_switch_alignment_engine);

INT dz_switch_alignment_engine_multi_block_read = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_alignment_engine_multi_block_read);

INT dz_switch_alignment_engine_multi_block_write = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_alignment_engine_multi_block_write);

INT dz_switch_alignment_engine_partial_block_read = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_alignment_engine_partial_block_read);

INT dz_switch_alignment_engine_partial_block_write = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_alignment_engine_partial_block_write);

/////////////////////////////////////////////
// Disabling/Enabling/Passthrough Mode
/////////////////////////////////////////////

INT dz_switch_passthrough_mode    = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_passthrough_mode);

//passthrough_read and passthrough_write will be active if the passthrough_mode is enabled
INT dz_switch_passthrough_read    = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_passthrough_read);

INT dz_switch_passthrough_write   = DZ_ENABLED;
EXPORT_SYMBOL(dz_switch_passthrough_write);

INT dz_switch_fake_write          = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_fake_write);

INT dz_switch_fake_read           = DZ_DISABLED;
EXPORT_SYMBOL(dz_switch_fake_read);



/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

//INFRA Engine
SPINLOCK            print_iorequest_lock;
EXPORT_SYMBOL(print_iorequest_lock);

PMD_LOCK    md_lock_table   = NULL; // Array of spinlocks for MetaData Lock
EXPORT_SYMBOL(md_lock_table);

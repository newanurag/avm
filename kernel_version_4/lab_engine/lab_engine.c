/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

static int dz_lab_engine_module_init(void)
{
    LOGINFF(" ______________________________________________________ \n");
    LOGINFF("|                                                      |\n");
    LOGINFF("|           ZAIDSTOR LAB Engine Module Loaded          |\n");
    LOGINFF("|______________________________________________________|\n");

    RETURNS;
}

static void dz_lab_engine_module_exit(void)
{
    LOGINFF(" ______________________________________________________ \n");
    LOGINFF("|                                                      |\n");
    LOGINFF("|           ZAIDSTOR LAB Engine Module UnLoaded        |\n");
    LOGINFF("|______________________________________________________|\n");
}

module_init(dz_lab_engine_module_init);
module_exit(dz_lab_engine_module_exit);
MODULE_LICENSE(DZ_MODULE_LICENSE);
MODULE_AUTHOR(DZ_MODULE_AUTHOR);

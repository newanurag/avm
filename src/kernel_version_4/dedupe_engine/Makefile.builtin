include $(srctree)/mycompany/avm/mobile_kernel.mk

KBUILD_EXTRA_SYMBOLS := $(obj)/../infra_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../bec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../metadata_engine/Module.symvers

#########################################################################
# Dedupe Engine Source Files
#########################################################################

SRC_DIR                     := 
DEDUPE_ENGINE_SRC_FILES     := $(SRC_DIR)dd_main.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dd_tables.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dd_read.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dd_write.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dd_pools.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dd_hash.c
SRC_FILES                   += $(DEDUPE_ENGINE_SRC_FILES)
OBJ_FILES                   := $(SRC_FILES:.c=.o)

#########################################################################
# Kernel Module Generation
#########################################################################

KERNEL_MODULE_NAME          := $(DZ_DEDUPE_ENGINE_KERNEL_MODULE_NAME)
$(KERNEL_MODULE_NAME)-y 	:= $(OBJ_FILES)
obj-m 						:= $(KERNEL_MODULE_NAME).o

include $(DZ_PROJECT_PATH)/include.mk
KBUILD_EXTRA_SYMBOLS := $(DZ_PROJECT_PATH)/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/infra_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/bec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/dedupe_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/fec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/target_engine/Module.symvers


SCRIPT_NAME:=sysfs.py
OUT_C_FILE:=sysfs_engine_autogen.c


#########################################################################
# Sysfs Engine Source Files
#########################################################################

SRC_DIR                     := 
SYSFS_ENGINE_SRC_FILES      := $(SRC_DIR)sysfs_kernel_autogen.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)sysfs_target_engine_autogen.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)sysfs_alignment_engine_autogen.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)sysfs_bec_engine_autogen.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)sysfs_fec_engine_autogen.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)sysfs_dedupe_engine_autogen.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)sysfs_volume_engine_autogen.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)sysfs_metadata_engine_autogen.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)sysfs_engine_functions.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)target_engine_sysfs_functions.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)alignment_engine_sysfs_functions.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)bec_engine_sysfs_functions.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)fec_engine_sysfs_functions.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)dedupe_engine_sysfs_functions.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)volume_engine_sysfs_functions.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)metadata_engine_sysfs_functions.c
SYSFS_ENGINE_SRC_FILES      += $(SRC_DIR)sysfs_engine.c
SRC_FILES                   += $(SYSFS_ENGINE_SRC_FILES)

#########################################################################
# Kernel Module Generation
#########################################################################

OBJ_FILES                   := $(SRC_FILES:.c=.o)
EXTRA_CFLAGS                +=-g -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK -Wall -Werror -Wfatal-errors

KERNEL_MODULE_NAME           = $(DZ_SYSFS_ENGINE_KERNEL_MODULE_NAME)
$(KERNEL_MODULE_NAME)-objs   = $(OBJ_FILES)
obj-m                       := $(KERNEL_MODULE_NAME).o

.PHONY: autogen

autogen:
	@echo "################################"
	@echo "Building Sysfs Engine"
	@echo "################################"
	./$(SCRIPT_NAME)
	#./$(SCRIPT_NAME) > $(OUT_C_FILE)

all:autogen
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf $(OBJ_FILES)
	rm -rf objdump.txt

ins:
	insmod datazaid_avm_sysfs_engine_module.ko

objdump:
	objdump -D -d -S  $(KERNEL_MODULE_NAME).ko > objdump.txt


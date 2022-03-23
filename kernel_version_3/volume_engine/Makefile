include $(DZ_PROJECT_PATH)/include.mk
KBUILD_EXTRA_SYMBOLS := $(DZ_PROJECT_PATH)/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/infra_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/bec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/dedupe_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/fec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/alignment_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/target_engine/Module.symvers

#########################################################################
# Volume Engine Source Files
#########################################################################

SRC_DIR                     := 
VOLUME_ENGINE_SRC_FILES     := $(SRC_DIR)volume_engine.c
VOLUME_ENGINE_SRC_FILES     += $(SRC_DIR)volume_engine_datavols.c
VOLUME_ENGINE_SRC_FILES     += $(SRC_DIR)volume_engine_vmvols.c
VOLUME_ENGINE_SRC_FILES     += $(SRC_DIR)volume_engine_tables.c
VOLUME_ENGINE_SRC_FILES     += $(SRC_DIR)volume_engine_snapshot.c
SRC_FILES                   += $(VOLUME_ENGINE_SRC_FILES)

#########################################################################
# Kernel Module Generation
#########################################################################

OBJ_FILES                   := $(SRC_FILES:.c=.o)
#EXTRA_CFLAGS                +=-g -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK -Wall -Werror -Wfatal-errors

EXTRA_CFLAGS                += -g -DCONFIG_KALLSYMS -DCONFIG_KALLSYMS_ALL -Wall -Werror -Wfatal-errors
EXTRA_CFLAGS                += -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK


KERNEL_MODULE_NAME           = $(DZ_VOLUME_ENGINE_KERNEL_MODULE_NAME)
$(KERNEL_MODULE_NAME)-objs   = $(OBJ_FILES)
obj-m                       := $(KERNEL_MODULE_NAME).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf $(OBJ_FILES)
	rm -rf objdump.txt

ins:
	insmod $(KERNEL_MODULE_NAME).ko

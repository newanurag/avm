include $(DZ_PROJECT_PATH)/include.mk
KBUILD_EXTRA_SYMBOLS := $(DZ_PROJECT_PATH)/infra_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/bec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/metadata_engine/Module.symvers
#KBUILD_EXTRA_SYMBOLS := $(DZ_PROJECT_PATH)/Module.symvers

#########################################################################
# Dedupe Engine Source Files
#########################################################################

SRC_DIR                     := 
DEDUPE_ENGINE_SRC_FILES     := $(SRC_DIR)dedupe_engine.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dedupe_engine_tables.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dedupe_engine_read.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dedupe_engine_write.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dedupe_engine_pools.c
DEDUPE_ENGINE_SRC_FILES     += $(SRC_DIR)dedupe_engine_hash.c
SRC_FILES                   += $(DEDUPE_ENGINE_SRC_FILES)

#########################################################################
# Kernel Module Generation
#########################################################################

OBJ_FILES                   := $(SRC_FILES:.c=.o)
#EXTRA_CFLAGS                +=-g -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK -Wall -Werror -Wfatal-errors

EXTRA_CFLAGS                += -g -DCONFIG_KALLSYMS -DCONFIG_KALLSYMS_ALL -Wall -Werror -Wfatal-errors
EXTRA_CFLAGS                += -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK


KERNEL_MODULE_NAME           = $(DZ_DEDUPE_ENGINE_KERNEL_MODULE_NAME)
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

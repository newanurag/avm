include $(DZ_PROJECT_PATH)/include.mk
KBUILD_EXTRA_SYMBOLS := $(DZ_PROJECT_PATH)/infra_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/bec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/metadata_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/dedupe_engine/Module.symvers


#########################################################################
# Alignment Engine Source Files
#########################################################################

SRC_DIR                     := 
ALIGN_ENGINE_SRC_FILES        := $(SRC_DIR)alignment_engine.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)alignment_engine_read.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)alignment_engine_read_single.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)alignment_engine_read_partial.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)alignment_engine_read_multi.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)alignment_engine_write.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)alignment_engine_write_single.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)alignment_engine_write_partial.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)alignment_engine_write_multi.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)alignment_engine_thread_pool.c
SRC_FILES                   += $(ALIGN_ENGINE_SRC_FILES)


#########################################################################
# Kernel Module Generation
#########################################################################

OBJ_FILES                   := $(SRC_FILES:.c=.o)
#EXTRA_CFLAGS                +=-g -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK -Wall -Werror -Wfatal-errors

EXTRA_CFLAGS                += -g -DCONFIG_KALLSYMS -DCONFIG_KALLSYMS_ALL -Wall -Werror -Wfatal-errors
EXTRA_CFLAGS                += -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK


KERNEL_MODULE_NAME           = $(DZ_ALIGNMENT_ENGINE_KERNEL_MODULE_NAME)
$(KERNEL_MODULE_NAME)-objs   = $(OBJ_FILES)
obj-m                       := $(KERNEL_MODULE_NAME).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf $(OBJ_FILES)
	rm -rf objdump.txt

objdump:
	objdump -S alignment_engine_read_single.o > ob.txt
	#objdump -d -S  $(KERNEL_MODULE_NAME).ko > objdump.txt
	#objdump -S  $(KERNEL_MODULE_NAME).ko > objdump.txt

ins:
	insmod $(KERNEL_MODULE_NAME).ko

include $(srctree)/mycompany/avm/mobile_kernel.mk
$(info,"AVERMA AE MAKEFILE")

KBUILD_EXTRA_SYMBOLS := $(obj)/../infra_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../bec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../metadata_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../dedupe_engine/Module.symvers


#########################################################################
# Alignment Engine Source Files
#########################################################################

SRC_DIR                     := 
ALIGN_ENGINE_SRC_FILES        := $(SRC_DIR)align_main.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)align_read.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)align_read_single.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)align_read_partial.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)align_read_multi.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)align_write.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)align_write_single.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)align_write_partial.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)align_write_multi.c
ALIGN_ENGINE_SRC_FILES        += $(SRC_DIR)align_thread_pool.c
SRC_FILES                     += $(ALIGN_ENGINE_SRC_FILES)
OBJ_FILES                     := $(SRC_FILES:.c=.o)

#########################################################################
# Kernel Module Generation
#########################################################################

KERNEL_MODULE_NAME          := $(DZ_ALIGNMENT_ENGINE_KERNEL_MODULE_NAME)
$(KERNEL_MODULE_NAME)-y 	:= $(OBJ_FILES)
obj-m 						:= $(KERNEL_MODULE_NAME).o

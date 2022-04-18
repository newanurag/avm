include $(srctree)/mycompany/avm/mobile_kernel.mk

KBUILD_EXTRA_SYMBOLS := $(obj)/../infra_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../lab_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../bec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../metadata_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../dedupe_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../fec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../alignment_engine/Module.symvers


#########################################################################
# Target Engine Source Files
#########################################################################

SRC_DIR                     := 
BASE_ENGINE_SRC_FILES       := $(SRC_DIR)tgt_main.c
SRC_FILES                   += $(BASE_ENGINE_SRC_FILES)
OBJ_FILES                   := $(SRC_FILES:.c=.o)

#########################################################################
# Kernel Module Generation
#########################################################################

KERNEL_MODULE_NAME          := $(DZ_TARGET_ENGINE_KERNEL_MODULE_NAME)
$(KERNEL_MODULE_NAME)-y 	:= $(OBJ_FILES)
obj-m 						:= $(KERNEL_MODULE_NAME).o

include $(srctree)/mycompany/avm/mobile_kernel.mk

KBUILD_EXTRA_SYMBOLS := $(obj)/../infra_engine/Module.symvers

#########################################################################
# Backend Cache Engine Source Files
#########################################################################
 
SRC_DIR                     := 
BEC_ENGINE_SRC_FILES        := $(SRC_DIR)bec_main.c
BEC_ENGINE_SRC_FILES        += $(SRC_DIR)bec_radix_tree.c
BEC_ENGINE_SRC_FILES        += $(SRC_DIR)bec_mempool.c
SRC_FILES                   += $(BEC_ENGINE_SRC_FILES)
OBJ_FILES                   := $(SRC_FILES:.c=.o)

#########################################################################
# Kernel Module Generation
#########################################################################

KERNEL_MODULE_NAME          := $(DZ_BEC_ENGINE_KERNEL_MODULE_NAME)
$(KERNEL_MODULE_NAME)-y 	:= $(OBJ_FILES)
obj-m 						:= $(KERNEL_MODULE_NAME).o

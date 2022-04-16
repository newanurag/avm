include $(srctree)/mycompany/avm/mobile_kernel.mk

#KBUILD_EXTRA_SYMBOLS := $(obj)/../Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../infra_engine/Module.symvers


#########################################################################
# Infrastructure Engine Source Files
#########################################################################

SRC_DIR                   := 
LAB_ENGINE_SRC_FILES      := $(SRC_DIR)lab_main.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_binary_tree.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_bplus_tree.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_bplus_tree_insert.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_bplus_tree_search.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_bplus_tree_traversal.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_bplus_tree_delete.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_bplus_tree_get_set.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_heap.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_interval.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_locks.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_linked_list.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_pipe.c
SRC_FILES                 += $(LAB_ENGINE_SRC_FILES)
OBJ_FILES                   := $(SRC_FILES:.c=.o)


#########################################################################
# Kernel Module Generation
#########################################################################

KERNEL_MODULE_NAME          := $(DZ_LAB_ENGINE_KERNEL_MODULE_NAME)
$(KERNEL_MODULE_NAME)-y 	:= $(OBJ_FILES)
obj-m 						:= $(KERNEL_MODULE_NAME).o

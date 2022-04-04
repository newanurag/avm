include $(DZ_PROJECT_PATH)/include.mk.x86
KBUILD_EXTRA_SYMBOLS := $(DZ_PROJECT_PATH)/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(DZ_PROJECT_PATH)/infra_engine/Module.symvers


#########################################################################
# Infrastructure Engine Source Files
#########################################################################

SRC_DIR                   := 
LAB_ENGINE_SRC_FILES      := $(SRC_DIR)lab_engine.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_binary_tree.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_bplus_tree.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_bplus_tree_insert.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_bplus_tree_search.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_bplus_tree_traversal.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_bplus_tree_delete.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_bplus_tree_get_set.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_heap.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_interval.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_locks.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_linked_list.c
LAB_ENGINE_SRC_FILES      += $(SRC_DIR)lab_engine_pipe.c
SRC_FILES                 += $(LAB_ENGINE_SRC_FILES)


#########################################################################
# Kernel Module Generation
#########################################################################

OBJ_FILES                   := $(SRC_FILES:.c=.o)
EXTRA_CFLAGS                +=-g -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK -Wall -Werror -Wfatal-errors

KERNEL_MODULE_NAME           = $(DZ_LAB_ENGINE_KERNEL_MODULE_NAME)
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

objdump:
	objdump -D -d -S -l -p -r -s --stabs -t --special-syms --all-headers --disassemble-zeroes  $(KERNEL_MODULE_NAME).ko > objdump.txt
	objdump -D -d -S  $(KERNEL_MODULE_NAME).ko > objdump.txt

obj:objdump


loc:
	@echo ""
	@echo "############################"
	@echo "Generating Lines of Code"
	@date
	@make clean > /dev/null 2>&1
	@rm -rf tags
	@rm -rf cscope.out
	@echo ""

	@echo " Kernel Space Lines of Code"
	@wc -l $(DZ_HEADERS_PATH)/lab_engine_*.h $(SRC_FILES) Makefile |grep total
	@echo ""
	@echo "############################"
	@echo ""


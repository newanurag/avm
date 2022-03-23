KERNEL_MODULE_NAME           = tree_deep_copy_kernel_module

#########################################################################
# Deep Copy Source Files
#########################################################################

SRC_DIR                   := 
TREE_DEEP_COPY_SRC_FILES  := $(SRC_DIR)tree_deep_copy.c
TREE_DEEP_COPY_SRC_FILES  += $(SRC_DIR)tree_deep_utils.c
SRC_FILES                 += $(TREE_DEEP_COPY_SRC_FILES)


#########################################################################
# Kernel Module Generation
#########################################################################

OBJ_FILES                   := $(SRC_FILES:.c=.o)
EXTRA_CFLAGS                +=-g -I . -Wall -Werror -Wfatal-errors

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

rm:
	rmmod $(KERNEL_MODULE_NAME)

objdump:
	objdump -D -d -S -l -p -r -s --stabs -t --special-syms --all-headers --disassemble-zeroes  $(KERNEL_MODULE_NAME).ko > objdump.txt
	objdump -D -d -S  $(KERNEL_MODULE_NAME).ko > objdump.txt

cc:
	rm -rf /var/crash/*

obj:objdump

out:
	dmesg > dmesg.out
	dmesg --clear
	insmod $(KERNEL_MODULE_NAME).ko
	dmesg

r:run

rerun:
	dmesg > dmesg.out
	dmesg --clear
	rmmod $(KERNEL_MODULE_NAME)
	insmod $(KERNEL_MODULE_NAME).ko
	dmesg
	
rr:rerun

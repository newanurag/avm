include $(DZ_PROJECT_PATH)/include.mk

DZ_PKG_BUILD_SCRIPT = $(DZ_PROJECT_PATH)/build_pkg/dz_prepkg.sh
DZ_RPM_BUILD_SCRIPT = $(DZ_PROJECT_PATH)/build_pkg/dz_makerpm.sh

DZ_PACKAGE_DIR_BASE = $(DZ_PROJECT_PATH)/packaging
DZ_PACKAGE_DIR_SHIP_BASE = $(DZ_PACKAGE_DIR_BASE)
DZ_PACKAGE_DIR_SHIP_COMPLETE = $(DZ_PACKAGE_DIR_SHIP_BASE)/release

DZ_CURRENT_BUILD_PATH = $(DZ_PROJECT_PATH)
DZ_HEADERS_PATH="${DZ_CURRENT_BUILD_PATH}/headers"
DZ_HASH_PATH="${DZ_CURRENT_BUILD_PATH}/hash_engine"

DZ_LOG_ENGINE_PATH       := $(DZ_CURRENT_BUILD_PATH)/log_engine
DZ_INFRA_ENGINE_PATH     := $(DZ_CURRENT_BUILD_PATH)/infra_engine
DZ_LAB_ENGINE_PATH       := $(DZ_CURRENT_BUILD_PATH)/lab_engine
DZ_BEC_ENGINE_PATH       := $(DZ_CURRENT_BUILD_PATH)/bec_engine
DZ_METADATA_ENGINE_PATH  := $(DZ_CURRENT_BUILD_PATH)/metadata_engine
DZ_DEDUPE_ENGINE_PATH    := $(DZ_CURRENT_BUILD_PATH)/dedupe_engine
DZ_ALIGNMENT_ENGINE_PATH := $(DZ_CURRENT_BUILD_PATH)/alignment_engine
DZ_FEC_ENGINE_PATH       := $(DZ_CURRENT_BUILD_PATH)/fec_engine
DZ_TARGET_ENGINE_PATH    := $(DZ_CURRENT_BUILD_PATH)/target_engine
DZ_VOLUME_ENGINE_PATH    := $(DZ_CURRENT_BUILD_PATH)/volume_engine
DZ_SYSFS_ENGINE_PATH     := $(DZ_CURRENT_BUILD_PATH)/sysfs_engine
DZ_KERNEL_OBJECTS_PATH   := $(DZ_CURRENT_BUILD_PATH)/kernel_objects	



#DZ_PACKAGE_NAME_BASE = avm
DZ_PACKAGE_NAME_BASE = zaidstor_databank
#DZ_PACKAGE_NAME_BASE = "zaidstor_databank_""`date +'%d_%m_%Y_%H_%M_%S'`"
DZ_PACKAGE_NAME_SHIP_COMPLETE = $(DZ_PACKAGE_NAME_BASE)
DZ_PACKAGE_NAME_SHIP_SOURCE_CODE = avm_src

#########################################################################
# Kernel Module Generation
#########################################################################
ARG_TARGET_KERNEL_SRC_PATH  = /home/averma/kernel/linux-4.14.271/linux-4.14.271

OBJ_FILES                   := $(SRC_FILES:.c=.o)
EXTRA_CFLAGS                +=-g -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK -Wall -Wfatal-errors -DDZ_KERNEL_VERSION_4
EXTRA_CFLAGS2               +=-g -E -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK -Wall -Werror -Wfatal-errors -DDZ_KERNEL_VERSION_4

KERNEL_MODULE_NAME           = datumsoft_zaidstor_avm_aisa_target_module
$(KERNEL_MODULE_NAME)-objs   = $(OBJ_FILES)
obj-m                       := $(KERNEL_MODULE_NAME).o


.PHONY: subdirs $(DZ_SUBDIRS) tag

_infra:
	@echo ""
	@echo "############################################"
	@echo "2/11 - Building AVM Infrastructure Engine"
	@date
	@echo $(DZ_LOG_ENGINE_PATH)
	@echo "############################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_INFRA_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#@cp $(DZ_CURRENT_BUILD_PATH)/infra_engine/*.ko .
	@echo ""
	@date

infra:_infra

_lab:
	@echo ""
	@echo "#########################################"
	@echo "3/11 - Building AVM LAB Engine"
	@date
	@echo $(DZ_LAB_ENGINE_PATH)
	@echo "#########################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_LAB_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#@cp $(DZ_CURRENT_BUILD_PATH)/lab_engine/*.ko .
	@echo ""
	@date

lab:infra _lab

_bec:
	@echo ""
	@echo "###############################################"
	@echo "4/11 - Building AVM BEC (Back End Cache) Engine"
	@date
	@echo $(DZ_BEC_ENGINE_PATH)
	@echo "###############################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_BEC_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#@cp $(DZ_CURRENT_BUILD_PATH)/bec_engine/*.ko .
	@echo ""
	@date

bec:infra _bec

_md:
	@echo ""
	@echo "#################################################"
	@echo "5/11 - Building AVM MetaData Engine"
	@date
	@echo $(DZ_METADATA_ENGINE_PATH)
	@echo "#################################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_METADATA_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#@cp $(DZ_CURRENT_BUILD_PATH)/metadata_engine/*.ko .
	@echo ""
	@date

md:bec _md


_dedupe:
	@echo ""
	@echo "###############################################"
	@echo "6/11 - Building AVM Dedupe Engine"
	@date
	@echo $(DZ_DEDUPE_ENGINE_PATH)
	@echo "###############################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_DEDUPE_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#@cp $(DZ_CURRENT_BUILD_PATH)/dedupe_engine/*.ko .
	@echo ""
	@date

dedupe:md _dedupe

_fec:
	@echo ""
	@echo "################################################"
	@echo "7/11 - Building AVM FEC (Front End Cache) Engine"
	@date
	@echo $(DZ_FEC_ENGINE_PATH)
	@echo "################################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_FEC_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#@cp $(DZ_CURRENT_BUILD_PATH)/fec_engine/*.ko .
	@echo ""
	@date

fec:dedupe _fec

_ae:
	@echo ""
	@echo "################################################"
	@echo "8/11 - Building AVM Alignment Engine"
	@date
	@echo $(DZ_ALIGNMENT_ENGINE_PATH)
	@echo "################################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_ALIGNMENT_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#@cp $(DZ_CURRENT_BUILD_PATH)/alignment_engine/*.ko .
	@echo ""
	@date

ae:fec _ae

_target:
	@echo ""
	@echo "################################################"
	@echo "9/11 - Building AVM Target Engine"
	@date
	@echo $(DZ_TARGET_ENGINE_PATH)
	@echo "################################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_TARGET_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#@cp $(DZ_CURRENT_BUILD_PATH)/target_engine/*.ko .
	@echo ""
	@date

target:ae _target

_volume:
	@echo ""
	@echo "################################################"
	@echo "10/11 - Building AVM Volume Engine"
	@date
	@echo $(DZ_VOLUME_ENGINE_PATH)
	@echo "################################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_VOLUME_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#@cp $(DZ_CURRENT_BUILD_PATH)/volume_engine/*.ko .
	@echo ""
	@date

volume:target _volume
	
_sysfs:
	@echo ""
	@echo "################################################"
	@echo "11/11 - Building AVM Sysfs Engine"
	@echo `pwd`
	@date
	@echo $(DZ_SYSFS_ENGINE_PATH)
	@echo "################################################"
	@echo ""
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_SYSFS_ENGINE_PATH) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
	@echo ""
	#cp $(DZ_CURRENT_BUILD_PATH)/sysfs_engine/*.ko .
	@echo ""
	date

sysfs:volume _sysfs

KERNEL_MODULES := $(DZ_LOG_ENGINE_KERNEL_MODULE_NAME).ko
KERNEL_MODULES += $(DZ_INFRA_ENGINE_KERNEL_MODULE_NAME).ko
KERNEL_MODULES += $(DZ_BEC_ENGINE_KERNEL_MODULE_NAME).ko
KERNEL_MODULES += $(DZ_METADATA_ENGINE_KERNEL_MODULE_NAME).ko
KERNEL_MODULES += $(DZ_DEDUPE_ENGINE_KERNEL_MODULE_NAME).ko
KERNEL_MODULES += $(DZ_FEC_ENGINE_KERNEL_MODULE_NAME).ko
KERNEL_MODULES += $(DZ_TARGET_ENGINE_KERNEL_MODULE_NAME).ko
KERNEL_MODULES += $(DZ_VOLUME_ENGINE_KERNEL_MODULE_NAME).ko
KERNEL_MODULES += $(DZ_SYSFS_ENGINE_KERNEL_MODULE_NAME).ko
KERNEL_MODULES += $(DZ_LAB_ENGINE_KERNEL_MODULE_NAME).ko

#all:$(KERNEL_MODULES)
alld:tags sysfs
	logger `date`
	@echo ""
	@echo "##################################################################################"
	@echo "List and Checksum of Kernel Modules generated" 
	@echo "##################################################################################"
	@cksum *.ko
	@echo "##################################################################################"
	@echo "Compilation Done"
	@date
	@echo "##################################################################################"

d:alld




all:sysfs
	logger `date`
	@echo ""
	@echo "##################################################################################"
	@echo "List and Checksum of Kernel Modules generated" 
	@echo "##################################################################################"
	@cksum *.ko
	@echo "##################################################################################"
	@echo "Compilation Done"
	@date
	@echo "##################################################################################"

clean:

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning Log Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_LOG_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning Infra Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_INFRA_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning LAB Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_LAB_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning BEC Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_BEC_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning FEC Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_FEC_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning Alignment Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_ALIGNMENT_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning Dedupe Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_DEDUPE_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning Metadata Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_METADATA_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning Target Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_TARGET_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning Volume Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_VOLUME_ENGINE_PATH)/ clean > /dev/null

	@echo ""
	@echo "#########################################################"
	@echo "Cleaning Sysfs Engine" `date`
	@echo "#########################################################"
	make -C $(ARG_TARGET_KERNEL_SRC_PATH) M=$(DZ_SYSFS_ENGINE_PATH)/ clean > /dev/null

	@rm -rf $(OBJ_FILES)
	@rm -rf objdump.txt
	@rm -rf *.ko
	@echo ""
	@echo "#########################################"
	@echo "Cleaning Completed"
	@date
	@echo "#########################################"

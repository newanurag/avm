#include $(DZ_PROJECT_PATH)/include.mk

DZ_PKG_BUILD_SCRIPT = $(DZ_PROJECT_PATH)/build_pkg/dz_prepkg.sh
DZ_RPM_BUILD_SCRIPT = $(DZ_PROJECT_PATH)/build_pkg/dz_makerpm.sh

DZ_PACKAGE_DIR_BASE = $(DZ_PROJECT_PATH)/packaging
DZ_PACKAGE_DIR_SHIP_BASE = $(DZ_PACKAGE_DIR_BASE)
DZ_PACKAGE_DIR_SHIP_COMPLETE = $(DZ_PACKAGE_DIR_SHIP_BASE)/release


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
EXTRA_CFLAGS                +=-g -I $(DZ_HEADERS_PATH) -I $(DZ_HASH_PATH) -I . -DDZ_BIT_SPIN_LOCK -Wall -Werror -Wfatel-errors

KERNEL_MODULE_NAME           = datumsoft_zaidstor_avm_aisa_target_module
$(KERNEL_MODULE_NAME)-objs   = $(OBJ_FILES)
obj-m                       := $(KERNEL_MODULE_NAME).o

CLI_ARGS := $(shell $$*)


.PHONY: subdirs $(DZ_SUBDIRS) tag

help:
	@echo "################################################################################"
	@echo "      DATAUMSOFT AVM Build/Make Help "
	@echo "################################################################################"
	@echo ""
	@echo "1) To build all engines/modules"
	@echo "# make 3|3c -> For building/cleaning Kernel 3.10.1"
	@echo "# make 4|4c -> For building/cleaning Kernel 4.14.271"
	@echo "# make r|rc -> For building/cleaning live/running Kernel"
	@echo ""
	@echo "2) To build selcted engine module."
	@echo "# NOTE: It is not implemented yet for Kernel Version 3"
	@echo "# make 4log    -> For Log                    Engine"
	@echo "# make 4infra  -> For Infrastructre          Engine"
	@echo "# make 4lab    -> For Lab                    Engine"
	@echo "# make 4bec    -> For BackEnd  Cache         Engine"
	@echo "# make 4fec    -> For FrontEnd Cache         Engine"
	@echo "# make 4dd     -> For Data Deduplication     Engine"
	@echo "# make 4md     -> For MetaData               Engine"
	@echo "# make 4ae     -> For Aignment               Engine"
	@echo "# make 4volume -> For Logical Volume Manager Engine"
	@echo "# make 4target -> For Target                 Engine"
	@echo "# make 4sysfs  -> For sysfs entries          Engine"
	@echo ""
	@echo "3) To build RPM Package. It will build CLI and Kernel modules in one rpm package "
	@echo "# make pkg"
	@echo ""
	@echo "4) Normal Usage:"
	@echo "# make 3   -> For building kernel 3.10.1 "
	@echo "# make 3c  -> For cleaning kernel 3.10.1 "
	@echo "# make 3ca -> For cleaning and building kernel 3.10.1 "
	@echo "# make 4   -> For building kernel 4.14.271 "
	@echo "# make 4c  -> For cleaning kernel 4.14.271 "
	@echo "# make 4ca -> For cleaning and building kernel 4.14.271 "
	@echo "# make 3 4 -> For building both 3 and 4 kernel targets"
	@echo "################################################################################"

disclaimer:
	for f in `find . -name test.c`; do echo $$f; cat disclaimer.txt > temp.c; cat $$f >> temp.c; mv temp.c $$f; done

pkg:
	@echo ""
	@echo "##################################################################################"
	@echo "Building RPM Package of AVM" 
	@date
	@echo "##################################################################################"
	@echo ""
	@DZ_PROJECT_PATH=$(DZ_PROJECT_PATH) \
		DZ_PACKAGE_DIR=$(DZ_PACKAGE_DIR_SHIP_COMPLETE) DZ_PACKAGE_NAME=${DZ_PACKAGE_NAME_SHIP_COMPLETE} \
		${DZ_RPM_BUILD_SCRIPT} > /dev/null
	@echo "##################################################################################"
	@echo ""
	@date
	#scp packaging/release/avm-1.0-0.el7.centos.x86_64.rpm root@192.168.122.233:/home/aisa/

package:pkg


srcpkg:
	@echo ""
	@echo "##################################################################################"
	@echo "Building RPM Package of AVM Source Code" 
	@echo "##################################################################################"
	@echo ""
	@DZ_PROJECT_PATH=$(DZ_PROJECT_PATH) \
		DZ_PACKAGE_DIR=$(DZ_PACKAGE_DIR_SHIP_COMPLETE) DZ_PACKAGE_NAME=${DZ_PACKAGE_NAME_SHIP_SOURCE_CODE} \
		${DZ_RPM_BUILD_SCRIPT}
	@echo "##################################################################################"
	@echo ""
	#scp packaging/release/avm-1.0-0.el7.centos.x86_64.rpm root@192.168.122.233:/home/aisa/
	date

c:clean

a:all

p:pkg


all:help

clean:


ca:clean all
	

. PHONY avm:
	avm aisa
lsmod:log
	lsmod |grep datazaid_avm_aisa_module


install:
	

load:ins

drm:log
	dmsetup remove AISA
	rmmod datazaid_avm_aisa_target_module.ko

dmr:log
	dmsetup remove AISA
	rmmod datazaid_avm_aisa_target_module.ko

rm:log
	rmmod datazaid_avm_sysfs_engine_module.ko
	rmmod datazaid_avm_volume_engine_module.ko
	rmmod datazaid_avm_aisa_target_module.ko

ls:lsmod
	dmsetup ls

clear:log
	#dmsetup -f remove aisavfv
	losetup -D
	rmmod datazaid_avm_aisa_module.ko

.PHONY: tags tag

tag:tags

tags:
	rm -rf tags
	ctags -R
	

log:

	logger `date`
	@echo ""
	@echo "#########################################"
	@echo "Building AVM Log Engine"
	@date
	@echo "#########################################"
	@echo ""
	@make -C /home/averma/mobile/kernel.org/4.14.270/linux-4.14.270/ M=$(PWD)/log_engine modules
	@echo ""
	@cp $(PWD)/log_engine/*.ko .
	@echo ""
	@date

3:
	make -f kernel_version_3/Makefile.3.10.1 all

3c:
	make -f kernel_version_3/Makefile.3.10.1 clean

3a:3

3ca:3c 3a

###############################################################################
# FOR KERNEL VERSION 4 BEGIN
###############################################################################

4:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 all)

4all:4

4c:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 clean)

4log:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _log)

4lab:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _lab)

4infra:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _infra)

4lab:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _lab)

4bec:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _bec)

4md:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _md)

4dedupe:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _dedupe)

4dd: 4dedupe

4fec:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _fec)

4ae:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _ae)

4target:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _target)

4volume:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _volume)

4sysfs:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 _sysfs)

4pkg:
	(cd kernel_version_4/;pwd;	. ./setup; make -f Makefile.x86 pkg)


4a:4

4ca:4c 4a
###############################################################################
# FOR KERNEL VERSION 4 END
###############################################################################

r:
	make -f Makefile.running_kernel all

rc:
	make -f Makefile.running_kernel clean

ra:r

rca:rc ra




ca:clean all

cap:clean all package
ap:all package
at:all test
cat: clean all test
captain:clean all package test
capt:clean all package test

capt: cap test

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
	@wc -l headers/*.h $(SRC_FILES) log_engine/{Makefile,*.c} alignment_engine/{Makefile,*.c} infra_engine/{Makefile,*.c} bec_engine/{Makefile,*.c} fec_engine/{Makefile,*.c}  dedupe_engine/{Makefile,*.c} metadata_engine/{Makefile,*.c} target_engine/{Makefile,*.c} volume_engine/{Makefile,*.c} sysfs_engine/{Makefile,*.c,*.py,*.raw} Makefile |grep total
	@echo ""
	@echo " User   Space Lines of Code"
	@wc -l cli_engine/AISA_Avm.py cli_engine/AISA_AVM*.py cli_engine/AISA_Utils.py cli_engine/AISA_Defaults.py scripts/avmconf.sh |grep total
	@echo ""
	@echo " Total Lines of Code"
	@wc -l headers/*.h $(SRC_FILES) log_engine/{Makefile,*.c} alignment_engine/{Makefile,*.c} infra_engine/{Makefile,*.c} bec_engine/{Makefile,*.c} fec_engine/{Makefile,*.c} dedupe_engine/{Makefile,*.c} metadata_engine/{Makefile,*.c}  target_engine/{Makefile,*.c} volume_engine/{Makefile,*.c} sysfs_engine/{Makefile,*.c,*.py,*.raw} Makefile cli_engine/AISA_Avm.py cli_engine/AISA_AVM*.py cli_engine/AISA_Utils.py cli_engine/AISA_Defaults.py scripts/avmconf.sh build_pkg/dz_makerpm.sh build_pkg/dz_rpm_basic.spec build_pkg/dz_prepkg.sh |grep total
	@echo "############################"
	@echo ""

obj:objdump

objdump:
	objdump -D -d -S  $(KERNEL_MODULE_NAME).ko > objdump.txt

#########################################################################
# Miscellaneous Commands
#########################################################################

.PHONY: dde fec bec db
dde:
	cat /sys/kernel/avm/dedupe

sysfec:
	cat /sys/kernel/avm/fec

sysbec:
	cat /sys/kernel/avm/bec

db:
	cat /sys/kernel/avm/bec
	cat /sys/kernel/avm/dedupe
	cat /sys/kernel/avm/fec
	dmesg

zero:
	#dd if=/dev/zero of=/dev/mapper/AISA bs=8192 
	dd if=/dev/zero of=/dev/md0 bs=8192 count=1024
	dd if=/dev/zero of=/dev/md1 bs=8192 count=1024

#Clean /var/crash
varc:
	rm -rf /var/crash/*
	sync

.PHONY: configfs
configfs:
	mount -t configfs none /config

vi:
	gvim -p  fec_engine_flush.c $(VI_FILES)

#ln -s /aisa/aisa_tree/src/storage_engine/user/cli/AISA_FlashVol.py avm

backup:
	date +'%d_%m_%Y_%H_%M_%S'
	(cd ../../../../; pwd)
	pwd

pagec:
	echo 1 > /proc/sys/vm/drop_caches
	sync
	
scp:
	#scp *.ko /home/aisa/flash_volume_manager/cli/* aisa@192.168.122.233:/home/aisa/avm/
	#scp *.ko makefile.remote /home/aisa/avmdev/cli/*.py root@testvm:/home/averma/test/
	scp -r /home/averma/avm/kernel_version_4/kernel_objects root@testvm:/home/averma/avm/
	scp -r /home/averma/avm/kernel_version_4/scripts        root@testvm:/home/averma/avm/

treboot:
	ssh root@192.168.122.233 "cd /home/aisa/avm; make -f /home/aisa/avm/makefile.remote treboot"

tshut:
	ssh root@192.168.122.233 "cd /home/aisa/avm; make -f /home/aisa/avm/makefile.remote tshut"

list:
	find ${PWD} -name "*.ko"

cs:
	rm -rf tags cscope.out ; ctags -R ; cscope -R

scppkg:
	@scp packaging/release/zaidstor_databank-1.1.a_3.10-101.el7.centos.x86_64.rpm root@testvm:/home/aisa/

tstop:
	-@ssh root@testvm "avm stop"

tinstall:
	@ssh root@testvm "make -f /home/aisa/avm/makefile.testvm reinstall"

tstart:
	@ssh root@testvm "avm start"

test:pkg scppkg tstop tinstall tstart

findrpm:
	date
	find . -name "*.rpm" |xargs ls -lrth

crash:
	alias cr="crash /usr/lib/debug/lib/modules/3.10.0-514.el7.x86_64/vmlinux `ls -d /var/crash/* |tail -1`/vmcore"


dm:
	dmesg

dmesg:dm

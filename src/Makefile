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

all:help

lsmod:log
	lsmod |grep avm


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
# FOR ARM64 Architecture
###############################################################################

arm:
	(cd kernel_version_4/;pwd;	. ./setup.arm64; make -f Makefile.arm64 all)

carm:
	(cd kernel_version_4/;pwd;	. ./setup.arm64; make -f Makefile.arm64 clean all)

###############################################################################
# FOR copying kernel objects on test machine
###############################################################################
	
scp:
	scp -r /home/averma/avm/src/kernel_version_4/kernel_objects root@testvm:/home/averma/avm/
	scp -r /home/averma/avm/src/kernel_version_4/scripts        root@testvm:/home/averma/avm/

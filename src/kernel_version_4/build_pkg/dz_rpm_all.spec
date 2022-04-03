# Spec file for creating RPM packages for DatumSoft AISA Volume Manager

Name: %{_NAME}
Vendor:         DatumSoft
Version:        %{_VERSION}
Release:        %{_RELEASE}%{?dist}
Summary:        DATUMSOFT DATABANK ZAIDSTOR AVM
Group:          Development/storage
License:        Proprietary

# Default install path is /opt/datumsoft
Prefix: /opt

#A directory named as DATUMSOFT will be created in /opt folder
%define RPM_DIR %{prefix}/DATUMSOFT/%{_NAME}
#%define CONFIG_PATH  /usr/local/etc/
%define CONFIG_PATH  /etc/datumsoft/
%define CLEAN_DIR %{prefix}/DATUMSOFT/

%define AVM_DIR %{prefix}/DATUMSOFT/%{_NAME}/avm
%define LIO_DIR %{prefix}/DATUMSOFT/%{_NAME}/lio
%define DM_DIR %{prefix}/DATUMSOFT/%{_NAME}/dm

%description
DatumSoft AISA Volume Manager (AVM) for ZaidStor Storage Stack RPM

%pre
	###########################
	#Pre Installation
	###########################

# Make sure installtion filesystem has enough space
if [ $(df -m %{prefix} | tail -1 | awk '{print $4}') -lt 500 ]; then 
    echo -n "$0: error: You need at least 500MB free space to install "
    echo "DataZAid Aisa Volume Manager"
    exit 1
fi

%install
mkdir -p  "$RPM_BUILD_ROOT%{RPM_DIR}"
DZ_PROJECT_SOURCE_PATH=%{_BUILD_DIR} DZ_PACKAGE_DIR=${RPM_BUILD_ROOT}%{RPM_DIR} ${DZ_PKG_BUILD_SCRIPT} &> %{_STAT_FILE}

%post

	###########################
	#Post Installation
	###########################

	#Copy the config file to /etc folder
	#echo "   Preparing config setup..."
	mkdir -p %{CONFIG_PATH}
	#ln -s %{AVM_DIR}/etc %{CONFIG_PATH}
	cp %{AVM_DIR}/etc/avm.conf %{CONFIG_PATH}

	#Create soft links for avm
	rm -rf %{AVM_DIR}/bin/avm
	ln -s %{AVM_DIR}/bin/AISA_Avm.py  %{AVM_DIR}/bin/avm

	rm -rf /usr/bin/avm
	ln -s %{AVM_DIR}/bin/avm /usr/bin/avm

	rm -rf /usr/bin/dzcat
	ln -s %{AVM_DIR}/bin/dzcat /usr/bin/dzcat

	rm -rf /usr/bin/avmlog
	ln -s %{AVM_DIR}/scripts/avmlog.sh /usr/bin/avmlog

	rm -rf /usr/bin/avmconf
	ln -s %{AVM_DIR}/scripts/avmconf.sh /usr/bin/avmconf


	################################################
	#Save the dmesg out and clear the ring buffer
	################################################
    dmesg -H      > %{AVM_DIR}/logs/dmesg_old.log 2>&1
    dmesg --clear > /dev/null 2>&1

	################################################
	#Load the Kernel Modules. Sequence is important
	################################################
	echo ""
	echo "   Loading Kernel Modules"
	echo ""
	ierror=0
	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_1_B_log_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_1_B_log_engine_module.ko       Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_2_B_infra_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_2_B_infra_engine_module.ko     Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_3_B_lab_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_3_B_lab_engine_module.ko       Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_4_B_bec_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_4_B_bec_engine_module.ko       Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_5_B_metadata_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_5_B_metadata_engine_module.ko  Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_6_B_dedupe_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_6_B_dedupe_engine_module.ko    Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_7_B_alignment_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_7_B_alignment_engine_module.ko Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_8_B_fec_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_8_B_fec_engine_module.ko       Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_9_B_target_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_9_B_target_engine_module.ko    Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_A_B_volume_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_A_B_volume_engine_module.ko    Loaded"
	else
		ierror=1
	fi

	insmod %{AVM_DIR}/kernel/datumsoft_zaidstor_avm_B_B_sysfs_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_B_B_sysfs_engine_module.ko     Loaded"
	else
		ierror=1
	fi


	if [ $ierror -eq 0 ]; then
		%{AVM_DIR}/bin/AISA_Avm.py --sizeof > %{AVM_DIR}/logs/sizeof_datastructures.log
		echo ""
		echo "   ############################################################################"
		echo ""
		echo "   Checking existence of Data and MetaData Disks"
		#%{AVM_DIR}/bin/AISA_Avm.py --config  > /dev/null 2>&1
		%{AVM_DIR}/bin/AISA_Avm.py --config  > %{AVM_DIR}/logs/config.log 2>&1
		if [ $? -eq 0 ]; then
			idisks=`%{AVM_DIR}/bin/AISA_Avm.py --disks > /dev/null 2>&1`
			
			echo "   Good ! Data and MetaData Disks found"
			echo ""
			echo "   Creating AVM AISA Target of 5 GB. Wait for few seconds"
			%{AVM_DIR}/bin/AISA_Avm.py --create AISA --size 5 > /dev/null 2>&1
			if [ $? -eq 0 ]; then
				echo "   AVM AISA Target Created successfully."
				echo ""
				echo "   Congratulations !!!"
				echo "   Package is installed successfully in %{CLEAN_DIR}"
			else
				echo "   AVM AISA Target Creation Failed."
				echo "   Check /var/log/messages or 'cat /dev/dzlog'"
				echo ""
				echo "   Congratulations With Warning !!!"
				echo "   Package is installed successfully in %{CLEAN_DIR},"
				echo "   but AISA Target was not created."
				echo "   Check /var/log/messages or 'cat /dev/dzlog' for root cause."
			fi
		else
			echo "   [1] Data Disk or MetaDisk or both is/are NOT accessible/present"
			echo "   OR"
			echo "   [2] Kernel Modules are not loaded correctly or missing"

			echo "   Kindly set the proper path in config file"
		fi

		                               dmesg -H > %{AVM_DIR}/logs/dmesg.log 2>&1
		%{AVM_DIR}/bin/AISA_Avm.py --info  all  > %{AVM_DIR}/logs/devices_info.log 2>&1
		%{AVM_DIR}/bin/AISA_Avm.py --stats -s   > %{AVM_DIR}/logs/superblock.log 2>&1
		%{AVM_DIR}/bin/AISA_Avm.py --stats all  > %{AVM_DIR}/logs/engines_stats.log 2>&1
		%{AVM_DIR}/bin/AISA_Avm.py --list  all  > %{AVM_DIR}/logs/volumes_list.log 2>&1
		%{AVM_DIR}/bin/AISA_Avm.py --status     > %{AVM_DIR}/logs/sys_entries.log 2>&1
		%{AVM_DIR}/bin/AISA_Avm.py --km         > %{AVM_DIR}/logs/kernel_modules.log 2>&1
		%{AVM_DIR}/bin/AISA_Avm.py --size all   > %{AVM_DIR}/logs/disks_size.log 2>&1

		echo ""
		echo "   Please refer to README.cli in %{AVM_DIR}/docs folder for cli commands"
		echo ""
		echo "   ZAIDSTOR AVM Config file Dir         : %{CONFIG_PATH}"
		echo "   ZAIDSTOR AVM Config file Name        : %{CONFIG_PATH}avm.conf"
		echo "   ZAIDSTOR AVM Target Info file Name   : %{AVM_DIR}/logs/devices_info.log"
		echo "   ZAIDSTOR AVM Initial Stats file Name : %{AVM_DIR}/logs/engines_stats.log"
		echo "   ZAIDSTOR AVM Superblock Header       : %{AVM_DIR}/logs/superblock.log"
		echo "   ZAIDSTOR AVM Storage Stack Logs      : %{AVM_DIR}/logs/avm.log"
		echo "   ZAIDSTOR AVM Cli interface Name      : avm"
		echo ""
		echo "   ############################################################################"
		echo ""
		echo ""
	else
		echo ""
		echo "   ############################################################################"
		echo ""
		echo "   Congratulations With Warning !!!"
		echo "   Package is installed successfully in %{CLEAN_DIR},"
		echo "   but some kernel modules loading resulted in error"
		echo "   Please uninstall it, reboot and then install the package again."
		echo "   Please refer to README.cli in %{AVM_DIR}/docs folder for cli commands"
		echo ""
		echo ""
		echo "   Config file Dir  : %{CONFIG_PATH}"
		echo "   Config file Name : %{CONFIG_PATH}avm.conf"
		echo "   Main cli command : avm"
		echo ""
		echo "   ############################################################################"
		echo ""
		echo ""

	fi

	################################################
	#Log everything in avm.log file
	################################################
	%{AVM_DIR}/scripts/avmlog.sh f %{AVM_DIR}/logs/avm.log > /dev/null 2>&1

	#Creating alias
	alias cdavm="cd %{AVM_DIR}"
	alias cdlog="cd %{AVM_DIR}/logs"
	alias cdlogs="cd %{AVM_DIR}/logs"
	alias cdl="cd %{AVM_DIR}/logs"
%preun
	###########################
	#echo "Inside PreUninstall"
	###########################

	echo ""
	echo "   ###################################################################"
	echo ""

	ls -l /dev/mapper/AISA > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		echo "   AISA Target found. Destroying it."
		%{AVM_DIR}/bin/AISA_Avm.py destroy > /dev/null
	fi

%postun
	###########################
	#echo "Inside PostUninstall"
	###########################

	#################################################
	#UnLoad the Kernel Modules. Sequence is important
	#################################################

	ls -d /sys/module/datumsoft_zaidstor_avm_B_B_sysfs_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
			echo "   Unloading Kernel Modules:"
			rmmod datumsoft_zaidstor_avm_B_B_sysfs_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_A_B_volume_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_A_B_volume_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_9_B_target_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_9_B_target_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_8_B_fec_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_8_B_fec_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_7_B_alignment_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_7_B_alignment_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_6_B_dedupe_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_6_B_dedupe_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_5_B_metadata_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_5_B_metadata_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_4_B_bec_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_4_B_bec_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_3_B_lab_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_3_B_lab_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_2_B_infra_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_2_B_infra_engine_module
	fi

	ls -d /sys/module/datumsoft_zaidstor_avm_1_B_log_engine_module > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		rmmod datumsoft_zaidstor_avm_1_B_log_engine_module
	fi

	#sleep 1

	echo "   Deleting config directory %{CONFIG_PATH}"
	rm -rf %{CONFIG_PATH}

	echo "   Deleting installation directory $RPM_BUILD_ROOT%{CLEAN_DIR}"
	rm -rf /usr/bin/avm
	rm -rf %{AVM_DIR}/bin/avm
	rm -rf "$RPM_BUILD_ROOT%{CLEAN_DIR}"
	#rm -rf /usr/bin/avm
	#sleep 1

	echo "   Package is uninstalled successfully on `date`"
	echo ""
	echo "   ###################################################################"
	echo ""
	echo ""

%clean
#[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%{CLEAN_DIR}

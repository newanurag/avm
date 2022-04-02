AVM_DIR="/opt/DATUMSOFT/zaidstor_databank/avm"
CONFIG_DIR="/etc/datumsoft"
MODULE_DIR="${AVM_DIR}/kernel"

	ls -d /sys/module/datumsoft_zaidstor_avm_2_B_infra_engine_module > /dev/null 2>&1
    if [ $? -eq 0 ]; then
		echo ""
		echo "ZAIDSTOR AVM Already started"
		echo ""
		exit -1
    fi 

	################################################
	#Load the Kernel Modules. Sequence is important
	################################################

	################################################
	#Save the dmesg out and clear the ring buffer
	################################################
	dmesg -H      > ${AVM_DIR}/logs/dmesg_old.log 2>&1
	dmesg --clear > /dev/null 2>&1

	echo "   ################################################################################################"
	echo ""
	echo "   Loading Kernel Modules"
	echo ""
	ierror=0
	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_1_B_log_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_1_B_log_engine_module.ko       Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_2_B_infra_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_2_B_infra_engine_module.ko     Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_3_B_lab_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_3_B_lab_engine_module.ko       Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_4_B_bec_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_4_B_bec_engine_module.ko       Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_5_B_metadata_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_5_B_metadata_engine_module.ko  Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_6_B_dedupe_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_6_B_dedupe_engine_module.ko    Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_7_B_alignment_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_7_B_alignment_engine_module.ko Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_8_B_fec_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_8_B_fec_engine_module.ko       Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_9_B_target_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_9_B_target_engine_module.ko    Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_A_B_volume_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_A_B_volume_engine_module.ko    Loaded"
	else
		ierror=1
	fi

	insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_B_B_sysfs_engine_module.ko
	if [ $? -eq 0 ]; then
		echo "   Module datumsoft_zaidstor_avm_B_B_sysfs_engine_module.ko     Loaded"
	else
		ierror=1
	fi


	if [ $ierror -eq 0 ]; then
		${AVM_DIR}/bin/AISA_Avm.py --sizeof > ${AVM_DIR}/logs/sizeof_datastructures.log
		echo ""
		echo "   ################################################################################################"
		echo ""
		echo "   Checking existence of Data and MetaData Disks"
		${AVM_DIR}/bin/AISA_Avm.py --config  > ${AVM_DIR}/logs/config.log 2>&1
		if [ $? -eq 0 ]; then
			idisks=`${AVM_DIR}/bin/AISA_Avm.py --disks > /dev/null 2>&1`
			
			echo "   Good ! Data and MetaData Disks found"
			echo ""
			echo "   Loading Config Parameters into Kernel"
			${AVM_DIR}/bin/AISA_Avm.py --loadconfig > /dev/null 2>&1
			#${AVM_DIR}/bin/AISA_Avm.py --loadconfig 
			echo ""
			echo "   Creating AISA Target of 5 GB. Wait for few seconds"
			${AVM_DIR}/bin/AISA_Avm.py --create AISA --size 5 > /dev/null 2>&1
			if [ $? -eq 0 ]; then
				# Sleep for 3 secs and switch the target iopath from default to avm_map
				echo "   Waiting for 5 Seconds."
				sleep 5
				${AVM_DIR}/bin/AISA_Avm.py --iopath > /dev/null 2>&1
				echo "   AISA Target Created successfully."
				echo ""
				echo "   Congratulations !!!"
			else
				echo "   AISA Target Creation Failed."
				echo "   Check /var/log/messages or 'cat /dev/dzlog'"
				echo ""
				echo "   Congratulations With Warning !!!"
				echo "   AISA Target was not created."
				echo "   Check /var/log/messages or 'cat /dev/dzlog' for root cause."
			fi
		else
			echo "   [1] Data Disk or MetaDisk or both is/are NOT accessible/present"
			echo "   OR"
			echo "   [2] Kernel Modules are not loaded correctly or missing"

			echo "   Kindly set the proper path in config file"
		fi

									dmesg -H    > ${AVM_DIR}/logs/dmesg.log 2>&1
		${AVM_DIR}/bin/AISA_Avm.py --info  all  > ${AVM_DIR}/logs/devices_info.log 2>&1
		${AVM_DIR}/bin/AISA_Avm.py --stats -s   > ${AVM_DIR}/logs/superblock.log 2>&1
		${AVM_DIR}/bin/AISA_Avm.py --stats all  > ${AVM_DIR}/logs/engines_stats.log 2>&1
		${AVM_DIR}/bin/AISA_Avm.py --list  all  > ${AVM_DIR}/logs/volumes_list.log 2>&1
		${AVM_DIR}/bin/AISA_Avm.py --status     > ${AVM_DIR}/logs/sys_entries.log 2>&1
		${AVM_DIR}/bin/AISA_Avm.py --km         > ${AVM_DIR}/logs/kernel_modules.log 2>&1
		${AVM_DIR}/bin/AISA_Avm.py --size all   > ${AVM_DIR}/logs/disks_size.log 2>&1

		echo ""
		echo "   Please refer to README.cli in ${AVM_DIR}/docs folder for cli commands"
		echo ""
		echo "   AVM Config file Dir         : ${CONFIG_DIR}"
		echo "   AVM Config file Name        : ${CONFIG_DIR}/avm.conf"
		echo "   AVM Target Info file Name   : ${AVM_DIR}/logs/devices_info.log"
		echo "   AVM Initial Stats file Name : ${AVM_DIR}/logs/engines_stats.log"
		echo "   AVM Superblock Header       : ${AVM_DIR}/logs/superblock.log"
		echo "   AVM Storage Stack Logs      : ${AVM_DIR}/logs/avm.log"
		echo "   AVM Cli interface Name      : avm"
		echo ""
		echo "   ################################################################################################"
		echo ""
		echo ""
	else
		echo ""
		echo "   ################################################################################################"
		echo ""
		echo "   Congratulations With Warning !!!"
		echo "   Package is installed successfully in %{CLEAN_DIR},"
		echo "   but some kernel modules loading resulted in error"
		echo "   Please uninstall it, reboot and then install the package again."
		echo "   Please refer to README.cli in %{RPM_DIR}/docs folder for cli commands"
		echo ""
		echo ""
		echo "   Config file Dir  : ${CONFIG_DIR}"
		echo "   Config file Name : ${CONFIG_DIR}avm.conf"
		echo "   Main cli command : avm"
		echo ""
		echo "   ################################################################################################"
		echo ""
		echo ""

	fi

	################################################
	#Log everything in avm.log file
	################################################
	${AVM_DIR}/scripts/avmlog.sh f ${AVM_DIR}/logs/avm.log > /dev/null 2>&1

	#Creating alias
	alias cdavm="cd ${AVM_DIR}"
	alias cdlog="cd ${AVM_DIR}/logs"
	alias cdlogs="cd ${AVM_DIR}/logs"
	alias cdl="cd ${AVM_DIR}/logs"

	exit 0

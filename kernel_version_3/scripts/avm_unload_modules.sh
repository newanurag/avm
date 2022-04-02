AVM_DIR="/opt/DATUMSOFT/zaidstor_databank/avm"
CONFIG_DIR="/etc/datumsoft"
MODULE_DIR="${AVM_DIR}/kernel"


	echo ""
	echo "   ###################################################################"
	echo ""

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

	echo "   AVM  Kernel Modules Unloaded "
	echo ""
	echo "   ###################################################################"
	echo ""
	echo ""

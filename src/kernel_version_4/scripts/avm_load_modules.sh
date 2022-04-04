AVM_DIR="/opt/DATUMSOFT/zaidstor_databank/avm"
CONFIG_DIR="/etc/datumsoft"
MODULE_DIR="${AVM_DIR}/kernel"
MODULE_DIR="/home/averma/avm/src/kernel_version_4/kernel_objects"

LEM="avm_1_log_engine_module.ko"
IEM="avm_2_infra_engine_module.ko"
REM="avm_3_lab_engine_module.ko"
BEM="avm_4_bec_engine_module.ko"
MEM="avm_5_metadata_engine_module.ko"
DEM="avm_6_dedupe_engine_module.ko"
AEM="avm_7_alignment_engine_module.ko"
FEM="avm_8_fec_engine_module.ko"
TEM="avm_9_target_engine_module.ko"
VEM="avm_A_volume_engine_module.ko"
SEM="avm_B_sysfs_engine_module.ko"


	################################################
	#Load the Kernel Modules. Sequence is important
	################################################
	echo "   ################################################################################################"
	echo ""
	echo "   Loading Kernel Modules"
	echo ""
	ierror=0
	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_1_B_log_engine_module.ko
	insmod ${MODULE_DIR}/${LEM}
	if [ $? -eq 0 ]; then
		echo "   Module $LEM       Loaded"
	else
		ierror=1
		echo "   Module $LEM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi



	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_2_B_infra_engine_module.ko
	insmod ${MODULE_DIR}/${IEM}
	if [ $? -eq 0 ]; then
		echo "   Module $IEM     Loaded"
	else
		ierror=1
		echo "   Module $IEM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi

	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_3_B_lab_engine_module.ko
	insmod ${MODULE_DIR}/${REM}
	if [ $? -eq 0 ]; then
		echo "   Module $REM       Loaded"
	else
		ierror=1
		echo "   Module $REM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi

	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_4_B_bec_engine_module.ko
	insmod ${MODULE_DIR}/${BEM}
	if [ $? -eq 0 ]; then
		echo "   Module $BEM       Loaded"
	else
		ierror=1
		echo "   Module $BEM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi

	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_5_B_metadata_engine_module.ko
	insmod ${MODULE_DIR}/${MEM}
	if [ $? -eq 0 ]; then
		echo "   Module $MEM  Loaded"
	else
		ierror=1
		echo "   Module $MEM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi

	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_6_B_dedupe_engine_module.ko
	insmod ${MODULE_DIR}/${DEM}
	if [ $? -eq 0 ]; then
		echo "   Module $DEM    Loaded"
	else
		ierror=1
		echo "   Module $DEM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi

	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_7_B_alignment_engine_module.ko
	insmod ${MODULE_DIR}/${AEM}
	if [ $? -eq 0 ]; then
		echo "   Module $AEM Loaded"
	else
		ierror=1
		echo "   Module $AEM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi

	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_8_B_fec_engine_module.ko
	insmod ${MODULE_DIR}/${FEM}
	if [ $? -eq 0 ]; then
		echo "   Module $FEM       Loaded"
	else
		ierror=1
		echo "   Module $FEM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi

	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_9_B_target_engine_module.ko
	insmod ${MODULE_DIR}/${TEM}
	if [ $? -eq 0 ]; then
		echo "   Module $TEM    Loaded"
	else
		ierror=1
		echo "   Module $TEM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi

	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_A_B_volume_engine_module.ko
	insmod ${MODULE_DIR}/${VEM}
	if [ $? -eq 0 ]; then
		echo "   Module $VEM    Loaded"
	else
		ierror=1
		echo "   Module $VEM NOT Loaded"
		echo "   Loading of other AVM Kernel modules will not be initiated"
		exit
	fi

	#insmod ${AVM_DIR}/kernel/datumsoft_zaidstor_avm_B_B_sysfs_engine_module.ko
	insmod ${MODULE_DIR}/${SEM}
	if [ $? -eq 0 ]; then
		echo "   Module $SEM     Loaded"
	else
		ierror=1
		echo "   Module $SEM NOT Loaded"
		exit
	fi
	echo "   ################################################################################################"

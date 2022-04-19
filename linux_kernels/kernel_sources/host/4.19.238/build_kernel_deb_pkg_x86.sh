#Build script for building linux kernel for X86 architecture 

export AVM_OBJECT_DIR=OUT_X86
export AVM_BUILD_KERNEL_LOG_FILE=avm_build_kernel_x86.log
export AVM_NPROC=2

#Kernel build/make parameters
export KBUILD_OUTPUT=${AVM_OBJECT_DIR}
export KBUILD_VERBOSE=0

#This variable we have passed externally to deal with unwanted warning/errors
export AVM_EXTRA_CFLAGS="-Wno-unused-const-variable -Wno-strict-prototypes"
#export AVM_MAKE_BUILD_ARGS=" all EXTRA_CFLAGS=${AVM_EXTRA_CFLAGS}"
export AVM_MAKE_BUILD_ARGS=" bindeb-pkg EXTRA_CFLAGS=${AVM_EXTRA_CFLAGS}"
export AVM_MAKE_CONFIG_ARGS=" olddefconfig"

header()
{

	mkdir -p ${AVM_OBJECT_DIR}
	mv ${AVM_BUILD_KERNEL_LOG_FILE} ${AVM_BUILD_KERNEL_LOG_FILE}.old > /dev/null 2>&1 
	echo "################################################################################" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "Build Start Time : `date` " >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "Current Directory: `pwd`" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "################################################################################" >> ${AVM_BUILD_KERNEL_LOG_FILE}
}

footer()
{
	echo "################################################################################" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "Build End   Time : `date` " >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "Log file for build logs: "${AVM_BUILD_KERNEL_LOG_FILE}
	echo "################################################################################" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo ""

}

function make_config()
{
	cp /boot/config-`uname -r` .config
	
	make_cfg_cmd="make -j${AVM_NPROC} ${AVM_MAKE_CONFIG_ARGS}"
	echo "################# KERNEL CONFIG STARTED ####################################### " >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" |tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "MAKE_CONFIG_COMMAND      : ${make_cfg_cmd}" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" |tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	time ${make_cfg_cmd} | tee -a ${AVM_BUILD_KERNEL_LOG_FILE} 2>&1
	rccfg=$?
	echo "MAKE_CONFIG_COMMAND RC   : $rccfg" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}


	echo "################# KERNEL CONFIG COMPLETED ##################################### " >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" |tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	rm .config

	if [ "$rccfg" -eq "0" ]; then
		echo "-------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "RESULT: KERNEL MAKE CONFIG SUCCESSFUL " | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "-------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	else
		echo "----------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "RESULT: KERNEL CONFIG FAILURE with RC $rc" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "----------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		exit $rccfg
	fi

	return $rccfg

}

function make_build()
{
	make_bld_cmd="make -j${AVM_NPROC} $AVM_MAKE_BUILD_ARGS"

	echo "############ CORE KERNEL BUILD/MAKE STARTED #################################### " |tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "MAKE_BUILD_COMMAND      : ${make_bld_cmd}" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	time ${make_bld_cmd} | tee -a ${AVM_BUILD_KERNEL_LOG_FILE} 2>&1
	rcbld=$?
	echo "MAKE_BUILD_COMMAND RC   : $rcbld" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "############ CORE KERNEL BUILD/MAKE COMPLETED ##################################" |tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	if [ "$rcbld" -eq "0" ]; then
		echo "------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "RESULT: KERNEL MAKE/BUILD SUCCESSFUL " | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	else
		echo "--------------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "RESULT: KERNEL MAKE/BUILD FAILURE with RC $rcbld" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "--------------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		exit $rcbld
	fi
	return $rcbld

}
function make_build2()
{
	make_bld_cmd="make -j${AVM_NPROC} modules_install"

	echo "############ KERNEL MODULES STARTED #################################### " |tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "MAKE_BUILD_COMMAND      : ${make_bld_cmd}" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	time ${make_bld_cmd} | tee -a ${AVM_BUILD_KERNEL_LOG_FILE} 2>&1
	rcbld=$?
	echo "MAKE_BUILD_COMMAND RC   : $rcbld" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "############ KERNEL MODULES COMPLETED ##################################" |tee -a ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	if [ "$rcbld" -eq "0" ]; then
		echo "------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "RESULT: KERNEL MAKE/BUILD SUCCESSFUL " | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
	else
		echo "--------------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "RESULT: KERNEL MAKE/BUILD FAILURE with RC $rcbld" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "--------------------------------------------"  | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		exit $rcbld
	fi
	return $rcbld

}

main()
{
	header
	make_config
	make_build
	footer
	#make_build2
}

main


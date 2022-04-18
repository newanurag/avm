#Build script for building linux kernel for X86 architecture 

export AVM_OBJECT_DIR=OUTOBJ_x86
export AVM_BUILD_KERNEL_LOG_FILE=avm_build_kernel_x86.log
#Kernel build/make parameters
export KBUILD_OUTPUT=${AVM_OBJECT_DIR}

if [ $1 -ge 1 ]; then
	export KBUILD_VERBOSE=1
	export AVM_NPROC=$1
else
	export KBUILD_VERBOSE=0
	export AVM_NPROC=2
fi

#This variable we have passed externally to deal with unwanted warning/errors
export AVM_EXTRA_CFLAGS="-Wno-unused-const-variable -Wno-strict-prototypes"

export AVM_MAKE_BUILD_ARGS=" all EXTRA_CFLAGS=${AVM_EXTRA_CFLAGS}"

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

main_make()
{
	make_bld_cmd="make -j${AVM_NPROC} $AVM_MAKE_BUILD_ARGS"

	echo "############ CORE KERNEL BUILD/MAKE STARTED #################################### " >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "MAKE_BUILD_COMMAND      : ${make_bld_cmd}" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "MAKE_BUILD_COMMAND      : ${make_bld_cmd}"
	echo ""

	time ${make_bld_cmd} | tee -a ${AVM_BUILD_KERNEL_LOG_FILE} 2>&1
	rcbld=$?
	echo "MAKE_BUILD_COMMAND RC   : $rcbld" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "MAKE_BUILD_COMMAND RC   : $rcbld"
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo ""

	echo "############ CORE KERNEL BUILD/MAKE COMPLETED ##################################" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo ""

	if [ $rcbld -eq 0 ]; then
		echo "------------------------------------"
		echo "RESULT: KERNEL MAKE/BUILD SUCCESSFUL " | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "------------------------------------"
	else
		echo "--------------------------------------------"
		echo "RESULT: KERNEL MAKE/BUILD FAILURE with RC $rcbld" | tee -a ${AVM_BUILD_KERNEL_LOG_FILE}
		echo "--------------------------------------------"
	fi
	echo ""
}

header
main_make
footer


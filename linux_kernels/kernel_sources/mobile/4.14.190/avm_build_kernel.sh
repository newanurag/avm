#Build script for building linux kernel for ARM64 architecture 
#with google android toolchain and CLANG compiler
#Note: This script is not for x86 platform

export AVM_OBJECT_DIR=OUTOBJ

if [ $1 -ge 1 ]; then
	export KBUILD_VERBOSE=1
	export AVM_NPROC=$1
else
	export KBUILD_VERBOSE=0
	export AVM_NPROC=1
fi


#Config file for Samsung mobile A70 model
export AVM_CONFIG_FILE=a70q_sea_open_defconfig

export AVM_CROSS_COMPILE=/home/averma/kernel/toolchain/bin/aarch64-linux-android-
export AVM_CLANG_TRIPLE=aarch64-linux-gnu-
export AVM_REAL_CC=/home/averma/kernel/samsung/clang_llvm-10.0.0/bin/clang
export AVM_KERNEL_MAKE_ENV="DTC_EXT=`pwd`/tools/dtc CONFIG_BUILD_ARM64_DT_OVERLAY=y"
export AVM_BUILD_KERNEL_LOG_FILE=avm_build_kernel.log
export AVM_ARCH=arm64

#This variable we have passed externally to deal with unwanted warning/errors
export AVM_EXTRA_CFLAGS="-Wno-unused-const-variable -Wno-strict-prototypes"


#Kernel build/make parameters
export KBUILD_OUTPUT=${AVM_OBJECT_DIR}


export AVM_MAKE_CONFIG_ARGS="$KERNEL_MAKE_ENV ARCH=$AVM_ARCH REAL_CC=$AVM_REAL_CC CROSS_COMPILE=$AVM_CROSS_COMPILE LLVM=1 CLANG_TRIPLE=$AVM_CLANG_TRIPLE"

export AVM_MAKE_BUILD_ARGS="$KERNEL_MAKE_ENV ARCH=$AVM_ARCH REAL_CC=$AVM_REAL_CC CROSS_COMPILE=$AVM_CROSS_COMPILE LLVM=1 CLANG_TRIPLE=$AVM_CLANG_TRIPLE EXTRA_CFLAGS=${AVM_EXTRA_CFLAGS}"

header()
{

	mkdir -p ${AVM_OBJECT_DIR}
	mv ${AVM_BUILD_KERNEL_LOG_FILE} ${AVM_BUILD_KERNEL_LOG_FILE}.old > /dev/null 2>&1 
	echo "################################################################################" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "Build Start Time : `date` " >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "Current Directory: `pwd`" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "############ MAKE CONFIG ARGUMENTS/PARAMETERS ##################################" >> ${AVM_BUILD_KERNEL_LOG_FILE}


	echo "AVM_CROSS_COMPILE=$AVM_CROSS_COMPILE" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "AVM_CLANG_TRIPLE=$AVM_CLANG_TRIPLE" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "AVM_REAL_CC=$AVM_REAL_CC" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "AVM_ARCH=$AVM_ARCH" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "AVM_KERNEL_MAKE_ENV=$AVM_KERNEL_MAKE_ENV" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "AVM_MAKE_CONFIG_ARGS=$AVM_MAKE_CONFIG_ARGS" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "AVM_MAKE_BUILD_ARGS=$AVM_MAKE_BUILD_ARGS" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "################################################################################" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}
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
	make_cfg_cmd="make -j${AVM_NPROC} -C `pwd` $AVM_MAKE_CONFIG_ARGS $AVM_CONFIG_FILE "
	make_bld_cmd="make -j${AVM_NPROC} -C `pwd` $AVM_MAKE_BUILD_ARGS"

	echo "############ CORE KERNEL MAKE CONFIG STARTED ###################################" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}


	echo "MAKE_CONFIG_COMMAND     : ${make_cfg_cmd}" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

	echo "MAKE_CONFIG_COMMAND     : ${make_cfg_cmd}"
	echo ""

	time ${make_cfg_cmd} | tee -a ${AVM_BUILD_KERNEL_LOG_FILE} 2>&1
	rccfg=$?
	echo "MAKE_CONFIG_COMMAND RC  : $rccfg" >> ${AVM_BUILD_KERNEL_LOG_FILE}
	echo ""

	echo "" >> ${AVM_BUILD_KERNEL_LOG_FILE}

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


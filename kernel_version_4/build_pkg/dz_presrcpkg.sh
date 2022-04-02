#!/bin/bash
# All the paths in this file are relative to DZ_PROJECT_SOURCE_PATH
# All package paths below are relative to DZ_PACKAGE_DIR
DZ_BASE_ENGINE="bin"
DZ_BEC_ENGINE="bin"
DZ_CLI_ENGINE="bin"
DZ_DEDUPE_ENGINE="bin"
DZ_FEC_ENGINE="bin"
DZ_HASH_ENGINE="bin"
DZ_HEADERS="bin"
DZ_INFRA_ENGINE="bin"
DZ_MD_ENGINE="bin"
DZ_MISC="bin"
DZ_PACKAGING="bin"
DZ_SYSFS_ENGINE="bin"
DZ_VOLUME_ENGINE="bin"

#===============================================================================
# Main
#===============================================================================
# Pre-requisites
echo "Creating directores inside RPM package"
mkdir -p ${DZ_PACKAGE_DIR}/${DZ_BIN_DIR_NAME} || exit 1
mkdir -p ${DZ_PACKAGE_DIR}/${DZ_DOCS_DIR_NAME} || exit 1
mkdir -p ${DZ_PACKAGE_DIR}/${DZ_KERNEL_DIR_NAME} || exit 1
mkdir -p ${DZ_PACKAGE_DIR}/${DZ_ETC_DIR_NAME} || exit 1
mkdir -p ${DZ_PACKAGE_DIR}/${DZ_LICENSE_DIR_NAME} || exit 1

# Copy all the files to be shipped

#Copy Drivers/Kernel Modules files
cp -r ${DZ_PROJECT_SOURCE_PATH}/*.ko ${DZ_PACKAGE_DIR}/${DZ_KERNEL_DIR_NAME}/

#Copy License file
cp -r ${DZ_PROJECT_SOURCE_PATH}/License.txt ${DZ_PACKAGE_DIR}/${DZ_LICENSE_DIR_NAME}/

#Copy config file
cp -r ${DZ_PROJECT_SOURCE_PATH}/*.conf ${DZ_PACKAGE_DIR}/${DZ_ETC_DIR_NAME}/

#Copy documentation file
cp -r ${DZ_PROJECT_SOURCE_PATH}/*.cli ${DZ_PACKAGE_DIR}/${DZ_DOCS_DIR_NAME}/

#Copy CLI Files
cp -r ${DZ_PROJECT_SOURCE_PATH}/cli_engine/AISA_Error.py ${DZ_PACKAGE_DIR}/${DZ_BIN_DIR_NAME}/
cp -r ${DZ_PROJECT_SOURCE_PATH}/cli_engine/AISA_Defaults.py ${DZ_PACKAGE_DIR}/${DZ_BIN_DIR_NAME}/
cp -r ${DZ_PROJECT_SOURCE_PATH}/cli_engine/AISA_Utils.py ${DZ_PACKAGE_DIR}/${DZ_BIN_DIR_NAME}/
cp -r ${DZ_PROJECT_SOURCE_PATH}/cli_engine/AISA_AVMCore.py ${DZ_PACKAGE_DIR}/${DZ_BIN_DIR_NAME}/
cp -r ${DZ_PROJECT_SOURCE_PATH}/cli_engine/AISA_AVMGlobals.py ${DZ_PACKAGE_DIR}/${DZ_BIN_DIR_NAME}/
cp -r ${DZ_PROJECT_SOURCE_PATH}/cli_engine/AISA_AVMHelp.py ${DZ_PACKAGE_DIR}/${DZ_BIN_DIR_NAME}/
cp -r ${DZ_PROJECT_SOURCE_PATH}/cli_engine/AISA_Avm.py ${DZ_PACKAGE_DIR}/${DZ_BIN_DIR_NAME}/


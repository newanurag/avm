#!/bin/bash

export DZ_BUILD_PKG_DIR=${DZ_PROJECT_PATH}/build_pkg
export DZ_PKG_BUILD_SCRIPT=${DZ_BUILD_PKG_DIR}/dz_prepkg.sh
export DZ_PROJECT_SOURCE_PATH=${DZ_PROJECT_PATH}

#VERSION_FILE=${DZ_PROJECT_PATH}/VERSION
VERSION_FILE=${DZ_BUILD_PKG_DIR}/VERSION
VERSION_STRING=$( head -n 2 $VERSION_FILE | tail -1)
NAME=$(head -n 1 $VERSION_FILE)
#VERSION=$(cut -c -3 $VERSION_FILE | tail -1)
#RELEASE=$( head -n 2 $VERSION_FILE | tail -c -2)

#Version String  is 1.1.a-3.10.101 in VERSION file
VERSION=$(cut -c -10 $VERSION_FILE | tail -1)
#This will come to 1.1.a-3.10

RELEASE=$( head -n 2 $VERSION_FILE | tail -c +16)
#This will come to 101

RPM_TOPDIR=${DZ_BUILD_PKG_DIR}/RPM
SRCDIR=$RPM_TOPDIR/SOURCES
SPECDIR=$RPM_TOPDIR/SPECS
RPMDIR=$RPM_TOPDIR/RPMS/x86_64
SPEC_FILE=${DZ_BUILD_PKG_DIR}/dz_rpm_basic.spec
# DZ_BUILD_PKG_SCRIPT stores it's output in this file
# This is needed because in --quiet mode all the CLI output is suppressed.
# The output displays stats on how many number of test cases were copied for each module.
STAT_FILE=${RPM_TOPDIR}/BUILDROOT/stat.txt

# Create RPM Build directories
mkdir -p $RPM_TOPDIR
mkdir -p $RPM_TOPDIR/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}

# Create the RPM package
cp $SPEC_FILE $SPECDIR

#ZAIDSTOR_RPM_NAME=%define _rpmfilename %%{NAME}.%%{OS}.%%{ARCH}.rpm

echo "Invoking rpmbuild ..."
#rpmbuild -ba --define "_topdir $RPM_TOPDIR" \
#rpmbuild --quiet -ba --define "_topdir $RPM_TOPDIR" \
rpmbuild  -ba --define "_topdir $RPM_TOPDIR" \
             --define "_BUILD_DIR $DZ_PROJECT_SOURCE_PATH" \
             --define "_VERSION $VERSION" \
             --define "_RELEASE $RELEASE" \
	     --define "_NAME ${DZ_PACKAGE_NAME}" \
	     --define "_STAT_FILE ${STAT_FILE}" $SPEC_FILE

echo "RPMDIR          : ${RPMDIR}"
echo "Version String  : $VERSION_STRING (CLI and Kernel Version seperated by underscore(_))"
echo "Version         : $VERSION"
echo "Release         : $RELEASE"
echo "DZ_PACKAGE_DIR  : $DZ_PACKAGE_DIR"
echo "DZ_PACKAGE_NAME : $DZ_PACKAGE_NAME"

sleep 1

# Move the rpm(s) to destination directory
#echo "Moving RPM to destination directory"
#echo "Generaging Package: "${DZ_PACKAGE_NAME}
echo "Finding rpm directory"
find ${RPMDIR} -name "${DZ_PACKAGE_NAME}*.rpm" -exec mv {} $DZ_PACKAGE_DIR \;


# Display RPM package path to user
cat ${STAT_FILE}
echo "RPM Package built successfully"
echo "RPM Package Name : "`basename $DZ_PACKAGE_DIR/*.rpm`
echo "RPM Package Path : "`dirname  $DZ_PACKAGE_DIR/*.rpm`
echo "Soft Link is also created in the current directory"

# Cleanup the RPM build directory
rm -rf $RPM_TOPDIR

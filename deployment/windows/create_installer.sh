#!/bin/bash
# This script creates a windows installer using the nsis script "installer.nsi".

########## Script Requirements #################################################
#
# 1. You must install Cygwin, Qt SDK, NSIS, and Git.
# Cygwin - http://cygwin.com/setup.exe
# Qt SDK - http://qt.nokia.com/downloads/
# NSIS   - http://nsis.sourceforge.net/Download
# Git    - http://code.google.com/p/msysgit/downloads/list
#
# 2. Cygwin's bin, NSIS, Git, and both Qt's bin and Qt's Mingw bin
# directories must be in the windows PATH environment variable.
# example - C:\cygwin\bin;C:\Qt\2010.01\qt\bin;C:\Qt\2010.01\mingw\bin;
# C:\Program Files\NSIS;C:\Program Files\Git\bin
#
# 3. This script should be run from within a Cygwin Bash Shell.
#
# 4. This script must be run as administrator when run within Windows 7.
#
################################################################################

# Move to directory holding this script
#  In case the script is run from a different directory: windows/create_installer.sh
SCRIPT_DIR=$(echo ${BASH_SOURCE[0]} | sed 's/\(.*\)\([\\\/][^\\\/]*$\)/\1/g')
cd "${SCRIPT_DIR}"

# Move to the project root.
cd ../..

# Capture the project name from the root directory's name.
export PROJECT_NAME=$(echo $PWD | sed 's/\/\([^\/]\+\/\)\+\(.*\)/\2/g')
# Retrieve the application name by from the project name (ex: "file-monitor" -> "File Monitor").
export APP_NAME=$(echo $PROJECT_NAME | sed 's/\([^-]*\)-\([^-]*\)/\u\1 \u\2/g')


#### Project specific defines (for nsis script). ###################################
export APP_COMPANY_NAME="XmlSoft"                                                  #
export APP_COPYRIGHT_HOLDER="Mark Turney"                                          #
export APP_LINK="http://code.google.com/p/${PROJECT_NAME}"                         #
export APP_DOWNLOAD_LINK="http://code.google.com/p/${PROJECT_NAME}/downloads/list" #
####################################################################################


# Capture the version number of this project from the last git tag.
GIT_TAGS=`git tag`
export APP_VERSION=$(echo $GIT_TAGS | sed 's/\(v[0-9]\+\.[0-9]\+\.[0-9]\+ \)*v\([0-9]\+\.[0-9]\+\.[0-9]\+\)/\2/g')

# Determine the CPU architecture.
if [$PROCESSOR_ARCHITEW6432 == ""]
then APP_ARCHITECTURE=$(echo $PROCESSOR_ARCHITECTURE | tr "[:upper:]" "[:lower:]")
else APP_ARCHITECTURE=$(echo $PROCESSOR_ARCHITEW6432 | tr "[:upper:]" "[:lower:]")
fi

# Build the installer name.
export APP_INSTALLER_NAME=${PROJECT_NAME}-${APP_VERSION}-${APP_ARCHITECTURE}-installer.exe

BUILD_DIR=_build/.compiled
INSTALL_SOURCE_DIR=$BUILD_DIR/source

# Make sure we are totally clean before we begin.
function deleteBuildFiles {
	rm Makefile
	rm Makefile.Debug
	rm Makefile.Release
	rm object_script.file-monitor.Release
	rm object_script.file-monitor.Debug
	rm -rf _build
}
deleteBuildFiles

# Copy this entire project into the build directory as source
mkdir -p $INSTALL_SOURCE_DIR
cp -r ../$PROJECT_NAME $INSTALL_SOURCE_DIR

# Create the Makefile and build the project
qmake
mingw32-make release

# Find the Qt and MinGW bin directories.
MINGW_BIN_DIR=$(echo `which libstdc++-6.dll` | sed 's/\(.*\)\([\\\/][^\\\/]*$\)/\1/g')
QT_BIN_DIR=$(echo `which QtCore4.dll` | sed 's/\(.*\)\([\\\/][^\\\/]*$\)/\1/g')

# Copy the required dlls into the build directory.
cp $MINGW_BIN_DIR/libstdc++-6.dll $BUILD_DIR
cp $QT_BIN_DIR/libgcc_s_dw2-1.dll $BUILD_DIR
cp $QT_BIN_DIR/mingwm10.dll $BUILD_DIR
cp $QT_BIN_DIR/QtCore4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtGui4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtSvg4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtWebKit4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtNetwork4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtXml4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtXmlPatterns4.dll $BUILD_DIR
cp $QT_BIN_DIR/phonon4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtWebKit4.dll $BUILD_DIR

# Copy the readme, license, icon, and installer script into the build directory.
cp deployment/windows/installer.nsi $BUILD_DIR
cp deployment/windows/file-monitor.ico $BUILD_DIR
cp documentation/readme $BUILD_DIR/readme.txt
cp documentation/license $BUILD_DIR/license.txt

# Move to the build directory and compile the nsis script.
cd $BUILD_DIR
makensis installer.nsi

# Move the installer to project parent directory.
cd ../../
mv ${BUILD_DIR}/${APP_INSTALLER_NAME} ../

# Clean up all the build files.
deleteBuildFiles


#!/bin/bash
# This script creates a windows installer using the nsis script "installer.nsi".

# Cygwin's bin, and NSIS must be in the PATH environment variable.
# example: C:\cygwin\bin;C:\Qt\2010.01\qt\bin;C:\Qt\2010.01\mingw\bin;C:\Program Files\NSIS

#TODO: if qmake can not be found, use the below information, otherwise use the "which"
#            command to locate it and populate the below variables.

QT_DRIVE=/cygdrive/c
QT_BASE_DIR=$QT_DRIVE/Qt/2010.02.1
QT_BIN_DIR=$QT_BASE_DIR/qt/bin
QT_MINGW_BIN_DIR=$QT_BASE_DIR/mingw/bin

# Add Qt's bin and Qt's mingw bin to the path environment variable.
export PATH=$QT_BIN_DIR:$QT_MINGW_BIN_DIR:$PATH
# Make sure that the QMAKESPEC is set to use mingw.
export QMAKESPEC=Win32-g++

# Move to directory holding this script.
SCRIPT_DIR=${0%/*}
cd $SCRIPT_DIR

# Move to the project root.
cd ../..

# Isolate the project name.
x=$PWD
PROJECT_NAME=${x##*/}

BUILD_DIR=_build/.compiled
INSTALL_SOURCE_DIR=$BUILD_DIR/source

# Make sure we are totally clean before we begin.
rm -rf _build
rm Makefile
rm Makefile.Debug
rm Makefile.Release

# Copy this entire project into the build directory as source
mkdir -p $INSTALL_SOURCE_DIR
cp -r ../$PROJECT_NAME $INSTALL_SOURCE_DIR
# A build directory will be copied into the install source dir, delete it.
rm -rf $INSTALL_SOURCE_DIR/$PROJECT_NAME/_build

# Create the Makefile and build the project
qmake
mingw32-make release

# Copy the required dlls into the build directory.
cp $QT_BIN_DIR/libgcc_s_dw2-1.dll $BUILD_DIR
cp $QT_BIN_DIR/mingwm10.dll $BUILD_DIR
cp $QT_BIN_DIR/QtCore4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtGui4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtSvg4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtWebKit4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtNetwork4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtXmlPatterns4.dll $BUILD_DIR
cp $QT_BIN_DIR/phonon4.dll $BUILD_DIR
cp $QT_BIN_DIR/QtWebKit4.dll $BUILD_DIR

# Copy the readme, license, and installer script into the build directory.
cp deployment/windows/installer.nsi $BUILD_DIR
cp documentation/readme $BUILD_DIR/readme.txt
cp documentation/license $BUILD_DIR/license.txt

# Move to the build directory and compile the nsis script.
cd $BUILD_DIR
makensis installer.nsi

# Move the installer to project parent directory.
cd ../../
mv $BUILD_DIR/$PROJECT_NAME-installer.exe ../

# Clean up all the build files.
rm Makefile
rm Makefile.Debug
rm Makefile.Release


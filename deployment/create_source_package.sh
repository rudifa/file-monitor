#!/bin/bash
# This script creates the source package for this applicaiton.

# Move to directory holding this script
#  In case the script is run from a different directory: windows/create_installer.sh
SCRIPT_DIR=$(echo ${BASH_SOURCE[0]} | sed 's/\(.*\)\([\\\/][^\\\/]*$\)/\1/g')

cd "${SCRIPT_DIR}"

# Move to the project root.
cd ../

# Capture the version number of this project from the last git tag.
GIT_TAGS=`git tag`
LAST_VERSION=$(echo $GIT_TAGS | sed 's/\(v[0-9]\+\.[0-9]\+\.[0-9]\+ \)*v\([0-9]\+\.[0-9]\+\.[0-9]\+\)/\2/g')

# Capture the project name from the root directory's name.
PROJECT_NAME=$(echo $PWD | sed 's/\/\([^\/]\+\/\)\+\(.*\)/\2/g')

# Make sure we are totally clean before we begin.
rm Makefile
rm Makefile.Debug
rm Makefile.Release
rm object_script.file-monitor.Release
rm object_script.file-monitor.Debug
rm -rf _build

# Make sure all bash scripts have execute permissions.
find . -type f -name \*.sh -exec chmod +x {} \;

cd ../

# Create the source package while preserving permissions.
tar -pcvf ${PROJECT_NAME}-${LAST_VERSION}.tar ${PROJECT_NAME}/
gzip ${PROJECT_NAME}-${LAST_VERSION}.tar


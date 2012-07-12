#!/bin/bash

# This script creates a valid Debian Package.  If the user passes in the argument
#  -verify, this script will thoroughly verify the resulting package but
#  will not build the package itself.

# Packages that are required to run this script:
sudo apt-get install build-essential
sudo apt-get install qt4-dev-tools
sudo apt-get install libqt4-dev
sudo apt-get install dh-make
sudo apt-get install gnupg
sudo apt-get install pbuilder
sudo apt-get install debootstrap
sudo apt-get install devscripts
sudo apt-get install git-core

# Make sure to create a key to replace the existing $MAINTAINER_GPG_KEY and
#  make sure that this key is set to "default-key" in the .gnupg/options file.
MAINTAINER_EMAIL=markturney@gmail.com
MAINTAINER_GPG_KEY=140597E3
# GPG key creation details - https://help.ubuntu.com/community/GnuPrivacyGuardHowto

# Move to directory holding this script.
SCRIPT_DIR=$(echo ${BASH_SOURCE[0]} | sed 's/\(.*\)\([\\\/][^\\\/]*$\)/\1/g')
cd "${SCRIPT_DIR}"

# Move to the project root.
cd ../../../

# Capture the version number of this project from the last git tag.
GIT_TAGS=`git tag`
LAST_VERSION=$(echo $GIT_TAGS | sed 's/\(v[0-9]\+\.[0-9]\+\.[0-9]\+ \)*v\([0-9]\+\.[0-9]\+\.[0-9]\+\)/-\2/g')

# Capture the project name from the root directory's name.
PROJECT_NAME=$(echo $PWD | sed 's/\/\([^\/]\+\/\)\+\(.*\)/\2/g')

# Rename the root directory to include the version number (the debian packaging utilities require this).
PROJECT_NAME_WITH_VERSION=${PROJECT_NAME}${LAST_VERSION}
cd ../
mv $PROJECT_NAME $PROJECT_NAME_WITH_VERSION
cd $PROJECT_NAME_WITH_VERSION

# Ensure file permissions are correct.
find -type f -print0 | xargs -0 chmod 644
chmod +x deployment/unix/debian/create_deb.sh
chmod +x deployment/unix/debian/rules

# Create the Makefile.
qmake
# Remove absolute paths that QMake creates in it's generated Makefile.
sed -i 's|'"$PWD"'/||g' Makefile

# Debianize the project.
# Downgraded to gpl to build on Ubuntu 9.04.  Upgrade to gpl3.
dh_make --email $MAINTAINER_EMAIL --copyright gpl --single --createorig

# Remove all the generated example files.
rm debian/*.ex
rm debian/*.EX
rm debian/README.Debian

# Copy our four modified control files into the debian build directory.
cp deployment/unix/debian/changelog debian
cp deployment/unix/debian/control debian
cp deployment/unix/debian/copyright debian
cp deployment/unix/debian/rules debian

# If the user passed in "verify" as the command line param.
if [ "$1" = "-verify" ] ; then
	# Run through an entire verify pass.
	debuild -S -rfakeroot -k$MAINTAINER_GPG_KEY
	sudo pbuilder create
	sudo pbuilder build ../*.dsc
else
	# Otherwise, create the .deb file.
	debuild -k$MAINTAINER_GPG_KEY
fi

# Clean by removing all the build files.
rm Makefile
rm -rf _build
rm -rf debian
rm build-stamp
rm configure-stamp

# Rename the project directory by removing the version string.
cd ../
mv $PROJECT_NAME_WITH_VERSION $PROJECT_NAME
cd $PROJECT_NAME


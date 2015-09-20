#!/bin/sh

set -x
set -e


git config --global user.name 'Georg Sieber'
git config --global user.email 'sieber@cern.ch'
git config --global user.github 'claria'

# Mount cvmfs
/etc/cvmfs/run-cvmfs.sh
export SCRAM_ARCH=slc6_amd64_gcc481
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch

printenv

mkdir -p /home/build && cd /home/build

. $VO_CMS_SW_DIR/cmsset_default.sh

# Setup everything
/home/travis/checkout.sh

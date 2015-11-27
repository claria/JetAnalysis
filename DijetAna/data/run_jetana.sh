#!/bin/bash

# source $MY_LANDINGZONE/gc-:run.lib || exit 101
set -x
printenv

echo "---------------------"
echo "Prepare to run JetAna"
echo "---------------------"

if [ -z "$CMSSW_BASE" ]; then
    echo "No CMSSW environment found."
    echo "Try to setup one."
    export SCRAM_ARCH=slc6_amd64_gcc481
    # export VO_CMS_SW_DIR=/afs/cern.ch/cms
    export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
    source $VO_CMS_SW_DIR/cmsset_default.sh

    export CMSSW_DIR=${CMSSW_DIR}
    eval $(cd $CMSSW_DIR && scramv1 runtime -sh)
fi

echo $PATH

echo "-------------------------"
echo "Listing current directory"
echo "-------------------------"

echo $PWD
ls -l
echo

echo "---------------------------------"
echo "Running with the following config"
echo "---------------------------------"
# $FILE_NAMES=echo "${FILE_NAMES//\"}"
dijetana.py -i ${FILE_NAMES//\"} --save-config config.json -c $CONFIG --log-level info 2>&1 | tee out.log

RC=${PIPESTATUS[0]}
if [ ${RC} -eq 0 ]
then
  exit 0
else
  # do stuff
  exit 1
fi

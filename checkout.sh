#!/bin/bash

set -e

# Checkout script for the dijet analysis.
#
# Prepares a CMSSW area, checks out and compiles all neccessary packages.

# Setup a scram env
. $VO_CMS_SW_DIR/cmsset_default.sh

# Setup CMSSW env
scram project CMSSW_7_2_3

cd CMSSW_7_2_3/src

cmsenv

git-cms-addpkg CondFormats/JetMETObjects
git-cms-addpkg RecoLuminosity

# Checkout development branch of Kappa
git clone -b development git@github.com:KappaAnalysis/Kappa.git Kappa
make -C Kappa/DataFormats/test/ -j4

# Checkout master of KappaTools
git clone git@github.com:KappaAnalysis/KappaTools.git KappaTools
make -C KappaTools -j4

# Checkout master of Artus
git clone git@github.com:artus-analysis/Artus.git

# Checkout master of DijetAna
if [ "$TRAVIS" = true ] ; then
  git clone git@github.com:claria/JetAnalysis.git JetAnalysis
else
  git clone --branch ${TRAVIS_BRANCH} git@github.com:claria/JetAnalysis.git JetAnalysis
  cd JetAnalysis && git checkout -qf ${TRAVIS_COMMIT} && cd ..
fi

# JetAnalysis needs RooUnfold for the Unfolding part

git clone git@github.com:skluth/RooUnfold.git RooUnfold
make -C RooUnfold -j4

# Moving the tool files for the parts not built within scram
# These provide the correct lib/include dirs when using cmsenv
cp JetAnalysis/cmssw_tools/kappa.xml ../config/toolbox/${SCRAM_ARCH}/tools/selected/
scram setup kappa
cp JetAnalysis/cmssw_tools/kappatools.xml ../config/toolbox/${SCRAM_ARCH}/tools/selected/
scram setup kappatools
cp JetAnalysis/cmssw_tools/roounfold.xml ../config/toolbox/${SCRAM_ARCH}/tools/selected/
scram setup roounfold

# Again cmsenv to update paths
cmsenv

# Build the stuff
scram b -j4


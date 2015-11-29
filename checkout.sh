#!/bin/bash

# set -e
trap 'echo "# $BASH_COMMAND"' DEBUG
shopt -s expand_aliases

# Checkout script for the dijet analysis.
#
# Prepares a CMSSW area, checks out and compiles all neccessary packages.

# Setup a scram env
. $VO_CMS_SW_DIR/cmsset_default.sh

# Setup CMSSW env
scram project CMSSW_7_2_3
cd CMSSW_7_2_3/src
cmsenv

# Need git credentials for cms package chekout
# therefore setting dummy ones if noting set.
if [ -z "$(git config --get user.name)" ]; then
  git config user.name "${USER}"
  git config user.email "${USER}@cern.ch"
  git config user.github "${USER}"
fi

git-cms-addpkg CondFormats/JetMETObjects
git-cms-addpkg RecoLuminosity

# Checkout development branch of Kappa
# git clone -b development https://github.com/KappaAnalysis/Kappa.git Kappa
git clone -b development https://github.com/KappaAnalysis/Kappa.git Kappa
make -C Kappa/DataFormats/test/ -j4

# Checkout master of KappaTools
git clone https://github.com/KappaAnalysis/KappaTools.git KappaTools
make -C KappaTools -j4

# Checkout master of Artus
git clone https://github.com/artus-analysis/Artus.git Artus

# Checkout master of DijetAna
if [ "$TRAVIS" = true ] ; then
  git clone --branch ${TRAVIS_BRANCH} https://github.com/claria/JetAnalysis.git JetAnalysis
  cd JetAnalysis && git checkout -qf ${TRAVIS_COMMIT} && cd ..
else
  git clone https://github.com/claria/JetAnalysis.git JetAnalysis
fi

# JetAnalysis needs RooUnfold for the Unfolding part

git clone https://github.com/skluth/RooUnfold.git RooUnfold
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

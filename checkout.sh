#!/bin/bash




scram project CMSSW_7_2_3
scram b -j8
git-cms addpkg CondFormats/JetMETObjects
git-cms addpkg RecoLuminosity


cd src
git checkout Kappa
git checkout KappaTools
git checkout Artus
git checkout JetAnalysis

#setup tools....
cp JetAnalysis/cmssw_tools/kappa.xml 

# install RooUnfold

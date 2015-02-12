#! /usr/bin/env python2

import ROOT
from ROOT import TFile

def main():


	ROOT.gSystem.Load('/afs/desy.de/user/g/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

	datafile = TFile('~/dust/testDATA.root', 'UPDATE')
	responsefile = TFile('/nfs/dust/cms/user/gsieber/ARTUS/2015-02-11_10-23_analysis/output/response_p8.root')

	hist = datafile.Get('default/h_jet12rap')
	response = responsefile.Get('default/response_matrix')

	unfold = ROOT.RooUnfoldBayes(response, hist, 4)
	hReco = unfold.Hreco()

	datafile.cd()
	hReco.Write('unf_hjet12rap')




if __name__ == '__main__':
	main()

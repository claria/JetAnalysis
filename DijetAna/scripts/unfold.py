#! /usr/bin/env python2

import os
import sys
import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
from JetAnalysis.DijetAna.tools import *

def main():

    ROOT.gSystem.Load('/afs/desy.de/user/g/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

    parser = argparse.ArgumentParser(description='Unfold distribution with a given response matrix')

    parser.add_argument('--recohisto', help='Path to root file with the distribution')
    parser.add_argument('--response', help='Path to root file with the response matrix')
    parser.add_argument('--niters', help='Number of iterations in unfolding' type=int)

    args = vars(parser.parse_args())


    hist = get_root_object(args['inputhisto'], option='UPDATE')
    response = get_root_object(args['responsehisto'])

    unfold = ROOT.RooUnfoldBayes(response, hist, 4)
    print unfold.NToys()
    unfold.SetNToys(2)
    print unfold.NToys()
    unfold.RunToy()
    hReco = unfold.Hreco()
    hRecoCov = unfold.Ereco(3)

    #hRecoCorr = hRecoCov.Clone('hrecocorr')
    #print type(hRecoCorr)
    #print dir(hRecoCorr)
    #print hRecoCorr.GetNrows()

    #for i in range(0, hRecoCorr.GetNrows()):
    #    for j in range(i,hRecoCorr.GetNrows()):
    #        tmp = (hRecoCov[i][i]*hRecoCov[j][j])
    #        if tmp <= 0.:
    #            hRecoCorr[i][j] = 0.
    #        else:
    #            hRecoCorr[i][j] = (hRecoCov[i][j] / (hRecoCov[i][i]*hRecoCov[j][j]))
    #            if hRecoCorr[i][j] > 1.:
    #                print i, j, hRecoCorr[i][j]
    #            hRecoCorr[i][j] = 0.0
    #        hRecoCorr[j][i] = hRecoCorr[i][j]

    # for i in range 

    # outputfile = TFile('unfolded.root', 'RECREATE')

    datafile.cd('/')
    if datafile.GetDirectory('unf_default') != None:
        datafile.Delete('unf_default;*')

    datafile.mkdir('unf_default')
    datafile.cd('unf_default')
    # datafile.cd()
    hReco.Write('unf_hjet12rap')
    hRecoCov.Write('unf_hjet12rap_cov')
    hRecoCorr.Write('unf_hjet12rap_corr')


if __name__ == '__main__':
    main()

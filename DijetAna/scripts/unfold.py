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

    parser.add_argument('--measured-histo', help='Path to root file with the distribution.')
    parser.add_argument('--response-matrix', help='Path to root file with the response matrix')
    parser.add_argument('--niters', type=int, default=4, help='Number of iterations in Bayes Unfolding.')
    parser.add_argument('--ntoys', type=int, default=1, help='Number of toys in cov determination.')

    args = vars(parser.parse_args())

    measured_histo = get_root_object(args['measured_histo'], option='UPDATE')
    response_matrix = get_root_object(args['response_matrix'])

    unfold = ROOT.RooUnfoldBayes(response_matrix, measured_histo, args['niters'])

    # Unfold distribution
    recotruth_histo = unfold.Hreco()
    # Run Toys
    unfold.SetNToys(args['ntoys'])
    unfold.RunToy()
    recotruth_cov = unfold.Ereco(3)

    input_path = measured_histo.GetDirectory().GetPath()
    print input_path
    output_file = input_path.split(':')[0]
    output_path = input_path.split(':')[1].split('/')
    output_path[-1] = "unf_{0}".format(output_path[-1])
    output_path = '/'.join(output_path)

    print output_path

    # ROOT.gDirectory.cd(output_path)
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

    if ROOT.gDirectory.GetDirectory(output_path) != None:
        ROOT.gDirectory.Delete('unf_default;*')

    ROOT.gDirectory.mkdir(output_path)
    ROOT.gDirectory.cd(output_path)
    # datafile.cd()
    hReco.Write('unf_hjet12rap')
    hRecoCov.Write('unf_hjet12rap_cov')
#     hRecoCorr.Write('unf_hjet12rap_corr')


if __name__ == '__main__':
    main()

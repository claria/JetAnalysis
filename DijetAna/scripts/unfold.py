#! /usr/bin/env python2

import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
from ROOT import TFile

def main():

    ROOT.gSystem.Load('/afs/desy.de/user/g/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

    parser = argparse.ArgumentParser(description='Unfold distribution with a given response matrix')
    parser.add_argument('--inputfile', help='Path to root file with the distribution')
    parser.add_argument('--inputhisto', help='Path to histo in rootfile')

    parser.add_argument('--responsefile', help='Path to root file with the response matrix')
    parser.add_argument('--responsehisto', help='Path to response matrix in rootfile')

    args = vars(parser.parse_args())


    datafile = TFile(args['inputfile'], 'UPDATE')
    responsefile = TFile(args['responsefile'])

    hist = datafile.Get(args['inputhisto'])
    response = responsefile.Get(args['responsehisto'])

    # hist = datafile.Get('default/h_jet12rap')
    # response = responsefile.Get('default/response_matrix')

    unfold = ROOT.RooUnfoldBayes(response, hist, 4)
    hReco = unfold.Hreco()

    outputfile = TFile('unfolded.root', 'RECREATE')
    outputfile.mkdir('default')
    outputfile.cd('default')
    # datafile.cd()
    hReco.Write('unf_hjet12rap')


if __name__ == '__main__':
    main()

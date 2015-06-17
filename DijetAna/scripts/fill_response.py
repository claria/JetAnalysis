#! /usr/bin/env python2

import os
import sys
import argparse

import ROOT
from math import sqrt
ROOT.PyConfig.IgnoreCommandLineOptions = True
from JetAnalysis.DijetAna.tools import *

def main():

    ROOT.gSystem.Load('/afs/desy.de/user/g/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

    # Load gen matrix from fastNLO
    # create uniform distr. entries weight from fnlo
    # smear and fill response matrix 

    parser = argparse.ArgumentParser(description='Create response matrix from fixed order prediction.')

    parser.add_argument('--gen-histo', help='Path to root file with the distribution.')
    parser.add_argument('--response-matrix', help='Path to root file with the response matrix')
    parser.add_argument('--algo', default='iterative', choices=['iterative', 'binbybin', 'svd'], 
                        help='Unfolding alogrithm.')
    parser.add_argument('--reg-parameter', type=int, default=4, help='Number of iterations in Bayes Unfolding.')
    parser.add_argument('--ntoys', type=int, default=1, help='Number of toys in cov determination.')
    parser.add_argument('--output-file', help='Unfolded distributions will be written to that file if specified.')

    args = vars(parser.parse_args())

    measured_histo = get_root_object(args['measured_histo'], option='UPDATE')
    response_matrix = get_root_object(args['response_matrix'])

    if args['algo'] == 'iterative':
        unfold = ROOT.RooUnfoldBayes(response_matrix, measured_histo, args['reg_parameter'])
    elif args['algo'] == 'svd':
        unfold = ROOT.RooUnfoldSvd(response_matrix, measured_histo, args['reg_parameter'])
    elif args['algo'] == 'binbybin':
        unfold = ROOT.RooUnfoldBinByBin(response_matrix, measured_histo)
    else:
        raise ValueError()

    # Unfold distribution

    # Run Toys
    unfold.SetNToys(args['ntoys'])
    unfold.RunToy()

    recotruth_histo = unfold.Hreco(3)
    # recotruth_histo.SetName(measured_histo.GetName())
    recotruth_cov = unfold.Ereco(3)
    # recotruth_cov.SetName("cov_{0}".format(measured_histo.GetName()))

    input_path = measured_histo.GetDirectory().GetPath()
    print input_path
    output_file = input_path.split(':')[0]
    output_path = input_path.split(':')[1].split('/')
    output_path[-1] = "unf_{0}".format(output_path[-1])
    output_path = '/'.join(output_path).lstrip('/')

    print output_path

    # ROOT.gDirectory.cd(output_path)
    recotruth_corr = recotruth_cov.Clone("recotruth_corr")
    for i in range(0, recotruth_corr.GetNrows()):
        for j in range(i,recotruth_corr.GetNrows()):
            tmp = (recotruth_cov[i][i]*recotruth_cov[j][j])
            if tmp <= 0.:
                recotruth_corr[i][j] = 0.
            else:
                recotruth_corr[i][j] = (recotruth_cov[i][j] / (sqrt(recotruth_cov[i][i])*sqrt(recotruth_cov[j][j])))
                # if recotruth_corr[i][j] > 1.:
                    # print i, j, recotruth_corr[i][j]
                    # recotruth_corr[i][j] = 0.0
                recotruth_corr[j][i] = recotruth_corr[i][j]

    if args['output_file']:
        out_file = ROOT.TFile(args['output_file'], 'RECREATE')
        out_file.cd("")
    else:
        ROOT.gDirectory.cd(measured_histo.GetDirectory().GetPath())
        ROOT.gDirectory.cd('/')

        if ROOT.gDirectory.GetDirectory(output_path) != None:
            print "Folder {0} exists. Will be overwritten".format(output_path)
            ROOT.gDirectory.Delete('{0};*'.format(output_path))
        ROOT.gDirectory.mkdir(output_path)
        ROOT.gDirectory.cd(output_path)
 
    # datafile.cd()
    recotruth_histo.Write(measured_histo.GetName())
    response_matrix.Hmeasured().Write(response_matrix.Hmeasured().GetName())
    response_matrix.Htruth().Write(response_matrix.Htruth().GetName())
    recotruth_cov.Write('cov_{0}'.format(measured_histo.GetName()))
    recotruth_corr.Write('corr_{0}'.format(measured_histo.GetName()))


if __name__ == '__main__':
    main()

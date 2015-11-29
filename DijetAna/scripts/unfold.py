#! /usr/bin/env python2

"""
Unfolds a given distribution using the supplied
response matrix with the possibility to choose the
unfolding algorithm and the error calculation.
"""

import os
import sys
import argparse
import array

import ROOT
import math
from math import sqrt

ROOT.PyConfig.IgnoreCommandLineOptions = True
from JetAnalysis.DijetAna.tools import *

def main():

    ROOT.gSystem.Load('/afs/desy.de/user/g/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

    parser = argparse.ArgumentParser(description='Unfold distribution with a given response matrix')

    parser.add_argument('--measured-histo', help='Path to root file with the distribution.')
    parser.add_argument('--response-matrix', help='Path to root file with the response matrix')
    parser.add_argument('--normalize-binwidth', action='store_true',  help='Normalize input histogram by binwidth')
    parser.add_argument('--algo', default='iterative', choices=['iterative', 'binbybin', 'svd'], 
                        help='Unfolding alogrithm.')
    parser.add_argument('--reg-parameter', type=int, default=4, help='Number of iterations in Bayes Unfolding.')
    parser.add_argument('--ntoys', type=int, default=1, help='Number of toys in cov determination.')
    parser.add_argument('--output-file', help='Unfolded distributions will be written to that file if specified.')

    args = vars(parser.parse_args())

    measured_histo = get_root_object(args['measured_histo'], option='UPDATE')

    if args['normalize_binwidth']:
        measured_histo.Scale(1.0, "width")

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
    output_path[-1] = "{0}".format(output_path[-1])
    output_path = '/'.join(output_path).lstrip('/')
    print output_path

    # convert tmatrixd into a th2d
    cov_matrix = response_matrix.Hresponse().Clone("cov_matrix")
    cov_matrix.Reset()
    for x in xrange(1, cov_matrix.GetNbinsX() + 1):
        for y in xrange(1, cov_matrix.GetNbinsY() + 1):
            cov_matrix.SetBinContent(x, y, recotruth_cov[x-1][y-1])
            cov_matrix.SetBinError(x, y, 0.)


    # ROOT.gDirectory.cd(output_path)
    corr_matrix = cov_matrix.Clone("corr_matrix")
    for x in xrange(1, corr_matrix.GetNbinsX() + 1):
        for y in xrange(1, corr_matrix.GetNbinsY() + 1):
            try:
                corr_matrix.SetBinContent(x, y, cov_matrix.GetBinContent(x,y) / (math.sqrt(cov_matrix.GetBinContent(x,x)) * math.sqrt(cov_matrix.GetBinContent(y,y))))
                corr_matrix.SetBinError(x, y, 0.)
            except ZeroDivisionError:
                corr_matrix.SetBinContent(x, y, 0.)
                corr_matrix.SetBinError(x, y, 0.)

    if args['output_file']:
        out_file = ROOT.TFile(args['output_file'], 'UPDATE')
        out_file.cd("")

    # ROOT.gDirectory.cd(measured_histo.GetDirectory().GetPath())

    out_file.cd('/')
    # ROOT.gDirectory.cd('/')

    if ROOT.gDirectory.GetDirectory(output_path) != None:
        print "Folder {0} exists. Will be overwritten".format(output_path)
        ROOT.gDirectory.Delete('{0};*'.format(output_path))
    ROOT.gDirectory.mkdir(output_path)
    ROOT.gDirectory.cd(output_path)
 
    # datafile.cd()
    recotruth_histo.Write('{0}'.format(measured_histo.GetName()))
    recotruth_histo_binedges = array.array("d", [recotruth_histo.GetBinLowEdge(i+1) for i in xrange(recotruth_histo.GetNbinsX() +1)])
    recotruth_histo_rebinned = recotruth_histo.Rebin(len(recotruth_histo_binedges)-1, "{0}_rebinned".format(measured_histo.GetName()), recotruth_histo_binedges)
    recotruth_histo_rebinned.Write()
    response_matrix.Hmeasured().Write()
    response_matrix.Htruth().Write()
    response_matrix.Hresponse().Write()
    cov_matrix.Write('cov_{0}'.format(measured_histo.GetName()))
    corr_matrix.Write('corr_{0}'.format(measured_histo.GetName()))


if __name__ == '__main__':
    main()

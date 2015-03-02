#! /usr/bin/env python2
import os
import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(True)

# import matplotlib
# import matplotlib.pyplot as plt

import numpy as np

def main():

    parser = argparse.ArgumentParser(description='xyz')
    parser.add_argument('--input-file', required=True,
                        help='Path to root file with the distribution')

    args = vars(parser.parse_args())


    rap_bins = ['00_05','05_10','10_15','15_20','20_25','25_30']

    for rap_bin in rap_bins:
        histo = get_root_object(args['input_file'], '{0}/h2GenVsRecoPt'.format(rap_bin))
        n_ptbins = histo.GetNbinsY()

        # pt_bins = [histo.GetYaxis().GetBinCenter(i) for i in range(1, n_ptbins + 1)]
        # pt_bins_width = [histo.GetYaxis().GetBinWidth(i) for i in range(1, n_ptbins + 1)]
        print n_ptbins

        graph = ROOT.TGraphErrors(n_ptbins)

        pt_bins = []
        pt_bins_err = []

        pt_res = []
        pt_res_err = []
        for i in range(1, n_ptbins + 1):
            hslice = histo.ProjectionX("ptslice_{0}".format(i),i,i)
            hslice.Fit("gaus")
            fcn = hslice.GetFunction("gaus")

            if (fcn != None):
                graph.SetPoint(i, histo.GetYaxis().GetBinCenter(i), fcn.GetParameter(2))
                graph.SetPointError(i, histo.GetYaxis().GetBinWidth(i), fcn.GetParError(2))
                # pt_bins.append(histo.GetYaxis().GetBinCenter(i))
                # pt_bins_err.append(histo.GetYaxis().GetBinWidth(i))
                # pt_res.append(fcn.GetParameter(2))
                # pt_res_err.append(fcn.GetParError(2))

        graph.Draw()

        res_fcn = ROOT.TF1("res_fcn", "sqrt(([0]/x)**2+(([1]**2)/x)+[2]**2)")
        graph.Fit("res_fcn")
        graph.DrawClone()

        # print pt_bins
        # print pt_bins_err
        # print pt_res
        # print pt_res_err



def get_root_object(filename, objectpath, option="READ"):
    rootfile = ROOT.TFile(filename, option)
    ROOT.SetOwnership(rootfile, 0)
    obj = rootfile.Get(objectpath)
    ROOT.SetOwnership(obj, 0)
    if obj == None:
        raise Exception("Requested object {0} not found in rootfile {1}.".format(objectpath, filename))
    return obj


if __name__ == '__main__':
    main()

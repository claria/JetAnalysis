#! /usr/bin/env python2.6

import os
import sys
import argparse
import code

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
from ROOT import TFile
from array import array
import numpy as np

from fastnlo import fastNLOLHAPDF


def smear_pt(rap, pt_truth):

    def nsc_formula(x, N, S, C):
        return np.sqrt(np.sign(N)*(N/x)**2 + S**2/x + C**2)

    res = 0.0
    if np.abs(rap) < 0.5:
        res= nsc_formula(pt_truth, 0.874, 0.033, 0.0)
    elif (np.abs(rap) < 1.0):
        res = nsc_formula(pt_truth, 0.898, 0.035, 0.0)
    elif (np.abs(rap) < 1.5):
        res = nsc_formula(pt_truth, 0.865, 0.043, 0.0)
    elif (np.abs(rap) < 2.0):
        res = nsc_formula(pt_truth, 0.545, 0.031, 0.0)
    elif (np.abs(rap) < 2.5):
        res = nsc_formula(pt_truth, 0.572, 0.0266, 0.0)
    elif (np.abs(rap) < 3.0):
        res = nsc_formula(pt_truth, 0.608, 0.000, 0.0)
    else:
        raise Exception("out of range")

    # print res*pt_truth
    return ROOT.gRandom.Gaus(pt_truth, res*pt_truth)

def main():

    ROOT.gSystem.Load('/afs/desy.de/user/g/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

    fnlo = fastNLOLHAPDF('/nfs/dust/cms/user/gsieber/ANALYSIS/FNLO/triplediff/TripleDiffJets.tab')
    fnlo.SetLHAPDFFilename('CT10nlo.LHgrid')
    fnlo.SetLHAPDFMember(0)
    x_binning = sorted(list(set([item for sublist in fnlo.GetDim0BinBoundaries() for item in sublist])))
    y_binning = sorted(list(set([item for sublist in fnlo.GetDim1BinBoundaries(0) for item in sublist])))
    z_binning = sorted(list(set([item for sublist in fnlo.GetDim2BinBoundaries(0, 0) for item in sublist])))
    th3 = ROOT.TH3D("xs_nlo","",len(x_binning)-1, array('d', x_binning),
                                len(y_binning)-1, array('d', y_binning),
                                len(z_binning)-1, array('d', z_binning),)
    # print fnlo.GetCrossSection()
    xs = np.array(fnlo.GetCrossSection3Dim())
    xs[xs <= 0.] = 0.
    # print xs
    for i in range(0, fnlo.GetNDim0Bins()):
        for j in range(0, fnlo.GetNDim1Bins(i)):
            for k in range(0, fnlo.GetNDim2Bins(i,j)):
                th3.SetBinContent(i+1,j+1,k+1,xs[i][j][k])

    # y1, y2, pt1 = ROOT.Double(), ROOT.Double(), ROOT.Double()

    response = ROOT.RooUnfoldResponse(th3, th3);
    th3check = th3.Clone("check")
    th3check.Reset()
    for i in xrange(10000):
        y1 = ROOT.gRandom.Uniform(-3.0,3.0)
        y2 = ROOT.gRandom.Uniform(-3.0,3.0)
        pt1 = ROOT.gRandom.Uniform(50.,3000)
        w = th3.GetBinContent(th3.FindBin(y1,y2,pt1))
        # try:
            # w = 1./th3.GetBinContent(th3.FindBin(y1,y2,pt1))
        # except ZeroDivisionError:
            # w = 0.
        # print y1, y2, pt1, w
        # print y1,y2,pt1, smear_pt(y1, pt1)
        for i in xrange(1000):
            smearpt1 = smear_pt(y1, pt1)
            response.Fill(y1, y2, smearpt1, y1, y2, pt1)
            th3check.Fill(y1, y2, pt1)

    x = ROOT.TFile('response.root', 'RECREATE')
    response.Write('nloresponse')
    th3check.Write("check")

if __name__ == '__main__':
    main()

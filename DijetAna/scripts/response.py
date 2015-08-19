#! /usr/bin/env python2.6

import os
import sys
import argparse
import code

import math
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
from ROOT import TFile
from array import array
import numpy as np

# from fastnlo import fastNLOLHAPDF


def smear_pt(rap_bin, pt_truth):

    def nsc_formula(x, N, S, C):
        # return np.sqrt(np.sign(N)*(N/x)**2 + S**2/x + C**2)
        return np.sqrt((N/x)**2 + (S**2)/x + C**2)

    res_pars = {
            'ptavg_yb_00_10_ys_00_10' : [5.22275e+00, 9.60557e-01, 3.49387e-02], 
            'ptavg_yb_00_10_ys_10_20' : [6.21719e+00, 8.38538e-01, 3.69017e-02],
            'ptavg_yb_00_10_ys_20_30' : [6.05380e+00, 8.59112e-01, -5.60583e-07],
            'ptavg_yb_10_20_ys_00_10' : [6.06379e+00, 8.57709e-01, -3.49501e-02],
            'ptavg_yb_10_20_ys_10_20' : [5.30264e+00, 9.35802e-01, -2.40824e-02],
            # 'ptavg_yb_10_20_ys_20_30' : [-2828.50171988, 0.5, -7.33267613295e-15, 0.0],
            'ptavg_yb_20_30_ys_00_10' : [5.85437e+00, 8.73750e-01, 1.76659e-08],
            # 'ptavg_yb_20_30_ys_10_20' : [-2828.50171988, 0.5, -7.33267613295e-15, 0.0],
            # 'ptavg_yb_20_30_ys_20_30' : [-2828.50171988, 0.5, -7.33267613295e-15, 0.0],

            }
    res = nsc_formula(pt_truth, res_pars[rap_bin][0], res_pars[rap_bin][1], res_pars[rap_bin][2])

    shift = ROOT.gRandom.Gaus(0., res*pt_truth)
    # print shift, pt_truth + shift

    return pt_truth + shift

def main():

    ROOT.gSystem.Load('/nfs/dust/cms/user/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

    rap_bins = ['ptavg_yb_00_10_ys_00_10',
                'ptavg_yb_00_10_ys_10_20',
                'ptavg_yb_00_10_ys_20_30',
                'ptavg_yb_10_20_ys_00_10',
                'ptavg_yb_10_20_ys_10_20',
                # 'ptavg_yb_10_20_ys_20_30',
                'ptavg_yb_20_30_ys_00_10',
                # 'ptavg_yb_20_30_ys_10_20',
                # 'ptavg_yb_20_30_ys_20_30',
            ]

    pt_binning_gen = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
    pt_binning_reco = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]

    nlofile = ROOT.TFile('/afs/desy.de/user/g/gsieber/XSNLO.root', 'READ')
    f = ROOT.TFile('response_fastNLO.root', 'RECREATE')

    fit_start_params = {
            'ptavg_yb_00_10_ys_00_10' : [1.56951e-04, 4.72895e+00, 1.25370e+01, 4.41885e+03],
            'ptavg_yb_00_10_ys_10_20' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],
            'ptavg_yb_00_10_ys_20_30' : [2.71653e+00, 4.28902e+00, 7.67473e+00, 5.83335e+02],
            'ptavg_yb_10_20_ys_00_10' : [4.56147e-04, 4.88011e+00, 1.26727e+01, 2.80141e+03],
            'ptavg_yb_10_20_ys_10_20' : [1.62403e-01, 3.89047e+00, 1.73026e+01, 1.40653e+03],
            # 'ptavg_yb_10_20_ys_20_30' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],
            'ptavg_yb_20_30_ys_00_10' : [2.39237e-02, 4.39508e+00, 1.41838e+01, 1.16487e+03],
            # 'ptavg_yb_20_30_ys_10_20' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],
            # 'ptavg_yb_20_30_ys_20_30' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],

            }

    for rap_bin in rap_bins:
        print rap_bin
        xsnlo = nlofile.Get("{0}/xsnlo".format(rap_bin))
        # Fitting spectrum
        ROOT.TVirtualFitter.SetMaxIterations(9999)
        nlo_fcn = ROOT.TF1("nlo_fcn", "[0]*(x/[3])**(-1*[1])*(1-(x/[3]))**[2]")
        nlo_fcn.SetParameters(*fit_start_params[rap_bin])
        # nlo_fcn.SetRange(100., 1000.)
        # res = xsnlo.Fit("nlo_fcn", "RSW", "")
        # res = xsnlo.Fit("nlo_fcn", "RS", "")
        nlo_fcn.SetRange(74., 3000.)
        res = xsnlo.Fit("nlo_fcn", "RSI0", "")

        #TODO Fit this function
        f.mkdir(rap_bin)
        f.Cd("/" + rap_bin)
        reco_binning_ptavg = ROOT.TH1D("reco_binning_ptavg","reco_binning_ptavg",len(pt_binning_reco)-1, array('d', pt_binning_reco))
        reco_binning_ptavg.Sumw2()
        gen_binning_ptavg = ROOT.TH1D("gen_binning_ptavg","gen_binning_ptavg",len(pt_binning_gen)-1, array('d', pt_binning_gen))
        gen_binning_ptavg.Sumw2()

        h_recoptavg = ROOT.TH1D("h_recoptavg","h_recoptavg",len(pt_binning_reco)-1, array('d', pt_binning_reco))
        # h_recoptavg = ROOT.TH1D("h_recoptavg","h_recoptavg",100, 0, 5000)
        h_recoptavg.Sumw2()
        h_genptavg = ROOT.TH1D("h_genptavg","h_genptavg",len(pt_binning_gen)-1, array('d', pt_binning_gen))
        # h_genptavg = ROOT.TH1D("h_genptavg","h_genptavg", 100, 0, 5000.)
        h_genptavg.Sumw2()



        response_ptavg = ROOT.RooUnfoldResponse(reco_binning_ptavg, gen_binning_ptavg)

        nlo_fcn.SetRange(36., 3000.)

        n_evts = 10000000
        for i in xrange(n_evts):
            pt_truth = ROOT.gRandom.Uniform(50.,4000)
            pt_smeared = smear_pt(rap_bin, pt_truth)
            w = nlo_fcn.Eval(pt_truth)
            if (w< 0.) or math.isnan(w):
                w = 0.
            # if pt_smeared < 62.:
                # response_ptavg.Miss(pt_truth, w)
            # else:
            if pt_truth > 62. and pt_smeared > 62.:
                response_ptavg.Fill(pt_smeared, pt_truth, w)
            elif pt_truth > 62. and pt_smeared <=62.:
                response_ptavg.Miss(pt_truth, w)
            elif pt_truth <= 62. and pt_smeared > 62.:
                response_ptavg.Fake(pt_truth, w)

            h_genptavg.Fill(pt_truth, w)
            h_recoptavg.Fill(pt_smeared, w)

        response_ptavg.Write('res_matrix_ptavg')
        # h_recoptavg.Scale(1.0, "width")
        h_recoptavg.Write()
        # h_genptavg.Scale(1.0, "width")
        h_genptavg.Write()
        del response_ptavg
        del h_recoptavg
        del h_genptavg


    f.Close()

if __name__ == '__main__':
    main()

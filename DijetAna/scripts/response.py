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

    def nsc_formula(x, N, S,  C, s):
        return np.sqrt(np.sign(N)*(N/x)**2 + (S**2/x)*(x**s) + C**2)
        # return np.sqrt((N/x)**2 + (S**2)/x + C**2)

    res_pars = {
            'yb0ys0' : [-5.96225e+00, 3.50717e+00, 4.25235e-02, -4.52550e-01], 
            'yb0ys1' : [-2.52071e+01, 2.29680e+01, 4.64303e-02, -9.26323e-01],
            'yb0ys2' : [4.02190e+00, 1.86641e+00, 2.18110e-02, -2.92844e-01],
            'yb1ys0' : [-3.37585e+01, 3.19434e+01, 4.63648e-02, -9.56920e-01],
            'yb1ys1' : [-1.94071e+00, 2.50898e+00, 3.65491e-02, -3.73551e-01],
            'yb2ys0' : [4.13130e+00, 1.63916e+00, 1.59900e-02, -2.32467e-01],
            }
    res = nsc_formula(pt_truth, res_pars[rap_bin][0], res_pars[rap_bin][1], res_pars[rap_bin][2], res_pars[rap_bin][3])

    shift = ROOT.gRandom.Gaus(0., res*pt_truth)
    # print shift, pt_truth + shift

    return pt_truth + shift

def main():

    ROOT.gSystem.Load('/nfs/dust/cms/user/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

    rap_bins = ['yb0ys0',
                'yb0ys1',
                'yb0ys2',
                'yb1ys0',
                'yb1ys1',
                'yb2ys0',
            ]

    pt_binning_gen = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
    pt_binning_reco = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]

    nlofile = ROOT.TFile('/afs/desy.de/user/g/gsieber/PTAVG_YBYS_NLO.root', 'READ')
    f = ROOT.TFile('response_fastNLO.root', 'RECREATE')

    fit_start_params = {
            'yb0ys0' : [1.56951e-04, 4.72895e+00, 1.25370e+01, 4.41885e+03],
            'yb1ys0' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],
            'yb2ys0' : [2.71653e+00, 4.28902e+00, 7.67473e+00, 5.83335e+02],
            'yb0ys1' : [4.56147e-04, 4.88011e+00, 1.26727e+01, 2.80141e+03],
            'yb1ys1' : [1.62403e-01, 3.89047e+00, 1.73026e+01, 1.40653e+03],
            'yb0ys2' : [2.39237e-02, 4.39508e+00, 1.41838e+01, 1.16487e+03],

            }

    for rap_bin in rap_bins[0:]:
        print rap_bin
        xsnlo = nlofile.Get("{0}/CT10nlo_xs".format(rap_bin))
        # Fitting spectrum
        ROOT.TVirtualFitter.SetMaxIterations(9999)
        nlo_fcn = ROOT.TF1("nlo_fcn", "[0]*(x/[3])**(-1*[1])*(1-(x/[3]))**[2]")
        nlo_fcn.SetParameters(*fit_start_params[rap_bin])
        # nlo_fcn.SetRange(100., 1000.)
        # res = xsnlo.Fit("nlo_fcn", "RSW", "")
        # res = xsnlo.Fit("nlo_fcn", "RS", "")
        nlo_fcn.SetRange(74., 3000.)
        res = xsnlo.Fit("nlo_fcn", "RSI0")

        if res.Get() == None or res.Status() != 0:
            raise Exception('Could not fit the function.')


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

        n_evts = 100000
        for i in xrange(n_evts):
            pt_truth = ROOT.gRandom.Uniform(36.,3000)
            pt_smeared = smear_pt(rap_bin, pt_truth)
            w = nlo_fcn.Eval(pt_truth)
            if (w< 0.) or math.isnan(w):
                w = 0.
            # if pt_smeared < 62.:
                # response_ptavg.Miss(pt_truth, w)
            # else:
            # if pt_truth > 62. and pt_smeared > 62.:
            # elif pt_truth > 62. and pt_smeared <=62.:
                # response_ptavg.Miss(pt_truth, w)
            if pt_truth <= 74. and pt_smeared <=74.:
                continue
            else:
                response_ptavg.Fill(pt_smeared, pt_truth, w)

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

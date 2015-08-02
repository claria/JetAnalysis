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
            'ptavg_yb_00_10_ys_00_10' : [4.55108862204, 0.664407443168, 0.0283713960455, 0.0],
            'ptavg_yb_00_10_ys_10_20' : [6.54217125781, 0.369798623098, 0.0350309905636, 0.0],
            'ptavg_yb_00_10_ys_20_30' : [4.7553191831, 0.623381451417, 5.86709966666e-07, 0.0],
            'ptavg_yb_10_20_ys_00_10' : [6.30077879419, 0.402135274691, 0.0345126768626, 0.0],
            'ptavg_yb_10_20_ys_10_20' : [4.5465663911, 0.639092518774, 0.0206529461783, 0.0],
            'ptavg_yb_10_20_ys_20_30' : [-2828.50171988, 0.5, -7.33267613295e-15, 0.0],
            'ptavg_yb_20_30_ys_00_10' : [5.11347922038, 0.602555697342, 7.9639541027e-07, 0.0],
            'ptavg_yb_20_30_ys_10_20' : [-2828.50171988, 0.5, -7.33267613295e-15, 0.0],
            'ptavg_yb_20_30_ys_20_30' : [-2828.50171988, 0.5, -7.33267613295e-15, 0.0],

            }
    res = nsc_formula(pt_truth, res_pars[rap_bin][0], res_pars[rap_bin][1], res_pars[rap_bin][2])

    shift = ROOT.gRandom.Gaus(0., res*pt_truth)
    # print shift, pt_truth + shift

    return pt_truth + shift

def main():

    ROOT.gSystem.Load('/nfs/dust/cms/user/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

    # fnlo = fastNLOLHAPDF('/nfs/dust/cms/user/gsieber/FNLO/TRIPLEDIFF_YSB_PTAVG/triplediff.tab')
    # fnlo.SetLHAPDFFilename('CT10nlo')
    # fnlo.SetLHAPDFMember(0)

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
    f = ROOT.TFile('unf_fastNLO.root', 'RECREATE')

    fit_start_params = {
            'ptavg_yb_00_10_ys_00_10' : [1.56951e-04, 4.72895e+00, 1.25370e+01, 4.41885e+03],
            'ptavg_yb_00_10_ys_10_20' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],
            'ptavg_yb_00_10_ys_20_30' : [2.71653e+00, 4.28902e+00, 7.67473e+00, 5.83335e+02],
            'ptavg_yb_10_20_ys_00_10' : [4.56147e-04, 4.88011e+00, 1.26727e+01, 2.80141e+03],
            'ptavg_yb_10_20_ys_10_20' : [1.62403e-01, 3.89047e+00, 1.73026e+01, 1.40653e+03],
            'ptavg_yb_10_20_ys_20_30' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],
            'ptavg_yb_20_30_ys_00_10' : [2.39237e-02, 4.39508e+00, 1.41838e+01, 1.16487e+03],
            'ptavg_yb_20_30_ys_10_20' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],
            'ptavg_yb_20_30_ys_20_30' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],

            }

    for rap_bin in rap_bins[0:1]:
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
        # if res.Get() == None or res.Status() != 0:
        #     nlo_fcn.SetRange(100., 1000.)
        #     # res = xsnlo.Fit("nlo_fcn", "RSW", "")
        #     res = xsnlo.Fit("nlo_fcn", "RS", "")
        #     nlo_fcn.SetRange(74., 2500.)
        #     res = xsnlo.Fit("nlo_fcn", "RSI", "")
        #     if res.Get() == None or res.Status() != 0:
        #         continue


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

        n_evts = 1000000
        for i in xrange(n_evts):
            pt_truth = ROOT.gRandom.Uniform(62..,4000)
            pt_smeared = smear_pt(rap_bin, pt_truth)
            w = nlo_fcn.Eval(pt_truth)
            if pt_smeared < 62.:
                response_ptavg.Miss(pt_truth, w)
            # else:
            response_ptavg.Fill(pt_smeared, pt_truth, w)

            # pt_truth = ROOT.gRandom.Uniform(30.,4000)
            # pt_smeared = smear_pt(rap_bin, pt_truth)
            # w = nlo_fcn.Eval(pt_truth)
            h_genptavg.Fill(pt_truth, w)
            h_recoptavg.Fill(pt_smeared, w)

        response_ptavg.Write('response_ptavg')
        # h_recoptavg.Scale(1.0, "width")
        h_recoptavg.Write()
        # h_genptavg.Scale(1.0, "width")
        h_genptavg.Write()

if __name__ == '__main__':
    main()

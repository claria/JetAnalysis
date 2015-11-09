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
from JetAnalysis.DijetAna.tools import get_root_object

# from fastnlo import fastNLOLHAPDF
def multiply(obj, with_obj):
    obj = obj.Clone('ratio_{0}'.format(obj.GetName()))

    if isinstance(with_obj, ROOT.TH1):
        mult_vals = np.zeros(obj.GetNbinsX())
        for i in xrange(1, with_obj.GetNbinsX() + 1):
            mult_vals[i-1] = with_obj.GetBinContent(i)
    elif isinstance(with_obj, ROOT.TGraph):
        mult_vals = np.zeros(with_obj.GetN())
        for i in xrange(with_obj.GetN()):
            tmp_x, tmp_y = ROOT.Double(0), ROOT.Double(0)
            with_obj.GetPoint(i, tmp_x, tmp_y)
            mult_vals[i] = tmp_y

    if isinstance(obj, ROOT.TH1):
        for i in xrange(1, obj.GetNbinsX() + 1):
            obj.SetBinContent(i, obj.GetBinContent(i) * mult_vals[i-1])
            obj.SetBinError(i, obj.GetBinError(i) * mult_vals[i-1])
    elif isinstance(obj, ROOT.TGraph):
        for i in xrange(obj.GetN()):
            tmp_x, tmp_y = ROOT.Double(0), ROOT.Double(0)
            obj.GetPoint(i, tmp_x, tmp_y)
            obj.SetPoint(i, tmp_x, tmp_y * mult_vals[i])
    else:
        raise TypeError('Invalid types passed: {0} and {1}'.format(type(obj), type(to_obj)))
    return obj



def smear_pt(rap_bin, pt_truth):

    def nsc_formula(x, N, S,  C, s):
        return np.sqrt(np.sign(N)*(N/x)**2 + (S**2/x)*(x**s) + C**2)
        # return np.sqrt((N/x)**2 + (S**2)/x + C**2)

    res_pars = {
            'yb0ys2' : [-6.559428568585036, 4.02291103462898, -0.016990587323112594, -0.43605726206523737],
            'yb2ys0' : [-7.123740641706069, 3.9669364250448425, 2.5303982170308084e-08, -0.4182342978522098],
            'yb0ys1' : [-25.95681928096833, 23.05783077220221, 0.05129690418134727, -0.9079835335486168],
            'yb0ys0' : [-6.257073956874592, 3.6239281199683497, 0.04602778688758654, -0.43123391587348026],
            'yb1ys0' : [-28.331027205702384, 25.514396527787525, 0.05089882152879723, -0.9197037074885134],
            'yb1ys1' : [-5.7866157049523546, 3.133659684781229, 0.029466564514641694, -0.34876443160744663],
            }
    res = nsc_formula(pt_truth, res_pars[rap_bin][0], res_pars[rap_bin][1], res_pars[rap_bin][2], res_pars[rap_bin][3])

    shift = ROOT.gRandom.Gaus(0., res*pt_truth)

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

    f = ROOT.TFile('response_fastNLO.root', 'RECREATE')

    fit_start_params = {
            'yb0ys0' : [1.56951e-04, 4.72895e+00, 1.25370e+01, 4.41885e+03],
            'yb0ys1' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],
            'yb0ys2' : [2.71653e+00, 4.28902e+00, 7.67473e+00, 5.83335e+02],
            'yb1ys0' : [4.56147e-04, 4.88011e+00, 1.26727e+01, 2.80141e+03],
            'yb1ys1' : [1.62403e-01, 3.89047e+00, 1.73026e+01, 1.40653e+03],
            'yb2ys0' : [2.39237e-02, 4.39508e+00, 1.41838e+01, 1.16487e+03],

            }

    for rap_bin in rap_bins:
        print rap_bin
        xsnlo = get_root_object("/nfs/dust/cms/user/gsieber/dijetana/ana/CMSSW_7_2_3/PTAVG_YBYS_NLO.root?{0}/CT14nlo_xs".format(rap_bin))
        np_corr = get_root_object('~/dust/dijetana/plot/plots/np_factors_calc_{0}.root?res_np_factor'.format(rap_bin))
        f.cd()
        xsnlo = multiply(xsnlo, np_corr)
        # Fitting spectrum
        ROOT.TVirtualFitter.SetMaxIterations(9999)
        nlo_fcn = ROOT.TF1("nlo_fcn", "[0]*(x/[3])**(-1*[1])*(1-(x/[3]))**[2]")
        nlo_fcn.SetParameters(*fit_start_params[rap_bin])
        # nlo_fcn.SetRange(100., 1000.)
        # res = xsnlo.Fit("nlo_fcn", "RSW", "")
        # res = xsnlo.Fit("nlo_fcn", "RS", "")
        nlo_fcn.SetRange(74.,3000.)
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
        h2_genvsreco = ROOT.TH2D("h2_genvsreco","h2_genvsreco",
                                      len(pt_binning_gen)-1, array('d', pt_binning_gen),
                                      len(pt_binning_gen)-1, array('d', pt_binning_gen))
        h2_genvsreco.Sumw2()




        # response_ptavg = ROOT.RooUnfoldResponse(reco_binning_ptavg, gen_binning_ptavg)

        nlo_fcn.SetRange(50., 3000.)

        n_evts = 10000000
        for i in xrange(n_evts):
            pt_truth = ROOT.gRandom.Uniform(74.,2500)
            pt_smeared = smear_pt(rap_bin, pt_truth)
            w = nlo_fcn.Eval(pt_truth)
            if (w <= 0.) or math.isnan(w):
                continue
            if pt_smeared >= 133.:
                h_recoptavg.Fill(pt_smeared, w)
            h_genptavg.Fill(pt_truth, w)
            if pt_smeared >= 133.:
                h2_genvsreco.Fill(pt_smeared, pt_truth, w)

        h_recoptavg.Write()
        h_genptavg.Write()
        response_ptavg = ROOT.RooUnfoldResponse(h_recoptavg, h_genptavg, h2_genvsreco)
        response_ptavg.Write('res_matrix_ptavg')

    f.Close()

if __name__ == '__main__':
    main()

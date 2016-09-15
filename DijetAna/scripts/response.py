#! /usr/bin/env python2.6

"""
Simple script to fill the response matrix using
the (hardcoded) resolution parameters for the 
triple-differential dijet measurement.
"""

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

    # def nsc_formula(x, N, S,  C, s):
    #     return np.sqrt(np.sign(N)*(N/x)**2 + (S**2/x)*(x**s) + C**2)

    def nsc_formula(x, N, S,  C):
        return np.sqrt((N/x)**2 + (S**2/x) + C**2)

    res_pars = {
                    #smeared
                    # 'yb0ys2' : [3.8188948881395355, 1.063083011370687, 3.4200917738180227e-06, -0.11918694682854095],
                    # 'yb2ys0' : [3.5323885442010594, 1.3741219633927806, -3.7276096763528617e-07, -0.21574785904274932],
                    # 'yb0ys1' : [-14.645039124737186, 12.464052372211727, 0.036715027837093614, -0.8798508791305507],
                    # 'yb0ys0' : [-2.7791518880890305, 1.4505596110112358, 0.032105415772409183, -0.2651411508680278],
                    # 'yb1ys0' : [-9.90637367182563, 7.638967976186312, 0.035941137670415575, -0.7879286176269638],
                    # 'yb1ys1' : [3.337117251543133, 0.9556731938830301, 0.0206413977495409, -0.09920693952683875],
                    #notsmeared
                    # 'yb0ys2' : [2.8748926482161914, 0.916884034760364, 0.007228356036913385, -0.14151346541772292],
                    # 'yb2ys0' : [3.3795319620943847, 0.8870514729016068, -9.942359703366933e-06, -0.13669426508442137],
                    # 'yb0ys1' : [-17.11127496255949, 15.48017973263991, 0.03313372476791994, -0.9272873503802115],
                    # 'yb0ys0' : [-2.543973103611753, 1.322717874124485, 0.029415266355339125, -0.26471358711396054],
                    # 'yb1ys0' : [-8.659235455114217, 6.705105536558293, 0.032319195256631204, -0.7915800693959144],
                    # 'yb1ys1' : [3.0400190253323336, 0.6480502336932839, 0.018088161278271474, -0.011984497679986642]
                    # no_s params
                    #smeared2
                    'yb0ys2' : [3.6964307051912018, 0.833650145986526, -1.639102427397553e-07, 0.0],
                    'yb2ys0' : [4.2529195511315265, 0.8271953848464056, 9.724618349824852e-08, 0.0],
                    'yb0ys1' : [3.871684869539761, 0.6632895459252569, 0.031207578917220462, 0.0],
                    'yb0ys0' : [2.5698099523479434, 0.7150830545035407, 0.030073979031654163, 0.0],
                    'yb1ys0' : [3.9361647268448965, 0.6763914723407488, 0.02963927619804816, 0.0],
                    'yb1ys1' : [2.8708829986100044, 0.8542776129157013, 1.014667344736689e-07, 0.0],
                    #smeared
                    # 'yb0ys2' : [1.875558696882837, 0.8322770837961365, -3.10419369943653e-08, 0.0],
                    # 'yb2ys0' : [2.6478965736807853, 0.8324430231036144, -8.24390587631416e-08, 0.0],
                    # 'yb0ys1' : [2.7328890160784987, 0.676172537228774, 0.031050138834928482, 0.0],
                    # 'yb0ys0' : [1.335811210426792, 0.706008508097077, 0.030465123467446342, 0.0],
                    # 'yb1ys0' : [2.7609185999842536, 0.6897669322719426, 0.029431784240958773, 0.0],
                    # 'yb1ys1' : [2.9581960640444186e-05, 0.8358961518578184, 0.011123335699668172, 0.0],
                    # not smeared
                    # 'yb0ys2' : [3.5587511225447397, 0.6603383593837494, -6.877554270204412e-07, 0.0],
                    # 'yb2ys0' : [4.015417100639938, 0.6576447283611095, 3.881264784372802e-08, 0.0],
                    # 'yb0ys1' : [3.4860916357259515, 0.5574067976757405, 0.029237813223096813, 0.0],
                    # 'yb0ys0' : [2.2389792895795373, 0.6352638587569062, 0.028431021125868, 0.0],
                    # 'yb1ys0' : [3.5681242479371162, 0.5623150964996525, 0.028136733594075605, 0.0],
                    # 'yb1ys1' : [2.7312883912887496, 0.69844070238645, 0.012732110109432433, 0.0],
            }
    res = nsc_formula(pt_truth, res_pars[rap_bin][0], res_pars[rap_bin][1], res_pars[rap_bin][2])
    # print pt_truth, res
    smeared = ROOT.gRandom.Gaus(pt_truth, res*pt_truth)
    return smeared

def main():

    ROOT.gSystem.Load('/nfs/dust/cms/user/gsieber/dijetana/ana/CMSSW_7_2_3/src/RooUnfold/libRooUnfold.so')

    rap_bins = [
                'yb0ys0',
                'yb0ys1',
                'yb0ys2',
                'yb1ys0',
                'yb1ys1',
                'yb2ys0',
               ]

    pt_binning_gen = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
    pt_binning_reco = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]

    f = ROOT.TFile('response_fastNLO_test.root', 'RECREATE')

    fit_start_params = {
            # 'yb0ys0' : [1.56951e-04, 4.72895e+00, 1.25370e+01, 4.41885e+03],
            # 'yb0ys1' : [1.25998e-02, 4.86297e+00, 7.80386e+00, 1.58397e+03],
            # 'yb0ys2' : [2.71653e+00, 4.28902e+00, 7.67473e+00, 5.83335e+02],
            # 'yb1ys0' : [4.56147e-04, 4.88011e+00, 1.26727e+01, 2.80141e+03],
            # 'yb1ys1' : [1.62403e-01, 3.89047e+00, 1.73026e+01, 1.40653e+03],
            # 'yb2ys0' : [2.39237e-02, 4.39508e+00, 1.41838e+01, 1.16487e+03],
            'yb0ys0' : [0.000211072329612, 4.87709548825, 9.82891107965, 3824.60314481],
            'yb0ys1' : [0.000169411292588, 4.85163640334, 18.3152756054, 3632.68792646],
            'yb0ys2' : [0.0114158299718, 5.06424208174, 9.68473457079, 976.283670437],
            'yb1ys0' : [0.00872849664081, 4.59730478959, 13.5532071286, 2118.68579277],
            'yb1ys1' : [0.0499068529382, 4.5689979335, 12.5978652543, 1234.29933591],
            'yb2ys0' : [5.18984314601, 4.0596259301, 8.53397822336, 580.484637396],
            }
    data_lim_max = { 'yb0ys0' : 1784.,
                     'yb0ys1' : 1248.,
                     'yb0ys2' : 548.,
                     'yb1ys0' : 1032.,
                     'yb1ys1' : 686.,
                     'yb2ys0' : 430.,
                   }

    for rap_bin in rap_bins:
        print rap_bin
        # xsnlo = get_root_object("/nfs/dust/cms/user/gsieber/dijetana/ana/CMSSW_7_2_3/PTAVG_YBYS_NLO.root?{0}/CT14nlo_xs".format(rap_bin))
        xsnlo = get_root_object("/nfs/dust/cms/user/gsieber/dijetana/ana/CMSSW_7_2_3/PTAVG_YBYS_NLO.root?{0}/CT14nlo_xs".format(rap_bin))
        # xsnlo = get_root_object("/nfs/dust/cms/user/gsieber/dijetana/ana/CMSSW_7_2_3/DATA.root?{0}/h_ptavg".format(rap_bin))
        # xsnlo.Scale(1.0, "width")
        # xsnlo = get_root_object("/nfs/dust/cms/user/gsieber/dijetana/ana/CMSSW_7_2_3/QCDMGP6.root?{0}/CT14nlo_xs".format(rap_bin))
        # np_corr = get_root_object('~/dust/dijetana/plot/plots/np_factors_calc_{0}.root?res_np_factor'.format(rap_bin))
        f.cd()
        # xsnlo = multiply(xsnlo, np_corr)
        # Fitting spectrum
        ROOT.TVirtualFitter.SetMaxIterations(99999)
        nlo_fcn = ROOT.TF1("nlo_fcn_{0}".format(rap_bin), "[0]*(x/[3])**(-1*[1])*(1-(x/[3]))**[2]")
        nlo_fcn.SetParameters(*fit_start_params[rap_bin])
        nlo_fcn.SetRange(133.,data_lim_max[rap_bin])
        res = xsnlo.Fit("nlo_fcn_{0}".format(rap_bin), "RSI")

        if res.Get() == None or res.Status() != 0:
            raise Exception('Could not fit the function.')
        print '\'{0}\' : [{1}, {2}, {3}, {4}],'.format(rap_bin, nlo_fcn.GetParameter(0),nlo_fcn.GetParameter(1), nlo_fcn.GetParameter(2), nlo_fcn.GetParameter(3))


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

        n_evts = 1000000
        for i in xrange(n_evts):
            pt_truth = ROOT.gRandom.Uniform(40.,data_lim_max[rap_bin]*1.5)
            pt_smeared = smear_pt(rap_bin, pt_truth)
            w = nlo_fcn.Eval(pt_truth)
            # print pt_truth, w
            if (w <= 0.) or math.isnan(w):
                continue

            if pt_truth >=133.:
                h_genptavg.Fill(pt_truth, w)
            if pt_smeared >= 133.:
                h_recoptavg.Fill(pt_smeared, w)
            if pt_smeared >=133. and pt_truth >= 133.:
               h2_genvsreco.Fill(pt_smeared, pt_truth, w)

        # h_recoptavg.Scale(1./n_evts)
        # h_genptavg.Scale(1./n_evts)
        # h2_genvsreco.Scale(1./n_evts)

        # Scale with fakes/miss hits
        mgp6_fakes = get_root_object("~/dust/dijetana/ana/CMSSW_7_2_3/response_study_notsmeared_lowptavg_QCDMGP6.root?{0}/h_unf_fake_ptavg".format(rap_bin))
        mgp6_reco = get_root_object("~/dust/dijetana/ana/CMSSW_7_2_3/response_study_notsmeared_lowptavg_QCDMGP6.root?{0}/h_unf_reco_ptavg".format(rap_bin))
        mgp6_miss = get_root_object("~/dust/dijetana/ana/CMSSW_7_2_3/response_study_notsmeared_lowptavg_QCDMGP6.root?{0}/h_unf_miss_ptavg".format(rap_bin))
        mgp6_gen = get_root_object("~/dust/dijetana/ana/CMSSW_7_2_3/response_study_notsmeared_lowptavg_QCDMGP6.root?{0}/h_unf_gen_ptavg".format(rap_bin))

        f.cd()
        f.Cd("/" + rap_bin)

        mgp6_fakes.Divide(mgp6_reco)
        mgp6_miss.Divide(mgp6_gen)

        for i in xrange(1, h_recoptavg.GetNbinsX() + 1):
            pass
            h_recoptavg.SetBinContent(i, h_recoptavg.GetBinContent(i)* (1+ mgp6_fakes.GetBinContent(i)))
            # h_genptavg.SetBinContent(i, h_genptavg.GetBinContent(i)* (1+ mgp6_miss.GetBinContent(i)))
        h_recoptavg.ComputeIntegral()
        # h_genptavg.ComputeIntegral()

        h_recoptavg.Write()
        h_genptavg.Write()
        response_ptavg = ROOT.RooUnfoldResponse(h_recoptavg, h_genptavg, h2_genvsreco)
        h2_response = response_ptavg.Hresponse()
        # h2_response.Write('h2_res_matrix_ptavg')
        h2_genvsreco.Write('h2_res_matrix_ptavg')
        response_ptavg.Write('res_matrix_ptavg')

    f.Close()

if __name__ == '__main__':
    main()

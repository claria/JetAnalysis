#!/usr/bin/env python2
import os
import sys
import array
import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(True)

def main():

    rap_bins = ['ptavg_yb_00_10_ys_00_10',
                'ptavg_yb_00_10_ys_10_20',
                'ptavg_yb_00_10_ys_20_30',
                'ptavg_yb_10_20_ys_00_10',
                'ptavg_yb_10_20_ys_10_20',
                'ptavg_yb_10_20_ys_20_30',
                'ptavg_yb_20_30_ys_00_10',
                'ptavg_yb_20_30_ys_10_20',
                'ptavg_yb_20_30_ys_20_30',
            ]

    pt_binning_gen = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
    pt_binning_reco = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]

    f = ROOT.TFile('lumi_unc_relative.root', 'RECREATE')

    for rap_bin in rap_bins:
        lumi_unc_up = ROOT.TH1D("lumi_unc_up","lumi_unc_up",len(pt_binning_reco)-1, array.array('d', pt_binning_reco))
        lumi_unc_up.Sumw2()
        lumi_unc_dw = ROOT.TH1D("lumi_unc_dw","lumi_unc_dw",len(pt_binning_reco)-1, array.array('d', pt_binning_reco))
        lumi_unc_dw.Sumw2()

        f.mkdir(rap_bin)
        f.Cd("/" + rap_bin)

        for i in xrange(1, lumi_unc_up.GetNbinsX() +1):
            lumi_unc_up.SetBinContent(i, 1.026)
            lumi_unc_up.SetBinError(i, 0.0)
            lumi_unc_dw.SetBinContent(i, 0.974)
            lumi_unc_dw.SetBinError(i, 0.0)

        lumi_unc_up.Write()
        lumi_unc_dw.Write()
    f.Close()

if __name__ == '__main__':
    main()

#!/usr/bin/env python2

import argparse
import array
import ROOT

from JetAnalysis.DijetAna.tools import *

def main():

    parser = argparse.ArgumentParser(description='Unfold distribution with a given response matrix')

    parser.add_argument('--input', required=True, help='Path to root file with the distribution.')

    args = vars(parser.parse_args())

    measured_histo = get_root_object(args['input'], option='READ')

    measured_histo.Print('all')

    print measured_histo.GetNbinsX()

    out_file = ROOT.TFile(args['input'].split('?')[0], 'UPDATE')
    out_file.cd('/')
    if ROOT.gDirectory.GetDirectory('idx') != None:
        print "Folder {0} exists. Will be overwritten".format('idx')
        ROOT.gDirectory.Delete('{0};*'.format('idx'))
    ROOT.gDirectory.mkdir('idx')
    ROOT.gDirectory.cd('idx')

    rap_bins = ['yb0ys0', 'yb0ys1', 'yb0ys2', 'yb1ys0', 'yb1ys1', 'yb2ys0']
    pt_bins = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
    print len(pt_bins)
    pt_bins_arr = array.array('d', pt_bins)

    histos = []
    for rap_bin in rap_bins:
        histo = ROOT.TH1D('h_{0}_ptavg'.format(rap_bin), 'h_{0}_ptavg'.format(rap_bin), len(pt_bins_arr) -1, pt_bins_arr)
        histo.Sumw2()
        histos.append(histo)

    for i in range(measured_histo.GetNbinsX()):
        print i
        pt_idx = int(i % (measured_histo.GetNbinsX()/6.))
        print 'pt_idx', pt_idx
        rap_idx = int(i//(measured_histo.GetNbinsX()/6.))
        print 'rap_idx', rap_idx
        histos[rap_idx].SetBinContent(pt_idx,measured_histo.GetBinContent(i))
        histos[rap_idx].SetBinError(pt_idx,measured_histo.GetBinError(i))

    for histo in histos:
        histo.Write()

    out_file.Close()



if __name__ == '__main__':
    main()

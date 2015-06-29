#! /usr/bin/env python2
import os
import sys
import argparse

import numpy as np
import itertools

import matplotlib
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(True)


from JetAnalysis.DijetAna.plotting.baseplot import BasePlot
from JetAnalysis.DijetAna.plotting.baseplot import GenericPlot
from JetAnalysis.DijetAna.plotting.baseplot import plot_contour1d
from JetAnalysis.DijetAna.plotting.baseplot import ensure_latex

from JetAnalysis.DijetAna.tools import get_root_object
from Artus.HarryPlotter.utility.mplhisto import MplHisto


def main():

    parser = argparse.ArgumentParser(description='xyz')
    parser.add_argument('-i', '--input', required=False, default='DATA.root',
                        help='Path to root file')

    args = vars(parser.parse_args())

    object = get_root_object(args['input'])

    res_matrix = object.Hresponse()

    nbins_x = res_matrix.GetNbinsX()
    nbins_y = res_matrix.GetNbinsY()
    y_sum = []
    for x in range(0, nbins_x +1):
        sumw = 0.
        for y in range(0, nbins_y +1):
            nbin = res_matrix.GetBin(x, y)
            sumw += res_matrix.GetBinContent(nbin)
        y_sum.append(sumw)
        for y in range(0, nbins_y +1):
            nbin = res_matrix.GetBin(x, y)
            if sumw > 0:
                bincontent = res_matrix.GetBinContent(nbin)/sumw
            else:
                bincontent = 0.
            res_matrix.SetBinContent(nbin, bincontent)

    # remove empty bins
    print y_sum
    nonempty_bins = len(np.nonzero(y_sum)[0])

    rebin_histo = ROOT.TH2D('rebin', 'rebin', nonempty_bins, 0, nonempty_bins +1,nonempty_bins, 0, nonempty_bins +1 )
    for i, x in enumerate(np.nonzero(y_sum)[0]):
        for j, y in enumerate(np.nonzero(y_sum)[0]):
            rebin_histo_nbin = rebin_histo.GetBin(i,j)
            res_matrix_nbin = res_matrix.GetBin(int(x),int(y))
            rebin_histo.SetBinContent(rebin_histo_nbin, res_matrix.GetBinContent(res_matrix_nbin))




    plot = GenericPlot(output_fn='plots/resmatrix')
    fig = plot.fig
    ax = plot.ax
    mplhisto = MplHisto(rebin_histo)

    art = plot_contour1d(mplhisto, ax=plot.ax, z_log=True, cmap='Blues')
    ax.set_xlim(min(mplhisto.xbinedges), max(mplhisto.xbinedges))
    ax.set_ylim(min(mplhisto.ybinedges), max(mplhisto.ybinedges))
    fig.colorbar(art)
    plot.finalize()

if __name__ == '__main__':
    main()

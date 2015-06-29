#! /usr/bin/env python2

import os
import sys
import numpy as np

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

import argparse
import matplotlib
import matplotlib.pyplot as plt

from JetAnalysis.DijetAna.plotting.baseplot import BasePlot, ensure_latex
from JetAnalysis.DijetAna.plotting.baseplot import plot_errorbar, plot_band, steppify_bin
from JetAnalysis.DijetAna.tools import *
from Artus.HarryPlotter.utility.mplhisto import MplHisto, MplGraph

from matplotlib.ticker import MultipleLocator, FixedLocator, AutoLocator
from matplotlib.colors import LogNorm


def main():
    parser = argparse.ArgumentParser(description='Plot triplediff')
    parser.add_argument('-i', '--inputfiles', nargs='+', default=['DATA.root:default/h3_jet12rap'],
                        help='Path to root file with the distribution with the syntax file.root:path/to/histo')
    parser.add_argument('--labels', nargs='+', help='Legend labels for each plot')
    parser.add_argument('--colors', nargs='+', help='Colors for each plot')
    parser.add_argument('--scale',  nargs='+', help='Scale each histo with factor')
    parser.add_argument('--x-label', help='xlabel')
    parser.add_argument('--y-label', help='ylabel')
    parser.add_argument('--z-label', help='ylabel')
    parser.add_argument('--output-prefix', help='Output prefix to put before filename')
    parser.add_argument('--ratio-lims', type=float, nargs=2, help='Output prefix to put before filename')

    args = vars(parser.parse_args())
    print args

    histos_3d = [get_root_object(*histo.split(':')) for histo in args['inputfiles']]

    if args['scale']:
        for i, histo in enumerate(histos_3d):
            if args['scale'][i] == 'width':
                print "Scale", histo.GetName(), "with ", args['scale'][i]
                histo.Scale(1.0, args['scale'][i])
            else:
                print "Scale", histo.GetName(), "with ", args['scale'][i]
                histo.Scale(float(args['scale'][i]))



    n_zbins = histos_3d[0].GetNbinsZ()
    print n_zbins
    # slice 3d histogram in 2d histograms
    for i in range(1, n_zbins + 1):
        histos = [get_2dhisto(histo_3d, i) for histo_3d in histos_3d]
        basename = args.get('output_prefix', 'td_ratio')
        plotproducer = TripleDiffRatioPlot(histos, output_fn='plots/{0}_{1}'.format(basename, i), figsize=(14.,10.), 
                                           labels=args['labels'], 
                                           colors=args['colors'],
                                           xlabel=args['x_label'],
                                           ylabel=args['y_label'],
                                           zlabel=args['z_label'],
                                           ratio_lims=args['ratio_lims'])
        plotproducer.do_plot()

def get_2dhisto(histo_3d, zbin):
    histo_3d.GetZaxis().SetRange(zbin, zbin)
    histo_2d = histo_3d.Project3D("yx")
    histo_2d.SetName("{0}_{1}".format(histo_3d.GetZaxis().GetBinLowEdge(zbin), histo_3d.GetZaxis().GetBinUpEdge(zbin)))
    return histo_2d

class TripleDiffRatioPlot(BasePlot):

    def __init__(self, histos, labels=None, colors=None, *args, **kwargs):
        self.ratio_lims = kwargs.pop('ratio_lims')
        self.ptbin = kwargs.pop('ptbin', None)
        self.xlabel = kwargs.pop('xlabel', '')
        self.ylabel = kwargs.pop('ylabel', '')
        self.zlabel = kwargs.pop('zlabel', '')
        self.ptbin = self.ptbin if self.ptbin else histos[0].GetName().split('_')
        # if not self.ratio_lims:
            # self.ratio_lims = [0.51, 1.49]
        super(TripleDiffRatioPlot, self).__init__(*args, **kwargs)
        self.histos = histos

        self.labels = labels if labels else [histo.GetName() for histo in self.histos]
        self.colors = colors if colors else len(histos)*[None]

        self.nbins = self.histos[0].GetNbinsY()
        self.axes = []



    def prepare(self):

        self.fig.text(x=0.07, y=0.5, s='Ratio to {0}'.format(ensure_latex(self.labels[0])), rotation='vertical', ha='right', va='center')

        plt.subplots_adjust(hspace=.1)
        for i in range(1, self.nbins+1):
            ax = self.fig.add_subplot(self.nbins,1,self.nbins+1-i)
            ax.axhline(y=1.0, color='black', lw=1.0, zorder=0)
            ax.yaxis.set_ticks_position('left')
            if i == self.nbins:
                if self.ptbin:
                    ax.text(x=0., y=1.02, s='${0} \leq {2} < {1}$'.format(self.ptbin[0], self.ptbin[1], self.zlabel), ha='left', va='bottom', transform=ax.transAxes)

            if self.ratio_lims:
                ax.set_ylim(*self.ratio_lims)

            # self.yaxis_major_locator   = MultipleLocator(0.1)
            self.yaxis_major_locator   = AutoLocator()
            ax.yaxis.grid(which='major')
            ax.yaxis.set_major_locator(self.yaxis_major_locator)

            if i==1:
                # ax.set_xlabel("$\mathrm{sgn}(y_1)\cdot y_2$")
                ax.set_xlabel(self.xlabel)
            else:
                ax.xaxis.set_ticklabels([])
            self.axes.append(ax)


    def produce(self):

        for i in range(1,self.nbins+1):
            ax = self.axes[i-1]
            ref_histo = None
            for j, histo in enumerate(self.histos):
                hist_slice = histo.ProjectionX("slice_{0}".format(i),i,i)
                if j==0:
                    ref_histo = hist_slice.Clone('ref_histo')
                # if hist_slice.Integral() > 0.:
                    # hist_slice.Scale(ref_histo.Integral()/hist_slice.Integral())
                hist_slice.Divide(ref_histo)
                # hist_slice.Scale(1./ref_histo.Integral())
                hist = MplHisto(hist_slice)
                show_yerr=True if j==0 else False
                if j == len(self.histos) -1:
                    plot_errorbar(hist, ax=ax, show_xerr=True, color=self.colors[j], show_yerr=show_yerr, marker='.', linestyle=None, step=True, label=self.labels[j])
                else:
                    plot_errorbar(hist, ax=ax, show_yerr=show_yerr, color=self.colors[j],  linestyle='-', step=True, label=self.labels[j])

            if (i == self.nbins):
                ax.legend(bbox_to_anchor=(1.00, 1.02), loc='lower right', borderaxespad=0.)
            ax.text(s="${0} \leq {2} < {1}$".format(histo.GetYaxis().GetBinLowEdge(i), histo.GetYaxis().GetBinLowEdge(i+1), self.ylabel),
                    x=0.99, y=0.96, transform=ax.transAxes, ha='right', va='top')
 


    def finalize(self):
        self._save_fig()
        plt.close(self.fig)
        pass


class TripleDiffUncertainties(TripleDiffRatioPlot):


    def prepare(self):
        super(TripleDiffUncertainties, self).prepare()
        yaxis_major_locator   = MultipleLocator(0.1)
        for ax in self.axes:
            ax.yaxis.grid(which='major')
            ax.yaxis.set_major_locator(yaxis_major_locator)

    def produce(self):
        super(TripleDiffUncertainties, self).produce()


if __name__ == '__main__':
    main()



#! /usr/bin/env python2

import os
import sys
import numpy as np

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

import argparse
import matplotlib
import matplotlib.pyplot as plt

from JetAnalysis.DijetAna.plotting.settings import SettingsDict, SettingList, SettingListAction
from JetAnalysis.DijetAna.plotting.baseplot import BasePlot, ensure_latex
from JetAnalysis.DijetAna.plotting.baseplot import plot_errorbar, plot_band, steppify_bin
from JetAnalysis.DijetAna.tools import *
from Artus.HarryPlotter.utility.mplhisto import MplHisto, MplGraph

from matplotlib.ticker import MultipleLocator, FixedLocator, AutoLocator
from matplotlib.colors import LogNorm


def main():

    parser = argparse.ArgumentParser(description='Plot triplediff')
    parser.add_argument('-i', '--input', nargs='+', default=None, action=SettingListAction, help='Path to root file with the distribution with the syntax file.root:path/to/histo')
    parser.add_argument('--labels', nargs='+', default='__nolegend__', action=SettingListAction, help='Legend labels for each plot')
    parser.add_argument('--colors', nargs='+', default=[], help='Colors for each plot')
    parser.add_argument('--scale',  nargs='+', default=[], action=SettingListAction, help='Scale each histo with factor')
    parser.add_argument('--output-prefix', help='Output prefix to put before filename')

    parser.add_argument('--x-lims', nargs=2, default=['none', 'none'], help='X limits of plot.')
    parser.add_argument('--y-lims', nargs=2, default=['none', 'none'], help='Y limits of plot.')

    parser.add_argument('--x-errs', action='store_true', help='Show x-errors.')
    parser.add_argument('--y-errs', action='store_true', help='Show y-errors.')

    parser.add_argument('--x-log', action='store_true', help='Show x-errors.')
    parser.add_argument('--y-log', action='store_true', help='Show y-errors.')

    parser.add_argument('--x-label', default='', help='xlabel')
    parser.add_argument('--y-label', default='', help='ylabel')

    args = vars(parser.parse_args())

    args['y_lims'] = [float(v) if (not v.lower() == 'none') else None for v in args['y_lims'] ]
    args['x_lims'] = [float(v) if (not v.lower() == 'none') else None for v in args['x_lims'] ]

    histos = [get_root_object(*histo.split(':')) for histo in args['input']]

    # ref_histo_range = histos[0].GetBinCenter(1), histos[0].GetBinCenter(histos[0].GetNbinsX())
    # print ref_histo_range
    # for histo in histos:
    #     histo.GetXaxis().SetRange(histo.FindBin(ref_histo_range[0]), histo.FindBin(ref_histo_range[1]))

    if any(i is not None for i in args['scale']):
        for i, histo in enumerate(histos):
            if args['scale'][i] == 'width':
                print "Scale", histo.GetName(), "with ", args['scale'][i]
                histo.Scale(1.0, args['scale'][i])
            else:
                print "Scale", histo.GetName(), "with ", args['scale'][i]
                histo.Scale(float(args['scale'][i]))


    ref_histo = histos[0].Clone('ref_histo')
    for histo in histos:
        histo = get_ratio(histo, ref_histo)


    plot = RatioPlot(histos, **args)
    plot.do_plot()

def get_ratio(histo, ref_histo):
    return histo.Divide(ref_histo)


class RatioPlot(BasePlot):

    def __init__(self, histos,  *args, **kwargs):

        super(RatioPlot, self).__init__(*args, **kwargs)
        self.ax = self.fig.add_subplot(111)
        self.histos = histos

        self.x_lims = kwargs.pop('x_lims', (None, None))
        self.y_lims = kwargs.pop('y_lims', (None, None))
        print self.x_lims
        print self.y_lims

        self.x_log = kwargs.pop('x_log', False)
        self.y_log = kwargs.pop('y_log', False)

        self.labels = kwargs.pop('labels')

        self.x_label = kwargs.pop('x_label', '')
        self.y_label = kwargs.pop('y_label', '')

    def prepare(self):

        pass


    def produce(self):

        for i, histo in enumerate(self.histos):
            mpl_histo = MplHisto(histo)
            label = self.labels[i]
            plot_errorbar(mpl_histo, ax=self.ax, show_xerr=True, label=label, show_yerr=True, marker='.', linestyle=None, step=True)

        self.ax.set_ylim(ymin=self.y_lims[0], ymax=self.y_lims[1])
        self.ax.set_xlim(xmin=self.x_lims[0], xmax=self.x_lims[1])

        self.ax.set_xlabel(self.x_label, position=(1., 0.), va='top', ha='right')
        self.ax.set_ylabel(self.y_label)

        self.ax.set_xscale('log' if self.x_log else 'linear')
        self.ax.set_yscale('log' if self.y_log else 'linear')

        self.ax.legend()


    def finalize(self):
        self._save_fig()
        plt.close(self.fig)
        pass



if __name__ == '__main__':
    main()

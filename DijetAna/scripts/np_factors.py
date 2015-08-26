#! /usr/bin/env python2
import os
import sys
import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(True)

import numpy as np
import itertools

from JetAnalysis.DijetAna.plotting.baseplot import BasePlot
from Artus.HarryPlotter.utility.mplhisto import MplHisto
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

from JetAnalysis.DijetAna.tools import get_root_object

def main():

    parser = argparse.ArgumentParser(description='xyz')
    parser.add_argument('--input-mpihad', required=True, nargs='+', help='Path to root file with the distribution')
    parser.add_argument('--input-nompinohad', required=True, nargs='+', help='Path to root file with the distribution')

    args = vars(parser.parse_args())

    rap_bins = ['genptavg_yb_00_10_ys_00_10',
                'genptavg_yb_00_10_ys_10_20',
                'genptavg_yb_00_10_ys_20_30',
                'genptavg_yb_10_20_ys_00_10',
                'genptavg_yb_10_20_ys_10_20',
                'genptavg_yb_10_20_ys_20_30',
                'genptavg_yb_20_30_ys_00_10',
                'genptavg_yb_20_30_ys_10_20',
                'genptavg_yb_20_30_ys_20_30',
            ]

    f = ROOT.TFile('np_factors.root', 'RECREATE')
    for rap_bin in rap_bins:
        f.cd('/')
        f.mkdir(rap_bin)
        f.cd("/" + rap_bin)
        np_histos = []
        print rap_bin
        for i in range(len(args['input_mpihad'])):
            histo =fit_np(args['input_mpihad'][i], args['input_nompinohad'][i], rap_bin)
            # Save function as histo to file.
            fcn = histo.GetFunction("np_fcn")
            if fcn != None:
                output_histo = histo.Clone("np_histo")
                output_histo.Reset()
                for i in xrange(1, output_histo.GetNbinsX() +1):
                    output_histo.SetBinContent(i, fcn.Eval(output_histo.GetBinCenter(i)))
                    output_histo.SetBinError(i, 0.)
            np_histos.append(histo)

        npplot = NPPlot(np_histos, output_fn='plots/np_{0}'.format(rap_bin))
        npplot.do_plot()
        f.cd()
        f.cd("/" + rap_bin)
        output_histo.Write()
    f.Close()




def fit_np(input_mpihad, input_nompinohad, rap_bin):

    ROOT.TVirtualFitter.SetMaxIterations(9999)

    np_histos = []

    histo_mpihad = get_root_object(input_mpihad, '{0}/h_genptavg'.format(rap_bin))
    histo_nompinohad = get_root_object(input_nompinohad, '{0}/h_genptavg'.format(rap_bin))


    histo_mpihad.Divide(histo_nompinohad)

    np_fcn = ROOT.TF1("np_fcn", "[0] + [1]/x**[2]")
    np_fcn.SetParameters(1., 1.0, 1.0)

    res = histo_mpihad.Fit("np_fcn", "I", "")

    print "a0:", np_fcn.GetParameter(0)
    print "a1:", np_fcn.GetParameter(1)
    print "a2:", np_fcn.GetParameter(2)

    return histo_mpihad

class NPPlot(BasePlot):

    def __init__(self, histos, *args, **kwargs):
        super(NPPlot, self).__init__(*args, **kwargs)

        self.histos = histos
        self.ax = self.fig.add_subplot(111)

        self.markers = itertools.cycle(('^', 'x', '+', '.', 'o', '*')) 
        self.lines = itertools.cycle(["-","--","-.",":"])

    def prepare(self):
        pass

    def produce(self):

        color_cycle = self.ax._get_lines.color_cycle
        for histo in self.histos:
            print histo.GetName()
            label = histo.GetName()
            # label = r"${0} \leq \vert y_s \vert < {1}$ ${2} \leq \vert y_b \vert < {3}$".format(float(histo.GetName().split('_')[2])/10.,
            #                                                                                     float(histo.GetName().split('_')[3])/10.,
            #                                                                                     float(histo.GetName().split('_')[5])/10.,
            #                                                                                     float(histo.GetName().split('_')[6])/10.)
            color = next(color_cycle)
            mplhisto = MplHisto(histo)
            fcn = histo.GetFunction("np_fcn")
            self.ax.errorbar(mplhisto.x, mplhisto.bincontents, xerr=mplhisto.xerr, color=color,
                             yerr=mplhisto.binerr, fmt=self.markers.next(), capthick=0., lw=1.0, label=label)

            if fcn != None:
                fcn_x = np.arange(50, 3000., 1.)
                fcn_y = np.array(map(fcn,fcn_x))

                self.ax.plot(fcn_x, fcn_y, color=color, lw=1.0, linestyle=self.lines.next())



    def finalize(self):
        self.ax.set_xlabel('Gen $p_\mathrm{{T,avg}}$ (GeV)')
        self.ax.set_ylim(0.8, 1.5)
        self.ax.set_xlim(70., 3000.)
        self.ax.set_xscale('log')
        self.ax.xaxis.set_minor_formatter(plt.FuncFormatter(self.log_locator_filter))
        xfmt = ScalarFormatter()
        xfmt.set_powerlimits((-9, 9))
        self.ax.xaxis.set_major_formatter(xfmt)
        self.ax.legend(prop={'size':14})

        self._save_fig()
        plt.close(self.fig)
        pass


if __name__ == '__main__':
    main()

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
from Artus.HarryPlotter.utility.mplhisto import MplGraph
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

from JetAnalysis.DijetAna.tools import get_root_object

def main():

    parser = argparse.ArgumentParser(description='xyz')
    parser.add_argument('--input-file', required=True,
                        help='Path to root file with the distribution')

    args = vars(parser.parse_args())

    res_graphs = fit_resolution(args['input_file'])

    resolutionplot = JetResolutionPlot(res_graphs, output_fn='plots/ptresolution')
    resolutionplot.do_plot()



def fit_resolution(inputfile):

    # rap_bins = ['00_05','05_10','10_15','15_20','20_25','25_30']
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
    ROOT.TVirtualFitter.SetMaxIterations(9999)
    # rootfile = ROOT.TFile("res_results.root", "RECREATE")
    # rootfile.cd('/')
    # if rootfile.GetDirectory('res') != None:
        # rootfile.Delete('res;*')
    # rootfile.mkdir('res')
    # rootfile.cd('res')

    # resolution_pars = {'N': [],'S': [],'C': [],}

    maxptfitlims = {'00_05' : 1200.,
                    '05_10': 1200.,
                    '10_15': 1000.,
                    '15_20': 900.,
                    '20_25': 800.,
                    '25_30': 400.}
    resolution_graphs = []
    for rap_bin in rap_bins:
        print rap_bin
        histo = get_root_object(inputfile, '{0}/h2GenVsRecoPtAvg'.format(rap_bin))
        n_ptbins = histo.GetNbinsY()

        # pt_bins = [histo.GetYaxis().GetBinCenter(i) for i in range(1, n_ptbins + 1)]
        # pt_bins_width = [histo.GetYaxis().GetBinWidth(i) for i in range(1, n_ptbins + 1)]

        graph = ROOT.TGraphErrors(n_ptbins)
        # ROOT.SetOwnership(graph, 0)

        for i in range(1, n_ptbins + 1):
            # if histo.GetYaxis().GetBinCenter(i) > max_lims[rap_bin]:
                # continue
            hslice = histo.ProjectionX("ptslice_{0}".format(i),i,i)
            if hslice.GetEntries() < 1:
                continue
            res = hslice.Fit("gaus", "SQ")
            if res.Get() != None and res.Status() == 0:
                fcn = hslice.GetFunction("gaus")
                # print "relerror", (fcn.GetParError(2)/fcn.GetParameter(2))
                if (fcn.GetParError(2)/fcn.GetParameter(2)) > 0.05:
                    continue
                graph.SetPoint(i, histo.GetYaxis().GetBinCenter(i), fcn.GetParameter(2))
                graph.SetPointError(i, histo.GetYaxis().GetBinWidth(i)/2., fcn.GetParError(2))
            else:
                continue

        print graph.GetN()
        # res_fcn = ROOT.TF1("res_fcn", "sqrt(TMath::Sign(1.,[0])*(([0]/x)**2) + (([1]**2)/x)*(x**[2]) + [3]**2)")
        res_fcn = ROOT.TF1("res_fcn", "sqrt(([0]/x)**2 + (([1]**2)/x) + [2]**2)")
        # res_fcn.SetParameters(6., 1., 0.1, 0.1)
        # res_fcn.SetParameters(10., 0.1, 0.1, 0.0)
        res_fcn.SetParameters(6., 0.5, 0.01)
        # res_fcn.SetRange(0.,maxptfitlims.get(rap_bin,999999.))
        res_fcn.SetRange(0., 999999.)
        # graph.Fit("res_fcn", "", "", 60., maxptfitlims[rap_bin])
        res = graph.Fit("res_fcn", "RS", "")
        print res.Get()
        print res.Status()
        if res.Get() == None or res.Status() != 0:
            continue
        # resolution_pars['N'].append(res_fcn.GetParameter(1))
        # resolution_pars['S'].append(res_fcn.GetParameter(2))
        # resolution_pars['C'].append(res_fcn.GetParameter(3))


        # rootfile.cd()
        graph.SetName("{0}".format(rap_bin))
        resolution_graphs.append(graph)
        # graph.Write()
    for graph in resolution_graphs:
        fcn = graph.GetFunction("res_fcn")
        print "\'{0}\' : [{1}, {2}, {3}, {4}]".format(graph.GetName(), fcn.GetParameter(0), fcn.GetParameter(1), fcn.GetParameter(2), fcn.GetParameter(3))
        # print "N:", fcn.GetParameter(0)
        # print "S:", fcn.GetParameter(1)
        # print "m:", fcn.GetParameter(2)
        # print "C:", fcn.GetParameter(3)

    return resolution_graphs

class JetResolutionPlot(BasePlot):

    def __init__(self, graphs, *args, **kwargs):
        super(JetResolutionPlot, self).__init__(*args, **kwargs)

        self.graphs = graphs
        self.ax = self.fig.add_subplot(111)

        self.markers = itertools.cycle(('^', 'x', '+', '.', 'o', '*')) 
        self.lines = itertools.cycle(["-","--","-.",":"])

    def prepare(self):
        pass

    def produce(self):

        color_cycle = self.ax._get_lines.color_cycle
        for graph in self.graphs:
            label = r"${0} \leq \vert y_s \vert < {1}$ ${2} \leq \vert y_b \vert < {3}$".format(float(graph.GetName().split('_')[2])/10.,
                                                                                                float(graph.GetName().split('_')[3])/10.,
                                                                                                float(graph.GetName().split('_')[5])/10.,
                                                                                                float(graph.GetName().split('_')[6])/10.)
            color = next(color_cycle)
            mplgraph = MplGraph(graph)
            fcn = graph.GetFunction("res_fcn")
            print fcn
            self.ax.errorbar(mplgraph.x, mplgraph.y, xerr=mplgraph.xerr, color=color,
                             yerr=mplgraph.yerr, fmt=self.markers.next(), capthick=0., lw=1.0, label=label)

            fcn_x = np.arange(50, 3000., 1.)
            fcn_y = np.array(map(fcn,fcn_x))

            self.ax.plot(fcn_x, fcn_y, color=color, lw=1.0, linestyle=self.lines.next())



    def finalize(self):
        self.ax.set_xlabel('Gen $p_\mathrm{{T,avg}}$ (GeV)')
        self.ax.set_ylabel(r'$\langle p_\mathrm{{T,avg,reco}} / p_\mathrm{{T,avg,gen}} \rangle$')
        self.ax.set_ylim(0.0, 0.25)
        self.ax.set_xlim(50, 3000.)
        self.ax.set_xscale('log')
        self.ax.xaxis.set_minor_formatter(plt.FuncFormatter(self.log_locator_filter))
        xfmt = ScalarFormatter()
        xfmt.set_powerlimits((-9, 9))
        self.ax.xaxis.set_major_formatter(xfmt)
        self.ax.legend(prop={'size':10})

        self._save_fig()
        plt.close(self.fig)
        pass


if __name__ == '__main__':
    main()

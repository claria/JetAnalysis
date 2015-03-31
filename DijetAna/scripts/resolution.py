#! /usr/bin/env python2
import os
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

    rap_bins = ['00_05','05_10','10_15','15_20','20_25','25_30']

    # rootfile = ROOT.TFile("res_results.root", "RECREATE")
    # rootfile.cd('/')
    # if rootfile.GetDirectory('res') != None:
        # rootfile.Delete('res;*')
    # rootfile.mkdir('res')
    # rootfile.cd('res')

    # resolution_pars = {'N': [],'S': [],'C': [],}

    maxptfitlims = {'00_05' : 3000.,
                    '05_10': 1500.,
                    '10_15': 1000.,
                    '15_20': 900.,
                    '20_25': 800.,
                    '25_30': 400.}
    resolution_graphs = []
    for rap_bin in rap_bins:
        histo = get_root_object(inputfile, '{0}/h2GenVsRecoPt'.format(rap_bin))
        n_ptbins = histo.GetNbinsY()

        # pt_bins = [histo.GetYaxis().GetBinCenter(i) for i in range(1, n_ptbins + 1)]
        # pt_bins_width = [histo.GetYaxis().GetBinWidth(i) for i in range(1, n_ptbins + 1)]

        graph = ROOT.TGraphErrors(n_ptbins)
        # ROOT.SetOwnership(graph, 0)

        for i in range(1, n_ptbins + 1):
            # if histo.GetYaxis().GetBinCenter(i) > max_lims[rap_bin]:
                # continue
            hslice = histo.ProjectionX("ptslice_{0}".format(i),i,i)
            res = hslice.Fit("gaus", "S")
            if res.Get() != None and res.Status() == 0:
                fcn = hslice.GetFunction("gaus")
                # print "relerror", (fcn.GetParError(2)/fcn.GetParameter(2))
                if (fcn.GetParError(2)/fcn.GetParameter(2)) > 0.1:
                    continue
                graph.SetPoint(i, histo.GetYaxis().GetBinCenter(i), fcn.GetParameter(2))
                graph.SetPointError(i, histo.GetYaxis().GetBinWidth(i)/2., fcn.GetParError(2))

        res_fcn = ROOT.TF1("res_fcn", "sqrt(TMath::Sign(1,[0])*(([0]/x)**2) + (([1]**2)/x)*(x**[2]) + [3]**2)")
        # res_fcn = ROOT.TF1("res_fcn", "sqrt(([0]/x)**2 + (([1]**2)/x) + [2]**2)")
        # res_fcn.SetParameters(6., 1., 0., 0.)
        res_fcn.SetParameters(10., 0.1, 0.1, 0.0)
        # graph.Fit("res_fcn", "", "", 60., maxptfitlims[rap_bin])
        graph.Fit("res_fcn", "", "")
        # resolution_pars['N'].append(res_fcn.GetParameter(1))
        # resolution_pars['S'].append(res_fcn.GetParameter(2))
        # resolution_pars['C'].append(res_fcn.GetParameter(3))


        # rootfile.cd()
        graph.SetName("{0}".format(rap_bin))
        resolution_graphs.append(graph)
        # graph.Write()
    for graph in resolution_graphs:
        print graph.GetName()
        fcn = graph.GetFunction("res_fcn")
        print "N:", fcn.GetParameter(0)
        print "S:", fcn.GetParameter(1)
        print "m:", fcn.GetParameter(2)
        print "C:", fcn.GetParameter(3)

    return resolution_graphs

class JetResolutionPlot(BasePlot):

    def __init__(self, graphs, *args, **kwargs):
        super(JetResolutionPlot, self).__init__(*args, **kwargs)

        self.graphs = graphs
        self.ax = self.fig.add_subplot(111)

        self.markers = itertools.cycle(('^', 'x', '+', '.', 'o', '*')) 

    def prepare(self):
        pass

    def produce(self):

        color_cycle = self.ax._get_lines.color_cycle
        for graph in self.graphs:
            label = r"${0} \leq \vert y \vert < {1}$".format(float(graph.GetName().split('_')[0])/10.,float(graph.GetName().split('_')[1])/10.)
            color = next(color_cycle)
            mplgraph = MplGraph(graph)
            self.ax.errorbar(mplgraph.x, mplgraph.y, xerr=mplgraph.xerr, color=color,
                             yerr=mplgraph.yerr, fmt=self.markers.next(), capthick=0., lw=1.0, label=label)
            fcn = graph.GetFunction("res_fcn")
            fcn_x = np.arange(50, 3000., 1.)
            fcn_y = np.array(map(fcn,fcn_x))
            self.ax.plot(fcn_x, fcn_y, color=color, lw=1.0)

        self.ax.legend()


    def finalize(self):
        self.ax.set_xlabel('Gen jet $p_\mathrm{{T}}$ (GeV)')
        self.ax.set_ylabel(r'$\langle p_\mathrm{{T,reco}} / p_\mathrm{{T,gen}} \rangle$')
        self.ax.set_ylim(0.0, 0.25)
        self.ax.set_xlim(50, 3000.)
        self.ax.set_xscale('log')
        self.ax.xaxis.set_minor_formatter(plt.FuncFormatter(self.log_locator_filter))

        self._save_fig()
        plt.close(self.fig)
        pass


if __name__ == '__main__':
    main()

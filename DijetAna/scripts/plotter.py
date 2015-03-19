#! /usr/bin/env python2
import os
import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from JetAnalysis.DijetAna.plotting.baseplot import BasePlot
from JetAnalysis.DijetAna.plotting.baseplot import plot_errorbar, steppify_bin
from JetAnalysis.DijetAna.tools import *
from Artus.HarryPlotter.utility.mplhisto import MplHisto, MplGraph
# from Artus.HarryPlotter.utility.roottools import RootTools
# import Artus.HarryPlotter.plot_modules.plotmpl as mplplot

import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

from matplotlib.ticker import MultipleLocator, FixedLocator
from matplotlib.colors import LogNorm
import numpy as np


def main():

    parser = argparse.ArgumentParser(description='Unfold distribution with a given response matrix')
    parser.add_argument('--inputfiles', nargs='+', default=['DATA.root:default/h3_jet12rap',],
                        help='Path to root file with the distribution with the syntax file.root:path/to/histo')
    args = vars(parser.parse_args())

    histos = []
    for inputhisto in args['inputfiles']:
        histos.append(get_root_object(*inputhisto.split(':')))

    for i, histo in enumerate(histos):
        histo.SetName("{0}_{1}".format(histo.GetName(), i))

    n_zbins = histos[0].GetNbinsZ()

    # slice 3d histogram in 2d histograms
    for i in range(1, n_zbins+1):
        print "zbin " , i
        histos_sliced = []
        for histo in histos:
            print histo.GetName()
            histo.GetZaxis().SetRange(i,i)
            xyslice = histo.Project3D("xy")
            # normalize_to_binwidth(xyslice)
            histos_sliced.append(xyslice)

        # histos_sliced = [histo.ProjectionZ("slice_{0}".format(i), i, i) for histo in histos]
        plotproducer = TripleDiffRatioPlot(histos_sliced, output_fn='plots/tdratio_{0}'.format(i))
        plotproducer.do_plot()
        # tddistplot = TripleDiffHeatMapPlot(histos_sliced[0], output_fn='plots/tdhmplot_{0}'.format(i))
        # tddistplot.do_plot()

        # tddistplot = TripleDiff3DPlot(histos_sliced[0], output_fn='td3dplot_{0}'.format(i))
        # tddistplot.do_plot()

def get_root_object(filename, objectpath):
    rootfile = ROOT.TFile(filename, "READ")
    ROOT.SetOwnership(rootfile, 0)
    obj = rootfile.Get(objectpath)
    ROOT.SetOwnership(obj, 0)
    if obj == None:
        raise Exception("Requested object {0} not found in rootfile {1}.".format(objectpath, filename))
    return obj


class TripleDiff3DPlot(BasePlot):

    def __init__(self, histo, *args, **kwargs):
        super(TripleDiff3DPlot, self).__init__(*args, **kwargs)

        self.histo = histo

    def prepare(self):
        pass

    def produce(self):

        ax = self.fig.add_subplot(111, projection='3d')
        hist = MplHisto(self.histo)

        print hist.bincontents
        x_data, y_data = np.meshgrid( np.arange(hist.bincontents.shape[1]),
                                      np.arange(hist.bincontents.shape[0]) )
        x_data = x_data.flatten()
        y_data = y_data.flatten()
        z_data = hist.bincontents.flatten()
        ax.bar3d(x_data,
                 y_data,
                 np.zeros(len(z_data)),
                 1, 1, z_data )
        plt.show()


    def finalize(self):
        self._save_fig()
        plt.close(self.fig)
        pass

class TripleDiffHeatMapPlot(BasePlot):

    def __init__(self, histo, *args, **kwargs):
        super(TripleDiffHeatMapPlot, self).__init__(*args, **kwargs)

        self.histo = histo

    def prepare(self):
        pass

    def produce(self):

        ax = self.fig.add_subplot(111)
        hist = MplHisto(self.histo)
        cmap = plt.cm.get_cmap('jet')
        cmap.set_bad('black', alpha=None)
        Z = np.ma.masked_array(hist.bincontents)
        Z[Z <=0] = np.ma.masked
        artist = ax.pcolormesh(hist.xbinedges, hist.ybinedges, Z,
                               cmap=cmap,
                               norm =LogNorm(vmin=Z.min(), vmax=Z.max()))

        cbar = self.fig.colorbar(artist)
        cbar.set_label('Events')

        ax.set_xlabel('Leading jet rapidity')
        ax.set_ylabel('Second jet rapidity')


    def finalize(self):
        self._save_fig()
        plt.close(self.fig)
        pass


class TripleDiffRatioPlot(BasePlot):

    def __init__(self, histos, *args, **kwargs):
        super(TripleDiffRatioPlot, self).__init__(*args, **kwargs)
        self.histos = histos
        # for i in range(1, len(histos)):
            # histos[i].Scale(histos[i].GetEntries()/histos[0].GetEntries())
        self.nbins = self.histos[0].GetNbinsX()
        # self.ybinedges = 

        # self.yaxis_major_locator   = MultipleLocator(0.25)
        self.yaxis_major_locator   = FixedLocator([0.75, 1.25])


    def prepare(self):
        pass


    def produce(self):

        self.fig.text(x=-0.3, y=0.5, s='Ratio to Data', rotation='vertical', va='center')
        labels = ['Data', 'Pythia 8']
        # for histo in self.histos:
            # histo.Scale(1./histo.Integral())

        for i in range(1,self.nbins + 1):
            ax = self.fig.add_subplot(12,1,12+1-i)
            ax.axhline(y=1.0, color='black', lw=1.0, zorder=0)
            ax.spines['top'].set_visible(False)
            ax.spines['bottom'].set_visible(False)
            ax.spines['left'].set_visible(True)
            ax.spines['right'].set_visible(False)
            ax.yaxis.set_ticks_position('left')
            ref_histo = None
            if (i==self.nbins +1):
                ax.set_xlabel('Jet 1 Rapidity')
            for j, histo in enumerate(self.histos):
                hist_slice = histo.ProjectionX("slice_{0}".format(i),i,i)
                if j==0:
                    ref_histo = hist_slice.Clone('ref_histo')
                # if hist_slice.Integral() > 0.:
                    # hist_slice.Scale(ref_histo.Integral()/hist_slice.Integral())
                hist_slice.Divide(ref_histo)
                hist = MplHisto(hist_slice)
                plot_errorbar(hist, label=labels[j])

            ax.set_ylabel("${0} \leq y_2 < {1}$".format(histo.GetYaxis().GetBinLowEdge(i), histo.GetYaxis().GetBinLowEdge(i+1)),
                          rotation='horizontal')
            ax.set_ylim(0.51, 1.49)
            ax.yaxis.set_major_locator(self.yaxis_major_locator)
            ax.xaxis.set_visible(False)

            if i==12:
                ax.legend(bbox_to_anchor=(1.00, 1.02), loc='lower right', borderaxespad=0.)
            if i==1:
                ax.xaxis.set_visible(True)
                ax.set_xlabel("Leading jet rapidity $y_1$")

        plt.subplots_adjust(hspace=.200)


    def finalize(self):
        self._save_fig()
        plt.close(self.fig)
        pass


if __name__ == '__main__':
    main()



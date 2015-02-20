#! /usr/bin/env python2
import os
import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from JetAnalysis.DijetAna.plotting.baseplot import BasePlot
from JetAnalysis.DijetAna.plotting.baseplot import plot_errorbar, steppify_bin
from Artus.HarryPlotter.utility.mplhisto import MplHisto, MplGraph
from Artus.HarryPlotter.utility.roottools import RootTools
import Artus.HarryPlotter.plot_modules.plotmpl as mplplot

import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

from matplotlib.ticker import MultipleLocator, FixedLocator
import numpy as np


def main():


    parser = argparse.ArgumentParser(description='Unfold distribution with a given response matrix')
    parser.add_argument('--inputfiles', nargs='+', required=True,
                        help='Path to root file with the distribution with the syntax file.root:path/to/histo')

    args = vars(parser.parse_args())

    print args

    # inputfiles = ['DATA.root', 'unfolded.root']
    # objectpaths = ['default/h3_jet12rap', 'default/unf_hjet12rap']

    histos = []
    for inputhisto in args['inputfiles']:
        histos.append(get_root_object(*inputhisto.split(':')))

    # rootfiles = [ROOT.TFile(inputfile, "READ") for inputfile in inputfiles]
    # histos = [rootfile.Get('default/h3_jet12rap') for rootfile in rootfiles]
    # histos = [get_root_object(filename, objectpath) for filename, objectpath in zip(inputfiles, objectpaths)]
    for i, histo in enumerate(histos):
        histo.SetName("{0}_{1}".format(histo.GetName(), i))
    n_zbins = histos[0].GetNbinsZ()

    for i in range(1, n_zbins+1):
        print "zbin " , i
        histos_sliced = []
        for histo in histos:
            print histo.GetName()
            histo.GetZaxis().SetRange(i,i)
            xyslice = histo.Project3D("xy")
            histos_sliced.append(xyslice)

        # histos_sliced = [histo.ProjectionZ("slice_{0}".format(i), i, i) for histo in histos]
        plotproducer = TripleDiffRatioPlot(histos_sliced, output_fn='tdratio_{0}'.format(i))
        plotproducer.do_plot()
        tddistplot = TripleDiff3DPlot(histos_sliced[0], output_fn='td3dplot_{0}'.format(i))
        tddistplot.do_plot()



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
        labels = ['Data', 'P8', 'QCDMGP6']
        for i in range(1,self.nbins + 1):
            ax = self.fig.add_subplot(12,1,i)
            ax.axhline(y=1.0, color='black', lw=1.0, zorder=0)
            ax.spines['top'].set_visible(False)
            ax.spines['bottom'].set_visible(False)
            ax.spines['left'].set_visible(True)
            ax.spines['right'].set_visible(False)
            ax.yaxis.set_ticks_position('left')
            ref_histo = None
            if (i==self.nbins +1):
                ax.set_xlabel('Jet 1 Rapidity')
            ax.set_ylabel(i)
            for j, histo in enumerate(self.histos):
                hist_slice = histo.ProjectionY("slice_{0}".format(i),i,i)
                if j==0:
                    ref_histo = hist_slice.Clone('xyz')
                hist_slice.Scale(ref_histo.Integral()/hist_slice.Integral())
                hist_slice.Divide(ref_histo)
                hist = MplHisto(hist_slice)
                plot_errorbar(hist, label=labels[j])

            ax.set_ylim(0.51, 1.49)
            ax.yaxis.set_major_locator(self.yaxis_major_locator)
            ax.xaxis.set_visible(False)

            if i==1:
                ax.legend(bbox_to_anchor=(1.02, 1.0), loc='upper left', borderaxespad=0.)
            if i==12:
                ax.xaxis.set_visible(True)
                ax.set_xlabel("Leading jet rapidity")

        plt.subplots_adjust(hspace=.100)


    def finalize(self):
        self._save_fig()
        plt.close(self.fig)
        pass


if __name__ == '__main__':
    main()



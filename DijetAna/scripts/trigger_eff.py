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
from JetAnalysis.DijetAna.plotting.baseplot import plot_errorbar
from JetAnalysis.DijetAna.plotting.baseplot import ensure_latex

from JetAnalysis.DijetAna.tools import get_root_object
from Artus.HarryPlotter.utility.mplhisto import MplGraph


def main():

    parser = argparse.ArgumentParser(description='xyz')
    parser.add_argument('--input-file', required=False, default='DATA.root',
                        help='Path to root file')

    args = vars(parser.parse_args())
    folder_name = 'default/TriggerEffs'
    trigger_paths = ['HLT_PFJET40', 'HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260', 'HLT_PFJET320']

    for i in range(0, len(trigger_paths) - 1):
        trg_histo = get_root_object(args['input_file'], "{}/{}".format(folder_name, trigger_paths[i]))
        emul_trg_histo = get_root_object(args['input_file'], "{}/emul_{}".format(folder_name, trigger_paths[i+1]))

        eff = get_efficiency(emul_trg_histo, trg_histo)
        eff.SetName(emul_trg_histo.GetName().replace('emul_', ''))

        res_fcn = ROOT.TF1("res_fcn", "(1./2.)*(1 + TMath::Erf((x-[0])/(sqrt(2)*[1])))")
    
        #x = ROOT.Double(0)
        #y = ROOT.Double(0)
        #for i in range(eff.GetN()-1, 0, -1):
        #    eff.GetPoint(i,x,y)
        #    if y < 0.5:
        #        break

        threshold = float(eff.GetName().split('_')[1].replace('PFJET',''))
        res_fcn.SetParameters(threshold, 20.0, 1.)
        res = eff.Fit("res_fcn", "EX0", "")

        xmin = res_fcn.GetX(0.5)
        res_fcn.SetRange(xmin, 1000)
        res = eff.Fit("res_fcn", "rEX0", "")
        xmin = res_fcn.GetParameter(0)
        xmin = res_fcn.GetX(0.7)
        res_fcn.SetRange(xmin, 1000)
        res = eff.Fit("res_fcn", "rEX0", "")

        x99 = res_fcn.GetX(0.99)

        if res.Get() != None and res.Status() == 0:
            pass

        plot = GenericPlot(output_fn='eff_{0}'.format(eff.GetName()))
        fig = plot.fig
        ax = plot.ax
        mplgraph = MplGraph(eff)
        plot_errorbar(mplgraph, lw=1.0, fmt='o', markersize=5, ax=plot.ax, color='black')


        fcn = eff.GetFunction("res_fcn")
        fcn_x = np.arange(0.0, 1000., 10.)
        fcn_y = np.array(map(fcn,fcn_x))
        ax.axvline(x=x99, color='blue')
        ax.plot(fcn_x, fcn_y, color='red', lw=1.0)


        ax.text(s=ensure_latex(eff.GetName()), transform=ax.transAxes, color='Black', 
                x=0.02, y=0.98, va='top', ha='left')

        ax.text(s="$x_{{99}} = {0:.2f}$ GeV".format(x99),transform=ax.transAxes, color='Black', 
                x=0.02, y=0.90, va='top', ha='left')

        ax.set_ylim(0.5, 1.05)
        ax.set_xlim(xmin=80)

        ax.set_ylabel('Efficiency')
        ax.set_xlabel('Jet $p_\mathrm{T}$ (GeV)')
        ax.set_xscale('log')
        plot.finalize()


def get_efficiency(pass_histo, total_histo, option='cp'):
    """Returns TGraph with erros of efficiency calculated using 
       clopper-pearson algorithm.
    """
    return ROOT.TGraphAsymmErrors(pass_histo, total_histo, option)


if __name__ == '__main__':
    main()

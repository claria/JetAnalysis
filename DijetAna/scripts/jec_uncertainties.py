#! /usr/bin/env python2
import os
import argparse
import re

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(True)

import numpy as np
import itertools

from JetAnalysis.DijetAna.plotting.baseplot import BasePlot
from Artus.HarryPlotter.utility.mplhisto import MplGraph
import matplotlib
import matplotlib.pyplot as plt


from JetAnalysis.DijetAna.tools import *

def main():

    parser = argparse.ArgumentParser(description='xyz')
    parser.add_argument('--rootfile', required=True,
                        help='Path to root file with the distribution')
    parser.add_argument('--central-pipeline', default='default',
                        help='Path to root file with the distribution')
    parser.add_argument('--sources-list', nargs='*',
                        help='Sources list')
    parser.add_argument('--histogram', required=True,
                        help='Distribution')



    args = vars(parser.parse_args())

    root_file = ROOT.TFile(args['rootfile'], 'UPDATE')
    directory = root_file.GetDirectory('/')

    jec_sources = []

    for key in directory.GetListOfKeys():
        if isinstance(key.ReadObj(), ROOT.TDirectory):
            pipeline_name = key.GetName()
            if pipeline_name.startswith('jecu_'):
                source_name = re.sub(r"\_up$", "", pipeline_name)
                source_name = re.sub(r"\_dn$", "", source_name)
                jec_sources.append(source_name)

    # unique list of sources
    jec_sources = list(set(jec_sources))

    print jec_sources

    central_histo = root_file.Get("{0}/{1}".format(args['central_pipeline'], args['histogram'] ))

    output_dir = 'jecurel'
    if ROOT.gDirectory.GetDirectory(output_dir) != None:
        print "Folder {0} exists. Will be overwritten".format(output_dir)
        ROOT.gDirectory.Delete('{0};*'.format(output_dir))
    ROOT.gDirectory.mkdir(output_dir)
    ROOT.gDirectory.cd(output_dir)


    for jec_source in jec_sources:
        for var in ['dn', 'up']:
            path = "{0}_{1}/{2}".format(jec_source, var, args['histogram'])
            print path
            histo = root_file.Get(path)
            print jec_source
            print type(histo)
            histo.Divide(central_histo)
            histo.Write('jecurel_{0}_{1}'.format(jec_source, var))



if __name__ == '__main__':
    main()

#! /usr/bin/env python2

import glob
import os
import sys
import argparse
from ConfigParser import RawConfigParser

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

def main():

    parser = argparse.ArgumentParser(description='Merge and link output files')
    parser.add_argument('output_folder', help='Path to output folder.')
    parser.add_argument('--nick', help='Restrict to this nick.')
    parser.add_argument('--rename', action='store_true', default=False, help='Prompt user to rename links')

    args = vars(parser.parse_args())

    samples_config = RawConfigParser(allow_no_value=True)
    samples_config.read(os.path.expandvars('$CMSSW_BASE/src/JetAnalysis/DijetAna/data/samples.conf'))

    sample_summary = dict(samples_config.items('samples_summary'))
    print sample_summary

    print args
    all_dirs = [d for d in os.listdir(args['output_folder']) if os.path.isdir(os.path.join(args['output_folder'], d))]
    
    mergedict = {}
    for sample in sample_summary:
        mergedict[sample] = []
        for subsample in sample_summary[sample]:
            for dir in all_dirs:
                if subsample in dir:
                    mergedict[sample].append(dir)

    print mergedict

    # glob all folders in output folder
    # if nick in them
    # group folders according to nicks
    # merge these files
    # link merged files to CMSSW BASE DIR


if __name__ == '__main__':
    main()

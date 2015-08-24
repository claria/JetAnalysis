#! /usr/bin/env python2

import glob
import os
import sys
import argparse
from ConfigParser import RawConfigParser
import subprocess

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

def main():

    parser = argparse.ArgumentParser(description='Merge and link output files')
    parser.add_argument('output_folder', help='Path to output folder.')
    parser.add_argument('--nicks', nargs='+', help='Restrict to this nicks.')
    parser.add_argument('--list-nicks', action='store_true', help='Restrict to this nicks.')
    parser.add_argument('--rename', action='store_true', default=False, help='Prompt user to rename links')
    parser.add_argument('--prefix', default='', help='Prefix to put into the link name.')

    args = vars(parser.parse_args())

    samples_config = RawConfigParser(allow_no_value=True)
    samples_config.read(os.path.expandvars('$CMSSW_BASE/src/JetAnalysis/DijetAna/data/samples.conf'))

    # Bad hack
    sample_summary = dict(samples_config.items('samples_summary'))
    for sample in sample_summary:
        sample_summary[sample] = sample_summary[sample].split(',')

    all_dirs = [d for d in os.listdir(args['output_folder']) if os.path.isdir(os.path.join(args['output_folder'], d))]

    mergedict = {}
    for sample in sample_summary:
        for subsample in sample_summary[sample]:
            for dir in all_dirs:
                if subsample in dir:
                    if not sample in  mergedict:
                        mergedict[sample] = []
                    mergedict[sample] += glob.glob(os.path.join(args['output_folder'], "{0}/*.root".format(dir)))

    if args['list_nicks']:
        '\n'.join(mergedict.keys())

    for sample in mergedict:
        target_name = '{0}/{1}.root'.format(args['output_folder'], sample.upper())
        hdadd(mergedict[sample], target_name)
        link_name = '{0}{1}'.format(args['prefix'] + '_' if args['prefix'] else '',os.path.basename(target_name))
        if os.path.exists(link_name) and os.path.islink(link_name):
            os.remove(link_name)
        os.symlink(target_name, link_name)


def hdadd(files, output_filename):
    cmd = 'jethadd -f {0} {1}'.format(output_filename, ' '.join(files))
    rc = subprocess.call(cmd.split())
    if rc != 0:
        raise Exception('Merging failed.')


if __name__ == '__main__':
    main()

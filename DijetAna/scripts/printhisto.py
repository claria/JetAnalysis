#! /usr/bin/env python2

import os
import sys
import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True


from JetAnalysis.DijetAna.tools import get_root_object

def main():

    parser = argparse.ArgumentParser(description='Print full directory structure of root file')
    parser.add_argument('identifier', help='Path to root file.')
    parser.add_argument('--folder', default="/", help='Restrict listing to folder.')
    args = vars(parser.parse_args())

    root_object = get_root_object(args['identifier'])

    root_object.Print('all')


if __name__ == '__main__':
    main()

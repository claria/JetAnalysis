#! /usr/bin/env python2

import os
import sys
import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

def main():

    parser = argparse.ArgumentParser(description='Print full directory structure of root file')
    parser.add_argument('rootfile', help='Path to root file.')
    parser.add_argument('--folder', default="/", help='Restrict listing to folder.')
    args = vars(parser.parse_args())

    root_file = ROOT.TFile(args['rootfile'], "READ")
    directory = root_file.GetDirectory(args['folder'])
    key_dir = get_keys(directory)

    pretty(key_dir)


def get_keys(directory=""):
    """ Recursive iterate through root file."""
    res = {}
    for key in directory.GetListOfKeys():
        if isinstance(key.ReadObj(), ROOT.TDirectory):
            res[key.GetName()] = get_keys(key.ReadObj())
        # elif isinstance(key.ReadObj(), ROOT.TTree):
        else:
            res[key.GetName()] = key.GetClassName()
    return res


def pretty(d, indent=0):
    """Pretty print a nested dictionary with indentation."""
    for key, value in d.iteritems():
        print '  ' * indent + "{0}".format(key)
        if isinstance(value, dict):
            pretty(value, indent+1)

if __name__ == '__main__':
    main()

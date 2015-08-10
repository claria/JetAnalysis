#! /usr/bin/env python
# -*- coding: utf-8 -*-

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


def get_keys(directory=''):
    """ Recursive iterate through root file."""
    res = {}
    for key in directory.GetListOfKeys():
        if isinstance(key.ReadObj(), ROOT.TDirectory):
            res[key.GetName()] = get_keys(key.ReadObj())
        elif isinstance(key.ReadObj(), ROOT.TTree):
            res[key.GetName()] = {}
            for leaf in sorted(key.ReadObj().GetListOfLeaves(), key=lambda leaf: leaf.GetName()):
                quantity = leaf.GetName()
                if leaf.GetBranch().GetMother().GetName() != leaf.GetName():
                    quantity = leaf.GetBranch().GetMother().GetName()+"."+quantity
                res[key.GetName()][quantity] = leaf.GetTypeName()
        else:
            res[key.GetName()] = key.GetClassName()
    return res


def pretty(d, indent=''):
    """Pretty print a nested dictionary with indentation."""
    for i, (key, value) in enumerate(d.iteritems()):
        if not indent:
            fancy_str = ''
        elif i == len(d) -1:
            fancy_str = '└─'
        else:
            fancy_str = '├─'
        print indent + fancy_str + "{0}".format(key)
        if isinstance(value, dict):
            add_str = '│' if i != len(d) -1 else ' '
            pretty(value, indent + add_str +   '  ')

if __name__ == '__main__':
    main()

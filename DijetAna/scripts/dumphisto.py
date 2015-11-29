#! /usr/bin/env python2

"""
Prints the content of root histograms to the screen.
"""


import os
import sys
import argparse

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True


from JetAnalysis.DijetAna.tools import get_root_object

def main():

    parser = argparse.ArgumentParser(description='Print contents of root histogram')
    parser.add_argument('identifier', help='Path to root file object. Syntax is path/to/file.root?path/to/histo.')
    args = vars(parser.parse_args())

    histo = get_root_object(args['identifier'])
    
    print 'Path: {0}'.format(args['identifier'])
    template = "{0: <20} {1: <20} {2: <20} {3: <20} {4: <20}"
    print template.format('Low bin edge', 'High bin edge', 'Bin width', 'Bin content', 'Bin error')
    for i in xrange(1, histo.GetNbinsX() + 1):
        binl, binu = histo.GetXaxis().GetBinLowEdge(i), histo.GetXaxis().GetBinUpEdge(i)
        binwidth = histo.GetBinWidth(i)
        bincontent = histo.GetBinContent(i)
        binerror = histo.GetBinError(i)
        print template.format(binl, binu, binwidth, bincontent, binerror)


if __name__ == '__main__':
    main()

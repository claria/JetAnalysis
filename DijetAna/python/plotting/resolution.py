# -*- coding: utf-8 -*-

import logging
import Artus.Utility.logger as logger
log = logging.getLogger(__name__)

import ROOT
from array import array

from Artus.HarryPlotter.analysisbase import AnalysisBase


class JetResolution(AnalysisBase):
	""" Fit resolution vs pt."""

	def __init__(self):
		super(JetResolution, self).__init__()

	def modify_argument_parser(self, parser, args):
		super(JetResolution, self).modify_argument_parser(parser, args)
		
		self.resolution_options = parser.add_argument_group("Resolution")

	def prepare_args(self, parser, plotData):
		super(JetResolution, self).prepare_args(parser, plotData)
		
		# self.prepare_list_args(plotData, ["sum_nicks", "sum_scale_factors", "sum_result_nicks"])

	def run(self, plotData=None):
		super(JetResolution, self).run(plotData)

		nick = plotData.plotdict["nicks"][0]
		root_histogram = plotData.plotdict["root_objects"][nick]

		if not isinstance(root_histogram, ROOT.TH2):
			raise TypeException("I want a TH2D!")

		# y bin edges
		y_bins = [root_histogram.GetYaxis().GetBinUpEdge(i) for i in xrange(0, root_histogram.GetNbinsY()+1)]
		y_bincenter = [root_histogram.GetYaxis().GetBinCenter(i) for i in xrange(1, root_histogram.GetNbinsY()+1)]
		y_binerr = [root_histogram.GetYaxis().GetBinWidth(i)/2. for i in xrange(1, root_histogram.GetNbinsY()+1)]

		th1_histos = []
		a = []
		mu = []
		sigma = []
		sigmaerr = []
		for i in xrange(1, len(y_bins)):
			th1_nick = "{0}_proj_{1}".format(nick, i)
			th1_histogram = root_histogram.ProjectionX(th1_nick,i,i)
			th1_histogram.Fit("gaus")
			tf1 = th1_histogram.GetFunction("gaus")
			a.append(tf1.GetParameter(0))
			mu.append(tf1.GetParameter(1))
			sigma.append(tf1.GetParameter(2))
			sigmaerr.append(tf1.GetParError(2))
			th1_histos.append(th1_histogram)

		# create root tgraph
		x = array('d', y_bincenter)
		xerr = array('d', y_binerr)
		y = array('d', sigma)
		yerr = array('d', sigmaerr)
		graph = ROOT.TGraphErrors(len(x), x, y, xerr, yerr)

		plotData.plotdict['nicks'].append("mygraph")
		plotData.plotdict["root_objects"]["mygraph"] = graph


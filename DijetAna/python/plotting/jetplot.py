#  -*- coding: utf-8 -*-

"""
"""
import os
import logging
import Artus.Utility.logger as logger
log = logging.getLogger(__name__)

import ROOT

from Artus.HarryPlotter.plot_modules.plotmpl import PlotMpl

from Artus.Utility.mplhisto import MplHisto, MplGraph

import matplotlib
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
from matplotlib.colors import Normalize

import numpy as np

class JetPlot(PlotMpl):
	def __init__(self):
		super(JetPlot, self).__init__()
		self.set_matplotlib_defaults()

	@staticmethod
	def set_matplotlib_defaults():
		# Set matplotlib rc settings
		# matplotlib.rcParams['font.family'] = 'sans-serif'
		matplotlib.rcParams['mathtext.fontset'] = 'stixsans'
		matplotlib.rcParams['mathtext.default'] = 'rm'
		# matplotlib.rcParams['font.sans-serif'] = 'helvetica, Helvetica, Nimbus Sans L, Mukti Narrow, FreeSans'

		# figure
		matplotlib.rcParams['figure.figsize'] = 7., 7.
		# axes
		matplotlib.rcParams['axes.linewidth'] = 2
		matplotlib.rcParams['axes.labelsize'] = 20
		matplotlib.rcParams['xtick.labelsize'] = 16
		matplotlib.rcParams['xtick.major.size'] = 8
		matplotlib.rcParams['xtick.major.width'] = 1.5
		matplotlib.rcParams['xtick.minor.size'] = 6
		matplotlib.rcParams['xtick.minor.width'] = 1.
		matplotlib.rcParams['ytick.labelsize'] = 16
		matplotlib.rcParams['ytick.major.width'] = 1.5
		matplotlib.rcParams['ytick.major.size'] = 8
		matplotlib.rcParams['ytick.minor.size'] = 6
		matplotlib.rcParams['ytick.minor.width'] = 1.
		matplotlib.rcParams['lines.markersize'] = 8
		# default color cycle
		matplotlib.rcParams['axes.color_cycle'] = [(0.0, 0.0, 0.0),
		                                           (0.21568627450980393, 0.49411764705882355, 0.7215686274509804),
		                                           (0.30196078431372547, 0.6862745098039216, 0.2901960784313726),
		                                           (0.596078431372549, 0.3058823529411765, 0.6392156862745098),
		                                           (1.0, 0.4980392156862745, 0.0),
		                                           (1.0, 1.0, 0.2),
		                                           (0.6509803921568628, 0.33725490196078434, 0.1568627450980392),
		                                           (0.9686274509803922, 0.5058823529411764, 0.7490196078431373),
		                                           (0.6, 0.6, 0.6)]
		matplotlib.rcParams["axes.formatter.limits"] = [-5, 5]
		# legend
		matplotlib.rcParams['legend.numpoints'] = 1
		matplotlib.rcParams['legend.fontsize'] = 19
		matplotlib.rcParams['legend.labelspacing'] = 0.3
		matplotlib.rcParams['legend.frameon'] = False
		# Saving
		matplotlib.rcParams['savefig.bbox'] = 'tight'
		matplotlib.rcParams['savefig.dpi'] = 150
		matplotlib.rcParams['savefig.format'] = 'png'
		matplotlib.rcParams['agg.path.chunksize'] = 20000

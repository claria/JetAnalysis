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
		self.mpl_version = int(matplotlib.__version__.replace(".", ""))
		self.set_matplotlib_defaults()

	def make_plots(self, plotData):
		zip_arguments = self.get_zip_arguments(plotData)

		for nick, color, label, marker, errorbar, linestyle, step, zorder in zip(*zip_arguments):
			log.info("Process nick: {0}".format(nick))
			root_object = plotData.plotdict["root_objects"][nick]

			if isinstance(root_object, ROOT.TGraph):
				self.plot_dimension = 1
				mplhist = MplGraph(root_object)
				self.plot_errorbar(mplhist, ax=self.ax, color=color, fmt=marker, capsize=0, linestyle=linestyle, label=label, zorder=zorder)
			elif isinstance(root_object, ROOT.TH2):
				mplhist = MplHisto(root_object)
				self.plot_dimension = mplhist.dimension
				vmin = plotData.plotdict["z_lims"][0] if plotData.plotdict["z_lims"] else None
				vmax = plotData.plotdict["z_lims"][1] if plotData.plotdict["z_lims"] else None
				cmap = plt.cm.get_cmap(plotData.plotdict["colormap"])
				self.plot_contour1d(mplhist, ax=self.ax, vmin=vmin, vmax=vmax, z_log=plotData.plotdict["z_log"], cmap=cmap)

			elif isinstance(root_object, ROOT.TH1):
				mplhist = MplHisto(root_object)
				self.plot_dimension = mplhist.dimension

				if marker=="bar":
					self.plot_hist1d(mplhist, style='bar', ax=self.ax, show_yerr=errorbar, label=label, color=color, alpha=1.0, zorder=zorder)
				elif marker=='fill':
					self.plot_hist1d(mplhist, style='fill', ax=self.ax, show_yerr=errorbar, label=label, color=color, alpha=1.0, zorder=zorder)
				else:
					self.plot_errorbar(mplhist, ax=self.ax, step=step, color=color, fmt=marker, capsize=0, linestyle=linestyle, label=label, zorder=zorder)
			# draw functions from dictionary
			elif isinstance(root_object, ROOT.TF1):
				x_values = []
				y_values = []
				sampling_points = 1000
				x_range = [ root_object.GetXaxis().GetXmin(), root_object.GetXaxis().GetXmax()] 
				for x in np.arange(x_range[0], x_range[1], (float(x_range[1])-float(x_range[0]))/sampling_points):
					x_values.append(x)
					y_values.append(root_object.Eval(x))
				self.ax.plot(x_values, y_values, label=label, color=color, linestyle=linestyle, linewidth=2)

		if plotData.plotdict["ratio"]:
			for root_object, ratio_color, ratio_marker, in zip(plotData.plotdict["root_ratio_histos"],
				                                               plotData.plotdict["ratio_colors"],
				                                               plotData.plotdict["ratio_markers"]):
				self.ax2.axhline(1.0, color='black')
				mplhist_ratio = MplHisto(root_object)
				# self.ax2.errorbar(mplhist_ratio.x, mplhist_ratio.y, mplhist_ratio.yerr, ecolor=ratio_color, fmt=ratio_marker)
				self.plot_errorbar(mplhist_ratio, ax=self.ax2, step=step, color=ratio_color, fmt=ratio_marker, capsize=0, linestyle=linestyle, zorder=zorder)

	def modify_axes(self, plotData):
		# do what is needed for all plots:
		super(JetPlot, self).modify_axes(plotData)

		self.ax.grid(plotData.plotdict["grid"])

		if not plotData.plotdict["ratio"]:
			self.ax.set_xlabel(plotData.plotdict["x_label"], position=(1., 0.), va='top', ha='right')
		self.ax.set_ylabel(plotData.plotdict["y_label"], position=(0., 1.), va='top', ha='right')
		self.ax.ticklabel_format(style='sci',scilimits=(-3,4),axis='both')

		# do special things for 1D Plots
		if self.plot_dimension == 1:
			if plotData.plotdict["x_log"]: 
				self.ax.set_xscale('log', nonposx='clip')
			if plotData.plotdict["y_log"]: 
				self.ax.set_yscale('log', nonposy='clip')

			if plotData.plotdict["ratio"]:
				self.ax2.set_xlabel(plotData.plotdict["x_label"],position=(1., 0.), va='top', ha='right')
				self.ax2.set_ylabel(plotData.plotdict["y_ratio_label"])
				self.ax2.grid(plotData.plotdict["ratio_grid"])
				# Don't show ticklabels on main plot
				self.ax.xaxis.set_ticklabels([])
				# Ratio plot shares xlims of main plot
				self.ax2.set_xlim(self.ax.get_xlim())
				if plotData.plotdict["x_log"]: 
					self.ax2.set_xscale('log', nonposx='clip')
		# do special things for 2D Plots
		elif self.plot_dimension == 2:
			cb = self.fig.colorbar(self.image, ax=self.ax)
			if plotData.plotdict["z_label"]:
				cb.set_label(plotData.plotdict["z_label"])

	def add_labels(self, plotData):
		super(JetPlot, self).add_labels(plotData)

		if plotData.plotdict["title"]:
			self.ax.set_title(plotData.plotdict["title"], fontsize=14)

		if not (plotData.plotdict["lumi"]==None):
			plt.text(-0.0, 1.030, "$\mathcal{L}=" + str(plotData.plotdict["lumi"]) + "\mathrm{fb^{-1}}$", transform=self.ax.transAxes, fontsize=10)
			#self.fig.suptitle("$\mathcal{L}=" + str(plotData.plotdict["lumi"]) + "\mathrm{fb^{-1}}$", ha="center", x=0.4)
		if not (plotData.plotdict["energy"] == None):
			energy = "+".join(plotData.plotdict["energy"])
			plt.text(1.0, 1.030, "$\sqrt{s}=" + energy + "\\ \mathrm{TeV}$", transform=self.ax.transAxes, fontsize=10, horizontalalignment="right")
			#self.fig.suptitle("$\sqrt{s}=" + energy + "\\ \mathrm{TeV}$", ha="right",x=0.9) 

		self.ax.legend(loc='upper right')

		plt.tight_layout()
		# Decrease vertical distance between subplots
		plt.subplots_adjust(hspace=0.2)

	def save_canvas(self, plotData):
		for output_filename in plotData.plotdict["output_filenames"]:
			self.fig.savefig(output_filename)
			log.info("Created plot \"%s\"." % output_filename)

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

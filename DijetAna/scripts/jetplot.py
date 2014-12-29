#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import logging
import Artus.Utility.logger as logger
log = logging.getLogger(__name__)

from Artus.HarryPlotter.core import HarryCore

def harryplotter(args_from_script=None):
	harry_core = HarryCore(args_from_script=args_from_script)
	harry_core.register_modules_dir("$CMSSW_BASE/python/JetAnalysis/DijetAna/plotting")
	if harry_core.args['plot_modules'] == harry_core.parser.get_default('plot_modules'):
		harry_core.args['plot_modules'] = ["JetPlot"]
	harry_core.run()

if __name__ == '__main__':
	harryplotter()

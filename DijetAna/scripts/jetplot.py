#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import logging
import Artus.Utility.logger as logger
log = logging.getLogger(__name__)

from Artus.HarryPlotter.core import HarryCore

def harryplotter():
	print os.path.abspath(__file__)
	harry_core = HarryCore()
	harry_core.run()

if __name__ == '__main__':
	harryplotter()

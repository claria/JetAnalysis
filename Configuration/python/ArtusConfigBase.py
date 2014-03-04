#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""ArtusConfigBase provides the tools to make a valid artus config.

The most used functions are:
  - BaseConfig to generate a default configuration
  - CreateFileList to create a list of input files
  - Run to acutally call artus and run it
"""

import os
import glob
import abc


class ArtusBaseConfig(object):
	__metaclass__ = abc.ABCMeta

	def __init__(self, inputtype, exec_name=None):

		self._inputtype = inputtype
		# Name of executable to be called by artus.py
		self._exec_name = exec_name

		self._config = {
			'SkipEvents': 0,
			'EventCount': -1,
			'GlobalProducers': [],
			'InputType': inputtype,
			'InputFiles': [],
			'OutputPath': "out",
			'Pipelines': {
			'default': {
				'LocalProducers': [],
				'Filters': [],
				'Cuts': [],
				'Consumer': [],
				'QuantitiesVector': [],
				}
			},
			}

	def get_config(self):
		return self._config

	def get_exec_name(self):
		return self._exec_name

	def set_exec_name(self, exec_name):
		self._exec_name = exec_name

	@abc.abstractmethod
	def expand(self):
		pass

	@abc.abstractmethod
	def set_mc_specific(self):
		pass
	
	@abc.abstractmethod
	def set_data_specific(self):
		pass

	def set_input_filenames(self, files):
		if isinstance(files, basestring):
			self._config['InputFiles'] = glob.glob(files)
		else:
			self._config['InputFiles'] = files

	def set_output_filename(self, filename):
		self._config['OutputPath'] = filename

	def pipelinediff(self, to=None):
		print "Comparing", len(self._config['Pipelines']), "pipelines:"
		if to is None:
			to = filter(lambda x: 'incut' in x, self._config['Pipelines'].keys())[0]

		for name, p in self._config['Pipelines'].items():
			if name != to:
				print "- Compare", name, "to", to
				pipelinediff2(p, self._config['Pipelines'][to])
		print



#
# Helper methods
#
def getPath(variable=None, nofail=False):
	try:
		if not variable:
			path = os.environ["CMSSW_BASE"] + "/src/Artus/"
		else:
			path = os.environ[variable]
		return path
	except:
		print variable, "is not in shell variables:", os.environ.keys()
		print "Please source ini script and CMSSW!"
		if nofail:
			return None
		exit(1)


def FindFileInList(conf, number):
	for f in conf['InputFiles']:
		if "_" + str(number) in f:
			print conf['InputFiles'].index(f)


def pipelinediff2(p1=None, p2=None):
	differencefound = False
	for k, v in p1.items():
		if k in p2.keys():
			if p1[k] != p2[k]:
				differencefound = True
				print "	different %s: %s != %s" % (k, str(p1[k]), str(p2[k]))
	if not differencefound:
		print "	identical"

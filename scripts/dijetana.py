#!/usr/bin/env python

import sys
from Artus.Configuration.artusWrapper import ArtusWrapper
import argparse

config = {}

def main():
	userParser = getUserParser()
	wrapper = ArtusWrapper('JetAna', userArgParsers=[userParser])
	config.update(baseconfig)
	config.update(wrapper.getConfig())

	if wrapper._args.data:
		SetDataSpecific()
	else:
		SetMCSpecific()

	wrapper.setConfig(config)

	print config['InputFiles']
	if not config['InputFiles']:
		wrapper.setInputFilenames('/nfs/dust/cms/user/gsieber/SKIMS/SKIM_JETS_2012/kappa_QCDP6*.root')
	if not config['OutputPath']:
		wrapper.setOutputFilename('output.root')

	sys.exit(wrapper.run())


def SetMCSpecific():
	config['InputIsData'] = 'true'
	pass

def SetDataSpecific():
	config['InputIsData'] = 'True'
	config['HltPaths'] = ['HLT_PFJET40','HLT_PFJET80','HLT_PFJET140','HLT_PFJET200','HLT_PFJET260','HLT_PFJET320',]
	#config['GlobalProcessors'].insert(0, 'producer:hlt_selector')

def getUserParser():
	parser = argparse.ArgumentParser(add_help=False)
	parser.add_argument('--data', default=False, action='store_true')
	return parser


baseconfig = {
	'SkipEvents': 0,
	'Jets' : 'AK5PFJets',
	'EventCount': -1,
	'GlobalProcessors': ['producer:valid_jets', 'filter:DiJetsFilter'],
	'InputIsData': 'false',
	'InputFiles': [],
	'OutputPath': 'output.root',
	'Pipelines': {
		'default': {
			'Processors': ['producer:DiJetsObservables'],
			'Consumers': ['JetNtupleConsumer', 'quantities_all'],
			'Quantities' : ['LeadingJetPt', 'LeadingJetRap', 'SecondJetPt', 'SecondJetRap']
		}
	},
}

if __name__ == '__main__':
	main()

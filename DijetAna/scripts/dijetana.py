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


	SetCuts()
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
	config['InputIsData'] = 'false'
	pass


def SetDataSpecific():
	config['InputIsData'] = 'true'
	config["LumiMetadata"] = "KLumiMetadata"
	config["EventMetadata"] = "KEventMetadata"
	config["TriggerObjects"] = "KTriggerObjects"
	config['HltPaths'] = [
						'HLT_PFJET40',
						'HLT_PFJET80',
						'HLT_PFJET140',
						'HLT_PFJET200',
						'HLT_PFJET260',
						'HLT_PFJET320',
						]
	config['GlobalProcessors'].append('filter:JetHltFilter')
	config['GlobalProcessors'].append('producer:JetHLTProducer')


def getUserParser():
	parser = argparse.ArgumentParser(add_help=False)
	parser.add_argument('--data', default=False, action='store_true')
	return parser


def SetCuts():
	config['MinJetPtCut'] = '100.'
	config['MaxJetRapCut'] = '3.'


baseconfig = {
	'SkipEvents': 0,
	'EventCount': -1,
	'GlobalProcessors': [
						'producer:valid_jets',
						'filter:DiJetsFilter',
						'filter:DiJetsRapFilter',
						'filter:DiJetsPtFilter',
						],
	'InputIsData': 'false',
	'InputFiles': [],
	'OutputPath': 'output.root',
	'JetID' : 'Tight',
	'Jets' : 'AK5PFJets',
	'Pipelines': {
		'default': {
			'Processors': ['producer:DiJetsObservables'],
			'Consumers': ['JetNtupleConsumer', 'cutflow_histogram'],
			'Quantities' : ['Jet1Pt', 'Jet1Rap', 'Jet2Pt', 'Jet2Rap']
		}
	},
}

if __name__ == '__main__':
	main()

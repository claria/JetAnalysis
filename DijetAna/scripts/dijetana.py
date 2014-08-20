#!/usr/bin/env python

import os
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
	config["GenLumiMetadata"] = "KLumiMetadata"
	config["GenEventMetadata"] = "KEventMetadata"
	config['PileupWeightFile'] = os.path.expandvars('$CMSSW_BASE/src/JetAnalysis/DijetAna/data/pileup.root')
	config['InputIsData'] = 'false'
	config['Processors'].insert(0, 'producer:CrossSectionWeightProducer')
	config['Processors'].insert(0, 'producer:GeneratorWeightProducer')
	config['Processors'].insert(0, 'producer:pu_weights')
	pass


def SetDataSpecific():
	config['InputIsData']    = 'true'
	config["LumiMetadata"]   = "KLumiMetadata"
	config["EventMetadata"]  = "KEventMetadata"
	config["TriggerInfos"]   = "KTriggerInfos",
	config["BeamSpot"]       = "offlineBeamSpot",
	#config["TriggerObjects"] = "KTriggerObjects"

	config['JsonFiles'] = [os.path.expandvars('$CMSSW_BASE/src/JetAnalysis/DijetAna/data/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt')]
	config['HltPaths'] = [
						'HLT_PFJET40',
						'HLT_PFJET80',
						'HLT_PFJET140',
						'HLT_PFJET200',
						'HLT_PFJET260',
						'HLT_PFJET320',
						]
	config['Processors'] += ['producer:hlt_selector']
	config['Processors'] += ['filter:json_filter']


def getUserParser():
	parser = argparse.ArgumentParser(add_help=False)
	parser.add_argument('--data', default=False, action='store_true')
	return parser


def SetCuts():
	config['MinJetPtCut'] = '100.'
	config['MaxJetRapCut'] = '0.5'


baseconfig = {
	'InputIsData': '',
	'SkipEvents': 0,
	'EventCount': -1,
	"LumiMetadata" : "KLumiMetadata",
	"GenLumiMetadata" : "",
	"EventMetadata" : "KEventMetadata",
	"GenEventMetadata" : "",
	"FilterMetadata" : "",
	"FilterSummary" : "",
	"VertexSummary" : "offlinePrimaryVerticesSummary",
	'Processors': [
						'producer:valid_jets',
						'filter:DiJetsFilter',
						'filter:DiJetsRapFilter',
						'filter:DiJetsPtFilter',
						],
	'InputFiles': [],
	'OutputPath': 'output.root',
	'JetID' : 'Tight',
	'Jets' : 'AK5PFJets',
	'Pipelines': {
		'default': {
			'Processors': ['producer:DiJetsObservables', 'producer:event_weight'],
			'Consumers': ["lambda_ntuple", 'cutflow_histogram'],
			'Quantities' : ['npv', 'npu', 'weight', 'jet1_pt', 'jet1_eta', 'jet1_phi', 'crossSectionPerEventWeight', 'hltPrescaleWeight', 'puWeight', 'generatorWeight'],
			'EventWeight' : 'EventWeight'

		}
	},
}

if __name__ == '__main__':
	main()

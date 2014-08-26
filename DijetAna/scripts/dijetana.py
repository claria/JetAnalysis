#!/usr/bin/env python

import os
import sys
from Artus.Configuration.artusWrapper import ArtusWrapper
import argparse

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
            'producer:ValidJetsFilter',
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
                'Processors': ['producer:DiJetsObservables', 
                    'producer:EventWeightProducer'],
                'Consumers': ['KappaLambdaNtupleConsumer', 
                    'cutflow_histogram'],
                'Quantities' : ['npv', 
                    'npu', 
                    'weight', 
                    'jet1_pt', 
                    'jet1_eta', 
                    'jet1_phi', 
                    'crossSectionPerEventWeight', 
                    'hltPrescaleWeight', 
                    'puWeight', 
                    'generatorWeight', 
                    'numberGeneratedEventsWeight'],
                'EventWeight' : 'EventWeight'

                }
            },
        }



def main():
    global config

    userParser = getUserParser()
    wrapper = ArtusWrapper('JetAna', userArgParsers=[userParser])

    # Use baseconfig with settings valid for data and all MC
    config = baseconfig
    config.update(wrapper.getConfig())

    # Identify input type based on Nickname
    nickname = wrapper.determineNickname('auto')


    # Define cuts
    config['MinJetPtCut'] = '80.'
    config['MaxJetRapCut'] = '2.0'


    # Add Producers etc specifict to data/MC
    if isData(nickname):
        SetDataSpecific(nickname)
    else:
        SetMCSpecific(nickname)

    wrapper.setConfig(config)

    print config

    if not config['OutputPath']:
        wrapper.setOutputFilename('output.root')

    sys.exit(wrapper.run())


def SetMCSpecific(nickname=None):
    config['InputIsData'] = 'false'
    config["GenLumiMetadata"] = "KLumiMetadata"
    config["GenEventMetadata"] = "KEventMetadata"
    config['PileupWeightFile'] = os.path.expandvars('$CMSSW_BASE/src/JetAnalysis/DijetAna/data/Data_Pileup_2012_ReReco-600bins_OVER_MC_Summer12_PU_S10-600bins.root')
    config['Processors'].append('producer:PUWeightProducer')
    config['Processors'].append('producer:CrossSectionWeightProducer')
    config['Processors'].append('producer:GeneratorWeightProducer')

    if nickname in mc_samples:
        config['Processors'].append('producer:NumberGeneratedEventsWeightProducer')
        config['NumberGeneratedEvents'] = mc_samples[nickname].get('NumberGeneratedEvents', 0)
        config['CrossSection'] = mc_samples[nickname].get('CrossSection', -1)
    pass


def SetDataSpecific(nickname=None):
    config['InputIsData']    = 'true'
    config["LumiMetadata"]   = "KLumiMetadata"
    config["EventMetadata"]  = "KEventMetadata"
    config["TriggerInfos"]   = "KTriggerInfos",
    config["BeamSpot"]       = "offlineBeamSpot",
    config["TriggerObjects"] = "KTriggerObjects"

    config['JsonFiles'] = [os.path.expandvars('$CMSSW_BASE/src/JetAnalysis/DijetAna/data/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt')]
    config['HltPaths'] = [
            'HLT_PFJET40',
            'HLT_PFJET80',
            'HLT_PFJET140',
            'HLT_PFJET200',
            'HLT_PFJET260',
            'HLT_PFJET320',
            'HLT_PFJET400',
            ]

    config['Processors'].insert(0, 'filter:JsonFilter')
    config['Processors'].append('producer:HltProducer')
    config['Processors'].append('producer:NumberGeneratedEventsWeightProducer')
    config['NumberGeneratedEvents'] = 815


def isData(nickname):
    if nickname.startswith('Jet_2012'):
        return True
    elif nickname in mc_samples:
        return False


def getUserParser():
    parser = argparse.ArgumentParser(add_help=False)
    # parser.add_argument('--data', default=False, action='store_true')
    return parser


# Dict with information about MC samples

mc_samples = {
        'QCDP6_Z2S_S10' : { 'Dataset' : '/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                            'NumberGeneratedEvents' : 9991674,
                            'CrossSection' : -1.
                           },
        'QCDP8_4C_S10' : { 'Dataset' : '',
                            'NumberGeneratedEvents' : -1,
                            'CrossSection' : -1.
                           },
        'QCDHW_EE3C_S10' : { 'Dataset' : '',
                            'NumberGeneratedEvents' : -1,
                            'CrossSection' : -1.
                           },
        }





if __name__ == '__main__':
    main()

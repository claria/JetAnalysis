#!/usr/bin/env python

import os
import sys
from Artus.Configuration.artusWrapper import ArtusWrapper
import argparse

baseconfig = {
    'InputIsData': '',
    'SkipEvents': 0,
    'EventCount': -1,
    "LumiMetadata": "KLumiMetadata",
    "GenLumiMetadata": "",
    "EventMetadata": "KEventMetadata",
    "GenEventMetadata": "",
    "FilterMetadata": "",
    "FilterSummary": "",
    "VertexSummary": "offlinePrimaryVerticesSummary",
    "JetEnergyCorrectionParameters": "",
    "JetEnergyCorrectionUncertaintyParameters": "",
    'Processors': [
        'producer:JetCorrectionsProducer',
        'producer:ValidJetsProducer',
        'producer:JetTriggerMatchingProducer',
        'filter:DiJetsFilter',
        'filter:LeadingJetPtFilter',
        'filter:LeadingJetRapFilter',
    ],
    'InputFiles': [],
    'OutputPath': 'output.root',
    'JetID': 'tight',
    'Jets': 'AK5PFJets',
    'JetArea': 'KT6Area',
    'Met': "PFMET",
    'Pipelines': {
        'default': {
            'Processors': [
                'producer:DiJetsObservables',
                'producer:EventWeightProducer',
            ],
            'Consumers': [
                'KappaLambdaNtupleConsumer',
                'cutflow_histogram',
                'KappaTriggerResultsHistogramConsumer',
            ],
            'Quantities': [
                'npv',
                'npu',
                'weight',
                'jet1_pt',
                'jet1_eta',
                'jet1_phi',
                'crossSectionPerEventWeight',
                'hltPrescaleWeight',
                'puWeight',
                'generatorWeight',
                'numberGeneratedEventsWeight'
            ],
            'EventWeight': 'EventWeight'

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
    print 'nickname', nickname

    # Define cuts
    config['MinLeadingJetPt'] = '25.'
    config['MinLeadingJetRap'] = '0.0'
    config['MaxLeadingJetRap'] = '2.0'

    # Add Producers etc specifict to data/MC
    if isData(nickname):
        SetDataSpecific(nickname)
    else:
        SetMCSpecific(nickname)


    walk_dic(config, os.path.expandvars)
    wrapper.setConfig(config)

    print config

    if not config['OutputPath']:
        wrapper.setOutputFilename('output.root')

    sys.exit(wrapper.run())


def SetMCSpecific(nickname=None):
    config['InputIsData'] = 'false'
    config["GenLumiMetadata"] = "KLumiMetadata"
    config["GenEventMetadata"] = "KEventMetadata"
    config['PileupWeightFile'] = '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/Data_Pileup_2012_ReReco-600bins_OVER_MC_Summer12_PU_S10-600bins.root'
    config['Processors'].append('producer:PUWeightProducer')
    config['Processors'].append('producer:CrossSectionWeightProducer')
    config['Processors'].append('producer:GeneratorWeightProducer')
    config['JetEnergyCorrectionParameters'] = [
                                                "$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V23__All_L1FastJet_AK5PF.txt",
                                                "$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V23__All_L2Relative_AK5PF.txt",
                                                "$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V23__All_L3Absolute_AK5PF.txt"
                                                ]
    if nickname in mc_samples:
        config['Processors'].append('producer:NumberGeneratedEventsWeightProducer')
        config['NumberGeneratedEvents'] = mc_samples[nickname].get('NumberGeneratedEvents', 0)
        config['CrossSection'] = mc_samples[nickname].get('CrossSection', -1)
    pass


def SetDataSpecific(nickname=None):
    config['InputIsData'] = 'true'
    config["LumiMetadata"] = "KLumiMetadata"
    config["EventMetadata"] = "KEventMetadata"
    config["TriggerInfos"] = "KTriggerInfos",
    config["BeamSpot"] = "offlineBeamSpot",
    config["TriggerObjects"] = "KTriggerObjects"
    config["TriggerInfos"] = "KTriggerInfos"
    config['JetEnergyCorrectionParameters'] = [
        "$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/FT_53_V21_AN4__All_L1FastJet_AK5PF.txt",
        "$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/FT_53_V21_AN4__All_L2Relative_AK5PF.txt",
        "$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/FT_53_V21_AN4__All_L3Absolute_AK5PF.txt",
        "$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/FT_53_V21_AN4__All_L2L3Residual_AK5PF.txt"
    ]

    config['JsonFiles'] = [
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    ]
    config['HltPaths'] = [
        'HLT_PFJET40',
        'HLT_PFJET80',
        'HLT_PFJET140',
        'HLT_PFJET200',
        'HLT_PFJET260',
        'HLT_PFJET320',
        'HLT_PFJET400',
    ]
    # Add HLT Paths to Pipeline for tests
    config['Pipelines']['default']['HltPaths'] = config['HltPaths']
    config['Pipelines']['default']['L1ObjectsPaths'] = [
            'L1SingleJet16', 
            'L1SingleJet36',
            'L1SingleJet68',
            'L1SingleJet128',
            'L1SingleJet128',
            'L1SingleJet128',
            ]
    config['Pipelines']['default']['HltObjectsPaths'] = [
           'PFJet40',
           'PFJet80',
           'PFJet140',
           'PFJet200',
           'PFJet260',
           'PFJet320',
           'PFJet400',
            ]
 
    config['Pipelines']['default']['L1SingleJetThresholds'] = [
            16.,
            36.,
            68.,
            128.,
            128.,
            128.,
            ]
    config['Pipelines']['default']['HltSingleJetThresholds'] = [
            40.,
            80.,
            140.,
            200.,
            260.,
            320.,
            400.,
            ]
    config['Pipelines']['default']['TriggerEfficiencyQuantity'] = 'jet1_pt'

    config['Processors'].insert(0, 'filter:JsonFilter')
    config['Processors'].append('producer:HltProducer')
    config['Processors'].append('filter:HltFilter')
    config['TriggerEfficiencyThresholdPerHLTPath'] = [50., 100., 175., 250., 325., 400., 500.]
    config['Pipelines']['default']['Consumers'].append('TriggerResultsHistogramConsumer')
    config['Pipelines']['2ndlevel'] = {}
    config['Pipelines']['2ndlevel']['Consumers'] = []
    # config['Pipelines']['2ndlevel']['Consumers'].append('TriggerHistogramFitConsumer')
    config['Pipelines']['2ndlevel']['HltPaths'] = config['HltPaths']
    # config['Pipelines']['2ndlevel']['EffectiveLumiPerHLTPath'] = [0.079769, 2.156015, 55.932865, 262.449, 1068.024, 19789.31, 19789.31]
    config['Pipelines']['2ndlevel']['Level'] = 2
    config['Pipelines']['2ndlevel']['PipelineNames'] = ['default']



def isData(nickname):
    if nickname.startswith('Jet_2012'):
        return True
    elif nickname in mc_samples:
        return False
    else:
        return True


def getUserParser():
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--data', default=None, type=bool)
    return parser


# Dict with information about MC samples

mc_samples = {
    'QCDP6_Z2S_S10': {
    'Dataset': '/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
    'NumberGeneratedEvents': 9991674,
    'CrossSection': -1.
    },
    'QCDP8_4C_S10': {'Dataset': '',
                     'NumberGeneratedEvents': -1,
                     'CrossSection': -1.
    },
    'QCDHW_EE3C_S10': {'Dataset': '',
                       'NumberGeneratedEvents': -1,
                       'CrossSection': -1.
    },
}


def walk_dic(node, func):
    """ Walks a dic containing dicts, lists or str and calls func on each leaf"""
    seq_iter = node.keys() if isinstance(node, dict) else xrange(len(node))
    for key in seq_iter:
        if isinstance(node[key], basestring):
            print "running node", node, key
            node[key] = func(node[key])
        elif isinstance(node[key], dict) or isinstance(node[key], list):
            walk_dic(node[key], func)


if __name__ == '__main__':
    main()

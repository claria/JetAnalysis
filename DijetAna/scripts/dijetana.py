#!/usr/bin/env python

import os
import sys
from Artus.Configuration.artusWrapper import ArtusWrapper
import argparse


# Change envs for artus run
kappatools_dir = os.path.join(os.path.expandvars('$CMSSW_BASE'),'src/KappaTools/lib')
kappa_dir = os.path.join(os.path.expandvars('$CMSSW_BASE'),'src/Kappa/lib')
artus_dir = os.path.join(os.path.expandvars('$CMSSW_BASE'),'src/Artus')
os.environ['PATH'] += ":{0}/grid-control".format(os.path.expandvars('$HOME'))
os.environ['LD_LIBRARY_PATH'] += ":{0}:{1}".format(kappatools_dir, kappa_dir)
os.environ['ARTUS_WORK_BASE'] = "/nfs/dust/cms/user/gsieber/ARTUS"
os.environ['ARTUSPATH'] = artus_dir

baseconfig = {
    'InputIsData': '',
    'SkipEvents': 0,
    'EventCount': -1,
    "LumiMetadata": "KLumiMetadata",
    "GenLumiMetadata": "",
    "EventMetadata": "KEventMetadata",
    "EventBlacklist" : [],
    "LumiBlacklist" : [],
    "RunBlacklist" : [],
    "GenEventMetadata": "",
    "FilterMetadata": "",
    "FilterSummary": "",
    "VertexSummary": "offlinePrimaryVerticesSummary",
    "JetEnergyCorrectionParameters": "",
    "JetEnergyCorrectionUncertaintyParameters": "",
    'Processors': [
        'producer:JetCorrectionsProducer',
        'producer:ValidJetsProducer',
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
            ],
            'Quantities': [
                'run',
                'lumi',
                'event',
                'npv',
                'npu',
                'weight',
                'njets',
                'jet1_pt',
                'jet1_eta',
                'jet1_rap',
                'jet1_phi',
                'jet2_pt',
                'jet2_eta',
                'jet2_rap',
                'jet2_phi',
                'trigweight',
                'puweight',
                'pathindex',
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

    # Define cuts
    config['MinLeadingJetPt'] = '50.'
    config['MinLeadingJetRap'] = '0.0'
    config['MaxLeadingJetRap'] = '2.0'

    # Add Producers etc specifict to data/MC
    if isData(nickname):
        SetDataSpecific(nickname)
    else:
        SetMCSpecific(nickname)


    walk_dic(config, os.path.expandvars)
    wrapper.setConfig(config)

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

    if 'MON' in nickname:
        config['HltPaths'] = [
            'HLT_PFJET40',
            'HLT_PFJET80',
            'HLT_PFJET140',
            'HLT_PFJET200',
            'HLT_PFJET260',
            ]
    elif 'HT' in nickname:
        config['HltPaths'] = [
            'HLT_PFJET320'
            ]

    config['TriggerEffPaths'] = ['HLT_PFJET40','HLT_PFJET80','HLT_PFJET140','HLT_PFJET200','HLT_PFJET260','HLT_PFJET320']
    config['TriggerEffThresholds'] = [60., 100., 170., 240., 320., 400.]

    config['HltPathsBlacklist'] = []
    # Add HLT Paths to Pipeline for tests
    config['Pipelines']['default']['HltPaths'] = config['HltPaths']
    config['Pipelines']['default']['L1FilterPattern'] = "(L1SingleJet)([0-9]+)"
    config['Pipelines']['default']['HltFilterPattern'] = "(PFJet)([0-9]+)"
    config['Pipelines']['default']['TriggerEfficiencyQuantity'] = 'jet1_pt'

    config['Processors'].insert(0, 'filter:JsonFilter')
    config['Processors'].append('producer:JetHltProducer')
    config['Processors'].append('filter:HltFilter')
    # config['TriggerEffThresholds'] = [0., 0., 0., 0., 0., 0.,0.]
   # config['Pipelines']['default']['Consumers'].append('TriggerResultsHistogramConsumer')


def isData(nickname):
    if nickname.startswith('Jet_2012'):
        return True
    elif nickname in mc_samples:
        return False
    else:
        return True


def getUserParser():
    parser = argparse.ArgumentParser(add_help=False)
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
            node[key] = func(node[key])
        elif isinstance(node[key], dict) or isinstance(node[key], list):
            walk_dic(node[key], func)


# class ArtusConfig(object):
#
#     _baseconfig = {
#         'SkipEvents': 0,
#         'EventCount': -1,
#         'Processors': [],
#         'InputFiles': [],
#         'OutputPath': 'output.root',
#         'Pipelines': {}
#         }
#
#     def __init__(config=None):
#
#         self._config = config if config else Artusconfig._baseconfig
#
#     def get_config():
#         return self._config
#
#     def add_pipeline(pipeline_name, level=1):
#
#         self._config['Pipelines'][pipeline_name] = {}
#         self._config['Pipelines'][pipeline_name]['Processors'] = []
#         self._config['Pipelines'][pipeline_name]['Consumers'] = []
#
#         if (level > 1):
#             self._config['Pipelines'][pipeline_name]['Level'] = level
#
#     def add_processor(processor_name, processor_type='auto', pipeline_name=None)
#
#         if (processor_type == 'auto'):
#             if ('filter' in processor_name.tolower()):
#                 processor_type = 'filter'
#             elif ('producer' in processor_name.tolower()):
#                 processor_type = 'producer'
#             else:
#                 raise ValueError('Processor type could not be determined automatically. Please specify explicitly')
#
#         if pipeline_name is None:
#             self._config['Processors'].append('{0}:{1}'.format(processor_type, processor_name))
#         else:
#             self._config['Processors'][pipeline_name].append('{0}:{1}'.format(processor_type, processor_name))
#
#     def expand_envs(self, node=None):
#         """ Expands all environment variables in the config."""
#         if node is None:
#             node = self._config
#
#         seq_iter = node.keys() if isinstance(node, dict) else xrange(len(node))
#         for key in seq_iter:
#             if isinstance(node[key], basestring):
#                 node[key] = os.path.expandvars(node[key])
#             elif isinstance(node[key], dict) or isinstance(node[key], list):
#                 self.expand_envs(node[key])
#
#  
#











if __name__ == '__main__':
    main()

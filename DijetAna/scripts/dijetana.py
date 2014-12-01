#!/usr/bin/env python

import os
import sys
from Artus.Configuration.artusWrapper import ArtusWrapper
import argparse


# Change envs for artus run
kappatools_dir = os.path.join(os.path.expandvars('$CMSSW_BASE'),'src/KappaTools/lib')
kappa_dir = os.path.join(os.path.expandvars('$CMSSW_BASE'),'src/Kappa/lib')
artus_dir = os.path.join(os.path.expandvars('$CMSSW_BASE'),'src/Artus')
os.environ['PATH'] += ':{0}/grid-control'.format(os.path.expandvars('$HOME'))
os.environ['LD_LIBRARY_PATH'] += ':{0}:{1}'.format(kappatools_dir, kappa_dir)
os.environ['ARTUS_WORK_BASE'] = '/nfs/dust/cms/user/gsieber/ARTUS'
os.environ['ARTUSPATH'] = artus_dir

def main():

    wrapper = ArtusWrapper('JetAna')

    # Use baseconfig with settings valid for data and all MC
    config = ArtusConfig(wrapper.getConfig())

    # Identify input type based on nick
    nick = wrapper.determineNickname('auto')

    print 'Prepare config for nick \'{0}\'.'.format(nick)

    # Define global cuts
    config['MinLeadingJetPt']  = '50.'
    config['MinLeadingJetRap'] = '0.0'
    config['MaxLeadingJetRap'] = '3.0'
    config['MinSecondJetRap'] = '0.0'
    config['MaxSecondJetRap'] = '3.0'
    # MET
    config['MaxMETSumEtRatio'] = 0.3

    config['LumiMetadata']  = 'KLumiMetadata'
    config['EventMetadata']    = 'KEventMetadata'
    config['VertexSummary']    = 'offlinePrimaryVerticesSummary'
    config['VertexSummary'] = 'offlinePrimaryVerticesSummary'
    config['Processors'] = [
        'producer:JetCorrectionsProducer',
        'producer:ValidJetsProducer',
        'filter:DiJetsFilter',
        'filter:LeadingJetPtFilter',
        'filter:LeadingJetRapFilter',
        'filter:SecondJetRapFilter',
        'filter:METSumEtFilter',
        ]
    config['JetID']   = 'tight'
    config['JetIDVersion']   = '2014'
    config['Jets']    = 'AK5PFJets'
    config['JetArea'] = 'KT6Area'
    config['Met']     = 'PFMET'


    # Define standard pipepeline
    def_pipeline =  {
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
                'dijet_mass',
                'dijet_ystar',
                'dijet_yboost',
                'trigweight',
                'puweight',
                'pathindex',
                'xsweight',
                'ngeneventsweight',
                'genweight',
                'met',
                'sumet',
            ],
            'EventWeight': 'EventWeight'
        }

    config['Pipelines']['default'] = def_pipeline
    # Add Producers etc specific to data/MC
    if nick:
        nick_info = get_nickinfo(nick)
        print nick_info
        if nick_info['is_data']:
            config['InputIsData'] = 'true'
            set_data_specific(config, nick_info)
        else:
            config['InputIsData'] = 'false'
            set_mc_specific(config, nick_info)



    # Expand all environment variables in config
    config.expand_envs()
    wrapper.setConfig(config)
    sys.exit(wrapper.run())


def set_mc_specific(config, nick_info=None):
    config['GenLumiMetadata'] = 'KLumiMetadata'
    config['GenEventMetadata'] = 'KEventMetadata'
    config['GenJets'] = 'AK5GenJets'
    config['Pipelines']['default']['Quantities'].append('genjet1_pt')
    config['Pipelines']['default']['Quantities'].append('genjet1_phi')
    config['Pipelines']['default']['Quantities'].append('genjet1_eta')
    config['Pipelines']['default']['Quantities'].append('genjet1_rap')
    config['Pipelines']['default']['Quantities'].append('gendijet_mass')
    config['PileupWeightFile'] = '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/pileup_weights.root'
    config['Processors'].append('producer:PUWeightProducer')
    config['Processors'].append('producer:CrossSectionWeightProducer')
    config['Processors'].append('producer:GeneratorWeightProducer')
    config['Processors'].append('producer:NumberGeneratedEventsWeightProducer')
    config['JetEnergyCorrectionParameters'] = [
                                                '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V26_L1FastJet_AK5PF.txt',
                                                '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V26_L2Relative_AK5PF.txt',
                                                '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V26_L3Absolute_AK5PF.txt'
                                                ]
    config['NumberGeneratedEvents'] = nick_info.get('sample_size', -1)
    config['CrossSection'] = nick_info.get('crosssection', -1)



def set_data_specific(config, nick_info=None):
    config['LumiMetadata'] = 'KLumiMetadata'
    config['EventMetadata'] = 'KEventMetadata'
    config['TriggerInfos'] = 'KTriggerInfos',
    config['BeamSpot'] = 'offlineBeamSpot',
    config['TriggerObjects'] = 'KTriggerObjects'
    config['TriggerInfos'] = 'KTriggerInfos'
    config['JetEnergyCorrectionParameters'] = [
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/FT53_V21A_AN6_L1FastJet_AK5PF.txt',
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/FT53_V21A_AN6_L2Relative_AK5PF.txt',
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/FT53_V21A_AN6_L3Absolute_AK5PF.txt',
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/FT53_V21A_AN6_L2L3Residual_AK5PF.txt'
    ]
    config['JsonFiles'] = [
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    ]

    if nick_info['data_stream'] == 'MON':
        config['HltPaths'] = ['HLT_PFJET40','HLT_PFJET80','HLT_PFJET140','HLT_PFJET200','HLT_PFJET260']
    elif nick_info['data_stream'] == 'HT':
        config['HltPaths'] = ['HLT_PFJET320']
    else:
        raise ValueError('No stream supplied for dataset')

    # Thresholds when a path gets efficient, need to be ordered increasingly
    config['TriggerEffPaths'] = ['HLT_PFJET40','HLT_PFJET80','HLT_PFJET140','HLT_PFJET200','HLT_PFJET260','HLT_PFJET320']
    config['TriggerEffThresholds'] = [74., 133., 220., 300., 395., 507.]

    config['HltPathsBlacklist'] = []
    # Add HLT Paths to Pipeline for tests
    config['Pipelines']['default']['HltPaths'] = config['HltPaths']
    config['Pipelines']['default']['L1FilterPattern'] = '(L1SingleJet)([0-9]+)'
    config['Pipelines']['default']['HltFilterPattern'] = '(PFJet)([0-9]+)'
    config['Pipelines']['default']['TriggerEfficiencyQuantity'] = 'jet1_pt'

    config['Pipelines']['default']['Consumers'].append('TriggerResultsHistogramConsumer')

    config['Processors'].insert(0, 'filter:JsonFilter')
    config['Processors'].append('producer:JetHltProducer')
    config['Processors'].append('filter:HltFilter')


def get_nickinfo(nick):
    nickinfo = {
            'Jet_2012A_MON' : {'is_data' : True,
                'ilumi'   : -1,
                'data_stream' : 'MON',
                },
            'Jet_2012A_HT' : {'is_data' : True,
                'ilumi'   : -1,
                'data_stream' : 'HT',
                },
            'Jet_2012B_MON' : {'is_data' : True,
                'ilumi'   : -1,
                'data_stream' : 'MON',
                },
            'Jet_2012B_HT' : {'is_data' : True,
                'ilumi'   : -1,
                'data_stream' : 'HT',
                },
            'Jet_2012C_MON' : {'is_data' : True,
                'ilumi'   : -1,
                'data_stream' : 'MON',
                },
            'Jet_2012C_HT' : {'is_data' : True,
                'ilumi'   : -1,
                'data_stream' : 'HT',
                },
            'Jet_2012D_MON' : {'is_data' : True,
                'ilumi'   : -1,
                'data_stream' : 'MON',
                },
            'Jet_2012D_HT' : {'is_data' : True,
                'ilumi'   : -1,
                'data_stream' : 'HT',
                },
            'QCDP6_Z2S_S10': {'is_data' : False,
                'dataset': '/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                'sample_size': 9991674,
                'crosssection': 2.998e10
                },
            'QCDMGP8_Z2S_100to250': {'is_data' : False,
                'dataset': '/QCD_HT-100To250_TuneZ2star_8TeV-madgraph-pythia/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                'sample_size': 50129518,
                'crosssection': 10360000.0
                },
            'QCDMGP8_Z2S_250to500': {'is_data' : False,
                'dataset': '/QCD_HT-250To500_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                'sample_size': 27062078,
                'crosssection': 276000.0
                },
            'QCDMGP8_Z2S_500to1000': {'is_data' : False,
                'dataset': '/QCD_HT-500To1000_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                'sample_size': 30599292,
                'crosssection': 8426.0 
                },
            'QCDMGP8_Z2S_1000toInf': {'is_data' : False,
                    'dataset': '/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 13843863,
                    'crosssection': 204.0
                    },
            'QCDP8_4C_30to50': {'is_data' : False,
                    'dataset': '/QCD_Pt-30to50_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 1000080,
                    'crosssection': 7.5e07,
                    },
            'QCDP8_4C_50to80': {'is_data' : False,
                    'dataset': '/QCD_Pt-50to80_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 1000026,
                    'crosssection': 9.264e06,
                    },
            'QCDP8_4C_80to120': {'is_data' : False,
                    'dataset': '/QCD_Pt-80to120_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 1000054,
                    'crosssection': 1.165e06,
                    },
            'QCDP8_4C_120to170': {'is_data' : False,
                    'dataset': '/QCD_Pt-120to170_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 800064,
                    'crosssection': 1.75e05,
                    },
            'QCDP8_4C_170to300': {'is_data' : False,
                    'dataset': '/QCD_Pt-170to300_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 800046,
                    'crosssection': 3.797e04,
                    },
            'QCDP8_4C_300to470': {'is_data' : False,
                    'dataset': '/QCD_Pt-300to470_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 500038,
                    'crosssection': 1939.,
                    },
            'QCDP8_4C_470to600': {'is_data' : False,
                    'dataset': '/QCD_Pt-470to600_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 500051,
                    'crosssection': 124.9,
                    },
            'QCDP8_4C_600to800': {'is_data' : False,
                    'dataset': '/QCD_Pt-600to800_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 492988,
                    'crosssection': 29.55,
                    },
            'QCDP8_4C_800to1000': {'is_data' : False,
                    'dataset': '/QCD_Pt-800to1000_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 400059,
                    'crosssection': 3.871,
                    },
            'QCDP8_4C_1000to1400': {'is_data' : False,
                    'dataset': '/QCD_Pt-1000to1400_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 400050,
                    'crosssection': 0.8031,
                    },
            'QCDP8_4C_1400to1800': {'is_data' : False,
                    'dataset': '/QCD_Pt-1400to1800_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 200070,
                    'crosssection': 0.03637,
                    },
            'QCDP8_4C_1800toInf': {'is_data' : False,
                    'dataset': '/QCD_Pt-1800_Tune4C_8TeV_pythia8/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
                    'sample_size': 200013,
                    'crosssection': 0.001977,
                    },

            }
    if nick in nickinfo:
        return nickinfo[nick]
    else:
        raise ValueError('Nickname not found in nickinfo dictionary.')

#def walk_dic(node, func):
#    ''' Walks a dic containing dicts, lists or str and calls func on each leaf'''
#    seq_iter = node.keys() if isinstance(node, dict) else xrange(len(node))
#    for key in seq_iter:
#        if isinstance(node[key], basestring):
#            node[key] = func(node[key])
#        elif isinstance(node[key], dict) or isinstance(node[key], list):
#            walk_dic(node[key], func)


class ArtusConfig(dict):

    def __init__(self, config=None):

       self['SkipEvents'] = 0
       self['EventCount'] = -1
       self['Processors'] = []
       self['InputFiles'] = []
       self['OutputPath'] = 'output.root'
       self['Pipelines']  = {}

       if config:
           self.update(config)

    def expand_config(self, nickname=''):

        self.set_global_settings()
        pass

    def add_pipeline(self, pipeline_name, level=1):

        self['Pipelines'][pipeline_name] = {}
        self['Pipelines'][pipeline_name]['Processors'] = []
        self['Pipelines'][pipeline_name]['Consumers'] = []

        if (level > 1):
            self['Pipelines'][pipeline_name]['Level'] = level

    def add_processor(self, processor_name, processor_type='auto', pipeline_name=None):

        if (processor_type == 'auto'):
            if ('filter' in processor_name.tolower()):
                processor_type = 'filter'
            elif ('producer' in processor_name.tolower()):
                processor_type = 'producer'
            else:
                raise ValueError('Processor type could not be determined automatically. Please specify explicitly')

        if pipeline_name is None:
            self['Processors'].append('{0}:{1}'.format(processor_type, processor_name))
        else:
            self['Processors'][pipeline_name].append('{0}:{1}'.format(processor_type, processor_name))

    def expand_envs(self, node=None):
        ''' Expands all environment variables in the config.'''
        if node is None:
            node = self
        seq_iter = node.keys() if isinstance(node, dict) else xrange(len(node))
        for key in seq_iter:
            if isinstance(node[key], basestring):
                node[key] = os.path.expandvars(node[key])
            elif isinstance(node[key], dict) or isinstance(node[key], list):
                self.expand_envs(node[key])

  












if __name__ == '__main__':
    main()

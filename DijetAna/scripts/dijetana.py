#!/usr/bin/env python

import os
import sys
import argparse
import copy
from ConfigParser import RawConfigParser

from Artus.Configuration.artusWrapper import ArtusWrapper

# Change envs for artus run
artus_dir = os.path.join(os.path.expandvars('$CMSSW_BASE'), 'src/Artus')
os.environ['PATH'] += ':{0}/grid-control'.format(os.path.expandvars('$HOME'))
os.environ['ARTUS_WORK_BASE'] = '/nfs/dust/cms/user/gsieber/ARTUS'
os.environ['ARTUSPATH'] = artus_dir


def main():

    wrapper = ArtusWrapper('JetAna')

    # Use baseconfig with settings valid for data and all MC
    config = ArtusConfig(wrapper.getConfig())
    # Identify input type based on nick
    nick = wrapper.determineNickname('auto')


    print 'Prepare config for nick \'{0}\'.'.format(nick)

    config = ArtusConfig(get_basic_config().items() + [(k,v) for (k,v) in config.items() if v])
    # Define standard pipeline
    config.add_pipeline('default', pipeline=get_default_pipeline())

    # Add Producers etc specific to data/MC
    if nick:
        # Read config for all nicknames/datasets
        samples_config = RawConfigParser()
        samples_config.read('/afs/desy.de/user/g/gsieber/dijetana/ana/CMSSW_7_2_3/src/JetAnalysis/DijetAna/data/samples.conf')
        if nick not in samples_config.sections():
            raise ValueError('Nickname {0} not found in samples config.'.format(nick))

        is_data = samples_config.getboolean(nick, 'is_data')

        if is_data:
            config['InputIsData'] = 'true'
            ilumi = samples_config.getfloat(nick, 'ilumi')
            data_stream = samples_config.get(nick, 'data_stream')
            set_data_specific(config,
                              nick=nick,
                              ilumi=ilumi,
                              data_stream=data_stream)
        else:
            sample_size = samples_config.getint(nick, 'sample_size')
            crosssection = samples_config.getfloat(nick, 'crosssection')
            config['InputIsData'] = 'false'
            set_mc_specific(config,
                            nick=nick,
                            sample_size=sample_size,
                            crosssection=crosssection)

    jet1_bins = [(74,114), (114,196), (196,300), (300,468), (468, 790), (790, 3000)]

    for bin in jet1_bins:
        pipeline_name = '{0}_{1}'.format(*bin)
        config['Pipelines'][pipeline_name] = copy.deepcopy(config['Pipelines']['default'])
        config['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:LeadingJetPtFilter')
        config['Pipelines'][pipeline_name]['MinLeadingJetPt'] = bin[0]
        config['Pipelines'][pipeline_name]['MaxLeadingJetPt'] = bin[1]

    # Expand all environment variables in config
    config.expand_envs()
    wrapper.setConfig(config)
    sys.exit(wrapper.run())

def get_basic_config():

    config = ArtusConfig()
    # Valid Jet Selection
    config['ValidJetsInput'] = 'corrected'
    config['JetID'] = 'tight'
    config['JetIDVersion'] = '2014'
    # Valid Jet Selection
    config['MinValidJetPt'] = '50.'
    config['MaxValidJetAbsRap'] = '3.0'
    # Global Cuts
    config['MinValidJets'] = '2'
    config['MinLeadingJetPt'] = '74.'
    # Define global cuts
    # MET
    config['MaxMETSumEtRatio'] = 0.3
    config['MaxPrimaryVertexZ'] = 24.
    config['MaxPrimaryVertexRho'] = 2.
    config['MinPrimaryVertexFitnDOF'] = 4

    config['TrackSummary'] = 'generalTracksSummary'
    config['MinPurityRatio'] = 'trackSummary'
    config['HCALNoiseSummary'] = 'hcalnoise'

    config['LumiMetadata'] = 'lumiInfo'
    config['EventMetadata'] = 'eventInfo'
    config['VertexSummary'] = 'offlinePrimaryVerticesSummary'
    config['Processors'] = [
        'producer:JetCorrectionsProducer',
        'producer:ValidJetsProducer',
        'filter:HCALNoiseFilter',
        'filter:NJetsFilter',
        'filter:METSumEtFilter',
        'filter:LeadingJetPtFilter',
        'filter:GoodPrimaryVertexFilter',
    ]
    config['BasicJets'] = 'ak7PFJets'
    config['PileupDensity'] = 'KT6Area'
    config['Met'] = 'met'
    # No pipelines
    config['Pipelines'] = {}

    return config

def get_default_pipeline():

    pipeline = {}
    pipeline['EventWeight'] = 'EventWeight'
    pipeline['Processors'] = [
                              'producer:JetQuantities',
                              'producer:EventWeightProducer'
                              ]
    pipeline['Consumers'] =  [
                              'KappaLambdaNtupleConsumer',
                              'cutflow_histogram',
                              'JetQuantitiesHistogramConsumer',
                              ]
    pipeline['Quantities'] = [
                              # 'run',
                              # 'lumi',
                              # 'event',
                              'npv',
                              # 'npu',
                              'weight',
                              'njets',
                              # 'incjets_pt',
                              # 'incjets_eta',
                              # 'incjets_rap',
                              # 'incjets_phi',
                              'jet1_pt',
                              'jet1_eta',
                              'jet1_rap',
                              'jet1_phi',
                              'jet2_pt',
                              'jet2_eta',
                              'jet2_rap',
                              'jet2_phi',
                              # 'dijet_mass',
                              # 'dijet_ystar',
                              # 'dijet_yboost',
                              # 'trigweight',
                              # 'puweight',
                              # 'pathindex',
                              # 'xsweight',
                              # 'ngeneventsweight',
                              # 'genweight',
                              # 'met',
                              # 'sumet',
                              ]
    return pipeline


def set_mc_specific(config, nick='', sample_size=-1, crosssection=-1.):
    config['GenLumiMetadata'] = 'lumiInfo'
    config['GenEventMetadata'] = 'eventInfo'
    config['GenJets'] = 'ak7GenJets'
    config['Pipelines']['default']['Quantities'].append('genjet1_pt')
    config['Pipelines']['default']['Quantities'].append('genjet1_phi')
    config['Pipelines']['default']['Quantities'].append('genjet1_eta')
    config['Pipelines']['default']['Quantities'].append('genjet1_rap')
    # config['Pipelines']['default']['Quantities'].append('gendijet_mass')
    config['PileupWeightFile'] = '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/pileup/pileup_weights_S10.root'
    config['Processors'].append('producer:PUWeightProducer')
    config['Processors'].append('producer:CrossSectionWeightProducer')
    config['Processors'].append('producer:GeneratorWeightProducer')
    config['Processors'].append('producer:NumberGeneratedEventsWeightProducer')
    config['Pipelines']['default']['Consumers'].append('JetGenQuantitiesHistogramConsumer')
    config['JetEnergyCorrectionParameters'] = ['$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V27_L1FastJet_AK7PF.txt',
                                               '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V27_L2Relative_AK7PF.txt',
                                               '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V27_L3Absolute_AK7PF.txt'
                                               ]
    config['NumberGeneratedEvents'] = sample_size
    config['CrossSection'] = crosssection


def set_data_specific(config, nick='', ilumi=-1., data_stream=''):
    config['LumiMetadata']   = 'lumiInfo'
    config['EventMetadata']  = 'eventInfo'
    config['BeamSpot']       = 'offlineBeamSpot',
    config['TriggerObjects'] = 'triggerObjects'
    config['TriggerInfos']   = 'triggerObjectMetadata'
    config['JetEnergyCorrectionParameters'] = [
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_L1FastJet_AK7PF.txt',
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_L2Relative_AK7PF.txt',
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_L3Absolute_AK7PF.txt',
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_L2L3Residual_AK7PF.txt'
    ]
    config['JsonFiles'] = [
        '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/json/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    ]

    # Trigger Selection
    if nick == 'Jet_2012A_MON':
        config['HltPaths'] = ['HLT_PFJET40', 'HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260', 'HLT_PFJET320']
    elif data_stream == 'MON':
        config['HltPaths'] = ['HLT_PFJET40', 'HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260']
    elif data_stream == 'HT':
        config['HltPaths'] = ['HLT_PFJET320']
    else:
        raise ValueError('No stream supplied for dataset')

    # Thresholds when a path gets efficient, need to be ordered increasingly
    config['TriggerEffPaths'] = ['HLT_PFJET40', 'HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260', 'HLT_PFJET320']
    config['TriggerEffThresholds'] = [74., 133., 220., 300., 395., 507., 9999999.]

    config['HltPathsBlacklist'] = []
    config['Pipelines']['default']['L1FilterThresholds'] = [16., 36., 68., 92., 128., 128.]
    config['Pipelines']['default']['HltFilterThresholds'] = [40., 80., 140., 200., 260., 320.]
    # config['Pipelines']['default']['L1FilterPattern'] = '(L1SingleJet)([0-9]+)'
    # config['Pipelines']['default']['HltFilterPattern'] = '(PFJet)([0-9]+)'
    config['Pipelines']['default']['TriggerEfficiencyQuantity'] = 'jet1_pt'
    # config['Pipelines']['default']['Consumers'].append('TriggerResultsHistogramConsumer')
    config.add_processor('filter:JsonFilter', idx=0)
    config.add_processor('producer:JetHltProducer', after='filter:NJetsFilter')
    config.add_processor('filter:JetHltFilter', after='producer:JetHltProducer')

# def walk_dic(node, func):
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
        self['Pipelines'] = {}

        if config:
            self.update(config)

    def expand_config(self, nickname=''):

        self.set_global_settings()
        pass

    def add_pipeline(self, pipeline_name, pipeline=None, level=1):

        if pipeline:
            self['Pipelines'][pipeline_name] = pipeline
        else:
            self['Pipelines'][pipeline_name] = {}
            self['Pipelines'][pipeline_name]['Processors'] = []
            self['Pipelines'][pipeline_name]['Consumers'] = []

        if (level > 1):
            self['Pipelines'][pipeline_name]['Level'] = level

    def add_processor(self, processor_name, processor_type='auto', pipeline_name=None, idx=None, before=None, after=None):

        if pipeline_name is None:
            processor_list = self['Processors']
        else:
            processor_list = self[pipeline_name]['Processors']

        if (processor_type == 'auto'):
            if ('filter' in processor_name.lower()):
                processor_type = 'filter'
            elif ('producer' in processor_name.lower()):
                processor_type = 'producer'
            else:
                raise ValueError('Processor type could not be determined automatically. Please specify explicitly')


        if idx is not None:
            pass
        elif before is not None:
            idx = processor_list.index(before)
        elif after is not None:
            idx = processor_list.index(after) + 1
        else:
            idx = len(processor_list)

        if 'filter:' in processor_name.lower() or 'producer:' in processor_name.lower():
            processor_str = '{0}'.format(processor_name)
        else:
            processor_str = '{0}:{1}'.format(processor_type, processor_name)
        processor_list.insert(idx, processor_str)

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

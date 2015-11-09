import os
import copy
import numpy as np
from ConfigParser import RawConfigParser

def get_pt_binning(ybys_bin):

    pt_binning = {
            'yb0ys0' : [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116],
            'yb0ys1' : [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032],
            'yb0ys2' : [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430],
            'yb1ys0' : [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327],
            'yb1ys1' : [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686],
            'yb2ys0' : [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592]
            }

    return pt_binning(ybys_bin)

class BaseConfig(dict):
    """ Starting config all other configs should derive from. Therefore, processors Filters etc should not be dependent
        if running on MC/DATA or GEN.
    """

    def __init__(self, nick=None, config=None):

        # Configs containing additional infos about xs, number of gen. events etc.
        samples_config = RawConfigParser(allow_no_value=True)
        samples_config.read(os.path.expandvars('$CMSSW_BASE/src/JetAnalysis/DijetAna/data/samples.conf'))

        self.add_global_settings()
        # Add a default pipeline
        self.add_default_pipeline()

        self.is_data = None
        if nick:
            self['nick'] = nick
            # Read config for all nicknames/datasets
            if nick not in samples_config.sections():
                raise ValueError('Nickname {0} not found in samples config.'.format(nick))

            self.is_data = samples_config.getboolean(nick, 'is_data')

            if self.is_data:
                self['InputIsData'] = 'true'
                ilumi = samples_config.getfloat(nick, 'ilumi')
                data_stream = samples_config.get(nick, 'data_stream')
                self.add_data_settings(ilumi=ilumi, data_stream=data_stream)
            else:
                self['InputIsData'] = 'false'
                sample_size = samples_config.getint(nick, 'sample_size')
                crosssection = samples_config.getfloat(nick, 'crosssection')
                self.add_mc_settings(sample_size=sample_size, crosssection=crosssection)
        else:
            self['nick'] = 'None'

        # Overwrite defined settings
        self.modify_settings()
        # Expand/multiply pipelines
        self.expand_pipelines()
        # Loop and replace all envs.
        self.expand_envs()

    def modify_settings(self):
        pass

    def expand_pipelines(self):
        pass

    def add_global_settings(self):

        self['SkipEvents'] = 0
        self['EventCount'] = -1
        self['Processors'] = []
        self['InputFiles'] = []
        self['OutputPath'] = 'output.root'
        self['Pipelines'] = {}

        # Observable binning in leading jet pT
        # Rapidity Binning in steps of 1
        self['RapidityAbsBinning'] = [0.0, 1.0, 2.0, 3.0]
        self['RapidityBinning'] = [-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0]
        # Rapidity Binning in steps of 0.5
        self['FineRapidityAbsBinning'] = [0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0]
        self['FineRapidityBinning'] = [-3.0, -2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0]
        # Pt Binning
        self['PtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['GenPtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        # self['GenPtBinning'] = [36, 44, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['TripleDiffPtBinning'] = [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['TripleDiffGenPtBinning'] = self['TripleDiffPtBinning']
        # Valid Jet Selection
        self['ValidJetsInput'] = 'corrected'
        self['JetID'] = 'tight'
        self['JetIDVersion'] = '2014'
        # Valid Jet Selection
        self['MinValidJetPt'] = '50.'
        self['MinValidGenJetPt'] = '50.'
        self['MaxValidJetAbsRap'] = '5.0'
        self['MaxValidGenJetAbsRap'] = '5.0'
        # Global Cuts
        self['MinValidJets'] = '2'
        self['MinValidGenJets'] = '2'
        # Rapidity cuts on leading two jets
        self['MaxDijetsAbsRap'] = '3.0'
        self['MaxGenDijetsAbsRap'] = '3.0'
        self['MinLeadingJetPt'] = '74.'
        self['MinLeadingGenJetPt'] = '74.'
        self['MinPtAvg'] = '133.'
        self['MinGentPtAvg'] = '133.'


        # Gen Jet Matching
        self['MaxDeltaR'] = 0.3
        self['MaxDeltaPtRel'] = 1E9
        # Define global cuts
        # MET
        self['MaxMETSumEtRatio'] = 0.3
        self['MaxPrimaryVertexZ'] = 24.
        self['MaxPrimaryVertexRho'] = 2.
        self['MinPrimaryVertexFitnDOF'] = 4

        # Event weight
        self['EventWeight'] = 'EventWeight'

        self['LumiMetadata'] = 'lumiInfo'
        self['EventMetadata'] = 'eventInfo'
        self['VertexSummary'] = 'offlinePrimaryVerticesSummary'
        self['Processors'] = [
            'producer:JetCorrectionsProducer',
            'producer:ValidJetsProducer',
            ]
        self['BasicJets'] = 'ak7PFJets'
        self['PileupDensity'] = 'KT6Area'
        self['Met'] = 'met'
        # No pipelines
        self['Pipelines'] = {}



    def add_default_pipeline(self):
        """ Adds an empty pipeline."""

        if not self['Pipelines']:
            self['Pipelines'] = {}

        self['Pipelines']['default'] = {}

        pipeline = self['Pipelines']['default']
        pipeline['Processors'] = [
                                  ]
        pipeline['Consumers'] =  []
        pipeline['Quantities'] = []

    def add_mc_settings(self, sample_size=-1, crosssection=-1.):
        """ Global settings to be applied for MC."""
        self['GenJets'] = 'ak7GenJets'
        self['GenLumiMetadata'] = 'lumiInfo'
        self['GenEventMetadata'] = 'eventInfo'
        self['PileupWeightFile'] = '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/pileup/pileup_weights_S10.root'
        self.add_processor('producer:ValidGenJetsProducer', after='producer:JetCorrectionsProducer')
        self.add_processor('producer:GenJetMatchingProducer', after='producer:ValidJetsProducer')
        self.add_processor('producer:GenJetQuantitiesProducer', after='producer:ValidGenJetsProducer')
        self.add_processor('producer:JetQuantitiesProducer', after='producer:ValidJetsProducer')
        self.add_processor('producer:PUWeightProducer', after='producer:ValidJetsProducer')
        self.add_processor('producer:CrossSectionWeightProducer', after='producer:ValidJetsProducer')
        self.add_processor('producer:GeneratorWeightProducer', after='producer:ValidJetsProducer')
        self.add_processor('producer:NumberGeneratedEventsWeightProducer', after='producer:ValidJetsProducer')
        self.add_processor('EventWeightProducer', after='producer:GenJetMatchingProducer')
        self['JetEnergyCorrectionParameters'] = ['$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V27_L1FastJet_AK7PF.txt',
                                                 '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V27_L2Relative_AK7PF.txt',
                                                 '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V27_L3Absolute_AK7PF.txt'
                                                ]
        self['NumberGeneratedEvents'] = sample_size
        self['CrossSection'] = crosssection


    def add_data_settings(self, ilumi=-1., data_stream=''):
        self['LumiMetadata']   = 'lumiInfo'
        self['EventMetadata']  = 'eventInfo'
        self['BeamSpot']       = 'offlineBeamSpot',
        self['TriggerObjects'] = 'triggerObjects'
        self['TriggerInfos']   = 'triggerObjectMetadata'
        self['JetEnergyCorrectionParameters'] = [
            '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_L1FastJet_AK7PF.txt',
            '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_L2Relative_AK7PF.txt',
            '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_L3Absolute_AK7PF.txt',
            '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_L2L3Residual_AK7PF.txt'
        ]
        self['JetEnergyCorrectionUncertaintyParameters'] = '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_UncertaintySources_AK7PF.txt'
        self['JetEnergyCorrectionUncertaintySource'] = 'Total'
        # No JEC shift, disables the JEC Uncertainties...
        self['JetEnergyCorrectionUncertaintyShift'] = 0.0

        self['JsonFiles'] = [
            '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/json/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        ]

        # Trigger Selection
        if self['nick'] == 'Jet_2012A_MON':
            self['HltPaths'] = ['HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260', 'HLT_PFJET320']
        elif data_stream == 'MON':
            self['HltPaths'] = ['HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260']
        elif data_stream == 'HT':
            self['HltPaths'] = ['HLT_PFJET320']
        else:
            raise ValueError('No stream supplied for dataset')

        # Thresholds when a path gets efficient, need to be ordered increasingly
        self['TriggerEffPaths'] = ['HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260', 'HLT_PFJET320']
        self['TriggerEffThresholds'] = [123., 192., 263., 353., 412., 3000.]

        self.add_processor('filter:JsonFilter', idx=0)
        self.add_processor('producer:JetQuantitiesProducer', after='producer:ValidJetsProducer')
        self.add_processor('producer:JetHltProducer')
        self.add_processor('producer:EventWeightProducer', after='producer:JetHltProducer')
        self.add_processor('producer:LuminosityWeightProducer', before='producer:EventWeightProducer')



# =================================================
# Starting from here only helper functions defined.
# =================================================

    def producer_before_filter(self, pipeline=None):
        if pipeline is None:
            pipeline = self
        sort_list = pipeline['Processors']
        pipeline['Processors'] = [item for item in sort_list if item.split(':')[0].lower() == 'producer'] + [item for item in sort_list if item.split(':')[0].lower() == 'filter']



    def add_pipeline(self, pipeline_name, pipeline=None, level=1):

        if pipeline:
            self['Pipelines'][pipeline_name] = pipeline
        else:
            self['Pipelines'][pipeline_name] = {}
            self['Pipelines'][pipeline_name]['Processors'] = []
            self['Pipelines'][pipeline_name]['Consumers'] = []

        if (level > 1):
            self['Pipelines'][pipeline_name]['Level'] = level

    def get_default_pipeline(self):
        return self['Pipelines']['default']


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

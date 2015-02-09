import os
import copy
from ConfigParser import RawConfigParser

class BaseConfig(dict):

    def __init__(self, nick=None, config=None):

        # Configs containing additional infos about xs, number of gen. events etc.
        samples_config = RawConfigParser(allow_no_value=True)
        samples_config.read('/afs/desy.de/user/g/gsieber/dijetana/ana/CMSSW_7_2_3/src/JetAnalysis/DijetAna/data/samples.conf')

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
                self.add_data_settings(config, ilumi=ilumi, data_stream=data_stream)
            else:
                self['InputIsData'] = 'false'
                sample_size = samples_config.getint(nick, 'sample_size')
                crosssection = samples_config.getfloat(nick, 'crosssection')
                self.add_mc_settings(config, sample_size=sample_size, crosssection=crosssection)
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

        # Valid Jet Selection
        self['ValidJetsInput'] = 'corrected'
        self['JetID'] = 'tight'
        self['JetIDVersion'] = '2014'
        # Valid Jet Selection
        self['MinValidJetPt'] = '50.'
        self['MaxValidJetAbsRap'] = '3.0'
        # Global Cuts
        self['MinValidJets'] = '2'
        self['MinLeadingJetPt'] = '74.'
        # Define global cuts
        # MET
        self['MaxMETSumEtRatio'] = 0.3
        self['MaxPrimaryVertexZ'] = 24.
        self['MaxPrimaryVertexRho'] = 2.
        self['MinPrimaryVertexFitnDOF'] = 4

        self['TrackSummary'] = 'generalTracksSummary'
        self['MinPurityRatio'] = 'trackSummary'
        self['HCALNoiseSummary'] = 'hcalnoise'

        self['LumiMetadata'] = 'lumiInfo'
        self['EventMetadata'] = 'eventInfo'
        self['VertexSummary'] = 'offlinePrimaryVerticesSummary'
        self['Processors'] = [
            'producer:JetCorrectionsProducer',
            'producer:ValidJetsProducer',
            'filter:HCALNoiseFilter',
            'filter:NJetsFilter',
            'filter:METSumEtFilter',
            'filter:LeadingJetPtFilter',
            'filter:GoodPrimaryVertexFilter',
        ]
        self['BasicJets'] = 'ak7PFJets'
        self['PileupDensity'] = 'KT6Area'
        self['Met'] = 'met'
        # No pipelines
        self['Pipelines'] = {}



    def add_default_pipeline(self):

        if not self['Pipelines']:
            self['Pipelines'] = {}

        self['Pipelines']['default'] = {}

        pipeline = self['Pipelines']['default']
        pipeline['EventWeight'] = 'EventWeight'
        pipeline['Processors'] = [
                                  'producer:JetQuantities',
                                  'producer:EventWeightProducer'
                                  ]
        pipeline['Consumers'] =  []
        pipeline['Quantities'] = []
        # Observable binning in leading jet pT
        pipeline['ObservableBinning'] = [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]

    def add_mc_settings(self, nick='', sample_size=-1, crosssection=-1.):
        self['GenLumiMetadata'] = 'lumiInfo'
        self['GenEventMetadata'] = 'eventInfo'
        self['GenJets'] = 'ak7GenJets'
       # self['Pipelines']['default']['Quantities'].append('gendijet_mass')
        self['PileupWeightFile'] = '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/pileup/pileup_weights_S10.root'
        self['Processors'].append('producer:PUWeightProducer')
        self['Processors'].append('producer:CrossSectionWeightProducer')
        self['Processors'].append('producer:GeneratorWeightProducer')
        self['Processors'].append('producer:NumberGeneratedEventsWeightProducer')
        self['JetEnergyCorrectionParameters'] = ['$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V27_L1FastJet_AK7PF.txt',
                                                   '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V27_L2Relative_AK7PF.txt',
                                                   '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/START53_V27_L3Absolute_AK7PF.txt'
                                                   ]
        self['NumberGeneratedEvents'] = sample_size
        self['CrossSection'] = crosssection

        # self['Pipelines']['epilog']['Consumers'].append('JetResolutionConsumer')


    def add_data_settings(self, nick='', ilumi=-1., data_stream=''):
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
        self['JsonFiles'] = [
            '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/json/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        ]

        # Trigger Selection
        if nick == 'Jet_2012A_MON':
            self['HltPaths'] = ['HLT_PFJET40', 'HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260', 'HLT_PFJET320']
        elif data_stream == 'MON':
            self['HltPaths'] = ['HLT_PFJET40', 'HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260']
        elif data_stream == 'HT':
            self['HltPaths'] = ['HLT_PFJET320']
        else:
            raise ValueError('No stream supplied for dataset')

        # Thresholds when a path gets efficient, need to be ordered increasingly
        self['TriggerEffPaths'] = ['HLT_PFJET40', 'HLT_PFJET80', 'HLT_PFJET140', 'HLT_PFJET200', 'HLT_PFJET260', 'HLT_PFJET320']
        self['TriggerEffThresholds'] = [74., 133., 220., 300., 395., 507., 9999999.]

        self['HltPathsBlacklist'] = []
        self['Pipelines']['default']['L1FilterThresholds'] = [16., 36., 68., 92., 128., 128.]
        self['Pipelines']['default']['HltFilterThresholds'] = [40., 80., 140., 200., 260., 320.]
        # self['Pipelines']['default']['L1FilterPattern'] = '(L1SingleJet)([0-9]+)'
        # self['Pipelines']['default']['HltFilterPattern'] = '(PFJet)([0-9]+)'
        self['Pipelines']['default']['TriggerEfficiencyQuantity'] = 'jet1_pt'
        # self['Pipelines']['default']['Consumers'].append('TriggerResultsHistogramConsumer')
        self.add_processor('filter:JsonFilter', idx=0)
        self.add_processor('producer:JetHltProducer', after='filter:NJetsFilter')
        self.add_processor('filter:JetHltFilter', after='producer:JetHltProducer')





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


class RunConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(RunConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):
        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  'KappaLambdaNtupleConsumer',
                                  'cutflow_histogram',
                                  'JetQuantitiesHistogramConsumer',
                                  ]
        default_pipeline['Quantities'] = [
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

        if self.is_data is True:
            pass
        elif self.is_data is False:
            default_pipeline['Consumers'].append('JetGenQuantitiesHistogramConsumer')
            default_pipeline['Quantities'].append('genjet1_pt')
            default_pipeline['Quantities'].append('genjet1_phi')
            default_pipeline['Quantities'].append('genjet1_eta')
            default_pipeline['Quantities'].append('genjet1_rap')
 
    def expand_pipelines(self):
        jet1_bins = [(74,114), (114,196), (196,300), (300,468), (468, 790), (790, 3000)]

        for bin in jet1_bins:
            pipeline_name = '{0}_{1}'.format(*bin)
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:LeadingJetPtFilter')
            self['Pipelines'][pipeline_name]['MinLeadingJetPt'] = bin[0]
            self['Pipelines'][pipeline_name]['MaxLeadingJetPt'] = bin[1]

class UnfoldingResponseConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(UnfoldingResponseConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on MC")

    def modify_settings(self):
        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  'cutflow_histogram',
                                  'JetUnfoldingResponseConsumer',
                                  ]


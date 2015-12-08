from runconfig import RunConfig
from baseconfig import BaseConfig
import copy

class JECConfig(RunConfig):

    def __init__(self, nick=None, config=None):
        super(JECConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):
        super(JECConfig, self).modify_settings()
        default_pipeline = self.get_default_pipeline()
        self['Processors'] = ["filter:JsonFilter", 
                              "filter:GoodPrimaryVertexFilter", 
                              "filter:METSumEtFilter",
                              "producer:JetCorrectionsProducer",]
        default_pipeline['Processors'] = [
                "producer:JetCorrectionsUncertaintyProducer", 
                "producer:JetValidJetsProducer", 
                "producer:LuminosityWeightProducer", 
                "producer:JetQuantitiesProducer", 
                "producer:JetHltProducer", 
                "producer:EventWeightProducer", 
                "filter:PtAvgFilter",
                "filter:DijetsRapFilter",
                "filter:JetHltFilter",
                "filter:NJetsFilter", 
                ]


    def expand_pipelines(self):

        jec_unc_names  = [
                          'AbsoluteScale','AbsoluteStat','AbsoluteMPFBias',
                          'Fragmentation',
                          'SinglePionECAL',
                          'SinglePionHCAL',
                          'FlavorQCD',
                          'TimeEta',
                          'TimePt',
                          'RelativeJEREC1','RelativeJEREC2','RelativeJERHF',
                          'RelativePtBB','RelativePtEC1','RelativePtEC2','RelativePtHF',
                          'RelativeFSR',
                          'RelativeStatEC2', 'RelativeStatHF', 'RelativeStatFSR',
                          'PileUpDataMC',
                          'PileUpPtRef',
                          'PileUpPtBB','PileUpPtEC1','PileUpPtEC2','PileUpPtHF',
                          'PileUpEnvelope',
                          'Total',
                          ]

        for i, j in [(0,0), (0, 1), (0, 2), (1, 0), (1, 1), (2, 0)]:
            yb_lo = self['RapidityAbsBinning'][i]
            yb_hi = self['RapidityAbsBinning'][i+1]
            ys_lo = self['RapidityAbsBinning'][j]
            ys_hi = self['RapidityAbsBinning'][j+1]
            # reco pipelines
            pipeline_name = 'yb{0}ys{1}'.format(i, j)
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name]['Processors'].insert(self['Pipelines'][pipeline_name]['Processors'].index('producer:JetQuantitiesProducer') + 1,'filter:YBoostFilter')
            self['Pipelines'][pipeline_name]['Processors'].insert(self['Pipelines'][pipeline_name]['Processors'].index('producer:JetQuantitiesProducer') + 1,'filter:YStarFilter')
            self['Pipelines'][pipeline_name]['MinYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxYBoost'] = yb_hi
            self['Pipelines'][pipeline_name]['JetEnergyCorrectionUncertaintyShift'] = 0.0
            self['Pipelines'][pipeline_name]['JetEnergyCorrectionUncertaintySource'] = "Total"

            for source in jec_unc_names:
                # Upwards 
                pipeline_name_up = '{0}_{1}_up'.format(pipeline_name, source)
                self['Pipelines'][pipeline_name_up] = copy.deepcopy(self['Pipelines'][pipeline_name])
                self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintySource'] = source
                self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintyShift'] = 1.0
                # Downwards 
                pipeline_name_dn = '{0}_{1}_dn'.format(pipeline_name, source)
                self['Pipelines'][pipeline_name_dn] = copy.deepcopy(self['Pipelines'][pipeline_name])
                self['Pipelines'][pipeline_name_dn]['JetEnergyCorrectionUncertaintySource'] = source
                self['Pipelines'][pipeline_name_dn]['JetEnergyCorrectionUncertaintyShift'] = -1.0

        del self['Pipelines']['default']


class GenJECConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(GenJECConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on GEN")

    def modify_settings(self):
        self['GenLumiMetadata'] = 'lumiInfo'
        self['GenEventMetadata'] = 'eventInfo'
        self['GenJets'] = 'ak7GenJets'
        self['GenParticles'] = ''
        self['TrackSummary'] = ''
        self['VertexSummary'] = ''
        self['BasicJets'] = ''
        self['PileupDensity'] = ''
        self['Met'] = ''


        self['MinValidGenJetPt'] = 50.
        self['MaxValidGenJetAbsRap'] = 3.
        self['MinValidGenJets'] = 2
        self['MinLeadingGenJetPt'] = 74.

        self['Processors'] = [

        ]
        default_pipeline = self.get_default_pipeline()
        self['JetEnergyCorrectionParameters'] = []
        self['JetEnergyCorrectionUncertaintyParameters'] = '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_UncertaintySources_AK7PF.txt'
        self['JetEnergyCorrectionUncertaintySource'] = 'Total'
        # No JEC shift, disables the JEC Uncertainties...
        self['JetEnergyCorrectionUncertaintyShift'] = 0.0

        default_pipeline['Processors'] = [
            'producer:ValidGenJetsProducer',
            'producer:GenJetCorrectionsUncertaintyProducer',
            'producer:GenJetQuantitiesProducer',
            'producer:CrossSectionWeightProducer',
            'producer:GeneratorWeightProducer',
            'producer:NumberGeneratedEventsWeightProducer',
            'producer:EventWeightProducer',
            'filter:NGenJetsFilter',
            'filter:LeadingGenJetPtFilter'
            ]
        default_pipeline['Consumers'] =  [
                                  'cutflow_histogram',
                                  'GenJetQuantitiesHistogramConsumer',
                                  ]


    def expand_pipelines(self):

        jec_unc_names  = [
                          'AbsoluteScale','AbsoluteStat','AbsoluteMPFBias',
                          'Fragmentation',
                          'SinglePionECAL',
                          'SinglePionHCAL',
                          'FlavorQCD',
                          'TimeEta',
                          'TimePt',
                          'RelativeJEREC1','RelativeJEREC2','RelativeJERHF',
                          'RelativePtBB','RelativePtEC1','RelativePtEC2','RelativePtHF',
                          'RelativeFSR',
                          'RelativeStatEC2', 'RelativeStatHF', 'RelativeStatFSR',
                          'PileUpDataMC',
                          'PileUpPtRef',
                          'PileUpPtBB','PileUpPtEC1','PileUpPtEC2','PileUpPtHF',
                          'PileUpEnvelope',
                          'Total',
                          ]
        jec_unc_names = ['Total']

        for i, j in [(0,0), (0, 1), (0, 2), (1, 0), (1, 1), (2, 0)][:1]:
            yb_lo = self['RapidityAbsBinning'][i]
            yb_hi = self['RapidityAbsBinning'][i+1]
            ys_lo = self['RapidityAbsBinning'][j]
            ys_hi = self['RapidityAbsBinning'][j+1]
            # reco pipelines
            pipeline_name = 'yb{0}ys{1}'.format(i, j)
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name]['Processors'].append('filter:GenYBoostFilter')
            self['Pipelines'][pipeline_name]['Processors'].append('filter:GenYStarFilter')
            self['Pipelines'][pipeline_name]['GenMinYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['GenMaxYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['GenMinYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['GenMaxYBoost'] = yb_hi
            self['Pipelines'][pipeline_name]['JetEnergyCorrectionUncertaintyShift'] = 0.0
            self['Pipelines'][pipeline_name]['JetEnergyCorrectionUncertaintySource'] = "Total"

            for source in jec_unc_names:
                # Upwards 
                pipeline_name_up = '{0}_{1}_up'.format(pipeline_name, source)
                self['Pipelines'][pipeline_name_up] = copy.deepcopy(self['Pipelines'][pipeline_name])
                self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintySource'] = source
                self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintyShift'] = 1.0
                # Downwards 
                pipeline_name_dn = '{0}_{1}_dn'.format(pipeline_name, source)
                self['Pipelines'][pipeline_name_dn] = copy.deepcopy(self['Pipelines'][pipeline_name])
                self['Pipelines'][pipeline_name_dn]['JetEnergyCorrectionUncertaintySource'] = source
                self['Pipelines'][pipeline_name_dn]['JetEnergyCorrectionUncertaintyShift'] = -1.0

        del self['Pipelines']['default']


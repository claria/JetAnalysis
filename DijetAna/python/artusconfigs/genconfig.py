from baseconfig import BaseConfig
import copy


class GenConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(GenConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on GEN")

    def modify_settings(self):
        self['GenLumiMetadata'] = 'lumiInfo'
        self['GenEventMetadata'] = 'eventInfo'
        self['GenJets'] = 'ak7GenJets'
        self['TrackSummary'] = ''
        self['VertexSummary'] = ''
        self['BasicJets'] = ''
        self['PileupDensity'] = ''
        self['Met'] = ''


        self['MinValidGenJetPt'] = 50.
        self['MaxValidGenJetAbsRap'] = 3.
        self['MinValidGenJets'] = 2
        self['MinLeadingGenJetPt'] = 74.

        self['MaxDeltaR'] = 0.3
        self['GenPtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['PtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]

        self['Processors'] = [
            'producer:ValidGenJetsProducer',
            'producer:GenJetQuantitiesProducer',
            'producer:CrossSectionWeightProducer',
            'producer:GeneratorWeightProducer',
            'producer:NumberGeneratedEventsWeightProducer',
            'producer:EventWeightProducer',
            'filter:NGenJetsFilter',
            'filter:LeadingGenJetPtFilter'

        ]

        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  'GenJetQuantitiesHistogramConsumer',
                                  ]


    def expand_pipelines(self):
        for i in range(0, len(self['RapidityAbsBinning']) -1):
            for j in range(0, len(self['RapidityAbsBinning']) -1):
                yb_lo = self['RapidityAbsBinning'][i]
                yb_hi = self['RapidityAbsBinning'][i+1]
                ys_lo = self['RapidityAbsBinning'][j]
                ys_hi = self['RapidityAbsBinning'][j+1]
                # gen pipelines
                if self.is_data is False:
                    gen_pipeline_name = 'gen_yb{0}ys{1}'.format(i, j)
                    self['Pipelines'][gen_pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
                    self['Pipelines'][gen_pipeline_name]['Processors'].insert(0,'filter:GenYStarFilter')
                    self['Pipelines'][gen_pipeline_name]['Processors'].insert(0,'filter:GenYBoostFilter')
                    self['Pipelines'][gen_pipeline_name]['MinGenYStar'] = ys_lo
                    self['Pipelines'][gen_pipeline_name]['MaxGenYStar'] = ys_hi
                    self['Pipelines'][gen_pipeline_name]['MinGenYBoost'] = yb_lo
                    self['Pipelines'][gen_pipeline_name]['MaxGenYBoost'] = yb_hi


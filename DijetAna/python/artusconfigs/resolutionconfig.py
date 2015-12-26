from baseconfig import BaseConfig
import copy


class ResolutionConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(ResolutionConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on MC")

    def modify_settings(self):
        self['MaxDeltaR'] = 0.3
        self['GenPtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['PtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['MaxValidGenJetAbsRap'] = 4.0

        self['MinValidGenJetPt'] = 15.
        self['MinValidJetPt'] = 15.
        self['MinLeadingGenJetPt'] = 0.0
        self['MinLeadingJetPt'] = 0.0
        self['Processors'] = [
            'producer:JetCorrectionsProducer',
            'producer:JetValidJetsProducer',
            'producer:ValidGenJetsProducer',
            'producer:PUWeightProducer',
            'producer:CrossSectionWeightProducer',
            'producer:GeneratorWeightProducer',
            'producer:NumberGeneratedEventsWeightProducer',
            'producer:EventWeightProducer',
                ]
        default_pipeline = self.get_default_pipeline()
        default_pipeline['Processors'] = [
                                  'producer:GenJetMatchingProducer',
                                  'producer:JERScalingProducer',
                                  'producer:GenJetPartonMatchingProducer',
                                  'producer:JetQuantitiesProducer',
                                  'producer:GenJetQuantitiesProducer',
                ]
        default_pipeline['Consumers'] =  [
                                  'cutflow_histogram',
                                  'GenJetQuantitiesHistogramConsumer',
                                  'JetQuantitiesHistogramConsumer',
                                  ]

    def expand_pipelines(self):
        for i, j in [(0,0), (0, 1), (0, 2), (1, 0), (1, 1), (2, 0)]:
        # for i, j in [(1, 1)]:
            yb_lo = self['RapidityAbsBinning'][i]
            yb_hi = self['RapidityAbsBinning'][i+1]
            ys_lo = self['RapidityAbsBinning'][j]
            ys_hi = self['RapidityAbsBinning'][j+1]
            # reco pipelines
            pipeline_name = 'yb{0}ys{1}'.format(i, j)
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            pipeline = self['Pipelines'][pipeline_name]
            pipeline['Processors'].append('filter:GenYStarFilter')
            pipeline['Processors'].append('filter:GenYBoostFilter')

            self['Pipelines'][pipeline_name]['MinGenYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxGenYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinGenYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxGenYBoost'] = yb_hi


class RapidityConfig(ResolutionConfig):

    def __init__(self, nick=None, config=None):
        super(RapidityConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on MC")

    def modify_settings(self):
        super(RapidityConfig, self).modify_settings()
        self['Processors'] = [
            'producer:JetCorrectionsProducer',
            # 'producer:RapidityCorrectionProducer',
            'producer:JetValidJetsProducer',
            'producer:ValidGenJetsProducer',
            'producer:PUWeightProducer',
            'producer:CrossSectionWeightProducer',
            'producer:GeneratorWeightProducer',
            'producer:NumberGeneratedEventsWeightProducer',
            'producer:EventWeightProducer',
                ]

        default_pipeline = self.get_default_pipeline()
        default_pipeline['Processors'] += [
                                              "filter:NGenJetsFilter", 
                                              "filter:LeadingGenJetPtFilter", 
                                          ]

        self['MinValidJetPt'] = 100.
        self['MinLeadingGenJetPt'] = 100.
        self['MinValidGenJetPt'] = 80.
        self['MaxValidJetAbsRap'] = 4.0
        self['MaxValidGenJetAbsRap'] = 4.0



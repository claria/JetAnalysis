from baseconfig import BaseConfig
import copy


class ResolutionConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(ResolutionConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on MC")

    def modify_settings(self):
        self['MinValidJetPt'] = 15.
        self['MaxDeltaR'] = 0.3
        self['GenPtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['PtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]


        self['MinValidGenJetPt'] = 00.
        self['MinValidJetPt'] = 00.
        self['MinLeadingGenJetPt'] = 0.0
        self['MinLeadingJetPt'] = 0.0
        self['Processors'].remove('producer:GenJetMatchingProducer')
        self['Processors'].insert(self['Processors'].index('producer:JetValidJetsProducer') + 1, 'producer:GenJetMatchingProducer')
        self['Processors'].insert(self['Processors'].index('producer:GenJetMatchingProducer') + 1, 'producer:JERScalingProducer')
        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  'cutflow_histogram',
                                  'GenJetQuantitiesHistogramConsumer',
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
            self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:GenYStarFilter')
            self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:GenYBoostFilter')
            self['Pipelines'][pipeline_name]['MinGenYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxGenYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinGenYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxGenYBoost'] = yb_hi


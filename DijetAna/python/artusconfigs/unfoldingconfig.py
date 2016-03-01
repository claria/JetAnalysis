from baseconfig import BaseConfig
import copy


class UnfoldingResponseConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(UnfoldingResponseConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on MC")

    def modify_settings(self):
        # self['Processors'].append('filter:LeadingJetPtFilter')
        # self['MinLeadingJetPt'] = '74.'
        # i = self['Processors'].index('filter:NJetsFilter')
        # self['Processors'].pop(i)
        # self['Processors'].insert(i,'filter:NJetsFilter')

        self['PtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['GenPtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        # self['PtBinning'] = [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        # self['GenPtBinning'] = [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['TripleDiffPtBinning'] = [30, 40, 50,60,74, 114, 196, 300, 468, 790, 3000]
        self['TripleDiffGenPtBinning'] = [30, 40, 50,60,74, 114, 196, 300, 468, 790, 3000]
        self['RapidityAbsBinning'] = [0.0, 1.0, 2.0, 3.0]
        self['MinValidJetPt'] = 50.
        self['MinPtAvg'] = 133.
        self['MinGenPtAvg'] = 50.
        self['MinValidGenJetPt'] = 50.
        self['MaxValidGenJetAbsRap'] = 3.0

        self.add_processor('producer:GenJetMatchingProducer', after='producer:JetValidJetsProducer')
        self.add_processor('producer:JERScalingProducer', after='producer:GenJetMatchingProducer')

        self['TaggingFilters'] = ['YStarFilter', 'YBoostFilter','GenYStarFilter', 'GenYBoostFilter', 'NGenJetsFilter', 'LeadingGenJetPtFilter', 'NJetsFilter', 'LeadingJetPtFilter']

        self.producer_before_filter()
        # Define global cuts

        default_pipeline = self.get_default_pipeline()
        self['JERScalingFactors'] = [1.079, 1.099, 1.121, 1.208, 1.254, 1.395, 1.056]

        default_pipeline['Processors'].append('filter:NGenJetsFilter')
        default_pipeline['Processors'].append('filter:GenPtAvgFilter')
        default_pipeline['Processors'].append('filter:NJetsFilter')
        default_pipeline['Processors'].append('filter:PtAvgFilter')
        default_pipeline['Processors'].append('filter:YStarFilter')
        default_pipeline['Processors'].append('filter:YBoostFilter')
        default_pipeline['Processors'].append('filter:GenYStarFilter')
        default_pipeline['Processors'].append('filter:GenYBoostFilter')

 
        default_pipeline['Consumers'] =  [
                                         'cutflow_histogram',
                                         'GenJetQuantitiesHistogramConsumer',
                                         'JetQuantitiesHistogramConsumer',
                                         'JetUnfoldingResponseConsumer',
                                         ]


    def expand_pipelines(self):

        for i, j in [(0,0), (0, 1), (0, 2), (1, 0), (1, 1), (2, 0)]:
        # for i, j in [(1, 1)]:
            yb_lo = self['RapidityAbsBinning'][i]
            yb_hi = self['RapidityAbsBinning'][i+1]
            ys_lo = self['RapidityAbsBinning'][j]
            ys_hi = self['RapidityAbsBinning'][j+1]
            pipeline_name = 'yb{0}ys{1}'.format(i, j)
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name]['TaggingFilters'] = ['YStarFilter', 'YBoostFilter','GenYStarFilter', 'GenYBoostFilter', 'NGenJetsFilter', 'GenPtAvgFilter', 'NJetsFilter', 'PtAvgFilter']
            self['Pipelines'][pipeline_name]['MinYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxYBoost'] = yb_hi
            self['Pipelines'][pipeline_name]['MinGenYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxGenYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinGenYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxGenYBoost'] = yb_hi


#                 gen_pipeline_name = 'genptavg_yb_{0}_{1}_ys_{2}_{3}'.format(yb_lo, yb_hi, ys_lo, ys_hi).replace('.','')
#                 self['Pipelines'][gen_pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
#                 self['Pipelines'][gen_pipeline_name]['Processors'].insert(0,'filter:GenYStarFilter')
#                 self['Pipelines'][gen_pipeline_name]['Processors'].insert(0,'filter:GenYBoostFilter')
#                 self['Pipelines'][gen_pipeline_name]['MinGenYStar'] = ys_lo
#                 self['Pipelines'][gen_pipeline_name]['MaxGenYStar'] = ys_hi
#                 self['Pipelines'][gen_pipeline_name]['MinGenYBoost'] = yb_lo
#                 self['Pipelines'][gen_pipeline_name]['MaxGenYBoost'] = yb_hi
#

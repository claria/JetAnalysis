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
        self['MinLeadingGenJetPt'] = 0.0
        self['MinValidGenJetPt'] = 00.
        self['MaxValidGenJetAbsRap'] = '3.0'

        self['TaggingFilters'] = ['NGenJetsFilter', 'LeadingGenJetPtFilter', 'NJetsFilter', 'LeadingJetPtFilter']

        self.producer_before_filter()
        # Define global cuts

        default_pipeline = self.get_default_pipeline()

        default_pipeline['Processors'].append('filter:NGenJetsFilter')
        default_pipeline['Processors'].append('filter:LeadingGenJetPtFilter')
        default_pipeline['Processors'].append('filter:NJetsFilter')
        default_pipeline['Processors'].append('filter:LeadingJetPtFilter')
 
 
        default_pipeline['Consumers'] =  [
                                         'cutflow_histogram',
                                         'GenJetQuantitiesHistogramConsumer',
                                         'JetQuantitiesHistogramConsumer',
                                         'JetUnfoldingResponseConsumer',
                                         ]


    def expand_pipelines(self):
        for i in range(0, len(self['RapidityAbsBinning']) -1):
            for j in range(0, len(self['RapidityAbsBinning']) -1):
                yb_lo = self['RapidityAbsBinning'][i]
                yb_hi = self['RapidityAbsBinning'][i+1]
                ys_lo = self['RapidityAbsBinning'][j]
                ys_hi = self['RapidityAbsBinning'][j+1]
                pipeline_name = 'ptavg_yb_{0}_{1}_ys_{2}_{3}'.format(yb_lo, yb_hi, ys_lo, ys_hi).replace('.','')
                self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
                self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:YStarFilter')
                self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:YBoostFilter')
                self['Pipelines'][pipeline_name]['MinYStar'] = ys_lo
                self['Pipelines'][pipeline_name]['MaxYStar'] = ys_hi
                self['Pipelines'][pipeline_name]['MinYBoost'] = yb_lo
                self['Pipelines'][pipeline_name]['MaxYBoost'] = yb_hi

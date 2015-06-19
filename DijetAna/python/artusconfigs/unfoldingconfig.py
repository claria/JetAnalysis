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

        # self['PtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['PtBinning'] = [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['GenPtBinning'] = [36, 44, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['TripleDiffPtBinning'] = [74, 114, 196, 300, 468, 790, 3000]
        # self['TripleDiffGenPtBinning'] = [46, 60, 74, 114, 196, 300, 468, 790, 3000]
        self['TripleDiffGenPtBinning'] = [74, 114, 196, 300, 468, 790, 3000]
        self.producer_before_filter()
        # Define global cuts

        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                         'cutflow_histogram',
                                         'GenJetQuantitiesHistogramConsumer',
                                         'JetQuantitiesHistogramConsumer',
                                         'JetUnfoldingResponseConsumer',
                                         ]


    def expand_pipelines(self):
        pass
#         jet1_bins = [(74,114), (114,196), (196,300), (300,468), (468, 790), (790, 3000)]
#
#         for bin in jet1_bins:
#             pipeline_name = '{0}_{1}'.format(*bin)
#             self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
#             self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:LeadingJetPtFilter')
#             self['Pipelines'][pipeline_name]['MinLeadingJetPt'] = bin[0]
#             self['Pipelines'][pipeline_name]['MaxLeadingJetPt'] = bin[1]

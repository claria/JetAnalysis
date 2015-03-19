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
        self['PtBinning'] = [50, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  'cutflow_histogram',
                                  'GenJetQuantitiesHistogramConsumer',
                                  ]


    def expand_pipelines(self):
        pass
        rap_bins = [(0.0,0.5),(0.5,1.0),(1.0,1.5),(1.5,2.0),(2.0,2.5),(2.5,3.0)]
        for bin in rap_bins:
            pipeline_name = '{0}_{1}'.format(*bin).replace('.','')
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:LeadingJetRapFilter')
            self['Pipelines'][pipeline_name]['MinLeadingJetAbsRap'] = bin[0]
            self['Pipelines'][pipeline_name]['MaxLeadingJetAbsRap'] = bin[1]

from baseconfig import BaseConfig
import copy


class ResolutionConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(ResolutionConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on MC")

    def modify_settings(self):
        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  'cutflow_histogram',
                                  'JetGenQuantitiesHistogramConsumer',
                                  ]


    def expand_pipelines(self):
        pass
        rap_bins = [(0.0,0.5),(0.5,1.0),(1.0,1.5),(1.5,2.0),(2.0,2.5),(2.5,3.0)]
        for bin in rap_bins:
            pipeline_name = '{0}_{1}'.format(*bin).replace('.','_')
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:LeadingJetRapFilter')
            self['Pipelines'][pipeline_name]['MinLeadingJetAbsRap'] = bin[0]
            self['Pipelines'][pipeline_name]['MaxLeadingJetAbsRap'] = bin[1]

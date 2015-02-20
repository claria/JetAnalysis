from baseconfig import BaseConfig
import copy


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


    def expand_pipelines(self):
        jet1_bins = [(74,114), (114,196), (196,300), (300,468), (468, 790), (790, 3000)]

        for bin in jet1_bins:
            pipeline_name = '{0}_{1}'.format(*bin)
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:LeadingJetPtFilter')
            self['Pipelines'][pipeline_name]['MinLeadingJetPt'] = bin[0]
            self['Pipelines'][pipeline_name]['MaxLeadingJetPt'] = bin[1]

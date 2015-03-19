from baseconfig import BaseConfig
import copy

class RunConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(RunConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):

        # Same leading jet Pt cut in MC as induced by first HLT path
        if self.is_data is False:
            self['Processors'].append('filter:LeadingJetPtFilter')
            self['MinLeadingJetPt'] = '74.'

        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  'KappaLambdaNtupleConsumer',
                                  'cutflow_histogram',
                                  'JetQuantitiesHistogramConsumer',
                                  ]
        default_pipeline['Quantities'] = [
                                  'run',
                                  'lumi',
                                  'event',
                                  'npv',
                                  # 'npu',
                                  'weight',
                                  'njets',
                                  'incjets_pt',
                                  # 'incjets_eta',
                                  'incjets_rap',
                                  # 'incjets_phi',
                                  'jet1_pt',
                                  'jet1_eta',
                                  'jet1_rap',
                                  'jet1_phi',
                                  'jet2_pt',
                                  'jet2_eta',
                                  'jet2_rap',
                                  'jet2_phi',
                                  # 'dijet_mass',
                                  # 'dijet_ystar',
                                  # 'dijet_yboost',
                                  'trigweight',
                                  'puweight',
                                  'pathindex',
                                  'xsweight',
                                  'ngeneventsweight',
                                  'genweight',
                                  # 'met',
                                  # 'sumet',
                                  ]

        if self.is_data is True:
            pass
        elif self.is_data is False:
            default_pipeline['Consumers'].append('GenJetQuantitiesHistogramConsumer')
            default_pipeline['Quantities'].append('genjet1_pt')
            default_pipeline['Quantities'].append('genjet1_phi')
            default_pipeline['Quantities'].append('genjet1_eta')
            default_pipeline['Quantities'].append('genjet1_rap')
 
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
#

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

        self['GenPtBinning'] = [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['TripleDiffGenPtBinning'] = [74, 114, 196, 300, 468, 790, 3000]
        # Binnings
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
                                  # 'incjets_pt',
                                  # 'incjets_eta',
                                  # 'incjets_rap',
                                  # 'incjets_phi',
                                  'jet1_pt',
                                  'jet1_eta',
                                  'jet1_rap',
                                  'jet1_phi',
                                  'jet2_pt',
                                  'jet2_eta',
                                  'jet2_rap',
                                  'jet2_phi',
                                  'dijet_mass',
                                  'pt12_avg',
                                  'dijet_ystar',
                                  'dijet_yboost',
                                  'dijet_yinner',
                                  'dijet_youter',
                                  'dijet_chi',
                                  'dijet_costhetastar',
                                  'dijet_deltaphi',
                                  'dijet_ymax',
                                  'trigweight',
                                  'puweight',
                                  'pathindex',
                                  'xsweight',
                                  'ngeneventsweight',
                                  'luminosityWeight',
                                  'genweight',
                                  'met',
                                  'sumet',
                                  ]

        if self.is_data is True:
            self['IntLuminosity']  = 19789.

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

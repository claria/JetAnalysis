from runconfig import RunConfig

class SyncConfig(RunConfig):

    def __init__(self, nick=None, config=None):
        super(RunConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):
        self['MinValidJets'] = '1'
        self['MinLeadingJetPt'] = '74.'
        self['MaxValidJetAbsRap'] = '2.5'
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
                                  'npu',
                                  'weight',
                                  'njets',
                                  'incjets_pt',
                                  'incjets_eta',
                                  'incjets_rap',
                                  'incjets_phi',
                                  'jet1_pt',
                                  'jet1_eta',
                                  'jet1_rap',
                                  'jet1_phi',
                                  'jet2_pt',
                                  'jet2_eta',
                                  'jet2_rap',
                                  'jet2_phi',
                                  'dijet_mass',
                                  'dijet_ystar',
                                  'dijet_yboost',
                                  'trigweight',
                                  'puweight',
                                  'pathindex',
                                  'xsweight',
                                  'ngeneventsweight',
                                  'genweight',
                                  'met',
                                  'sumet',
                                  ]

    def expand_pipelines(self):
        pass


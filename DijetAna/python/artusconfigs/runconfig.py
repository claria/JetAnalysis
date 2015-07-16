from baseconfig import BaseConfig
import copy

class RunConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(RunConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):

        # Same leading jet Pt cut in MC as induced by first HLT path

        self['GenPtBinning'] = [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        # self['TripleDiffPtBinning'] = [30, 40, 50, 60, 74, 114, 196, 300, 468, 790, 3000]
        # self['TripleDiffGenPtBinning'] = [30, 40, 50, 60, 74, 114, 196, 300, 468, 790, 3000]
        self['TripleDiffPtBinning'] = [74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['TripleDiffGenPtBinning'] = self['TripleDiffPtBinning']
        # Binnings
        default_pipeline = self.get_default_pipeline()

        default_pipeline['Consumers'] =  [
                                  # 'KappaLambdaNtupleConsumer',
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
                                  'dijet_ptavg',
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

        if self.is_data is False:
            self['Pipelines']['gen_default'] = copy.deepcopy(default_pipeline)
            gen_default_pipeline = self['Pipelines']['gen_default']
            gen_default_pipeline['Processors'].append('filter:NGenJetsFilter')
            gen_default_pipeline['Processors'].append('filter:LeadingGenJetPtFilter')
            gen_default_pipeline['Consumers'].append('GenJetQuantitiesHistogramConsumer')
            gen_default_pipeline['Quantities'] += ['genjet1_pt',
                                                   'genjet1_eta',
                                                   'genjet1_rap',
                                                   'genjet1_phi',
                                                   'genjet2_pt',
                                                   'genjet2_eta',
                                                   'genjet2_rap',
                                                   'genjet2_phi',
                                                   'gendijet_mass',
                                                   'gendijet_ptavg',
                                                   'gendijet_ystar',
                                                   'gendijet_yboost',
                                                   'gendijet_yinner',
                                                   'gendijet_youter',
                                                   'gendijet_chi',
                                                   'gendijet_costhetastar',
                                                   'gendijet_deltaphi',
                                                   'gendijet_ymax',
                                                   ]

        default_pipeline['Processors'].append('filter:NJetsFilter')

        if self.is_data is False:
            default_pipeline['Processors'].append('filter:LeadingJetPtFilter')


        if self.is_data is True:
            self['IntLuminosity']  = 19789.
            default_pipeline['Processors'].append('filter:JetHltFilter')

 
    def expand_pipelines(self):

        for i in range(0, len(self['RapidityAbsBinning']) -1):
            for j in range(0, len(self['RapidityAbsBinning']) -1):
                yb_lo = self['RapidityAbsBinning'][i]
                yb_hi = self['RapidityAbsBinning'][i+1]
                ys_lo = self['RapidityAbsBinning'][j]
                ys_hi = self['RapidityAbsBinning'][j+1]
                pipeline_name = 'pt_avg_yb_{0}_{1}_ys_{2}_{3}'.format(yb_lo, yb_hi, ys_lo, ys_hi).replace('.','')
                self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
                self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:YStarFilter')
                self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:YBoostFilter')
                self['Pipelines'][pipeline_name]['MinYStar'] = ys_lo
                self['Pipelines'][pipeline_name]['MaxYStar'] = ys_hi
                self['Pipelines'][pipeline_name]['MinYBoost'] = yb_lo
                self['Pipelines'][pipeline_name]['MaxYBoost'] = yb_hi

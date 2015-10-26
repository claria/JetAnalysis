from baseconfig import BaseConfig
import copy

class RunConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(RunConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):

        # Same leading jet Pt cut in MC as induced by first HLT path
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
                                  # 'pathindex',
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
            gen_default_pipeline['Processors'].append('filter:GenPtAvgFilter')
            gen_default_pipeline['Processors'].append('filter:GenDijetsRapFilter')
            gen_default_pipeline['Consumers'].append('GenJetQuantitiesHistogramConsumer')
            default_pipeline['Consumers'].append('GenJetQuantitiesHistogramConsumer')
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
        default_pipeline['Processors'].append('filter:PtAvgFilter')
        default_pipeline['Processors'].append('filter:DijetsRapFilter')


        if self.is_data is True:
            self['IntLuminosity']  = 19712.
            default_pipeline['Processors'].append('filter:JetHltFilter')

 
    def expand_pipelines(self):

        for i in range(0, len(self['RapidityAbsBinning']) -1):
            for j in range(0, len(self['RapidityAbsBinning']) -1):
                yb_lo = self['RapidityAbsBinning'][i]
                yb_hi = self['RapidityAbsBinning'][i+1]
                ys_lo = self['RapidityAbsBinning'][j]
                ys_hi = self['RapidityAbsBinning'][j+1]
                # reco pipelines
                pipeline_name = 'yb{0}ys{1}'.format(i, j)
                self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
                self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:YStarFilter')
                self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:YBoostFilter')
                self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:METSumEtFilter')
                self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:GoodPrimaryVertexFilter')
                self['Pipelines'][pipeline_name]['MinYStar'] = ys_lo
                self['Pipelines'][pipeline_name]['MaxYStar'] = ys_hi
                self['Pipelines'][pipeline_name]['MinYBoost'] = yb_lo
                self['Pipelines'][pipeline_name]['MaxYBoost'] = yb_hi
                # gen pipelines
                if self.is_data is False:
                    gen_pipeline_name = 'gen_yb{0}ys{1}'.format(i, j)
                    self['Pipelines'][gen_pipeline_name] = copy.deepcopy(self['Pipelines']['gen_default'])
                    self['Pipelines'][gen_pipeline_name]['Processors'].insert(0,'filter:GenYStarFilter')
                    self['Pipelines'][gen_pipeline_name]['Processors'].insert(0,'filter:GenYBoostFilter')
                    self['Pipelines'][gen_pipeline_name]['MinGenYStar'] = ys_lo
                    self['Pipelines'][gen_pipeline_name]['MaxGenYStar'] = ys_hi
                    self['Pipelines'][gen_pipeline_name]['MinGenYBoost'] = yb_lo
                    self['Pipelines'][gen_pipeline_name]['MaxGenYBoost'] = yb_hi


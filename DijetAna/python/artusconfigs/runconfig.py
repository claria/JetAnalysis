from baseconfig import BaseConfig
import copy

class RunConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(RunConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):

        # Same leading jet Pt cut in MC as induced by first HLT path
        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  # 'JetLambdaNtupleConsumer',
                                  'cutflow_histogram',
                                  'JetQuantitiesHistogramConsumer',
                                  ]
        default_pipeline['Quantities'] = [
                                  'run',
                                  'lumi',
                                  'event',
                                  'npv',
                                  'weight',
                                  'njets',
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
                                  'xsweight',
                                  'ngeneventsweight',
                                  'luminosityWeight',
                                  'genweight',
                                  'met',
                                  'sumet',
                                  ]

        if self.is_data is False:
            default_pipeline['Processors'] = [
                                              "filter:GoodPrimaryVertexFilter", 
                                              "filter:METSumEtFilter", 
                                              "filter:YBoostFilter", 
                                              "filter:YStarFilter", 
                                              "filter:NJetsFilter", 
                                              "filter:PtAvgFilter", 
                                              "filter:DijetsRapFilter", 
                                          ]

            self['JERScalingFactors'] = [1.079, 1.099, 1.121, 1.208, 1.254, 1.395, 1.056]
            self['Processors'] = [
                                  "producer:JetCorrectionsProducer", 
                                  "producer:RapidityCorrectionProducer",
                                  "producer:JERSmearingProducer", 
                                  "producer:ValidGenJetsProducer", 
                                  "producer:JetValidJetsProducer", 
                                  "producer:GenJetMatchingProducer", 
                                  "producer:GenJetQuantitiesProducer", 
                                  "producer:NumberGeneratedEventsWeightProducer", 
                                  "producer:GeneratorWeightProducer", 
                                  "producer:CrossSectionWeightProducer", 
                                  "producer:PUWeightProducer", 
                                  "producer:JetQuantitiesProducer", 
                                  "producer:EventWeightProducer", 
                                 ]

            self['Pipelines']['gen_default'] = copy.deepcopy(default_pipeline)
            gen_default_pipeline = self['Pipelines']['gen_default']
            gen_default_pipeline['Processors'] = [
                                              "filter:GenYBoostFilter", 
                                              "filter:GenYStarFilter", 
                                              "filter:NGenJetsFilter", 
                                              "filter:GenPtAvgFilter", 
                                              "filter:GenDijetsRapFilter"
                                          ]

            default_pipeline['Consumers'].append('GenJetQuantitiesHistogramConsumer')
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
        else:
            self['IntLuminosity']  = 19712.
            self['Processors'] = [
                                  "filter:JsonFilter", 
                                  "producer:JetCorrectionsProducer", 
                                  # "producer:RapidityCorrectionProducer",
                                  "producer:JetValidJetsProducer", 
                                  "producer:JetQuantitiesProducer", 
                                  "producer:JetHltProducer", 
                                  "producer:LuminosityWeightProducer", 
                                  "producer:EventWeightProducer"
                              ]
            default_pipeline['Processors'] = [
                                              "filter:GoodPrimaryVertexFilter", 
                                              "filter:METSumEtFilter", 
                                              "filter:YBoostFilter", 
                                              "filter:YStarFilter", 
                                              "filter:NJetsFilter", 
                                              "filter:PtAvgFilter", 
                                              "filter:DijetsRapFilter", 
                                              "filter:JetHltFilter"
                                          ]
 
    def expand_pipelines(self):

        for i, j in [(0,0), (0, 1), (0, 2), (1, 0), (1, 1), (2, 0)]:
        # for i, j in [(1, 1)]:
            yb_lo = self['RapidityAbsBinning'][i]
            yb_hi = self['RapidityAbsBinning'][i+1]
            ys_lo = self['RapidityAbsBinning'][j]
            ys_hi = self['RapidityAbsBinning'][j+1]
            # reco pipelines
            pipeline_name = 'yb{0}ys{1}'.format(i, j)
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name]['MinYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxYBoost'] = yb_hi
            # gen pipelines
            if self.is_data is False:
                gen_pipeline_name = 'gen_yb{0}ys{1}'.format(i, j)
                self['Pipelines'][gen_pipeline_name] = copy.deepcopy(self['Pipelines']['gen_default'])
                self['Pipelines'][gen_pipeline_name]['MinGenYStar'] = ys_lo
                self['Pipelines'][gen_pipeline_name]['MaxGenYStar'] = ys_hi
                self['Pipelines'][gen_pipeline_name]['MinGenYBoost'] = yb_lo
                self['Pipelines'][gen_pipeline_name]['MaxGenYBoost'] = yb_hi


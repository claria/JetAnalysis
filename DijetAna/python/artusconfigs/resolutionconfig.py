from baseconfig import BaseConfig
import copy


class ResolutionConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(ResolutionConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on MC")

    def modify_settings(self):
        self['MaxDeltaR'] = 0.3
        self['GenPtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['PtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['MaxValidGenJetAbsRap'] = 4.0

        self['MinValidGenJetPt'] = 15.
        self['MinValidJetPt'] = 15.
        self['MinLeadingGenJetPt'] = 0.0
        self['MinLeadingJetPt'] = 0.0
        self['MinGenPtAvg'] = 20
        self['MinPtAvg'] = 20.
        self['MaxValidGenJetAbsRap'] = 5.0
        self['MaxValidJetAbsRap'] = 5.0

        self['MinDijetsAbsRap'] = '0.0'
        self['MaxDijetsAbsRap'] = '5.0'
        self['MinGenDijetsAbsRap'] = '0.0'
        self['MaxGenDijetsAbsRap'] = '5.0'


        self['Processors'] = [
            'producer:JetCorrectionsProducer',
            'producer:RapidityCorrectionProducer',
            'producer:JERSmearingProducer',
            'producer:JetValidJetsProducer',
            'producer:ValidGenJetsProducer',
            'producer:GenJetMatchingProducer',
            'producer:GenJetQuantitiesProducer',
            'producer:JetQuantitiesProducer',
            'producer:PUWeightProducer',
            'producer:CrossSectionWeightProducer',
            'producer:GeneratorWeightProducer',
            'producer:NumberGeneratedEventsWeightProducer',
            'producer:EventWeightProducer',

                ]

        # central smearing factors
        self['JERScalingFactors'] = [1.079, 1.099, 1.121, 1.208, 1.254, 1.395, 1.056]
        # downwards smearing factors
        # self['JERScalingFactors'] = [1.053, 1.071, 1.092, 1.162, 1.192, 1.332, 0.865]
        # upwards smearing factors
        # self['JERScalingFactors'] = [1.105, 1.127, 1.150, 1.254, 1.316, 1.458, 1.247]

        default_pipeline = self.get_default_pipeline()
        default_pipeline['Processors'] = [
                                       "filter:GenDijetsRapFilter",
                                       "filter:GenYBoostFilter", 
                                       "filter:GenYStarFilter", 
                                       "filter:NGenJetsFilter", 
                                       "filter:GenPtAvgFilter", 

                ]
        default_pipeline['Consumers'] =  [
                                  'cutflow_histogram',
                                  'GenJetQuantitiesHistogramConsumer',
                                  'JetQuantitiesHistogramConsumer',
                                  ]

        default_pipeline['MinYStar'] = 0.0
        default_pipeline['MaxYStar'] = 3.0
        default_pipeline['MinYBoost'] = 0.0
        default_pipeline['MaxYBoost'] = 3.0
        default_pipeline['MinGenYStar'] = 0.0
        default_pipeline['MaxGenYStar'] = 3.0
        default_pipeline['MinGenYBoost'] = 0.0
        default_pipeline['MaxGenYBoost'] = 3.0


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
            pipeline = self['Pipelines'][pipeline_name]

            self['Pipelines'][pipeline_name]['MinGenYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxGenYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinGenYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxGenYBoost'] = yb_hi


class MikkoConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(MikkoConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on MC")

    def modify_settings(self):
        self['MaxDeltaR'] = 0.3
        self['GenPtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['PtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['MaxValidGenJetAbsRap'] = 4.0

        self['MinValidGenJetPt'] = 15.
        self['MinValidJetPt'] = 15.
        self['MinLeadingGenJetPt'] = 0.0
        self['MinLeadingJetPt'] = 0.0
        self['Processors'] = ['producer:JetCorrectionsProducer']

        # central smearing factors
        self['JERScalingFactors'] = [1.079, 1.099, 1.121, 1.208, 1.254, 1.395, 1.056]
        # downwards smearing factors
        # self['JERScalingFactors'] = [1.053, 1.071, 1.092, 1.162, 1.192, 1.332, 0.865]
        # upwards smearing factors
        # self['JERScalingFactors'] = [1.105, 1.127, 1.150, 1.254, 1.316, 1.458, 1.247]

        self['JetEnergyCorrectionUncertaintyParameters'] = '$CMSSW_BASE/src/JetAnalysis/DijetAna/data/jec/Winter14_V8_DATA_UncertaintySources_AK7PF.txt'

        default_pipeline = self.get_default_pipeline()
        default_pipeline['Processors'] = [
                                  'producer:JetCorrectionsUncertaintyProducer',
                                  'producer:JetValidJetsProducer',
                                  'producer:ValidGenJetsProducer',
                                  'producer:PUWeightProducer',
                                  'producer:CrossSectionWeightProducer',
                                  'producer:GeneratorWeightProducer',
                                  'producer:NumberGeneratedEventsWeightProducer',
                                  'producer:EventWeightProducer',

                                  'producer:GenJetMatchingProducer',
                                  # 'producer:JERScalingProducer',
                                  # 'producer:GenJetPartonMatchingProducer',
                                  'producer:JetQuantitiesProducer',
                                  'producer:GenJetQuantitiesProducer',
                ]
        default_pipeline['Consumers'] =  [
                                  'cutflow_histogram',
                                  'GenJetQuantitiesHistogramConsumer',
                                  'JetQuantitiesHistogramConsumer',
                                  ]

        default_pipeline['JetEnergyCorrectionUncertaintyShift'] = 0.0
        default_pipeline['JetEnergyCorrectionUncertaintySource'] = "Total"


    def expand_pipelines(self):
        jec_unc_names  = [
                          'FlavorQCD',
                          'Total'
                          ]


        for i, j in [(0,0), (0, 1), (0, 2), (1, 0), (1, 1), (2, 0)]:
        # for i, j in [(1, 1)]:
            yb_lo = self['RapidityAbsBinning'][i]
            yb_hi = self['RapidityAbsBinning'][i+1]
            ys_lo = self['RapidityAbsBinning'][j]
            ys_hi = self['RapidityAbsBinning'][j+1]
            # reco pipelines
            pipeline_name = 'yb{0}ys{1}'.format(i, j)
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            pipeline = self['Pipelines'][pipeline_name]
            pipeline['Processors'].append('filter:GenYStarFilter')
            pipeline['Processors'].append('filter:GenYBoostFilter')

            self['Pipelines'][pipeline_name]['MinGenYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxGenYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinGenYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxGenYBoost'] = yb_hi

            for source in jec_unc_names:
                # Upwards 
                print source
                pipeline_name_up = '{0}_{1}_up'.format(pipeline_name, source)
                self['Pipelines'][pipeline_name_up] = copy.deepcopy(self['Pipelines'][pipeline_name])
                self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintySource'] = source
                self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintyShift'] = 1.0
                # Downwards 
                pipeline_name_dn = '{0}_{1}_dn'.format(pipeline_name, source)
                self['Pipelines'][pipeline_name_dn] = copy.deepcopy(self['Pipelines'][pipeline_name])
                self['Pipelines'][pipeline_name_dn]['JetEnergyCorrectionUncertaintySource'] = source
                self['Pipelines'][pipeline_name_dn]['JetEnergyCorrectionUncertaintyShift'] = -1.0


class RapidityConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(RapidityConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on MC")

    def modify_settings(self):
        self['MaxDeltaR'] = 0.3
        self['GenPtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['PtBinning'] = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]

        self['MinValidGenJetPt'] = 15.
        self['MinValidJetPt'] = 15.
        self['MinLeadingGenJetPt'] = 0.0
        self['MinLeadingJetPt'] = 0.0
        self['MinGenPtAvg'] = 20
        self['MinPtAvg'] = 20.
        self['MaxValidGenJetAbsRap'] = 5.0
        self['MaxValidJetAbsRap'] = 5.0

        self['Processors'] = [
            'producer:JetCorrectionsProducer',
            'producer:RapidityCorrectionProducer',
            'producer:JetValidJetsProducer',
            'producer:ValidGenJetsProducer',
            'producer:GenJetMatchingProducer',
            # 'producer:JERScalingProducer',
            'producer:GenJetMatchingProducer',
            'producer:GenJetQuantitiesProducer',
            'producer:JetQuantitiesProducer',
            'producer:PUWeightProducer',
            'producer:CrossSectionWeightProducer',
            'producer:GeneratorWeightProducer',
            'producer:NumberGeneratedEventsWeightProducer',
            'producer:EventWeightProducer',

                ]

        # central smearing factors
        self['JERScalingFactors'] = [1.079, 1.099, 1.121, 1.208, 1.254, 1.395, 1.056]
        # downwards smearing factors
        # self['JERScalingFactors'] = [1.053, 1.071, 1.092, 1.162, 1.192, 1.332, 0.865]
        # upwards smearing factors
        # self['JERScalingFactors'] = [1.105, 1.127, 1.150, 1.254, 1.316, 1.458, 1.247]

        default_pipeline = self.get_default_pipeline()
        default_pipeline['Processors'] = [
                                       "filter:GenDijetsRapFilter",
                                       "filter:GenYBoostFilter", 
                                       "filter:GenYStarFilter", 
                                       "filter:NGenJetsFilter", 
                                       "filter:GenPtAvgFilter", 

                ]
        default_pipeline['Consumers'] =  [
                                  'cutflow_histogram',
                                  'GenJetQuantitiesHistogramConsumer',
                                  'JetQuantitiesHistogramConsumer',
                                  ]

        default_pipeline['MinYStar'] = 0.0
        default_pipeline['MaxYStar'] = 5.0
        default_pipeline['MinYBoost'] = 0.0
        default_pipeline['MaxYBoost'] = 5.0
        default_pipeline['MinGenYStar'] = 0.0
        default_pipeline['MaxGenYStar'] = 5.0
        default_pipeline['MinGenYBoost'] = 0.0
        default_pipeline['MaxGenYBoost'] = 5.0

        self['MinDijetsAbsRap'] = '0.0'
        self['MaxDijetsAbsRap'] = '5.0'
        self['MinGenDijetsAbsRap'] = '0.0'
        self['MaxGenDijetsAbsRap'] = '5.0'


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
            pipeline = self['Pipelines'][pipeline_name]

            self['Pipelines'][pipeline_name]['MinGenYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxGenYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinGenYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxGenYBoost'] = yb_hi



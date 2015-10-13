from runconfig import RunConfig
import copy

class JECConfig(RunConfig):

    def __init__(self, nick=None, config=None):
        super(JECConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):
        super(JECConfig, self).modify_settings()
        default_pipeline = self.get_default_pipeline()
        self['Processors'] = []
        default_pipeline['Processors'] = [
                "filter:JsonFilter", 
                "producer:JetCorrectionsProducer", 
                "producer:JetCorrectionsUncertaintyProducer", 
                "producer:ValidJetsProducer", 
                "producer:LuminosityWeightProducer", 
                "producer:EventWeightProducer", 
                "producer:JetHltProducer", 
                "filter:JetHltFilter",
                "filter:NJetsFilter", 
                "producer:JetQuantitiesProducer", 
                ]


    def expand_pipelines(self):

        jec_unc_names  = [
                          'AbsoluteScale','AbsoluteStat','AbsoluteMPFBias',
                          'Fragmentation',
                          'SinglePionECAL',
                          'SinglePionHCAL',
                          'FlavorQCD',
                          'TimeEta',
                          'TimePt',
                          'RelativeJEREC1','RelativeJEREC2','RelativeJERHF',
                          'RelativePtBB','RelativePtEC1','RelativePtEC2','RelativePtHF',
                          'RelativeFSR',
                          'RelativeStatEC2', 'RelativeStatHF', 'RelativeStatFSR',
                          'PileUpDataMC',
                          'PileUpPtRef',
                          'PileUpPtBB','PileUpPtEC1','PileUpPtEC2','PileUpPtHF',
                          'PileUpEnvelope',
                          'Total',
                          ]

        for i, j in [(0,0), (0, 1), (0, 2), (1, 0), (1, 1), (2, 0)]:
            yb_lo = self['RapidityAbsBinning'][i]
            yb_hi = self['RapidityAbsBinning'][i+1]
            ys_lo = self['RapidityAbsBinning'][j]
            ys_hi = self['RapidityAbsBinning'][j+1]
            # reco pipelines
            pipeline_name = 'yb{0}ys{1}'.format(i, j)
            self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name]['Processors'].insert(self['Pipelines'][pipeline_name]['Processors'].index('producer:JetQuantitiesProducer') + 1,'filter:YBoostFilter')
            self['Pipelines'][pipeline_name]['Processors'].insert(self['Pipelines'][pipeline_name]['Processors'].index('producer:JetQuantitiesProducer') + 1,'filter:YStarFilter')
            self['Pipelines'][pipeline_name]['MinYStar'] = ys_lo
            self['Pipelines'][pipeline_name]['MaxYStar'] = ys_hi
            self['Pipelines'][pipeline_name]['MinYBoost'] = yb_lo
            self['Pipelines'][pipeline_name]['MaxYBoost'] = yb_hi

            for source in jec_unc_names:
                # Upwards 
                pipeline_name_up = '{0}_{1}_up'.format(pipeline_name, source)
                self['Pipelines'][pipeline_name_up] = copy.deepcopy(self['Pipelines'][pipeline_name])
                self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintySource'] = source
                self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintyShift'] = 1.0
                # Downwards 
                pipeline_name_dn = '{0}_{1}_dn'.format(pipeline_name, source)
                self['Pipelines'][pipeline_name_dn] = copy.deepcopy(self['Pipelines'][pipeline_name])
                self['Pipelines'][pipeline_name_dn]['JetEnergyCorrectionUncertaintyShift'] = -1.0


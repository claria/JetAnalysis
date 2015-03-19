from runconfig import RunConfig
import copy

class JECConfig(RunConfig):

    def __init__(self, nick=None, config=None):
        super(JECConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):
        super(JECConfig, self).modify_settings()
        default_pipeline = self.get_default_pipeline()
        default_pipeline['Processors'] = self['Processors'] + default_pipeline['Processors']
        self['Processors'] = []
        # self['JetEnergyCorrectionUncertaintyParameters'] = ''

    def expand_pipelines(self):

        jec_unc_names  = ['AbsoluteScale','AbsoluteStat','AbsoluteMPFBias',
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
        for source in jec_unc_names:
            # Upwards 
            pipeline_name_up = 'jecu_{0}_up'.format(source)
            self['Pipelines'][pipeline_name_up] = copy.deepcopy(self['Pipelines']['default'])
            self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintySource'] = source
            self['Pipelines'][pipeline_name_up]['JetEnergyCorrectionUncertaintyShift'] = 1.0
            # Downwards 
            pipeline_name_dw = 'jecu_{0}_dn'.format(source)
            self['Pipelines'][pipeline_name_dw] = copy.deepcopy(self['Pipelines'][pipeline_name_up])
            self['Pipelines'][pipeline_name_dw]['JetEnergyCorrectionUncertaintyShift'] = -1.0


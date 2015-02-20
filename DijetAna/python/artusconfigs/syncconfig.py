from runconfig import RunConfig

class SyncConfig(RunConfig):

    def __init__(self, nick=None, config=None):
        super(SyncConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):
        super(SyncConfig, self).modify_settings()
        self['MinValidJets'] = '1'
        self['MinLeadingJetPt'] = '74.'
        self['MaxValidJetAbsRap'] = '2.5'
        # self['Processors'].insert(0,'filter:RunLumiEventFilter')
        # self['RunWhitelist'] = [190705]
        # self['LumiWhitelist'] = [7]

    def expand_pipelines(self):
        pass
        # jet1_bins = [(0.0, 0.5), (0.5, 1.0), (1.0, 1.5), (1.5, 2.0), (2.0, 2.5)]
#
        # for bin in jet1_bins:
            # pipeline_name = '{0}_{1}'.format(*bin)
            # self['Pipelines'][pipeline_name] = copy.deepcopy(self['Pipelines']['default'])
            # self['Pipelines'][pipeline_name]['Processors'].insert(0,'filter:LeadingJetPtFilter')
            # self['Pipelines'][pipeline_name]['MinLeadingJetPt'] = bin[0]
            # self['Pipelines'][pipeline_name]['MaxLeadingJetPt'] = bin[1]

from runconfig import RunConfig
import copy

class METConfig(RunConfig):
    """Config based on RunConfig but with removed METSumEtFilter."""

    def __init__(self, nick=None, config=None):
        super(METConfig, self).__init__(nick=nick, config=config)


    def modify_settings(self):
        super(METConfig, self).modify_settings()
        if 'filter:METSumEtFilter' in self['Processors']:
                self['Processors'].remove('filter:METSumEtFilter')

    def expand_pipelines(self):
        super(METConfig, self).expand_pipelines()
        for pipeline in self['Pipelines']:
            if 'filter:METSumEtFilter' in self['Pipelines'][pipeline]['Processors']:
                self['Pipelines'][pipeline]['Processors'].remove('filter:METSumEtFilter')


from baseconfig import BaseConfig
from runconfig import RunConfig
import copy

class JetIDConfig(RunConfig):

    def __init__(self, nick=None, config=None):
        super(JetIDConfig, self).__init__(nick=nick, config=config)

    def modify_settings(self):

        # Same leading jet Pt cut in MC as induced by first HLT path
        super(JetIDConfig, self).modify_settings()
        self['JetID'] = 'noid'
        self['JetIDVersion'] = '2014'
        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'].append('JetIDEfficiencyConsumer')
 

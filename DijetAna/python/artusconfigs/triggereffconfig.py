import copy
import sys

import numpy as np
from baseconfig import BaseConfig

class TriggerEffConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(TriggerEffConfig, self).__init__(nick=nick, config=config)

        if not self.is_data:
            print "Trigger config only valid for data."
            sys.exit(1)


    def modify_settings(self):

        self['LogPtBinning'] = list(np.logspace(0, 3, 100))

        if 'filter:JetHltFilter' in self['Processors']:
            self['Processors'].remove('filter:JetHltFilter')

        if self.is_data is True:
            self['IntLuminosity']  = 19712.


        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  # 'KappaLambdaNtupleConsumer',
                                  'cutflow_histogram',
                                  'JetQuantitiesHistogramConsumer',
                                  'TriggerEfficiencyHistogramConsumer',
                                  ]

    def expand_pipelines(self):
        pass

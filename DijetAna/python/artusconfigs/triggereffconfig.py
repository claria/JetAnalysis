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
        if 'producer:JetHltProducer' in self['Processors']:
            self['Processors'].remove('producer:JetHltProducer')

        if self.is_data is True:
            self['IntLuminosity']  = 19712.

        self['HltPathsBlacklist'] = []
        self['TriggerEffPaths'] = ['HLT_DiPFJETAve40', 'HLT_DiPFJETAve80','HLT_DiPFJETAve140','HLT_DiPFJETAve200','HLT_DiPFJETAve260','HLT_DiPFJETAve320','HLT_DiPFJETAve400']
        self['L1FilterThresholds'] = [16., 36., 68., 92., 128., 128., 128.]
        self['HltFilterThresholds'] = [40., 80., 140., 200., 260., 320., 400.]
        self['L1FilterPattern'] = '(L1SingleJet)([0-9]+)'
        self['HltFilterPattern'] = '(DiPFJetAve)([0-9]+)'

        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  # 'KappaLambdaNtupleConsumer',
                                  'cutflow_histogram',
                                  'JetQuantitiesHistogramConsumer',
                                  'TriggerEfficiencyHistogramConsumer'
                                  ]

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


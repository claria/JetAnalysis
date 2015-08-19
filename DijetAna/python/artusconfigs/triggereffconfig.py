
import copy
import sys

from baseconfig import BaseConfig

class TriggerEffConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(TriggerEffConfig, self).__init__(nick=nick, config=config)

        if not self.is_data:
            print "Trigger config only valid for data."
            sys.exit(1)


    def modify_settings(self):

        if 'filter:JetHltFilter' in self['Processors']:
            self['Processors'].remove('filter:JetHltFilter')

        if self.is_data is True:
            self['IntLuminosity']  = 19789.


        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  # 'KappaLambdaNtupleConsumer',
                                  'cutflow_histogram',
                                  'JetQuantitiesHistogramConsumer',
                                  'TriggerEfficiencyHistogramConsumer',
                                  ]
        default_pipeline['Quantities'] = [
                                  'run',
                                  'lumi',
                                  'event',
                                  'npv',
                                  # 'npu',
                                  'weight',
                                  'njets',
                                  'incjets_pt',
                                  # 'incjets_eta',
                                  'incjets_rap',
                                  # 'incjets_phi',
                                  'jet1_pt',
                                  'jet1_eta',
                                  'jet1_rap',
                                  'jet1_phi',
                                  'jet2_pt',
                                  'jet2_eta',
                                  'jet2_rap',
                                  'jet2_phi',
                                  # 'dijet_mass',
                                  # 'dijet_ystar',
                                  # 'dijet_yboost',
                                  'trigweight',
                                  'puweight',
                                  'pathindex',
                                  'xsweight',
                                  'ngeneventsweight',
                                  'genweight',
                                  # 'met',
                                  # 'sumet',
                                  ]

    def expand_pipelines(self):
        pass

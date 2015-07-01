from baseconfig import BaseConfig
import copy


class GenConfig(BaseConfig):

    def __init__(self, nick=None, config=None):
        super(GenConfig, self).__init__(nick=nick, config=config)

        if self.is_data is True:
            raise Exception("This config only works on GEN")

    def modify_settings(self):
        self['GenLumiMetadata'] = 'lumiInfo'
        self['GenEventMetadata'] = 'eventInfo'
        self['GenJets'] = 'ak7GenJets'
        self['TrackSummary'] = ''
        self['VertexSummary'] = ''
        self['BasicJets'] = ''
        self['PileupDensity'] = ''
        self['Met'] = ''


        self['MinValidGenJetPt'] = 50.
        self['MaxValidGenJetAbsRap'] = 3.
        self['MinValidGenJets'] = 2
        self['MinLeadingGenJetPt'] = 74.

        self['MaxDeltaR'] = 0.3
        self['PtBinning'] = [50, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]
        self['Processors'] = [
            'producer:ValidGenJetsProducer',
            'producer:GenJetQuantitiesProducer',
            'producer:CrossSectionWeightProducer',
            'producer:GeneratorWeightProducer',
            'producer:NumberGeneratedEventsWeightProducer',
            'producer:EventWeightProducer',
            'filter:NGenJetsFilter',
            'filter:LeadingGenJetPtFilter'

        ]

        default_pipeline = self.get_default_pipeline()
        default_pipeline['Consumers'] =  [
                                  'GenJetQuantitiesHistogramConsumer',
                                  ]


    def expand_pipelines(self):
        pass

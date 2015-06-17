#include "JetAnalysis/DijetAna/interface/JetFactory.h"

// Producers
#include "JetAnalysis/DijetAna/interface/Producers/JetQuantitiesProducer.h"
#include "JetAnalysis/DijetAna/interface/Producers/GenJetQuantitiesProducer.h"
#include "JetAnalysis/DijetAna/interface/Producers/GenJetMatchingProducer.h"
#include "JetAnalysis/DijetAna/interface/Producers/JetHltProducer.h"
#include "JetAnalysis/DijetAna/interface/Producers/JetValidJetsProducer.h"

// Filters
#include "JetAnalysis/DijetAna/interface/Filters/DiJetsFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/NJetsFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/NGenJetsFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/DijetsRapFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/JetHltFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/JetHltEfficiencyFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetPtFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/LeadingGenJetPtFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetRapFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/YStarFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/YBoostFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/SecondJetRapFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/METSumEtFilter.h"

// Consumers
// #include "JetAnalysis/DijetAna/interface/Consumers/JetNtupleConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/JetResolutionConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/TriggerEfficiencyHistogramConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/JetQuantitiesHistogramConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/GenJetQuantitiesHistogramConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"
// #include "JetAnalysis/DijetAna/interface/Consumers/TriggerHistogramFitConsumer.h"

JetFactory::JetFactory() : KappaFactory() {}
JetFactory::~JetFactory() {}

ProducerBaseUntemplated* JetFactory::createProducer(std::string const& id) {
  if (JetQuantitiesProducer().GetProducerId() == id)
    return new JetQuantitiesProducer();
  else if (GenJetQuantitiesProducer().GetProducerId() == id)
    return new GenJetQuantitiesProducer();
  else if (JetHltProducer().GetProducerId() == id)
    return new JetHltProducer();
  else if (GenJetMatchingProducer().GetProducerId() == id)
    return new GenJetMatchingProducer();
  else if (JetValidJetsProducer().GetProducerId() == id)
    return new JetValidJetsProducer();
  else
    return KappaFactory::createProducer(id);
}

ConsumerBaseUntemplated* JetFactory::createConsumer(std::string const& id) {
  if (JetLambdaNtupleConsumer().GetConsumerId() == id)
    return new JetLambdaNtupleConsumer();
  else if (TriggerEfficiencyHistogramConsumer().GetConsumerId() == id)
    return new TriggerEfficiencyHistogramConsumer();
  else if (JetQuantitiesHistogramConsumer().GetConsumerId() == id)
    return new JetQuantitiesHistogramConsumer();
  else if (JetResolutionConsumer().GetConsumerId() == id)
    return new JetResolutionConsumer();
  else if (GenJetQuantitiesHistogramConsumer().GetConsumerId() == id)
    return new GenJetQuantitiesHistogramConsumer();
  else if (JetUnfoldingResponseConsumer().GetConsumerId() == id)
    return new JetUnfoldingResponseConsumer();
  else
    return KappaFactory::createConsumer(id);
}

FilterBaseUntemplated* JetFactory::createFilter(std::string const& id) {
  if (DiJetsFilter().GetFilterId() == id)
    return new DiJetsFilter();
  else if (NJetsFilter().GetFilterId() == id)
    return new NJetsFilter();
  else if (NGenJetsFilter().GetFilterId() == id)
    return new NGenJetsFilter();
  else if (DijetsRapFilter().GetFilterId() == id)
    return new DijetsRapFilter();
  else if (LeadingJetPtFilter().GetFilterId() == id)
    return new LeadingJetPtFilter();
  else if (LeadingGenJetPtFilter().GetFilterId() == id)
    return new LeadingGenJetPtFilter();
  else if (LeadingJetRapFilter().GetFilterId() == id)
    return new LeadingJetRapFilter();
  else if (YStarFilter().GetFilterId() == id)
    return new YStarFilter();
  else if (YBoostFilter().GetFilterId() == id)
    return new YBoostFilter();

  else if (SecondJetRapFilter().GetFilterId() == id)
    return new SecondJetRapFilter();
  else if (METSumEtFilter().GetFilterId() == id)
    return new METSumEtFilter();
  else if (JetHltFilter().GetFilterId() == id)
    return new JetHltFilter();
  else if (JetHltEfficiencyFilter().GetFilterId() == id)
    return new JetHltEfficiencyFilter();
  else
    return KappaFactory::createFilter(id);
}

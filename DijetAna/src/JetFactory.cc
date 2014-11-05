#include "JetAnalysis/DijetAna/interface/JetFactory.h"

// Producers
#include "JetAnalysis/DijetAna/interface/Producers/DiJetsObservables.h"
#include "JetAnalysis/DijetAna/interface/Producers/JetHltProducer.h"

// Filters
#include "JetAnalysis/DijetAna/interface/Filters/DiJetsFilter.h"
// #include "JetAnalysis/DijetAna/interface/Filters/JetHltFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetPtFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetRapFilter.h"

// Consumers
#include "JetAnalysis/DijetAna/interface/Consumers/JetNtupleConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/TriggerResultsHistogramConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/TriggerHistogramFitConsumer.h"


JetFactory::JetFactory() : KappaFactory() {}
JetFactory::~JetFactory() {}

ProducerBaseUntemplated * JetFactory::createProducer ( std::string const& id )
{
	if ( DiJetsObservables().GetProducerId() == id )
		return new DiJetsObservables();
	else if (JetHltProducer().GetProducerId() == id)
		return new JetHltProducer();
	else
		return KappaFactory::createProducer( id );
}

ConsumerBaseUntemplated * JetFactory::createConsumer ( std::string const& id )
{
	if ( JetNtupleConsumer().GetConsumerId() == id )
		return new JetNtupleConsumer();
	else if (JetLambdaNtupleConsumer().GetConsumerId() == id)
		return new JetLambdaNtupleConsumer();
	else if (TriggerResultsHistogramConsumer().GetConsumerId() == id)
		return new TriggerResultsHistogramConsumer();
	else if (TriggerHistogramFitConsumer().GetConsumerId() == id)
		return new TriggerHistogramFitConsumer();
	else
		return KappaFactory::createConsumer( id );
}

FilterBaseUntemplated * JetFactory::createFilter ( std::string const& id )
{
	if (DiJetsFilter().GetFilterId() == id)
		return new DiJetsFilter();
	else if (LeadingJetPtFilter().GetFilterId() == id)
		return new LeadingJetPtFilter();
	else if (LeadingJetRapFilter().GetFilterId() == id)
		return new LeadingJetRapFilter();
	else
		return KappaFactory::createFilter( id );
}

/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */


#include "JetAnalysis/DijetAna/interface/JetFactory.h"


// Producers
// #include "Artus/KappaAnalysis/interface/Producers/EventWeightProducer.h"
// #include "JetAnalysis/DijetAna/interface/Producers/CrossSectionWeightProducer.h"
#include "JetAnalysis/DijetAna/interface/Producers/DiJetsObservables.h"

// Filters
#include "JetAnalysis/DijetAna/interface/Filters/DiJetsFilter.h"
// #include "JetAnalysis/DijetAna/interface/Filters/JetHltFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetPtFilter.h"
#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetRapFilter.h"

// Consumers
#include "JetAnalysis/DijetAna/interface/Consumers/JetNtupleConsumer.h"
#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"


JetFactory::JetFactory() : KappaFactory<JetTypes>() {}
JetFactory::~JetFactory() {}

JetProducerBase * JetFactory::createProducer ( std::string const& id )
{
	if ( DiJetsObservables().GetProducerId() == id )
		return new DiJetsObservables();
	//else if(CrossSectionWeightProducer().GetProducerId() == id)
	//	return new CrossSectionWeightProducer();
	else
		return KappaFactory<JetTypes>::createProducer( id );
}

JetConsumerBase * JetFactory::createConsumer ( std::string const& id )
{
	if ( JetNtupleConsumer().GetConsumerId() == id )
		return new JetNtupleConsumer();
	if (JetLambdaNtupleConsumer().GetConsumerId() == id)
		return new JetLambdaNtupleConsumer();
	else
		return KappaFactory<JetTypes>::createConsumer( id );
}

JetFilterBase * JetFactory::createFilter ( std::string const& id )
{
	if (DiJetsFilter().GetFilterId() == id)
		return new DiJetsFilter();
	else if (LeadingJetPtFilter().GetFilterId() == id)
		return new LeadingJetPtFilter();
	else if (LeadingJetRapFilter().GetFilterId() == id)
		return new LeadingJetRapFilter();
	else
		return KappaFactory<JetTypes>::createFilter( id );
}

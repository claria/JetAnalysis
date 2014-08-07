/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/KappaAnalysis/interface/KappaFactory.h"


#include "JetTypes.h"

#include "JetSettings.h"
#include "JetEvent.h"
#include "JetProduct.h"

// producer
//#include "ValidDiJetsProducer.h"
#include "Producer/DiJetsObservables.h"
//#include "Producer/JetHltProducer.h"
#include "Artus/KappaAnalysis/interface/Producers/EventWeightProducer.h"
#include "Producer/CrossSectionWeightProducer.h"

// filter
#include "Filter/DiJetsFilter.h"
#include "Filter/JetHltFilter.h"
#include "Filter/DiJetsPtFilter.h"
#include "Filter/DiJetsRapFilter.h"

// consumer
#include "Consumer/JetNtupleConsumer.h"

class JetFactory: public KappaFactory<JetTypes> {
public:

	JetFactory() : KappaFactory<JetTypes>() {}
	virtual ~JetFactory() {}

	virtual JetProducerBase * createProducer ( std::string const& id )
		ARTUS_CPP11_OVERRIDE
	{
        if ( DiJetsObservables().GetProducerId() == id )
			return new DiJetsObservables();
		else if(CrossSectionWeightProducer().GetProducerId() == id)
			return new CrossSectionWeightProducer();
		else
			return KappaFactory<JetTypes>::createProducer( id );
	}

	virtual JetConsumerBase * createConsumer ( std::string const& id )
		ARTUS_CPP11_OVERRIDE
	{
		if ( JetNtupleConsumer().GetConsumerId() == id )
			return new JetNtupleConsumer();
		else
			return KappaFactory<JetTypes>::createConsumer( id );
	}

	virtual JetFilterBase * createFilter ( std::string const& id )
		ARTUS_CPP11_OVERRIDE
	{
		if ( DiJetsFilter().GetFilterId() == id)
			return new DiJetsFilter();
		else if ( JetHltFilter().GetFilterId() == id)
			return new JetHltFilter();
		else if ( DiJetsPtFilter().GetFilterId() == id)
			return new DiJetsPtFilter();
		else if ( DiJetsRapFilter().GetFilterId() == id)
			return new DiJetsRapFilter();
		else
			return KappaFactory<JetTypes>::createFilter( id );
	}


};

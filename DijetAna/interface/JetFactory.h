/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/KappaAnalysis/interface/KappaFactory.h"


#include "JetTypes.h"

#include "JetPipelineSettings.h"
#include "JetEvent.h"
#include "JetProduct.h"

// producer
//#include "ValidDiJetsProducer.h"
#include "DiJetsObservables.h"
#include "JetHltProducer.h"

// filter
#include "DiJetsFilter.h"
#include "JetHltFilter.h"
#include "DiJetsPtFilter.h"
#include "DiJetsRapFilter.h"

// consumer
#include "JetNtupleConsumer.h"

class JetFactory: public KappaFactory<JetTypes> {
public:

	JetFactory() : KappaFactory<JetTypes>() {}

	virtual ~JetFactory() {}

	virtual JetProducerBase * createProducer ( std::string const& id )
		ARTUS_CPP11_OVERRIDE
	{
        if ( DiJetsObservables().GetProducerId() == id )
			return new DiJetsObservables();
		else if ( JetHltProducer().GetProducerId() == id )
			return new JetHltProducer();
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

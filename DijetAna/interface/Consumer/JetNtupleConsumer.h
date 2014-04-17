/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"

#include "Artus/Consumer/interface/NtupleConsumerBase.h"

#include "../JetTypes.h"


class JetNtupleConsumer: public NtupleConsumerBase<JetTypes> {

public:
	virtual std::string GetConsumerId() {
		return "JetNtupleConsumer";
	}

private:
	
	float returnvalue(std::string string, JetEvent const& event,
			JetProduct const& product) ARTUS_CPP11_OVERRIDE
	{
		std::string trigger("HLT");
		std::string weight("weight");
		// Observables
		if (string == "Jet1Pt")
			return product.m_Jet1Pt;
		else if (string == "Jet1Rap")
			return product.m_Jet1Eta;
		else if (string == "Jet2Pt")
			return product.m_Jet2Pt;
		else if (string == "Jet2Rap")
			return product.m_Jet2Eta;
		else if (string.find(weight) != std::string::npos){
			return product.m_weights.find(string)->second;
		}
		else if (string.compare(0, trigger.length(), trigger) == 0)
		{
			return product.m_HltTrigger.find(string)->second;
		}
		else
			LOG(FATAL) << "The quantity " << string << " could not be added to the Ntuple";
		return -999.0;
	}


};

/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"

#include "Artus/Consumer/interface/NtupleConsumerBase.h"

#include "JetTypes.h"


class JetNtupleConsumer: public NtupleConsumerBase<JetTypes> {

public:
	virtual std::string GetConsumerId() {
		return "JetNtupleConsumer";
	}

private:
	
	float returnvalue(std::string string, JetEvent const& event,
			JetProduct const& product) ARTUS_CPP11_OVERRIDE
	{
		if (string == "LeadingJetPt")
			return product.m_Jet1Pt;
		else if (string == "LeadingJetEta")
			return product.m_Jet1Eta;
		else if (string == "SecondJetPt")
			return product.m_Jet2Pt;
		else if (string == "SecondJetEta")
			return product.m_Jet2Eta;
		else
			LOG(FATAL) << "The quantity " << string << " could not be added to the Ntuple";
		return -999.0;
	}


};

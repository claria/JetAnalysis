/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "JetTypes.h"

class ValidDiJetsProducer: public JetProducerBase {
public:

	virtual std::string GetProducerId() const {
		return "ValidDiJetsProducer";
	}

	virtual void ProduceGlobal(JetEvent const& event,
			JetProduct & product,
			JetGlobalSettings const& globalSettings) const
		{
			if ( product.m_validJets.size() >= 2)
				return true;
			else
				return false;
		}

};

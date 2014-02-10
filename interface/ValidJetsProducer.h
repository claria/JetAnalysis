/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "JetTypes.h"

class JetObservableProducer: public JetLocalProducerBase {
public:

	virtual ~JetObservableProducer() {
	}

	virtual std::string GetProducerId() {
		return "valid_jets";
	}

	virtual void ProduceGlobal(JetEvent const& event,
			JetProduct & product,
			global_setting_type const& globalSettings) const
		{

			for (KDataPFJets::iterator jet = event._m
			bool good_jet = true;

			// Check for at least two

			good_jet = good_jet
						&& 
			product.m_leadingJetPt = event.m_ak5pfJets->at(0).p4.Pt();
			product.m_leadingJetEta = event.m_ak5pfJets->at(0).p4.Eta();
		}

};

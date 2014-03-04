/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "JetTypes.h"

class ValidJetsProducer: public JetGlobalProducerBase {
public:

	virtual std::string GetProducerId() {
		return "valid_jets";
	}

	virtual bool ProduceGlobal(JetEvent const& event,
			JetProduct & product,
			JetGlobalSettings const& globalSettings) const
		{


			for(KDataPFJets::iterator jet = event.m_ak5pfJets->begin(); jet != event.m_ak5pfJets->end(); ++jet)
			{
			// Select good jets in event
			// Not complete, just a first try
			bool good_jet = true;

			good_jet = good_jet && jet->p4.Pt() > 50.
						 		&& std::abs(jet->p4.Eta()) < 5.0;
			if (good_jet)
				product.m_validJets.push_back(*jet);
			else
				product.m_invalidJets.push_back(*jet);

			}

			// Only accept Events with at least two good jets
			if (product.m_validJets.size() >= 2)
				return true;
			else
				return false;

		}

};

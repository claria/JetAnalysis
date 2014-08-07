/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "../JetTypes.h"

class DiJetsObservables : public JetProducerBase {
public:

	virtual std::string GetProducerId() const {
		return "DiJetsObservables";
	}

	virtual void Produce(JetEvent const& event,
			JetProduct & product,
			JetSettings const& settings) const
		{
			product.m_Jet1Pt = product.m_validJets.at(0)->p4.Pt();
			product.m_Jet1Eta = product.m_validJets.at(0)->p4.Eta();
			product.m_Jet1Rap = product.m_validJets.at(0)->p4.Rapidity();
			product.m_Jet2Pt = product.m_validJets.at(1)->p4.Pt();
			product.m_Jet2Eta = product.m_validJets.at(1)->p4.Eta();
			product.m_Jet2Rap = product.m_validJets.at(1)->p4.Rapidity();
		}

};

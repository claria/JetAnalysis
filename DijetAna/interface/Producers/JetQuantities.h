/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "../JetTypes.h"

class JetQuantities : public JetProducerBase {
public:

	virtual std::string GetProducerId() const {
		return "JetQuantities";
	}

	virtual void Produce(JetEvent const& event, JetProduct & product, JetSettings const& settings) const
		{
			auto & specProduct = static_cast <JetProduct &> (product);

			specProduct.m_Jet1Pt = product.m_validJets.at(0)->p4.Pt();
			specProduct.m_Jet1Eta = product.m_validJets.at(0)->p4.Eta();
			specProduct.m_Jet1Rap = product.m_validJets.at(0)->p4.Rapidity();
			specProduct.m_Jet2Pt = product.m_validJets.at(1)->p4.Pt();
			specProduct.m_Jet2Eta = product.m_validJets.at(1)->p4.Eta();
			specProduct.m_Jet2Rap = product.m_validJets.at(1)->p4.Rapidity();

			for ( auto i = product.m_validJets.begin(); i != product.m_validJets.end(); i++ ) {
				specProduct.m_incJetsPt.push_back((*i)->p4.Pt());
				specProduct.m_incJetsEta.push_back((*i)->p4.Eta());
				specProduct.m_incJetsRap.push_back((*i)->p4.Rapidity());
				specProduct.m_incJetsPhi.push_back((*i)->p4.Phi());
			}

		}

};

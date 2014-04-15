/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */


#pragma once


#include "Artus/KappaAnalysis/interface/KappaProduct.h"

class JetProduct : public KappaProduct 
{
	public:

		double m_Jet1Pt;
		double m_Jet1Eta;
		double m_Jet1Rap;
		double m_Jet2Pt;
		double m_Jet2Eta;
		double m_Jet2Rap;
		double m_DiJetMass;
		std::map<std::string, bool> m_HltTrigger;
		std::map<std::string, double> m_weights;
};

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

		double m_leadingJetPt;
		double m_leadingJetEta;

};

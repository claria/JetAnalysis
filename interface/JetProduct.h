/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */


#pragma once


struct JetProduct {

	float m_leadingJetPt;
	float m_leadingJetEta;
	// use pointers to be able to handle tagged and untagged jets
	std::vector<KDataPFJet*> m_validJets;
	std::vector<KDataPFJet*> m_invalidJets;
	// Dijets
	std::vector<KDataPFJet*> m_validDiJets;
	std::vector<KDataPFJet*> m_invalDiidJets;


};

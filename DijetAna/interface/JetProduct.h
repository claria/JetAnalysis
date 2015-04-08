#pragma once

#include "Artus/KappaAnalysis/interface/KappaProduct.h"

class JetProduct : public KappaProduct 
{
public:

	// std::map<std::string, bool> m_HltTrigger;
	// std::map<std::string, double> m_weights;
	std::vector<KLV*> m_matchedGenJets;
	std::vector<KBasicJet*> m_matchedRecoJets;

	std::vector<double> m_incJetsPt;
	std::vector<double> m_incJetsEta;
	std::vector<double> m_incJetsRap;
	std::vector<double> m_incJetsPhi;

	std::vector<int> m_match_result_genjets;
	std::vector<int> m_match_result_recojets;
	// Main observables to be set in the JetQuantities Producer
	double obs1;
	double obs2;
	double obs3;
	// Main observables to be set in the GenJetQuantities Producer
	double genobs1;
	double genobs2;
	double genobs3;


// 	double m_jet1Pt;
// 	double m_jet1Eta;
// 	double m_jet1Rap;
// 	double m_jet1Phi;
// 	double m_jet2Pt;
// 	double m_jet2Eta;
// 	double m_jet2Rap;
// 	double m_jet2Phi;
//
// 	double m_dijet_mass;
// 	double m_dijet_yMax;
// 	double m_dijet_deltaPhi;
// 	double m_dijet_cosThetaStar;
// 	double m_dijet_yBoost;
// 	double m_dijet_yStar;
// 	double m_dijet_chi;
//
// 	double m_genjet1Pt;
// 	double m_genjet1Eta;
// 	double m_genjet1Rap;
// 	double m_genjet1Phi;
// 	double m_genjet2Pt;
// 	double m_genjet2Eta;
// 	double m_genjet2Rap;
// 	double m_genjet2Phi;
// 	double m_gendijet_mass;
};

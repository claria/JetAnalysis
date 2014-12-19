#pragma once

#include "Artus/KappaAnalysis/interface/KappaProduct.h"

class JetProduct : public KappaProduct 
{
public:

	// std::map<std::string, bool> m_HltTrigger;
	// std::map<std::string, double> m_weights;

	std::vector<double> m_incJetsPt;
	std::vector<double> m_incJetsEta;
	std::vector<double> m_incJetsRap;
	std::vector<double> m_incJetsPhi;

	float m_jet1Pt;
	float m_jet1Eta;
	float m_jet1Rap;
	float m_jet1Phi;
	float m_jet2Pt;
	float m_jet2Eta;
	float m_jet2Rap;
	float m_jet2Phi;

	float m_dijet_mass;
	float m_dijet_yMax;
	float m_dijet_deltaPhi;
	float m_dijet_cosThetaStar;
	float m_dijet_yBoost;
	float m_dijet_yStar;
	float m_dijet_chi;

	float m_genjet1Pt;
	float m_genjet1Eta;
	float m_genjet1Rap;
	float m_genjet1Phi;
	float m_genjet2Pt;
	float m_genjet2Eta;
	float m_genjet2Rap;
	float m_genjet2Phi;
	float m_gendijet_mass;
};

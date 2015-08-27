#pragma once

#include "Artus/KappaAnalysis/interface/KappaProduct.h"

class JetProduct : public KappaProduct {
 public:
  std::string m_selectedHltName;
  int m_selectedHltPosition;

  std::vector<KLV*> m_matchedGenJets;
  std::vector<KBasicJet*> m_matchedRecoJets;

  std::vector<KLV*> m_validGenJets;

  std::vector<double> m_incJetsPt;
  std::vector<double> m_incJetsEta;
  std::vector<double> m_incJetsRap;
  std::vector<double> m_incJetsPhi;

  std::vector<int> m_match_result_genjets;
  std::vector<int> m_match_result_recojets;
  // Main observables to be set in the JetQuantities Producer
  // Dijet observables
  double m_njets;

  double m_jet1Pt;
  double m_jet1Eta;
  double m_jet1Rap;
  double m_jet1Phi;
  double m_jet2Pt;
  double m_jet2Eta;
  double m_jet2Rap;
  double m_jet2Phi;

  double m_dijet_mass;
  double m_dijet_ptavg;
  double m_dijet_ymax;
  double m_dijet_yinner;
  double m_dijet_youter;
  double m_dijet_deltaPhi;
  double m_dijet_cosThetaStar;
  double m_dijet_yboost;
  double m_dijet_ystar;
  double m_dijet_chi;

  double m_genjet1Pt;
  double m_genjet1Eta;
  double m_genjet1Rap;
  double m_genjet1Phi;
  double m_genjet2Pt;
  double m_genjet2Eta;
  double m_genjet2Rap;
  double m_genjet2Phi;

  double m_gendijet_mass;
  double m_gendijet_ptavg;
  double m_gendijet_ymax;
  double m_gendijet_yinner;
  double m_gendijet_youter;
  double m_gendijet_deltaPhi;
  double m_gendijet_cosThetaStar;
  double m_gendijet_yboost;
  double m_gendijet_ystar;
  double m_gendijet_chi;
};

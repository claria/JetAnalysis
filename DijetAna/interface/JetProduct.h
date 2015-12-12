#pragma once

#include "Artus/KappaAnalysis/interface/KappaProduct.h"

class JetProduct : public KappaProduct {
 public:
  std::string m_selectedHltName;
  int m_selectedHltPosition;

  std::vector<KBasicJet> m_corrJets;
  std::vector<KLV> m_validGenJets;
  std::vector<KBasicJet> m_validRecoJets;
  std::vector<KBasicJet> m_invalidRecoJets;

  // Jets are always matched from gen --> reco 
  // if there is no match for a genjet a nullptr is returned

  std::map<const KLV*, KBasicJet*> m_matchedRecoJets;
  std::map<const KBasicJet*, KLV*> m_matchedGenJets;
  std::map<const KBasicJet*, KGenParticle*> m_matchedPartons;
  // std::vector<KGenParticle*> m_matchedPartons;
  std::vector<int> m_matchResultGenJets;
  std::vector<int> m_matchResultRecoJets;

  std::map<const KBasicJet*, bool> m_doPassID;

  // Main observables to be set in the JetQuantities Producer
  std::vector<double> m_incJetsPt;
  std::vector<double> m_incJetsEta;
  std::vector<double> m_incJetsRap;
  std::vector<double> m_incJetsPhi;
  // Dijet observables
  double m_njets = -99999.;

  double m_jet1Pt = -99999.;
  double m_jet1Eta = -99999.;
  double m_jet1Rap = -99999.;
  double m_jet1Phi = -99999.;
  double m_jet2Pt = -99999.;
  double m_jet2Eta = -99999.;
  double m_jet2Rap = -99999.;
  double m_jet2Phi = -99999.;

  double m_dijet_mass = -99999.;
  double m_dijet_ptavg = -99999.;
  double m_dijet_jet12PtRatio = -99999.;
  double m_dijet_ymax = -99999.;
  double m_dijet_yinner = -99999.;
  double m_dijet_youter = -99999.;
  double m_dijet_deltaPhi = -99999.;
  double m_dijet_cosThetaStar = -99999.;
  double m_dijet_yboost = -99999.;
  double m_dijet_ystar = -99999.;
  double m_dijet_chi = -99999.;

  double m_genjet1Pt = -99999.;
  double m_genjet1Eta = -99999.;
  double m_genjet1Rap = -99999.;
  double m_genjet1Phi = -99999.;
  double m_genjet2Pt = -99999.;
  double m_genjet2Eta = -99999.;
  double m_genjet2Rap = -99999.;
  double m_genjet2Phi = -99999.;

  double m_gendijet_mass = -99999.;
  double m_gendijet_ptavg = -99999.;
  double m_gendijet_jet12PtRatio = -99999.;
  double m_gendijet_ymax = -99999.;
  double m_gendijet_yinner = -99999.;
  double m_gendijet_youter = -99999.;
  double m_gendijet_deltaPhi = -99999.;
  double m_gendijet_cosThetaStar = -99999.;
  double m_gendijet_yboost = -99999.;
  double m_gendijet_ystar = -99999.;
  double m_gendijet_chi = -99999.;
};

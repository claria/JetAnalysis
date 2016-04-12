#pragma once

#include "Artus/KappaAnalysis/interface/KappaProduct.h"

class JetProduct : public KappaProduct {
 public:
  std::string m_selectedHltName;
  int m_selectedHltPosition;

  // vector containing all corrected Jets
  std::vector<KBasicJet> m_corrJets;
  // vector containing all gen jets passing the criteria
  std::vector<KLV> m_validGenJets;
  // vector containing all reco jets passing the criteria
  std::vector<KBasicJet> m_validRecoJets;
  // vector containing all reco jets not passing the criteria
  std::vector<KBasicJet> m_invalidRecoJets;

  // map containing the matched reco jet of a gen jet
  std::map<const KLV*, KBasicJet*> m_matchedRecoJets;
  // map containing the matched gen jet of a reco jet
  std::map<const KBasicJet*, KLV*> m_matchedGenJets;
  // map containing the matched gen particle of a reco jet
  std::map<const KBasicJet*, KGenParticle*> m_matchedPartons;

  // match result containing the indices of the matched jets
  std::vector<int> m_matchResultGenJets;
  // match result containing the indices of the matched jets
  std::vector<int> m_matchResultRecoJets;

  std::map<const KBasicJet*, bool> m_doPassID;

  // Main observables to be set in the JetQuantities Producer
  std::vector<double> m_incJetsPt;
  std::vector<double> m_incJetsEta;
  std::vector<double> m_incJetsRap;
  std::vector<double> m_incJetsPhi;


  // Kinematic jet observables
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
  double m_dijet_deltaR = -99999.;
  double m_dijet_cosThetaStar = -99999.;
  double m_dijet_yboost = -99999.;
  double m_dijet_yboostnoabs = -99999.;
  double m_dijet_ystar = -99999.;
  double m_dijet_ystarnoabs = -99999.;
  double m_dijet_chi = -99999.;
  double m_dijet_ysbidx = -99999.;
  double m_dijet_ptavgidx = -99999.;
  double m_dijet_idx = -99999.;

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
  double m_gendijet_ysbidx = -99999.;
  double m_gendijet_ptavgidx = -99999.;
  double m_gendijet_idx = -99999.;


  double m_genmatchjet1Pt = -99999.;
  double m_genmatchjet1Eta = -99999.;
  double m_genmatchjet1Rap = -99999.;
  double m_genmatchjet1Phi = -99999.;
  double m_genmatchjet2Pt = -99999.;
  double m_genmatchjet2Eta = -99999.;
  double m_genmatchjet2Rap = -99999.;
  double m_genmatchjet2Phi = -99999.;


  double m_genmatchdijet_ptavg = -99999.;
  double m_genmatchdijet_yboost = -99999.;
  double m_genmatchdijet_ystar = -99999.;

  double m_genmatchdijet_ysbidx = -99999.;
  double m_genmatchdijet_ptavgidx = -99999.;
  double m_genmatchdijet_idx = -99999.;



};

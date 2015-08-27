#include <Math/VectorUtil.h>
#include <cmath>
#include <algorithm>

#include "Artus/Utility/interface/SafeMap.h"
#include "Artus/Utility/interface/Utility.h"
#include "Artus/Utility/interface/DefaultValues.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

// std::string JetLambdaNtupleConsumer::GetConsumerId() const
//{
//	return "JetLambdaNtupleConsumer";
//}

void JetLambdaNtupleConsumer::Init(setting_type const& settings) {
  // add possible quantities for the lambda ntuples consumers

  //////////////
  // MetaData //
  //////////////
  LambdaNtupleConsumer<JetTypes>::AddIntQuantity(
      "evt", [](JetEvent const& event, JetProduct const& product) { return event.m_eventInfo->nEvent; });
  /////////////
  // Weights //
  /////////////
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity("puweight", [](JetEvent const& event, JetProduct const& product) {
    return SafeMap::GetWithDefault(product.m_weights, std::string("puWeight"), 1.0);
  });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity("trigweight", [](JetEvent const& event, JetProduct const& product) {
    return SafeMap::GetWithDefault(product.m_weights, std::string("hltPrescaleWeight"), 1.0);
  });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity("weight",
                                                   [settings](JetEvent const& event, JetProduct const& product) {
    return SafeMap::GetWithDefault(product.m_weights, settings.GetEventWeight(), 1.0);
  });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity("genweight", [](JetEvent const& event, JetProduct const& product) {
    return SafeMap::GetWithDefault(product.m_weights, std::string("generatorWeight"), 1.0);
  });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity("xsweight", [](JetEvent const& event, JetProduct const& product) {
    return SafeMap::GetWithDefault(product.m_weights, std::string("crossSectionPerEventWeight"), 1.0);
  });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity("ngeneventsweight",
                                                   [](JetEvent const& event, JetProduct const& product) {
    return SafeMap::GetWithDefault(product.m_weights, std::string("numberGeneratedEventsWeight"), 1.0);
  });

  // // Kappa Trigger path index
  // LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
  //     "pathindex", [](JetEvent const& event, JetProduct const& product) { return product.m_selectedHltPosition; });
  // MET
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "met", [](JetEvent const& event, JetProduct const& product) { return event.m_met->p4.Pt(); });
  // SumEt
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "sumet", [](JetEvent const& event, JetProduct const& product) { return event.m_met->sumEt; });

  /////////////
  // PF Jets //
  /////////////
  //

  // Number of jets
  LambdaNtupleConsumer<JetTypes>::AddIntQuantity(
      "njets", [](JetEvent const& event, JetProduct const& product) { return product.m_validJets.size(); });
  // inclusive jet pts
  LambdaNtupleConsumer<JetTypes>::AddVDoubleQuantity(
      "incjets_pt", [](JetEvent const& event, JetProduct const& product) { return product.m_incJetsPt; });
  LambdaNtupleConsumer<JetTypes>::AddVDoubleQuantity(
      "incjets_eta", [](JetEvent const& event, JetProduct const& product) { return product.m_incJetsEta; });
  LambdaNtupleConsumer<JetTypes>::AddVDoubleQuantity(
      "incjets_rap", [](JetEvent const& event, JetProduct const& product) { return product.m_incJetsRap; });
  LambdaNtupleConsumer<JetTypes>::AddVDoubleQuantity(
      "incjets_phi", [](JetEvent const& event, JetProduct const& product) { return product.m_incJetsPhi; });

  // Leading jet pT
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "jet1_pt", [](JetEvent const& event, JetProduct const& product) { return product.m_jet1Pt; });
  // Leading jet eta
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "jet1_eta", [](JetEvent const& event, JetProduct const& product) { return product.m_jet1Eta; });
  // Leading jet rapidity
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "jet1_rap", [](JetEvent const& event, JetProduct const& product) { return product.m_jet1Rap; });
  // Leading jet phi
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "jet1_phi", [](JetEvent const& event, JetProduct const& product) { return product.m_jet1Phi; });
  // Second jet pT
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "jet2_pt", [](JetEvent const& event, JetProduct const& product) { return product.m_jet2Pt; });
  // Second jet eta
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "jet2_eta", [](JetEvent const& event, JetProduct const& product) { return product.m_jet2Eta; });
  // Second jet rapidity
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "jet2_rap", [](JetEvent const& event, JetProduct const& product) { return product.m_jet2Rap; });
  // Second jet phi
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "jet2_phi", [](JetEvent const& event, JetProduct const& product) { return product.m_jet2Phi; });

  ////////////
  // Dijets //
  ////////////
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_mass", [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_mass; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_ptavg", [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_ptavg; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_ymax", [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_ymax; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_deltaphi", [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_deltaPhi; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_costhetastar",
      [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_cosThetaStar; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_yboost", [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_yboost; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_yinner", [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_yinner; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_youter", [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_youter; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_ystar", [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_ystar; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "dijet_chi", [](JetEvent const& event, JetProduct const& product) { return product.m_dijet_chi; });

  /////////////
  // GenJets //
  /////////////
  //
  // Leading jet pT
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "genjet1_pt", [](JetEvent const& event, JetProduct const& product) { return product.m_genjet1Pt; });
  // Leading jet eta
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "genjet1_eta", [](JetEvent const& event, JetProduct const& product) { return product.m_genjet1Eta; });
  // Leading jet rapidity
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "genjet1_rap", [](JetEvent const& event, JetProduct const& product) { return product.m_genjet1Rap; });
  // Leading jet phi
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "genjet1_phi", [](JetEvent const& event, JetProduct const& product) { return product.m_genjet1Phi; });
  // Second jet pT
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "genjet2_pt", [](JetEvent const& event, JetProduct const& product) { return product.m_genjet2Pt; });
  // Second jet eta
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "genjet2_eta", [](JetEvent const& event, JetProduct const& product) { return product.m_genjet2Eta; });
  // Second jet rapidity
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "genjet2_rap", [](JetEvent const& event, JetProduct const& product) { return product.m_genjet2Rap; });
  // Second jet phi
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "genjet2_phi", [](JetEvent const& event, JetProduct const& product) { return product.m_genjet2Phi; });

  ////////////
  // Dijets //
  ////////////
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_mass", [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_mass; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_ptavg", [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_ptavg; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_ymax", [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_ymax; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_deltaphi",
      [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_deltaPhi; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_costhetastar",
      [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_cosThetaStar; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_yboost", [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_yboost; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_yinner", [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_yinner; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_youter", [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_youter; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_ystar", [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_ystar; });
  LambdaNtupleConsumer<JetTypes>::AddFloatQuantity(
      "gendijet_chi", [](JetEvent const& event, JetProduct const& product) { return product.m_gendijet_chi; });

  KappaLambdaNtupleConsumer::Init(settings);
}

#include <Math/VectorUtil.h>
#include <cmath>
#include <algorithm>

#include "Artus/Utility/interface/SafeMap.h"
#include "Artus/Utility/interface/Utility.h"
#include "Artus/Utility/interface/DefaultValues.h"
#include "Artus/KappaAnalysis/interface/KappaTypes.h"

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
  LambdaNtupleConsumer<KappaTypes>::AddIntQuantity(
      "evt", [](KappaEvent const& event,
                KappaProduct const& product) { return event.m_eventInfo->nEvent; });
  /////////////
  // Weights //
  /////////////
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "puweight", [](KappaEvent const& event, KappaProduct const& product) {
        return SafeMap::GetWithDefault(product.m_weights, std::string("puWeight"), 1.0);
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "trigweight", [](KappaEvent const& event, KappaProduct const& product) {
        return SafeMap::GetWithDefault(product.m_weights, std::string("hltPrescaleWeight"), 1.0);
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "weight", [settings](KappaEvent const& event, KappaProduct const& product) {
        return SafeMap::GetWithDefault(product.m_weights, settings.GetEventWeight(), 1.0);
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "genweight", [](KappaEvent const& event, KappaProduct const& product) {
        return SafeMap::GetWithDefault(product.m_weights, std::string("generatorWeight"), 1.0);
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "xsweight", [](KappaEvent const& event, KappaProduct const& product) {
        return SafeMap::GetWithDefault(product.m_weights, std::string("crossSectionPerEventWeight"),
                                       1.0);
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "ngeneventsweight", [](KappaEvent const& event, KappaProduct const& product) {
        return SafeMap::GetWithDefault(product.m_weights,
                                       std::string("numberGeneratedEventsWeight"), 1.0);
      });

  // Kappa Trigger path index
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "pathindex", [](KappaEvent const& event,
                      KappaProduct const& product) { return product.m_selectedHltPosition; });
  // MET
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "met",
      [](KappaEvent const& event, KappaProduct const& product) { return event.m_met->p4.Pt(); });
  // SumEt
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "sumet",
      [](KappaEvent const& event, KappaProduct const& product) { return event.m_met->sumEt; });

  /////////////
  // PF Jets //
  /////////////
  //

  // Number of jets
  LambdaNtupleConsumer<KappaTypes>::AddIntQuantity(
      "njets", [](KappaEvent const& event,
                  KappaProduct const& product) { return product.m_validJets.size(); });
  // inclusive jet pts
  LambdaNtupleConsumer<KappaTypes>::AddVDoubleQuantity(
      "incjets_pt", [](KappaEvent const& event, KappaProduct const& product) {
        auto const& jetProduct = static_cast<JetProduct const&>(product);
        return jetProduct.m_incJetsPt;
      });
  LambdaNtupleConsumer<KappaTypes>::AddVDoubleQuantity(
      "incjets_eta", [](KappaEvent const& event, KappaProduct const& product) {
        auto const& jetProduct = static_cast<JetProduct const&>(product);
        return jetProduct.m_incJetsEta;
      });
  LambdaNtupleConsumer<KappaTypes>::AddVDoubleQuantity(
      "incjets_rap", [](KappaEvent const& event, KappaProduct const& product) {
        auto const& jetProduct = static_cast<JetProduct const&>(product);
        return jetProduct.m_incJetsRap;
      });
  LambdaNtupleConsumer<KappaTypes>::AddVDoubleQuantity(
      "incjets_phi", [](KappaEvent const& event, KappaProduct const& product) {
        auto const& jetProduct = static_cast<JetProduct const&>(product);
        return jetProduct.m_incJetsPhi;
      });

  // Leading jet pT
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "jet1_pt", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 1) ? product.m_validJets.at(0)->p4.Pt() : -999.;
      });
  // Leading jet eta
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "jet1_eta", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 1) ? product.m_validJets.at(0)->p4.Eta() : -999.;
      });
  // Leading jet rapidity
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "jet1_rap", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 1) ? product.m_validJets.at(0)->p4.Rapidity() : -999.;
      });
  // Leading jet phi
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "jet1_phi", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 1) ? product.m_validJets.at(0)->p4.Phi() : -999.;
      });
  // Second jet pT
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "jet2_pt", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2) ? product.m_validJets.at(1)->p4.Pt() : -999.;
      });
  // Second jet eta
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "jet2_eta", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2) ? product.m_validJets.at(1)->p4.Eta() : -999.;
      });
  // Second jet rapidity
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "jet2_rap", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2) ? product.m_validJets.at(1)->p4.Rapidity() : -999.;
      });
  // Second jet phi
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "jet2_phi", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2) ? product.m_validJets.at(1)->p4.Phi() : -999.;
      });

  ////////////
  // Dijets //
  ////////////
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "dijet_mass", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2)
                   ? (product.m_validJets.at(0)->p4 + product.m_validJets.at(1)->p4).mass()
                   : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "pt12_avg", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2)
                   ? 0.5 * (product.m_validJets.at(0)->p4.Pt() + product.m_validJets.at(1)->p4.Pt())
                   : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_ymax", [](KappaEvent const& event,
                                                                      KappaProduct const& product) {
    return (product.m_validJets.size() >= 2) ? std::max(product.m_validJets.at(0)->p4.Rapidity(),
                                                        product.m_validJets.at(1)->p4.Rapidity())
                                             : -999.;
  });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "dijet_deltaphi", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2) ? std::abs(product.m_validJets.at(0)->p4.Phi() -
                                                            product.m_validJets.at(1)->p4.Phi())
                                                 : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "dijet_costhetastar", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2)
                   ? std::tanh(product.m_validJets.at(0)->p4.Rapidity() -
                               product.m_validJets.at(1)->p4.Rapidity())
                   : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "dijet_yboost", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2)
                   ? 0.5 * (product.m_validJets.at(0)->p4.Rapidity() +
                            product.m_validJets.at(1)->p4.Rapidity())
                   : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "dijet_yinner", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2)
                   ? ((std::abs(product.m_validJets.at(0)->p4.Rapidity()) <=
                       std::abs(product.m_validJets.at(1)->p4.Rapidity()))
                          ? product.m_validJets.at(0)->p4.Rapidity()
                          : product.m_validJets.at(1)->p4.Rapidity())
                   : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "dijet_youter", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2)
                   ? ((std::abs(product.m_validJets.at(0)->p4.Rapidity()) >
                       std::abs(product.m_validJets.at(1)->p4.Rapidity()))
                          ? product.m_validJets.at(0)->p4.Rapidity()
                          : product.m_validJets.at(1)->p4.Rapidity())
                   : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "dijet_ystar", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2)
                   ? 0.5 * std::abs(product.m_validJets.at(0)->p4.Rapidity() -
                                    product.m_validJets.at(1)->p4.Rapidity())
                   : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "dijet_chi", [](KappaEvent const& event, KappaProduct const& product) {
        return (product.m_validJets.size() >= 2)
                   ? exp(std::abs(product.m_validJets.at(0)->p4.Rapidity() -
                                  product.m_validJets.at(1)->p4.Rapidity()))
                   : -999.;
      });

  /////////////
  // GenJets //
  /////////////

  // Dijet quantities
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "gendijet_mass", [](KappaEvent const& event, KappaProduct const& product) {
        return (event.m_genJets->size() >= 2)
                   ? (event.m_genJets->at(0).p4 + event.m_genJets->at(1).p4).mass()
                   : -999.;
      });
  // Leading GenJet
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "genjet1_pt", [](KappaEvent const& event, KappaProduct const& product) {
        return (event.m_genJets->size() >= 1) ? event.m_genJets->at(0).p4.Pt() : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "genjet1_eta", [](KappaEvent const& event, KappaProduct const& product) {
        return (event.m_genJets->size() >= 1) ? event.m_genJets->at(0).p4.Eta() : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "genjet1_rap", [](KappaEvent const& event, KappaProduct const& product) {
        return (event.m_genJets->size() >= 1) ? event.m_genJets->at(0).p4.Rapidity() : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "genjet1_phi", [](KappaEvent const& event, KappaProduct const& product) {
        return (event.m_genJets->size() >= 1) ? event.m_genJets->at(0).p4.Phi() : -999.;
      });
  // Second GenJet
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "genjet2_pt", [](KappaEvent const& event, KappaProduct const& product) {
        return (event.m_genJets->size() >= 2) ? event.m_genJets->at(1).p4.Pt() : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "genjet2_eta", [](KappaEvent const& event, KappaProduct const& product) {
        return (event.m_genJets->size() >= 2) ? event.m_genJets->at(1).p4.Eta() : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "genjet2_rap", [](KappaEvent const& event, KappaProduct const& product) {
        return (event.m_genJets->size() >= 2) ? event.m_genJets->at(1).p4.Rapidity() : -999.;
      });
  LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity(
      "genjet2_phi", [](KappaEvent const& event, KappaProduct const& product) {
        return (event.m_genJets->size() >= 2) ? event.m_genJets->at(1).p4.Phi() : -999.;
      });

  KappaLambdaNtupleConsumer::Init(settings);
}

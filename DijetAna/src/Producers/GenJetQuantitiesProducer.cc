#include "JetAnalysis/DijetAna/interface/Producers/GenJetQuantitiesProducer.h"

std::string GenJetQuantitiesProducer::GetProducerId() const {
  return "GenJetQuantitiesProducer";
}

void GenJetQuantitiesProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  auto& jetProduct = static_cast<JetProduct&>(product);

  jetProduct.m_genjet1Pt = (product.m_validGenJets.size() > 0) ? product.m_validGenJets.at(0).p4.Pt() : -999.;
  jetProduct.m_genjet1Eta = (product.m_validGenJets.size() > 0) ? product.m_validGenJets.at(0).p4.Eta() : -999.;
  jetProduct.m_genjet1Rap = (product.m_validGenJets.size() > 0) ? product.m_validGenJets.at(0).p4.Rapidity() : -999.;
  jetProduct.m_genjet1Phi = (product.m_validGenJets.size() > 0) ? product.m_validGenJets.at(0).p4.Phi() : -999.;

  jetProduct.m_genjet2Pt = (product.m_validGenJets.size() > 1) ? product.m_validGenJets.at(1).p4.Pt() : -999.;
  jetProduct.m_genjet2Eta = (product.m_validGenJets.size() > 1) ? product.m_validGenJets.at(1).p4.Eta() : -999.;
  jetProduct.m_genjet2Rap = (product.m_validGenJets.size() > 1) ? product.m_validGenJets.at(1).p4.Rapidity() : -999.;
  jetProduct.m_genjet2Phi = (product.m_validGenJets.size() > 1) ? product.m_validGenJets.at(1).p4.Phi() : -999.;

  jetProduct.m_gendijet_mass = (product.m_validGenJets.size() > 1)
                                   ? (product.m_validGenJets.at(0).p4 + product.m_validGenJets.at(1).p4).mass()
                                   : -999.;
  jetProduct.m_gendijet_ptavg =
      (product.m_validGenJets.size() > 1)
          ? 0.5 * (product.m_validGenJets.at(0).p4.Pt() + product.m_validGenJets.at(1).p4.Pt())
          : -999.;
  jetProduct.m_gendijet_jet12PtRatio =
      (product.m_validGenJets.size() > 1)
          ? (product.m_validGenJets.at(1).p4.Pt()/product.m_validGenJets.at(0).p4.Pt())
          : -999.;

  jetProduct.m_gendijet_ymax =
      (product.m_validGenJets.size() > 1)
          ? std::max(product.m_validGenJets.at(0).p4.Rapidity(), product.m_validGenJets.at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_yinner = (product.m_validGenJets.size() > 1)
                                     ? ((std::abs(product.m_validGenJets.at(0).p4.Rapidity()) <=
                                         std::abs(product.m_validGenJets.at(1).p4.Rapidity()))
                                            ? product.m_validGenJets.at(0).p4.Rapidity()
                                            : product.m_validGenJets.at(1).p4.Rapidity())
                                     : -999.;
  jetProduct.m_gendijet_youter = (product.m_validGenJets.size() > 1)
                                     ? ((std::abs(product.m_validGenJets.at(0).p4.Rapidity()) >
                                         std::abs(product.m_validGenJets.at(1).p4.Rapidity()))
                                            ? product.m_validGenJets.at(0).p4.Rapidity()
                                            : product.m_validGenJets.at(1).p4.Rapidity())
                                     : -999.;
  jetProduct.m_gendijet_deltaPhi =
      (product.m_validGenJets.size() > 1)
          ? std::abs(product.m_validGenJets.at(0).p4.Phi() - product.m_validGenJets.at(1).p4.Phi())
          : -999.;
  jetProduct.m_gendijet_cosThetaStar =
      (product.m_validGenJets.size() > 1)
          ? std::tanh(product.m_validGenJets.at(0).p4.Rapidity() - product.m_validGenJets.at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_yboost =
      (product.m_validGenJets.size() > 1)
          ? 0.5 * std::abs(product.m_validGenJets.at(0).p4.Rapidity() + product.m_validGenJets.at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_ystar =
      (product.m_validGenJets.size() > 1)
          ? 0.5 * std::abs(product.m_validGenJets.at(0).p4.Rapidity() - product.m_validGenJets.at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_chi =
      (product.m_validGenJets.size() > 1)
          ? exp(std::abs(product.m_validGenJets.at(0).p4.Rapidity() - product.m_validGenJets.at(1).p4.Rapidity()))
          : -999.;
}

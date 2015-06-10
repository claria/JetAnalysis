#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/Producers/JetQuantitiesProducer.h"

std::string JetQuantitiesProducer::GetProducerId() const {
  return "JetQuantitiesProducer";
}

void JetQuantitiesProducer::Produce(JetEvent const& event, JetProduct& product,
                                    JetSettings const& settings) const {
  auto& jetProduct = static_cast<JetProduct&>(product);

  // Inclusive Jets
  for (auto i = product.m_validJets.begin(); i != product.m_validJets.end();
       i++) {
    jetProduct.m_incJetsPt.push_back((*i)->p4.Pt());
    jetProduct.m_incJetsEta.push_back((*i)->p4.Eta());
    jetProduct.m_incJetsRap.push_back((*i)->p4.Rapidity());
    jetProduct.m_incJetsPhi.push_back((*i)->p4.Phi());
  }

  jetProduct.m_jet1Pt = (jetProduct.m_validJets.size() > 0)
                            ? jetProduct.m_validJets.at(0)->p4.Pt()
                            : -999.;
  jetProduct.m_jet1Eta = (jetProduct.m_validJets.size() > 0)
                             ? jetProduct.m_validJets.at(0)->p4.Eta()
                             : -999.;
  jetProduct.m_jet1Rap = (jetProduct.m_validJets.size() > 0)
                             ? jetProduct.m_validJets.at(0)->p4.Rapidity()
                             : -999.;
  jetProduct.m_jet1Phi = (jetProduct.m_validJets.size() > 0)
                             ? jetProduct.m_validJets.at(0)->p4.Phi()
                             : -999.;

  jetProduct.m_jet2Pt = (jetProduct.m_validJets.size() > 1)
                            ? jetProduct.m_validJets.at(1)->p4.Pt()
                            : -999.;
  jetProduct.m_jet2Eta = (jetProduct.m_validJets.size() > 1)
                             ? jetProduct.m_validJets.at(1)->p4.Eta()
                             : -999.;
  jetProduct.m_jet2Rap = (jetProduct.m_validJets.size() > 1)
                             ? jetProduct.m_validJets.at(1)->p4.Rapidity()
                             : -999.;
  jetProduct.m_jet2Phi = (jetProduct.m_validJets.size() > 1)
                             ? jetProduct.m_validJets.at(1)->p4.Phi()
                             : -999.;

  jetProduct.m_dijet_mass = (jetProduct.m_validJets.size() > 1)
                                ? (jetProduct.m_validJets.at(0)->p4 +
                                   jetProduct.m_validJets.at(1)->p4).mass()
                                : -999.;
  jetProduct.m_dijet_ptavg =
      (jetProduct.m_validJets.size() > 1)
          ? 0.5 * (jetProduct.m_validJets.at(0)->p4.Pt() +
                   jetProduct.m_validJets.at(1)->p4.Pt())
          : -999.;
  jetProduct.m_dijet_ymax =
      (jetProduct.m_validJets.size() > 1)
          ? std::max(jetProduct.m_validJets.at(0)->p4.Rapidity(),
                     jetProduct.m_validJets.at(1)->p4.Rapidity())
          : -999.;
  jetProduct.m_dijet_yinner =
      (jetProduct.m_validJets.size() > 1)
          ? ((std::abs(jetProduct.m_validJets.at(0)->p4.Rapidity()) <=
              std::abs(jetProduct.m_validJets.at(1)->p4.Rapidity()))
                 ? jetProduct.m_validJets.at(0)->p4.Rapidity()
                 : jetProduct.m_validJets.at(1)->p4.Rapidity())
          : -999.;
  jetProduct.m_dijet_youter =
      (jetProduct.m_validJets.size() > 1)
          ? ((std::abs(jetProduct.m_validJets.at(0)->p4.Rapidity()) >
              std::abs(jetProduct.m_validJets.at(1)->p4.Rapidity()))
                 ? jetProduct.m_validJets.at(0)->p4.Rapidity()
                 : jetProduct.m_validJets.at(1)->p4.Rapidity())
          : -999.;
  jetProduct.m_dijet_deltaPhi =
      (jetProduct.m_validJets.size() > 1)
          ? std::abs(jetProduct.m_validJets.at(0)->p4.Phi() -
                     jetProduct.m_validJets.at(1)->p4.Phi())
          : -999.;
  jetProduct.m_dijet_cosThetaStar =
      (jetProduct.m_validJets.size() > 1)
          ? std::tanh(jetProduct.m_validJets.at(0)->p4.Rapidity() -
                      jetProduct.m_validJets.at(1)->p4.Rapidity())
          : -999.;
  jetProduct.m_dijet_yboost =
      (jetProduct.m_validJets.size() > 1)
          ? 0.5 * (jetProduct.m_validJets.at(0)->p4.Rapidity() +
                   jetProduct.m_validJets.at(1)->p4.Rapidity())
          : -999.;
  jetProduct.m_dijet_ystar =
      (jetProduct.m_validJets.size() > 1)
          ? 0.5 * std::abs(jetProduct.m_validJets.at(0)->p4.Rapidity() -
                           jetProduct.m_validJets.at(1)->p4.Rapidity())
          : -999.;
  jetProduct.m_dijet_chi =
      (jetProduct.m_validJets.size() > 1)
          ? exp(std::abs(jetProduct.m_validJets.at(0)->p4.Rapidity() -
                         jetProduct.m_validJets.at(1)->p4.Rapidity()))
          : -999.;
}

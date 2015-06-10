#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/Producers/GenJetQuantitiesProducer.h"

std::string GenJetQuantitiesProducer::GetProducerId() const {
  return "GenJetQuantitiesProducer";
}

void GenJetQuantitiesProducer::Produce(JetEvent const& event,
                                       JetProduct& product,
                                       JetSettings const& settings) const {
  auto& jetProduct = static_cast<JetProduct&>(product);

  jetProduct.m_genjet1Pt =
      (event.m_genJets->size() > 0) ? event.m_genJets->at(0).p4.Pt() : -999.;
  jetProduct.m_genjet1Eta =
      (event.m_genJets->size() > 0) ? event.m_genJets->at(0).p4.Eta() : -999.;
  jetProduct.m_genjet1Rap = (event.m_genJets->size() > 0)
                                ? event.m_genJets->at(0).p4.Rapidity()
                                : -999.;
  jetProduct.m_genjet1Phi =
      (event.m_genJets->size() > 0) ? event.m_genJets->at(0).p4.Phi() : -999.;

  jetProduct.m_genjet2Pt =
      (event.m_genJets->size() > 1) ? event.m_genJets->at(1).p4.Pt() : -999.;
  jetProduct.m_genjet2Eta =
      (event.m_genJets->size() > 1) ? event.m_genJets->at(1).p4.Eta() : -999.;
  jetProduct.m_genjet2Rap = (event.m_genJets->size() > 1)
                                ? event.m_genJets->at(1).p4.Rapidity()
                                : -999.;
  jetProduct.m_genjet2Phi =
      (event.m_genJets->size() > 1) ? event.m_genJets->at(1).p4.Phi() : -999.;

  jetProduct.m_gendijet_mass =
      (event.m_genJets->size() > 1)
          ? (event.m_genJets->at(0).p4 + event.m_genJets->at(1).p4).mass()
          : -999.;
  jetProduct.m_gendijet_ptavg = (event.m_genJets->size() > 1)
                                    ? 0.5 * (event.m_genJets->at(0).p4.Pt() +
                                             event.m_genJets->at(1).p4.Pt())
                                    : -999.;
  jetProduct.m_gendijet_ymax =
      (event.m_genJets->size() > 1)
          ? std::max(event.m_genJets->at(0).p4.Rapidity(),
                     event.m_genJets->at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_yinner =
      (event.m_genJets->size() > 1)
          ? ((std::abs(event.m_genJets->at(0).p4.Rapidity()) <=
              std::abs(event.m_genJets->at(1).p4.Rapidity()))
                 ? event.m_genJets->at(0).p4.Rapidity()
                 : event.m_genJets->at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_youter =
      (event.m_genJets->size() > 1)
          ? ((std::abs(event.m_genJets->at(0).p4.Rapidity()) >
              std::abs(event.m_genJets->at(1).p4.Rapidity()))
                 ? event.m_genJets->at(0).p4.Rapidity()
                 : event.m_genJets->at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_deltaPhi =
      (event.m_genJets->size() > 1) ? std::abs(event.m_genJets->at(0).p4.Phi() -
                                               event.m_genJets->at(1).p4.Phi())
                                    : -999.;
  jetProduct.m_gendijet_cosThetaStar =
      (event.m_genJets->size() > 1)
          ? std::tanh(event.m_genJets->at(0).p4.Rapidity() -
                      event.m_genJets->at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_yboost =
      (event.m_genJets->size() > 1)
          ? 0.5 * (event.m_genJets->at(0).p4.Rapidity() +
                   event.m_genJets->at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_ystar =
      (event.m_genJets->size() > 1)
          ? 0.5 * std::abs(event.m_genJets->at(0).p4.Rapidity() -
                           event.m_genJets->at(1).p4.Rapidity())
          : -999.;
  jetProduct.m_gendijet_chi =
      (event.m_genJets->size() > 1)
          ? exp(std::abs(event.m_genJets->at(0).p4.Rapidity() -
                         event.m_genJets->at(1).p4.Rapidity()))
          : -999.;
}

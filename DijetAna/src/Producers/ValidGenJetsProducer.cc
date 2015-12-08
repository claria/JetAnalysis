#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include "JetAnalysis/DijetAna/interface/Producers/ValidGenJetsProducer.h"

std::string ValidGenJetsProducer::GetProducerId() const {
  return "ValidGenJetsProducer";
}

void ValidGenJetsProducer::Init(JetSettings const& settings) {
  minValidGenJetPt = settings.GetMinValidGenJetPt();
  minValidGenJetAbsRap = settings.GetMinValidGenJetAbsRap();
  maxValidGenJetAbsRap = settings.GetMaxValidGenJetAbsRap();
}
void ValidGenJetsProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  for (auto jet : *(event.m_genJets)) {
    bool validJet = true;
    validJet = validJet
      && (jet.p4.Pt() > minValidGenJetPt) 
      && (std::abs(jet.p4.Rapidity()) >= minValidGenJetAbsRap) 
      && (std::abs(jet.p4.Rapidity()) < maxValidGenJetAbsRap);
    if (validJet) 
      product.m_validGenJets.push_back(jet);
  }
}



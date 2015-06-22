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

  for (auto genjet = event.m_genJets->begin(); genjet != event.m_genJets->end(); genjet++) {
    if (AdditionalCriteria(&(*genjet), event, product, settings)) {
      product.m_validGenJets.push_back(&(*genjet));
    }
  }
  // for (auto i = product.m_validGenJets.begin(); i != product.m_validGenJets.end(); ++i)
  //   std::cout << (*i)->p4.Pt() << ' ';
  // std::cout <<std::endl;
}

bool ValidGenJetsProducer::AdditionalCriteria(KLV* jet, JetEvent const& event,
                                                 JetProduct& product, JetSettings const& settings) const
{
  if ((jet->p4.Pt() > minValidGenJetPt) &&
      (std::abs(jet->p4.Rapidity()) >= minValidGenJetAbsRap) &&
      (std::abs(jet->p4.Rapidity()) < maxValidGenJetAbsRap)) {
    return true;
  }
  return false;
}

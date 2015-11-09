#include "JetAnalysis/DijetAna/interface/Producers/JERScalingProducer.h"

std::string JERScalingProducer::GetProducerId() const {
  return "JERScalingProducer";
}

void JERScalingProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);
}

void JERScalingProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  for (auto jet = product.m_validJets.begin(); jet != product.m_validJets.end(); jet++) {
    size_t idx = jet - product.m_validJets.begin();
    double corr = this->GetScalingFactor( (*jet)->p4.Eta() );
    // double oldpt = (*jet)->p4.Pt();
    if (product.m_matchedGenJets.at(idx) != NULL) {
      (*jet)->p4.SetPt(  std::max(0., product.m_matchedGenJets.at(idx)->p4.Pt() + corr * ((*jet)->p4.Pt() - product.m_matchedGenJets.at(idx)->p4.Pt())));
      // std::cout << "Shifting old pt " << oldpt << " to " << (*jet)->p4.Pt() << std::endl;
    }
    //Scale reco (corrected) jet pT based on the pT difference between matched reco and gen jets:
    //pT->max[0.,pTgen+c*(pT–pTgen)], 
  }
}

double JERScalingProducer::GetScalingFactor(double eta) const {
  double abseta = std::abs(eta);
  if (abseta > 0.0 && abseta <= 0.5) {
    return 1.079;
  }
  else if (abseta > 0.5 && abseta <= 1.1) {
    return 1.099;
  }
  else if (abseta > 1.1 && abseta <= 1.7) {
    return 1.121;
  }
  else if (abseta > 1.7 && abseta <= 2.3) {
    return 1.208;
  }
  else if (abseta > 2.3 && abseta <= 2.8) {
    return 1.254;
  }
  else if (abseta > 2.8 && abseta <= 3.2) {
    return 1.395;
  }
  else if (abseta > 3.2 && abseta <= 5.0) {
    return 1.056;
  }
  else {
    LOG(FATAL) << "WARNING. Jet outside |eta| < 5" << std::endl;
    return 0.0;
  }
}

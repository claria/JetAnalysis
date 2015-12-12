#include "JetAnalysis/DijetAna/interface/Producers/JERScalingProducer.h"

std::string JERScalingProducer::GetProducerId() const {
  return "JERScalingProducer";
}

void JERScalingProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);
}

void JERScalingProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  for (auto & recojet : product.m_validRecoJets) {
    double corr = this->GetScalingFactor(recojet.p4.Eta());
    if (product.m_matchedGenJets.at(&recojet) != nullptr) {
      recojet.p4.SetPt(std::max(0., product.m_matchedGenJets.at(&recojet)->p4.Pt() + corr * (recojet.p4.Pt() - product.m_matchedGenJets.at(&recojet)->p4.Pt())));
    }
  }
}

// These are the JER MC--> Data Scaling factors for th 2012 run
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

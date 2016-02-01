#include "JetAnalysis/DijetAna/interface/Producers/JERScalingProducer.h"

std::string JERScalingProducer::GetProducerId() const {
  return "JERScalingProducer";
}

void JERScalingProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);
  m_jerScalingFactors = settings.GetJERScalingFactors();
  assert(m_jerScalingFactors.size() == 7);
}

void JERScalingProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  for (auto & recojet : product.m_validRecoJets) {
    double corr = this->GetScalingFactor(recojet.p4.Eta());
    if (product.m_matchedGenJets.at(&recojet) != nullptr) {
      recojet.p4.SetPt(std::max(0., product.m_matchedGenJets.at(&recojet)->p4.Pt() + corr * (recojet.p4.Pt() - product.m_matchedGenJets.at(&recojet)->p4.Pt())));
    }
  }
}

double JERScalingProducer::GetScalingFactor(double eta) const {
  double abseta = std::abs(eta);
  if (abseta > 0.0 && abseta <= 0.5) {
    return m_jerScalingFactors.at(0);
  }
  else if (abseta > 0.5 && abseta <= 1.1) {
    return m_jerScalingFactors.at(1);
  }
  else if (abseta > 1.1 && abseta <= 1.7) {
    return m_jerScalingFactors.at(2);
  }
  else if (abseta > 1.7 && abseta <= 2.3) {
    return m_jerScalingFactors.at(3);
  }
  else if (abseta > 2.3 && abseta <= 2.8) {
    return m_jerScalingFactors.at(4);
  }
  else if (abseta > 2.8 && abseta <= 3.2) {
    return m_jerScalingFactors.at(5);
  }
  else if (abseta > 3.2 && abseta <= 5.0) {
    return m_jerScalingFactors.at(6);
  }
  else {
    LOG(FATAL) << "WARNING. Jet outside |eta| < 5" << std::endl;
    return 0.0;
  }
}

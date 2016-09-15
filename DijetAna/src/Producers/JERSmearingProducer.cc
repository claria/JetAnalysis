#include "JetAnalysis/DijetAna/interface/Producers/JERSmearingProducer.h"

#include <KappaTools/RootTools/interface/Matching.h>
#include <TRandom.h>

std::string JERSmearingProducer::GetProducerId() const {
  return "JERSmearingProducer";
}

void JERSmearingProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);
  m_jerScalingFactors = settings.GetJERScalingFactors();
  assert(m_jerScalingFactors.size() == 7);
  random = new TRandom();
}

void JERSmearingProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {

  for (auto & corrJet :product.m_corrJets) {
    double sigma_rel = get_sigma_rel(corrJet.p4.Pt(), corrJet.p4.Eta());;
    // std::cout << "pt " << corrJet.p4.Pt() << " eta " << corrJet.p4.Eta() << " sigmarel " << sigma_rel << std::endl;
    // std::cout << "width " << sqrt(std::pow(GetScalingFactor(corrJet.p4.Eta()),2) -1) * sigma_rel * corrJet.p4.Pt() << std::endl;
    double shift = random->Gaus(0.0, sqrt(std::pow(GetScalingFactor(corrJet.p4.Eta()),2) -1) * sigma_rel * corrJet.p4.Pt());
    // std::cout << "shift " << shift << std::endl;
    corrJet.p4.SetPt(corrJet.p4.Pt() + shift);
  }

  std::sort(product.m_corrJets.begin(),
            product.m_corrJets.end(),
            [](KBasicJet jet1, KBasicJet jet2)
                -> bool { return jet1.p4.Pt() > jet2.p4.Pt(); });

}

double JERSmearingProducer::get_sigma_rel(double pt, double eta) const {
  if (std::abs(eta) < 0.5)
    return sqrt(std::pow(5.4165E-5,2)/std::pow(pt,2) + std::pow(0.903,2)/pt + std::pow(0.034,2));
  if (std::abs(eta) < 1.1)
    return sqrt(std::pow(2.28290,2)/std::pow(pt,2) + std::pow(0.8807,2)/pt + std::pow(0.038,2));
  if (std::abs(eta) < 1.7)
    return sqrt(std::pow(4.129299,2)/std::pow(pt,2) + std::pow(0.8916,2)/pt + std::pow(0.037,2));
  if (std::abs(eta) < 2.3)
    return sqrt(std::pow(4.879,2)/std::pow(pt,2) + std::pow(0.6925,2)/pt + std::pow(0.023,2));
  if (std::abs(eta) < 2.8)
    return sqrt(std::pow(6.0351,2)/std::pow(pt,2) + std::pow(0.6039,2)/pt + std::pow(0.031,2));
  if (std::abs(eta) < 3.2)
    return sqrt(std::pow(5.05786,2)/std::pow(pt,2) + std::pow(0.7751,2)/pt + std::pow(0.082,2));
  else
    return sqrt(std::pow(3.35720,2)/std::pow(pt,2) + std::pow(0.7587,2)/pt + std::pow(0.084,2));
}

double JERSmearingProducer::GetScalingFactor(double eta) const {
  double abseta = std::abs(eta);
  if (abseta >= 0.0 && abseta <= 0.5) {
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
    return 1.0;
  }
}

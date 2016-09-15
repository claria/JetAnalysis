#include "JetAnalysis/DijetAna/interface/Producers/JERScalingProducer.h"

#include <KappaTools/RootTools/interface/Matching.h>

std::string JERScalingProducer::GetProducerId() const {
  return "JERScalingProducer";
}


double get_sigma_rel(double pt, double eta) {
  if (std::abs(eta) < 0.5)
    return sqrt(std::pow(6.251,2)/std::pow(pt,2) + std::pow(1.062,2)/pt + std::pow(0.031,2));
  if (std::abs(eta) < 1.1)
    return sqrt(std::pow(6.710,2)/std::pow(pt,2) + std::pow(1.047,2)/pt + std::pow(0.036,2));
  if (std::abs(eta) < 1.7)
    return sqrt(std::pow(7.114,2)/std::pow(pt,2) + std::pow(1.119,2)/pt + std::pow(0.041,2));
  if (std::abs(eta) < 2.3)
    return sqrt(std::pow(7.845,2)/std::pow(pt,2) + std::pow(1.047,2)/pt + std::pow(1.08E-06,2));
  if (std::abs(eta) < 2.8)
    return sqrt(std::pow(7.330,2)/std::pow(pt,2) + std::pow(0.887,2)/pt + std::pow(1.58E-5,2));
  if (std::abs(eta) < 3.2)
    return sqrt(std::pow(6.389,2)/std::pow(pt,2) + std::pow(1.191,2)/pt + std::pow(1.818E-6,2));
  else
    return sqrt(std::pow(6.389,2)/std::pow(pt,2) + std::pow(1.191,2)/pt + std::pow(1.818E-6,2));
}

void JERScalingProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);
  m_jerScalingFactors = settings.GetJERScalingFactors();
  maxDeltaR = settings.GetMaxDeltaR();
  assert(m_jerScalingFactors.size() == 7);
}




void JERScalingProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  // for (auto & recojet : product.m_validRecoJets) {
  //   double corr = this->GetScalingFactor(recojet.p4.Eta());
  //   if (product.m_matchedGenJets.at(&recojet) != nullptr) {
  //     recojet.p4.SetPt(std::max(0., product.m_matchedGenJets.at(&recojet)->p4.Pt() + corr * (recojet.p4.Pt() - product.m_matchedGenJets.at(&recojet)->p4.Pt())));
  //   }
  // }
  // for (auto & genjet : product.m_validGenJets) {
  //   double corr = this->GetScalingFactor(genjet.p4.Eta());
  //   if (product.m_matchedRecoJets.at(&genjet) != nullptr) {
  //     std::cout << "shifting from reco pt " << product.m_matchedRecoJets.at(&genjet)->p4.Pt() << std::endl;
  //     product.m_matchedRecoJets.at(&genjet)->p4.SetPt(std::max(0., genjet.p4.Pt() + corr * (product.m_matchedRecoJets.at(&genjet)->p4.Pt() - genjet.p4.Pt())));
  //     std::cout << "shifting to reco pt " << product.m_matchedRecoJets.at(&genjet)->p4.Pt() << std::endl;
  //   }
  // }
  std::vector<KLV> genJets = *event.m_genJets;

  static matchSort_deltaRdeltaPtVar metric(maxDeltaR);
  std::vector<int> matchResult = matchSort_Matrix<KBasicJet, KLV>(product.m_corrJets, product.m_corrJets.size(), 
                                                                         genJets, genJets.size(), 
                                                                         metric);

  for (auto genJet = event.m_genJets->begin(); genJet != event.m_genJets->end(); ++genJet) {
    size_t idx = genJet - event.m_genJets->begin();

    if (matchResult[idx] < 0) {
      continue;
    }
    else {
      double shift = this->GetScalingFactor((*genJet).p4.Eta());
      product.m_corrJets.at(matchResult[idx]).p4.SetPt(std::max(0., genJet->p4.Pt() + shift*(product.m_corrJets.at(matchResult[idx]).p4.Pt() - genJet->p4.Pt())));
    }
  }

  std::sort(product.m_corrJets.begin(),
            product.m_corrJets.end(),
            [](KBasicJet jet1, KBasicJet jet2)
                -> bool { return jet1.p4.Pt() > jet2.p4.Pt(); });

}

double JERScalingProducer::GetScalingFactor(double eta) const {
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

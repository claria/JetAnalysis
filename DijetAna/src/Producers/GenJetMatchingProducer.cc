#include "JetAnalysis/DijetAna/interface/Producers/GenJetMatchingProducer.h"
#include <KappaTools/RootTools/interface/Matching.h>

std::string GenJetMatchingProducer::GetProducerId() const {
  return "GenJetMatchingProducer";
}

void GenJetMatchingProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);
  maxDeltaR = settings.GetMaxDeltaR();
  LOG(DEBUG) << "Initializing the GenJetMatchingProducer with MaxDeltaR=" << maxDeltaR << std::endl;
}

void GenJetMatchingProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const 
{
  product.m_matchedRecoJets.clear();
  product.m_matchedGenJets.clear();

  static matchSort_deltaR metric(maxDeltaR);

  // Matches a reco jet to a gen jet, size of genjets
  std::vector<int> matchResultGenJets = matchSort_Matrix<KBasicJet, KLV>(product.m_validRecoJets, product.m_validRecoJets.size(), 
                                                                         product.m_validGenJets, product.m_validGenJets.size(), 
                                                                         metric);
  product.m_matchResultGenJets = matchResultGenJets;
  // Matches a reco jet to a gen jet, size of recojets
  std::vector<int> matchResultRecoJets = matchSort_Matrix<KLV, KBasicJet>(product.m_validGenJets, product.m_validGenJets.size(), 
                                                                          product.m_validRecoJets, product.m_validRecoJets.size(), 
                                                                          metric);
  product.m_matchResultRecoJets = matchResultRecoJets;


  for (auto genJet = product.m_validGenJets.begin(); genJet != product.m_validGenJets.end(); ++genJet) {
    size_t idx = genJet - product.m_validGenJets.begin();

    if (matchResultGenJets[idx] < 0)
      product.m_matchedRecoJets[&*genJet] = nullptr;
    else
    {
      product.m_matchedRecoJets[&*genJet] = &(product.m_validRecoJets.at(matchResultGenJets[idx]));
    }
  }

  for (auto recoJet = product.m_validRecoJets.begin(); recoJet != product.m_validRecoJets.end(); ++recoJet) {
    size_t idx = recoJet - product.m_validRecoJets.begin();

    if (matchResultRecoJets[idx] < 0)
      product.m_matchedGenJets[&*recoJet] = nullptr;
    else
    {
      product.m_matchedGenJets[&*recoJet] = &(product.m_validGenJets.at(matchResultRecoJets[idx]));
    }
  }


}

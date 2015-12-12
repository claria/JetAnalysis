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

void GenJetMatchingProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {

  std::vector<KBasicJet> m_validRecoJets;

  static matchSort_deltaR metric(maxDeltaR);

  // Matches a reco jet to a gen jet, size of genjets
  std::vector<int> matchResultGenJets = matchSort_Matrix<KBasicJet, KLV>(m_validRecoJets, m_validRecoJets.size(), 
                                                                         product.m_validGenJets, product.m_validGenJets.size(), 
                                                                         metric);
  product.m_matchResultGenJets = matchResultGenJets;
  // Matches a reco jet to a gen jet, size of recojets
  std::vector<int> matchResultRecoJets = matchSort_Matrix<KLV, KBasicJet>(product.m_validGenJets, product.m_validGenJets.size(), 
                                                                          m_validRecoJets, m_validRecoJets.size(), 
                                                                          metric);
  product.m_matchResultRecoJets = matchResultRecoJets;


  for (auto genJet = product.m_validGenJets.begin(); genJet != product.m_validGenJets.end(); ++genJet) {
    size_t idx = genJet - product.m_validGenJets.begin();

    if (matchResultGenJets[idx] < 0)
      product.m_matchedRecoJets.at(&*genJet) = nullptr;
    else
    {
      product.m_matchedRecoJets.at(&*genJet) = &(product.m_validRecoJets.at(matchResultGenJets[idx]));
    }
  }

  for (auto recoJet = product.m_validRecoJets.begin(); recoJet != product.m_validRecoJets.end(); ++recoJet) {
    size_t idx = recoJet - product.m_validRecoJets.begin();

    if (matchResultRecoJets[idx] < 0)
      product.m_matchedGenJets.at(&*recoJet) = nullptr;
    else
    {
      product.m_matchedGenJets.at(&*recoJet) = &(product.m_validGenJets.at(matchResultRecoJets[idx]));
    }
  }


}

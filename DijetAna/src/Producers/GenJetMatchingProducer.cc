#include "JetAnalysis/DijetAna/interface/Producers/GenJetMatchingProducer.h"
#include <KappaTools/RootTools/interface/Matching.h>

std::string GenJetMatchingProducer::GetProducerId() const {
  return "GenJetMatchingProducer";
}

void GenJetMatchingProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);
  maxDeltaR = settings.GetMaxDeltaR();
  maxDeltaPtRel = settings.GetMaxDeltaPtRel();
}

void GenJetMatchingProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  // auto & jetProduct = static_cast <JetProduct &> (product)
  // TODO Poor/slow code but works...
  // Produce matched genjets to vector of valid reco jets
  //
  std::vector<KBasicJet> validJets;
  for (auto jet : product.m_validJets)
    validJets.push_back(*jet);

  static matchSort_deltaRdeltaPtRel metric(maxDeltaR, maxDeltaPtRel);

  //find a matching genjet for a recojet
  // std::vector<int> match_result_genjets = matchSort_Matrix<KLV, KBasicJet>(
      // *(event.m_genJets), event.m_genJets->size(), validJets, validJets.size(), metric);
  //find a matching recojet for a genjet
  std::vector<int> match_result_recojets = matchSort_Matrix<KBasicJet, KLV>(
      validJets, validJets.size(), *(event.m_genJets), event.m_genJets->size(), metric);

  // List of indices of matched genjets to each reco, size=recojet.size()
  // product.m_match_result_genjets = match_result_genjets;
  // List of indices of matched recojets to each genjet, size=genjets.size()
  product.m_match_result_recojets = match_result_recojets;

  product.m_matchedRecoJets.resize(event.m_genJets->size());
  product.m_matchedGenJets.resize(product.m_validJets.size());

  // for (std::vector<KBasicJet*>::iterator validJet = product.m_validJets.begin(); validJet != product.m_validJets.end();
  //      ++validJet) {
  //   size_t idx = validJet - product.m_validJets.begin();
  //   if (match_result_genjets[idx] < 0)
  //     product.m_matchedGenJets.at(idx) = NULL;
  //   else
  //     product.m_matchedGenJets.at(idx) = &(event.m_genJets->at(match_result_genjets[idx]));
  // }

  for (std::vector<KLV>::iterator genJet = event.m_genJets->begin(); genJet != event.m_genJets->end(); ++genJet) {
    size_t idx = genJet - event.m_genJets->begin();
    if (match_result_recojets[idx] < 0)
      product.m_matchedRecoJets.at(idx) = NULL;
    else
    {
      product.m_matchedRecoJets.at(idx) = product.m_validJets.at(match_result_recojets[idx]);
      product.m_matchedGenJets.at(match_result_recojets[idx]) = &(event.m_genJets->at(idx));
    }
  }
//   for (auto jet = product.m_matchedRecoJets.begin(); jet != product.m_matchedRecoJets.end(); ++jet)
//   {
//     size_t idx = jet - product.m_matchedRecoJets.begin();
//     if (*jet)
//       std::cout << "mGenJets " << idx << ' ' << event.m_genJets->at(idx).p4.Pt() << ' ' << (*jet)->p4.Pt() << ' ' << std::endl;
//     else
//       std::cout << "mGenJets " << idx << " nomatch " << std::endl;
//
//   }
//   for (auto jet = product.m_matchedGenJets.begin(); jet != product.m_matchedGenJets.end(); ++jet)
//   {
//     size_t idx = jet - product.m_matchedGenJets.begin();
//     if (*jet)
//       std::cout << "mRecoJets " << idx << ' ' << product.m_validJets.at(idx)->p4.Pt() << ' ' << (*jet)->p4.Pt() << ' ' << std::endl;
//     else
//       std::cout << "mRecoJets " << idx << " nomatch " << std::endl;
//
//   }


  // for (std::vector<KBasicJet*>::iterator validJet = product.m_validJets.begin(); validJet != product.m_validJets.end(); ++validJet) {
  //   size_t idx = validJet - product.m_validJets.begin();
  //   product.m_matchedGenJets.at(match_result_genjets[idx]) = NULL;
  //   if (match_result_genjets[idx] < 0)
  //     product.m_matchedGenJets.at(match_result_genjets[idx]) = NULL;
  //   else
  //     product.m_matchedGenJets.at(idx) = &(event.m_genJets->at(match_result_genjets[idx]));
  // }



}

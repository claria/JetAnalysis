#include "JetAnalysis/DijetAna/interface/Producers/GenJetPartonMatchingProducer.h"
#include "Artus/KappaAnalysis/interface/Producers/GenParticleMatchingProducers.h"

std::string GenJetPartonMatchingProducer::GetProducerId() const {
  return "GenJetPartonMatchingProducer";
}

void GenJetPartonMatchingProducer::Produce(KappaEvent const& kappaEvent, KappaProduct& kappaProduct, KappaSettings const& kappaSettings) const {
  auto const& event = static_cast<JetEvent const&>(kappaEvent);
  auto & product = static_cast<JetProduct &>(kappaProduct);
  auto const& settings = static_cast<JetSettings const&>(kappaSettings);

  product.m_matchedPartons.clear();
  for (auto & jet : product.m_validJets) {
    // size_t idx = jet - product.m_validJets.begin();
    KGenParticle* matchedParton = Match(event, product, settings, static_cast<KLV*>(&(*jet)));
    if (matchedParton != nullptr) {
      // Map of reco jets and matched gen partons for all requested correction levels
      product.m_matchedPartons.push_back(matchedParton);
    }
  }
}

#include "JetAnalysis/DijetAna/interface/Producers/GenJetPartonMatchingProducer.h"
#include "Artus/KappaAnalysis/interface/Producers/GenParticleMatchingProducers.h"

std::string GenJetPartonMatchingProducer::GetProducerId() const {
  return "GenJetPartonMatchingProducer";
}

void GenJetPartonMatchingProducer::Produce(KappaEvent const& kappaEvent, KappaProduct& kappaProduct, KappaSettings const& kappaSettings) const {
  auto const& event = static_cast<JetEvent const&>(kappaEvent);
  auto & product = static_cast<JetProduct &>(kappaProduct);
  auto const& settings = static_cast<JetSettings const&>(kappaSettings);

  assert(event.m_genParticles);

  product.m_matchedPartons.clear();
  for (auto & jet : product.m_validJets) {
    KGenParticle* matchedParton = Match(event, product, settings, static_cast<KLV*>(&(*jet)));
    if (matchedParton != nullptr) {
      product.m_matchedPartons.push_back(matchedParton);
    }
  }
}

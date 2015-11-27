#pragma once

#include "Artus/KappaAnalysis/interface/Producers/GenParticleMatchingProducers.h"
#include "../JetTypes.h"

class GenJetPartonMatchingProducer : public RecoJetGenParticleMatchingProducer 
{
 public:
  virtual std::string GetProducerId() const override;
  virtual void Produce(KappaEvent const& kappaEvent, KappaProduct& kappaProduct, KappaSettings const& kappaSettings) const override;
};

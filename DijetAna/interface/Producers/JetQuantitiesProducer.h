#pragma once

#include "../JetTypes.h"

class JetQuantitiesProducer : public JetProducerBase {
 public:
  virtual std::string GetProducerId() const override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
};

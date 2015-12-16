#pragma once

#include "../JetTypes.h"
#include "TGraphErrors.h"

class RapidityCorrectionProducer : public JetProducerBase {
 public:
  virtual std::string GetProducerId() const override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
  virtual void Init(JetSettings const& settings) override;
 private:
  TGraphErrors* m_rapidityCorrectionGraph;
};

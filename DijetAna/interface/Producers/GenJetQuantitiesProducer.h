#pragma once

#include "../JetTypes.h"
#include "TH1.h"

class GenJetQuantitiesProducer : public JetProducerBase {
 public:
  virtual std::string GetProducerId() const override;
  virtual void Init(setting_type const& settings) override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
 private:
  TH1D* m_h_ptavg = NULL;
};

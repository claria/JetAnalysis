#pragma once

#include "../JetTypes.h"
#include "TH2.h"
#include "TGraph2D.h"
#include "TProfile2D.h"

class RapidityCorrectionProducer : public JetProducerBase {
 public:
  virtual std::string GetProducerId() const override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
  virtual void Init(JetSettings const& settings) override;
 private:
  TProfile2D* m_rapidityCorrection;
};

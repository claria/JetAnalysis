#pragma once

#include "../JetTypes.h"
#include <TRandom.h>

class JERSmearingProducer : public JetProducerBase {
 public:
  virtual std::string GetProducerId() const override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
  virtual void Init(JetSettings const& settings) override;
  double GetScalingFactor(double eta) const;
  double get_sigma_rel(double pt, double eta) const;
 private:
  std::vector<double> m_jerScalingFactors;
  double maxDeltaR;
  TRandom* random;
};


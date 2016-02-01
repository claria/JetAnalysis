#pragma once

#include "../JetTypes.h"

class JERScalingProducer : public JetProducerBase {
 public:
  virtual std::string GetProducerId() const override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
  virtual void Init(JetSettings const& settings) override;
  double GetScalingFactor(double eta) const;
 private:
  std::vector<double> m_jerScalingFactors;
};

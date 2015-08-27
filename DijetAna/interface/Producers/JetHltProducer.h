#pragma once

// #include "Artus/KappaAnalysis/interface/KappaProducerBase.h"
#include "Artus/Utility/interface/DefaultValues.h"

#include "../JetTypes.h"

class JetHltProducer : public JetProducerBase {
 public:
  virtual std::string GetProducerId() const override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
  virtual void Init(JetSettings const& settings) override;

 private:
  std::map<std::string, std::pair<double, double>> triggerEffThresholds;
};

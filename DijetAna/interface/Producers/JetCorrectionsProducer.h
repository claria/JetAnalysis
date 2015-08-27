#pragma once

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "../JetTypes.h"

/**

*/
class JetCorrectionsProducer : public JetProducerBase {
 private:
  FactorizedJetCorrector* factorizedJetCorrector = nullptr;

 public:
  virtual std::string GetProducerId() const override;
  virtual void Init(JetSettings const& settings) override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
};

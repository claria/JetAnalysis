#pragma once

#include "../JetTypes.h"

/**
   \brief Producer for valid gen jets (simple KLV jets).

*/
class ValidGenJetsProducer : public JetProducerBase {
 private:
  double minValidGenJetPt;
  double minValidGenJetAbsRap;
  double maxValidGenJetAbsRap;

 public:
  virtual std::string GetProducerId() const override;
  virtual void Init(JetSettings const& settings) override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
};

#pragma once

#include "Artus/KappaAnalysis/interface/Producers/ValidJetsProducer.h"
#include "../JetTypes.h"
/**
   \brief Producer for valid jets (simple PF jets).

*/
class JetValidJetsProducer : public JetProducerBase {
 private:
  float minValidJetPt;
  float minValidJetAbsRap;
  float maxValidJetAbsRap;

  float maxNeutralHadronFraction;
  float maxNeutralEMFraction;
  int minNConstituents;
  float maxMuonFraction;
  float minChargedHadronFraction;
  int minChargedMultiplicity;
  float maxChargedEMFraction;




 public:
  virtual std::string GetProducerId() const override;
  virtual void Init(JetSettings const& settings) override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
};

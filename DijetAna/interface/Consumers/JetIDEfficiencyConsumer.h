#pragma once

#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include <boost/regex.hpp>

class JetIDEfficiencyConsumer : public JetConsumerBase {
 public:
  virtual std::string GetConsumerId() const { return "JetIDEfficiencyConsumer"; }

  virtual void Init(JetSettings const& settings);
  virtual void ProcessFilteredEvent(JetEvent const& event, JetProduct const& product, JetSettings const& settings);
  virtual void Finish(JetSettings const& settings);
 private:
  TH1D* m_h_doPassJetID = NULL;
  TH1D* m_h_allPassJetID = NULL;
};

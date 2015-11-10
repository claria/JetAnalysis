#pragma once

#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class SimpleTriggerEfficiencyConsumer : public JetConsumerBase {
 public:
  virtual std::string GetConsumerId() const { return "SimpleTriggerEfficiencyConsumer"; }

  virtual void Init(JetSettings const& settings);
  virtual void ProcessFilteredEvent(JetEvent const& event, JetProduct const& product, JetSettings const& settings);
  virtual void Finish(JetSettings const& settings);

 protected:
  std::vector<std::string> m_hltPaths;

  std::vector<TH1F*> m_passTriggerHists;
  std::vector<TH1F*> m_allTriggerHists;
};

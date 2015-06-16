
#pragma once

#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class TriggerHistogramFitConsumer : public ConsumerBase<JetTypes> {
 public:
  virtual std::string GetConsumerId() const { return "TriggerHistogramFitConsumer"; }

  virtual void Init(setting_type const &settings) ARTUS_CPP11_OVERRIDE;
  virtual void Process(setting_type const &settings) ARTUS_CPP11_OVERRIDE;
  virtual void Finish(setting_type const &settings) ARTUS_CPP11_OVERRIDE;

 protected:
  std::vector<std::string> m_triggerPaths;
  std::vector<std::string> m_pipelineNames;
  std::vector<std::vector<TH1F *> > m_triggerEffHists;
};

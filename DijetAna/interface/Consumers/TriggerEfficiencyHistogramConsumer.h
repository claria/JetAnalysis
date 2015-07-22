#pragma once

#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include <boost/regex.hpp>

class TriggerEfficiencyHistogramConsumer : public ConsumerBase<JetTypes> {
 public:
  virtual std::string GetConsumerId() const { return "TriggerEfficiencyHistogramConsumer"; }

  virtual void Init(setting_type const &settings);
  virtual void ProcessFilteredEvent(event_type const &event, product_type const &product, setting_type const &settings);
  virtual void Finish(setting_type const &settings);
  double GetL1FilterThreshold(event_type const &event, product_type const &product, std::string path);
  double GetHltFilterThreshold(event_type const &event, product_type const &product, std::string path);

 protected:
  std::vector<std::string> m_hltPaths;
  std::map<std::string, std::pair<double, double> > m_filterThresholds;

  // pattern to recognize the l1/hlt filter in all filters of trigger path.
  boost::regex m_patternL1Filter;
  boost::regex m_patternHltFilter;

  std::vector<TH1F *> m_triggerResultHists;
  std::vector<TH1F *> m_triggerEmulatedHists;
};

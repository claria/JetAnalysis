#pragma once

#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class GenMatchPtAvgFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "GenMatchPtAvgFilter"; }
  virtual void Init(JetSettings const& settings);
  virtual bool DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const;

 private:
  double maxGenMatchPtAvg;
  double minGenMatchPtAvg;
};

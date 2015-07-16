#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class PtAvgFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "PtAvgFilter"; }
  virtual void Init(JetSettings const &settings);
  virtual bool DoesEventPass(JetEvent const &event, JetProduct const &product, JetSettings const &settings) const;

 private:
  double maxPtAvg;
  double minPtAvg;
};

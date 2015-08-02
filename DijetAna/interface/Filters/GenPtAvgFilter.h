#pragma once

#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class GenPtAvgFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "GenPtAvgFilter"; }
  virtual void Init(JetSettings const &settings);
  virtual bool DoesEventPass(JetEvent const &event, JetProduct const &product, JetSettings const &settings) const;

 private:
  double maxGenPtAvg;
  double minGenPtAvg;
};
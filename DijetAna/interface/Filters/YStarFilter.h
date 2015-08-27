#pragma once

#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class YStarFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "YStarFilter"; }
  virtual void Init(JetSettings const& settings);
  virtual bool DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const;

 private:
  double maxYStar;
  double minYStar;
};

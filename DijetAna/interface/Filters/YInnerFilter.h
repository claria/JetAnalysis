#pragma once

#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class YInnerFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "YInnerFilter"; }
  virtual void Init(JetSettings const& settings);
  virtual bool DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const;

 private:
  double maxYInner;
  double minYInner;
};

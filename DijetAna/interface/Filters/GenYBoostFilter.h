#pragma once

#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class GenYBoostFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "GenYBoostFilter"; }
  virtual void Init(JetSettings const &settings);
  virtual bool DoesEventPass(JetEvent const &event, JetProduct const &product, JetSettings const &settings) const;

 private:
  double maxGenYBoost;
  double minGenYBoost;
};

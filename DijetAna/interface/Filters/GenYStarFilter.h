#pragma once

#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class GenYStarFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "GenYStarFilter"; }
  virtual void Init(JetSettings const &settings);
  virtual bool DoesEventPass(JetEvent const &event, JetProduct const &product, JetSettings const &settings) const;

 private:
  double maxGenYStar;
  double minGenYStar;
};

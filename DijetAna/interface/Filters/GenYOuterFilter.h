#pragma once

#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class GenYOuterFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "GenYOuterFilter"; }
  virtual void Init(JetSettings const &settings);
  virtual bool DoesEventPass(JetEvent const &event, JetProduct const &product, JetSettings const &settings) const;

 private:
  double maxGenYOuter;
  double minGenYOuter;
};

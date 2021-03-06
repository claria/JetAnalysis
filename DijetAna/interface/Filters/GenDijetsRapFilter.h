#pragma once

#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class GenDijetsRapFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "GenDijetsRapFilter"; }

  virtual void Init(JetSettings const& settings) override;
  virtual bool DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const;

 private:
  double minGenDijetsAbsRap;
  double maxGenDijetsAbsRap;
};

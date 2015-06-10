#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class DijetsRapFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "DijetsRapFilter"; }

  virtual void Init(JetSettings const& settings) override;
  virtual bool DoesEventPass(JetEvent const& event, JetProduct const& product,
                             JetSettings const& settings) const;

 private:
  double minDijetsAbsRap;
  double maxDijetsAbsRap;
};

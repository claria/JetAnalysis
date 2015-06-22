#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/FilterBase.h"
#include "../JetTypes.h"

class NGenJetsFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "NGenJetsFilter"; }
  virtual bool DoesEventPass(JetEvent const &event, JetProduct const &product,
                             JetSettings const &settings) const override;
  virtual void Init(JetSettings const &settings) override;

 private:
  unsigned int minValidGenJets;
};

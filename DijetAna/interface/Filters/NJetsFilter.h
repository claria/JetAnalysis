#pragma once

#include "Artus/Core/interface/FilterBase.h"
#include "../JetTypes.h"

class NJetsFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "NJetsFilter"; }
  virtual bool DoesEventPass(JetEvent const &event, JetProduct const &product,
                             JetSettings const &settings) const override;
  virtual void Init(JetSettings const &settings) override;

 private:
  unsigned int minValidJets;
};

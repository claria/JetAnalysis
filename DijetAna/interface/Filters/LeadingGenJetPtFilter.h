#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class LeadingGenJetPtFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "LeadingGenJetPtFilter"; }

  virtual void Init(JetSettings const &settings);
  virtual bool DoesEventPass(JetEvent const &event, JetProduct const &product, JetSettings const &settings) const;

 private:
  double minLeadingGenJetPt;
  double maxLeadingGenJetPt;
};

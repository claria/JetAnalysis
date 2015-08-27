#pragma once

#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class LeadingJetPtFilter : public JetFilterBase {
 public:
  virtual std::string GetFilterId() const { return "LeadingJetPtFilter"; }

  virtual void Init(JetSettings const& settings);
  virtual bool DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const;

 private:
  double minLeadingJetPt;
  double maxLeadingJetPt;
};

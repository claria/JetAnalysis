#include "JetAnalysis/DijetAna/interface/Filters/YBoostFilter.h"

void YBoostFilter::Init(JetSettings const& settings) {
  minYBoost = settings.GetMinYBoost();
  maxYBoost = settings.GetMaxYBoost();
}

bool YBoostFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  bool pass = false;
  if (0.5 * std::abs(product.m_validJets.at(0)->p4.Rapidity() + product.m_validJets.at(1)->p4.Rapidity()) >=
          minYBoost &&
      0.5 * std::abs(product.m_validJets.at(0)->p4.Rapidity() + product.m_validJets.at(1)->p4.Rapidity()) < maxYBoost) {
    pass = true;
  }
  return pass;
}

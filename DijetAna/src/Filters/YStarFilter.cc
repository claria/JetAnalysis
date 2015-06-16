#include "JetAnalysis/DijetAna/interface/Filters/YStarFilter.h"

void YStarFilter::Init(JetSettings const& settings) {
  minYStar = settings.GetMinYStar();
  maxYStar = settings.GetMaxYStar();
}

bool YStarFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  bool pass = false;
  if (0.5 * std::abs(product.m_validJets.at(0)->p4.Rapidity() - product.m_validJets.at(1)->p4.Rapidity()) >= minYStar &&
      0.5 * std::abs(product.m_validJets.at(0)->p4.Rapidity() - product.m_validJets.at(1)->p4.Rapidity()) < maxYStar) {
    pass = true;
  }
  return pass;
}

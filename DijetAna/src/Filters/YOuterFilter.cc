#include "JetAnalysis/DijetAna/interface/Filters/YOuterFilter.h"

void YOuterFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minYOuter = settings.GetMinYOuter();
  maxYOuter = settings.GetMaxYOuter();
}

bool YOuterFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  if (product.m_dijet_youter >= minYOuter && product.m_dijet_youter < maxYOuter) {
    return true;
  }
  return false;
}

#include "JetAnalysis/DijetAna/interface/Filters/YStarFilter.h"

void YStarFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minYStar = settings.GetMinYStar();
  maxYStar = settings.GetMaxYStar();
}

bool YStarFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  if (product.m_dijet_ystar >= minYStar && product.m_dijet_ystar < maxYStar) {
    return true;
  }
  return false;
}

#include "JetAnalysis/DijetAna/interface/Filters/YBoostFilter.h"

void YBoostFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minYBoost = settings.GetMinYBoost();
  maxYBoost = settings.GetMaxYBoost();
}

bool YBoostFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  if (product.m_dijet_yboost >= minYBoost &&
     product.m_dijet_yboost < maxYBoost) 
  {
    return true;
  }
  return false;
}

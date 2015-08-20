#include "JetAnalysis/DijetAna/interface/Filters/YInnerFilter.h"

void YInnerFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minYInner = settings.GetMinYInner();
  maxYInner = settings.GetMaxYInner();
}

bool YInnerFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  if (product.m_dijet_yinner >= minYInner &&
     product.m_dijet_yinner < maxYInner) 
  {
    return true;
  }
  return false;
}

#include "JetAnalysis/DijetAna/interface/Filters/GenYInnerFilter.h"

void GenYInnerFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minGenYInner = settings.GetMinGenYInner();
  maxGenYInner = settings.GetMaxGenYInner();
}

bool GenYInnerFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  if (product.m_gendijet_yinner >= minGenYInner &&
     product.m_gendijet_yinner < maxGenYInner) 
  {
    return true;
  }
  return false;
}

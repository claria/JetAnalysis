#include "JetAnalysis/DijetAna/interface/Filters/GenYStarFilter.h"

void GenYStarFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minGenYStar = settings.GetMinGenYStar();
  maxGenYStar = settings.GetMaxGenYStar();
}

bool GenYStarFilter::DoesEventPass(JetEvent const& event,
                                   JetProduct const& product,
                                   JetSettings const& settings) const {
  if (product.m_gendijet_ystar >= minGenYStar && product.m_gendijet_ystar < maxGenYStar) {
    return true;
  }
  return false;
}

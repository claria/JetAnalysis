#include "JetAnalysis/DijetAna/interface/Filters/GenYOuterFilter.h"

void GenYOuterFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minGenYOuter = settings.GetMinGenYOuter();
  maxGenYOuter = settings.GetMaxGenYOuter();
}

bool GenYOuterFilter::DoesEventPass(JetEvent const& event,
                                    JetProduct const& product,
                                    JetSettings const& settings) const {
  if (product.m_gendijet_youter >= minGenYOuter && product.m_gendijet_youter < maxGenYOuter) {
    return true;
  }
  return false;
}

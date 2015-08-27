#include "JetAnalysis/DijetAna/interface/Filters/GenYBoostFilter.h"

void GenYBoostFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minGenYBoost = settings.GetMinGenYBoost();
  maxGenYBoost = settings.GetMaxGenYBoost();
}

bool GenYBoostFilter::DoesEventPass(JetEvent const& event,
                                    JetProduct const& product,
                                    JetSettings const& settings) const {
  if (product.m_gendijet_yboost >= minGenYBoost && product.m_gendijet_yboost < maxGenYBoost) {
    return true;
  }
  return false;
}

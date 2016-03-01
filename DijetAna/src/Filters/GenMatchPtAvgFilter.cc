#include "JetAnalysis/DijetAna/interface/Filters/GenMatchPtAvgFilter.h"

void GenMatchPtAvgFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minGenMatchPtAvg = settings.GetMinGenMatchPtAvg();
  maxGenMatchPtAvg = settings.GetMaxGenMatchPtAvg();
}

bool GenMatchPtAvgFilter::DoesEventPass(JetEvent const& event,
                                   JetProduct const& product,
                                   JetSettings const& settings) const {
  if ((product.m_genmatchdijet_ptavg >= minGenMatchPtAvg) && product.m_genmatchdijet_ptavg < maxGenMatchPtAvg) {
    return true;
  }
  return false;
}

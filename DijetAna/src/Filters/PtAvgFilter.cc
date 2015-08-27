#include "JetAnalysis/DijetAna/interface/Filters/PtAvgFilter.h"

void PtAvgFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minPtAvg = settings.GetMinPtAvg();
  maxPtAvg = settings.GetMaxPtAvg();
}

bool PtAvgFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  if ((product.m_dijet_ptavg >= minPtAvg) && product.m_dijet_ptavg < maxPtAvg) {
    return true;
  }
  return false;
}

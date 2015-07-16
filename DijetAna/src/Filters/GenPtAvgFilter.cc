#include "JetAnalysis/DijetAna/interface/Filters/GenPtAvgFilter.h"

void GenPtAvgFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minGenPtAvg = settings.GetMinGenPtAvg();
  maxGenPtAvg = settings.GetMaxGenPtAvg();
}

bool GenPtAvgFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  if ((product.m_gendijet_ptavg >= minGenPtAvg) && product.m_gendijet_ptavg < maxGenPtAvg) 
  {
    return true;
  }
  return false;
}

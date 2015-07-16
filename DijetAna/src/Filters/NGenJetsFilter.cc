#include "JetAnalysis/DijetAna/interface/Filters/NGenJetsFilter.h"

void NGenJetsFilter::Init(JetSettings const& settings) { 
  JetFilterBase::Init(settings);
  minValidGenJets = settings.GetMinValidGenJets(); 
}

bool NGenJetsFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const 
{
  if (product.m_validGenJets.size() >= minValidGenJets) {
    return true;
  }
  return false;
}

#include "JetAnalysis/DijetAna/interface/Filters/NJetsFilter.h"

void NJetsFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minValidJets = settings.GetMinValidJets();
}

bool NJetsFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {
  if (product.m_validRecoJets.size() >= minValidJets) {
    return true;
  }
  return false;
}

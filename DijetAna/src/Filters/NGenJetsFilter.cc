#include "JetAnalysis/DijetAna/interface/Filters/NGenJetsFilter.h"

void NGenJetsFilter::Init(JetSettings const& settings) {
  minValidJets = settings.GetMinValidJets();
}

bool NGenJetsFilter::DoesEventPass(JetEvent const& event, JetProduct const& product,
                                   JetSettings const& settings) const {
  if (event.m_genJets->size() >= minValidJets) {
    return true;
  }
  return false;
}

#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetPtFilter.h"

void LeadingJetPtFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minLeadingJetPt = settings.GetMinLeadingJetPt();
  maxLeadingJetPt = settings.GetMaxLeadingJetPt();
}

bool LeadingJetPtFilter::DoesEventPass(JetEvent const& event,
                                       JetProduct const& product,
                                       JetSettings const& settings) const {
  if ((product.m_jet1Pt >= minLeadingJetPt) && product.m_jet1Pt < maxLeadingJetPt) {
    return true;
  }
  return false;
}

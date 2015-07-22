#include "JetAnalysis/DijetAna/interface/Filters/LeadingGenJetPtFilter.h"

void LeadingGenJetPtFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minLeadingGenJetPt = settings.GetMinLeadingGenJetPt();
  maxLeadingGenJetPt = settings.GetMaxLeadingGenJetPt();
}

bool LeadingGenJetPtFilter::DoesEventPass(JetEvent const& event, JetProduct const& product,
                                       JetSettings const& settings) const {
  if ((product.m_genjet1Pt >= minLeadingGenJetPt) &&
      (product.m_genjet1Pt < maxLeadingGenJetPt)) {
    return true;
  }
  return false;
}

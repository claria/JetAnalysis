#include "JetAnalysis/DijetAna/interface/Filters/LeadingGenJetPtFilter.h"

void LeadingGenJetPtFilter::Init(JetSettings const& settings) {
  minLeadingGenJetPt = settings.GetMinLeadingGenJetPt();
  maxLeadingGenJetPt = settings.GetMaxLeadingGenJetPt();
}

bool LeadingGenJetPtFilter::DoesEventPass(JetEvent const& event, JetProduct const& product,
                                       JetSettings const& settings) const {
  if ((product.m_validGenJets.at(0)->p4.Pt() >= minLeadingGenJetPt) &&
      (product.m_validGenJets.at(0)->p4.Pt() < maxLeadingGenJetPt)) {
    return true;
  }
  return false;
}

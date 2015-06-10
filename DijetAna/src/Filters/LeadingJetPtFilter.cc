#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetPtFilter.h"

void LeadingJetPtFilter::Init(JetSettings const& settings) {
  minLeadingJetPt = settings.GetMinLeadingJetPt();
  maxLeadingJetPt = settings.GetMaxLeadingJetPt();
}

bool LeadingJetPtFilter::DoesEventPass(JetEvent const& event,
                                       JetProduct const& product,
                                       JetSettings const& settings) const {
  if ((product.m_validJets.at(0)->p4.Pt() >= minLeadingJetPt) &&
      product.m_validJets.at(0)->p4.Pt() < maxLeadingJetPt) {
    return true;
  }
  return false;
}

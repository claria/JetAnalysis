#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetRapFilter.h"

void LeadingJetRapFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minLeadingJetAbsRap = settings.GetMinLeadingJetAbsRap();
  maxLeadingJetAbsRap = settings.GetMaxLeadingJetAbsRap();
}

bool LeadingJetRapFilter::DoesEventPass(JetEvent const& event,
                                        JetProduct const& product,
                                        JetSettings const& settings) const {
  bool pass = false;
  if ((std::abs(product.m_validRecoJets.at(0).p4.Rapidity()) >= minLeadingJetAbsRap) &&
      std::abs(product.m_validRecoJets.at(0).p4.Rapidity()) < maxLeadingJetAbsRap) {
    pass = true;
  }
  return pass;
}

#include "JetAnalysis/DijetAna/interface/Filters/SecondJetRapFilter.h"

void SecondJetRapFilter::Init(JetSettings const& settings) {
  minSecondJetAbsRap = settings.GetMinSecondJetAbsRap();
  maxSecondJetAbsRap = settings.GetMaxSecondJetAbsRap();
}

bool SecondJetRapFilter::DoesEventPass(JetEvent const& event, JetProduct const& product,
                                       JetSettings const& settings) const {
  bool pass = false;
  if ((std::abs(product.m_validJets.at(1)->p4.Rapidity()) >= minSecondJetAbsRap) &&
      std::abs(product.m_validJets.at(1)->p4.Rapidity()) < maxSecondJetAbsRap) {
    pass = true;
  }
  return pass;
}

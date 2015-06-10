#include "JetAnalysis/DijetAna/interface/Filters/DiJetsFilter.h"

bool DiJetsFilter::DoesEventPass(JetEvent const& event, JetProduct const& product,
                                 JetSettings const& settings) const {
  bool pass = false;

  if (product.m_validJets.size() >= 2) {
    pass = true;
  }
  return pass;
}

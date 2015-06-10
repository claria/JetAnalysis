#include "JetAnalysis/DijetAna/interface/Filters/DijetsRapFilter.h"

void DijetsRapFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minDijetsAbsRap = settings.GetMinDijetsAbsRap();
  maxDijetsAbsRap = settings.GetMaxDijetsAbsRap();
}

bool DijetsRapFilter::DoesEventPass(JetEvent const& event,
                                    JetProduct const& product,
                                    JetSettings const& settings) const {
  bool pass = false;
  // std::cout << "minmaxcuts " << minDijetsAbsRap << " " << maxDijetsAbsRap <<
  // std::endl;
  if ((std::abs(product.m_validJets.at(0)->p4.Rapidity()) >= minDijetsAbsRap) &&
      (std::abs(product.m_validJets.at(0)->p4.Rapidity()) < maxDijetsAbsRap) &&
      (std::abs(product.m_validJets.at(1)->p4.Rapidity()) >= minDijetsAbsRap) &&
      (std::abs(product.m_validJets.at(1)->p4.Rapidity()) < maxDijetsAbsRap)) {
    pass = true;
  }
  return pass;
}

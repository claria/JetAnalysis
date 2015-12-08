#include "JetAnalysis/DijetAna/interface/Filters/GenDijetsRapFilter.h"

void GenDijetsRapFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minGenDijetsAbsRap = settings.GetMinGenDijetsAbsRap();
  maxGenDijetsAbsRap = settings.GetMaxGenDijetsAbsRap();
}

bool GenDijetsRapFilter::DoesEventPass(JetEvent const& event,
                                    JetProduct const& product,
                                    JetSettings const& settings) const {
  bool pass = false;
  if ((std::abs(product.m_validGenJets.at(0).p4.Rapidity()) >= minGenDijetsAbsRap) &&
      (std::abs(product.m_validGenJets.at(0).p4.Rapidity()) < maxGenDijetsAbsRap) &&
      (std::abs(product.m_validGenJets.at(1).p4.Rapidity()) >= minGenDijetsAbsRap) &&
      (std::abs(product.m_validGenJets.at(1).p4.Rapidity()) < maxGenDijetsAbsRap)) {
    pass = true;
  }
  return pass;
}

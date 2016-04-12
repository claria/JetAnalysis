#include "JetAnalysis/DijetAna/interface/Filters/DijetsRapFilter.h"

void DijetsRapFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minDijetsAbsRap = settings.GetMinDijetsAbsRap();
  maxDijetsAbsRap = settings.GetMaxDijetsAbsRap();
  LOG(INFO) << "DijetsRapFilter: Accepting events with jet 1 and jet2 absolute rapidity between " << minDijetsAbsRap << " and " << maxDijetsAbsRap;
}

bool DijetsRapFilter::DoesEventPass(JetEvent const& event,
                                    JetProduct const& product,
                                    JetSettings const& settings) const {
  bool pass = false;
  if ((std::abs(product.m_validRecoJets.at(0).p4.Rapidity()) >= minDijetsAbsRap) &&
      (std::abs(product.m_validRecoJets.at(0).p4.Rapidity()) < maxDijetsAbsRap) &&
      (std::abs(product.m_validRecoJets.at(1).p4.Rapidity()) >= minDijetsAbsRap) &&
      (std::abs(product.m_validRecoJets.at(1).p4.Rapidity()) < maxDijetsAbsRap)) {
    pass = true;
  }
  return pass;
}

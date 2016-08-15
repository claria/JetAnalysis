#include "JetAnalysis/DijetAna/interface/Filters/DijetsRapFilter.h"

void DijetsRapFilter::Init(JetSettings const& settings) {
  JetFilterBase::Init(settings);
  minDijetsAbsRap = settings.GetMinDijetsAbsRap();
  maxDijetsAbsRap = settings.GetMaxDijetsAbsRap();
  LOG(INFO) << "DijetsRapFilter: Selecting events with j1 &&j2 fulfill " << minDijetsAbsRap << " <= |y| < " << maxDijetsAbsRap;
}

bool DijetsRapFilter::DoesEventPass(JetEvent const& event,
                                    JetProduct const& product,
                                    JetSettings const& settings) const {
  if (product.m_validRecoJets.size() < 2)
    return false;

  if ((std::abs(product.m_validRecoJets.at(0).p4.Rapidity()) >= minDijetsAbsRap) &&
      (std::abs(product.m_validRecoJets.at(0).p4.Rapidity()) < maxDijetsAbsRap) &&
      (std::abs(product.m_validRecoJets.at(1).p4.Rapidity()) >= minDijetsAbsRap) &&
      (std::abs(product.m_validRecoJets.at(1).p4.Rapidity()) < maxDijetsAbsRap)) 
  {
    return true;
  }
  else 
  {
    return false;
  }

}

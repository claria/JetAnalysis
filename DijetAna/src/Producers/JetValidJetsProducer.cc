#include "JetAnalysis/DijetAna/interface/Producers/JetValidJetsProducer.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

void JetValidJetsProducer::Init(KappaSettings const& settings) {
  ValidJetsProducer::Init(settings);

  auto const& jetSettings = static_cast<JetSettings const&>(settings);
  minValidJetPt = jetSettings.GetMinValidJetPt();
  minValidJetAbsRap = jetSettings.GetMinValidJetAbsRap();
  maxValidJetAbsRap = jetSettings.GetMaxValidJetAbsRap();
}

bool JetValidJetsProducer::AdditionalCriteria(KBasicJet* jet, KappaEvent const& event, KappaProduct& product,
                                              KappaSettings const& settings) const {
  bool validJet = ValidJetsProducer::AdditionalCriteria(jet, event, product, settings);
  // std::cout << "JET "  << validJet << std::endl;
  // std::cout << "CUT " << minValidJetPt << " " << minValidJetAbsRap << " " <<
  // maxValidJetAbsRap << std::endl;
  validJet = validJet && jet->p4.Pt() > minValidJetPt && std::abs(jet->p4.Rapidity()) > minValidJetAbsRap &&
             std::abs(jet->p4.Rapidity()) < maxValidJetAbsRap;
  // std::cout << "JET " << jet->p4.Pt() << " " << jet->p4.Rapidity() << " " <<
  // validJet << std::endl;
  return validJet;
}

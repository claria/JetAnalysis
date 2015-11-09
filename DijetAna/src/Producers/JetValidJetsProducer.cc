#include "JetAnalysis/DijetAna/interface/Producers/JetValidJetsProducer.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

// JetID
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
// https://github.com/cms-sw/cmssw/blob/CMSSW_7_5_X/PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h
// jets, all eta
//
//
std::string JetValidJetsProducer::GetProducerId() const {
  return "ValidJetsProducer";
}

void JetValidJetsProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);
  LOG(WARNING) << "Using the jetana ValidJetsProducer." << std::endl;

  minValidJetPt = settings.GetMinValidJetPt();
  minValidJetAbsRap = settings.GetMinValidJetAbsRap();
  maxValidJetAbsRap = settings.GetMaxValidJetAbsRap();

  std::string jetID = boost::algorithm::to_lower_copy(settings.GetJetID());

  if (jetID == "tight") {
    maxNeutralHadronFraction = 0.90f;
    maxNeutralEMFraction = 0.90f;
    minNConstituents = 1;
    maxMuonFraction = 0.8f;
    minChargedHadronFraction = 0.0f;
    minChargedMultiplicity = 0;
    maxChargedEMFraction = 0.90f;
  }
  else if (jetID == "loose") {
    maxNeutralHadronFraction = 0.99;
    maxNeutralEMFraction = 0.99;
    minNConstituents = 1;
    maxMuonFraction = 0.8;
    minChargedHadronFraction = 0.0;
    minChargedMultiplicity = 0;
    maxChargedEMFraction = 0.99;
  }
  else if (jetID == "noid") {
    maxNeutralHadronFraction = 9999.;
    maxNeutralEMFraction = 9999.;
    minNConstituents = -1;
    maxMuonFraction = 9999.;
    minChargedHadronFraction = -9999.;
    minChargedMultiplicity = -99;
    maxChargedEMFraction = 9999;
  }
  else {
    LOG(FATAL) << "The provided jet id is not implemented." << std::endl;
  }
}

void JetValidJetsProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const
{
  for (auto &jet : product.m_correctedJets) {
    bool validJet = true;
    validJet = validJet
      && (jet->neutralHadronFraction + jet->hfHadronFraction < maxNeutralHadronFraction)
      && (jet->photonFraction + jet->hfEMFraction < maxNeutralEMFraction)
      && (jet->nConstituents > minNConstituents)
      && (jet->muonFraction < maxMuonFraction);
    // jets inside tracker
    if (std::abs(jet->p4.eta()) <= 2.4f)
    {
      validJet = validJet
        && (jet->chargedHadronFraction > minChargedHadronFraction)
        && (jet->nCharged > minChargedMultiplicity)
        && (jet->electronFraction < maxChargedEMFraction);  // == CEM
    }
    // additional kinematic cuts
    validJet = validJet 
      && jet->p4.Pt() > minValidJetPt 
      && std::abs(jet->p4.Rapidity()) >= minValidJetAbsRap 
      && std::abs(jet->p4.Rapidity()) < maxValidJetAbsRap;

    if (validJet)
      product.m_validJets.push_back(&(*jet));
    else
      product.m_invalidJets.push_back(&(*jet));
  }
}

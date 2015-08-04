#include "JetAnalysis/DijetAna/interface/Producers/JetHltProducer.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

std::string JetHltProducer::GetProducerId() const { return "JetHltProducer"; }

void JetHltProducer::Init(JetSettings const &settings) {
  JetProducerBase::Init(settings);
  auto const &jetSettings = static_cast<JetSettings const &>(settings);

  if (settings.GetHltPaths().empty()) LOG(FATAL) << "No Hlt Trigger path list (tag \"HltPaths\") configured!";

  assert(jetSettings.GetTriggerEffThresholds().size() == jetSettings.GetTriggerEffPaths().size() + 1);
  for (size_t i = 0; i < jetSettings.GetTriggerEffPaths().size(); i++) {
    triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]] =
        std::make_pair(jetSettings.GetTriggerEffThresholds()[i], jetSettings.GetTriggerEffThresholds()[i + 1]);
    LOG(DEBUG) << "Trigger eff. thresholds for path " << jetSettings.GetTriggerEffPaths()[i] << " is: ("
               << triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]].first << ", "
               << triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]].second << ")." << std::endl;
  }
}

void JetHltProducer::Produce(JetEvent const &event, JetProduct &product, JetSettings const &settings) const {
  // auto const& jetEvent = static_cast <JetEvent const&> (event);
  // auto const& jetProduct = static_cast <JetProduct const&> (product);
  auto const &jetSettings = static_cast<JetSettings const &>(settings);

  LOG(DEBUG) << "Process: "
             << "run = " << event.m_eventInfo->nRun << ", "
             << "lumi = " << event.m_eventInfo->nLumi << ", "
             << "event = " << event.m_eventInfo->nEvent;

  assert(event.m_lumiInfo);
  assert(event.m_eventInfo);

  // If no valid Jets return immediately
  product.m_selectedHltName = "";
  product.m_weights["hltPrescaleWeight"] = 0.;
  product.m_selectedHltPosition = DefaultValues::UndefinedInt;

  if (product.m_validJets.size() == 0) return;

  product.m_hltInfo.setLumiInfo(event.m_lumiInfo);
  double triggerEffQuantity = product.m_validJets.at(0)->p4.Pt();

  for (std::vector<std::string>::const_iterator hltPath = jetSettings.GetHltPaths().begin();
       hltPath != jetSettings.GetHltPaths().end(); ++hltPath) {

    std::string hltName = product.m_hltInfo.getHLTName(*hltPath);

    if ( ! hltName.empty() ) {
      if (event.m_eventInfo->hltFired(hltName, event.m_lumiInfo)) {
        LOG(DEBUG) << "Trigger " << *hltPath << " fired." << std::endl;
        if ((triggerEffQuantity >= triggerEffThresholds.at(*hltPath).first) &&
            (triggerEffQuantity < triggerEffThresholds.at(*hltPath).second)) {
          LOG(DEBUG) << "Trigger: " << *hltPath << " Jet 1 pT: " << triggerEffQuantity << 
                        " is in range (" << triggerEffThresholds.at(*hltPath).first << ", " << triggerEffThresholds.at(*hltPath).second << ")." << std::endl;
          product.m_selectedHltName = *hltPath;
          product.m_weights["hltPrescaleWeight"] = product.m_hltInfo.getPrescale(hltName);
          product.m_selectedHltPosition = (int)product.m_hltInfo.getHLTPosition(hltName);
          LOG(DEBUG) << "Found a valid trigger: " << *hltPath << std::endl;
          break;
        }
      }
    } else {
      LOG(FATAL) << "Trigger " << *hltPath << " could not be resolved." << std::endl;
    }
  }
}

#include <TH1.h>
#include "TGraphAsymmErrors.h"
#include "TROOT.h"

#include "Artus/Utility/interface/RootFileHelper.h"

#include "JetAnalysis/DijetAna/interface/Consumers/SimpleTriggerEfficiencyConsumer.h"
#include "KappaTools/RootTools/HLTTools.h"

/**
   \brief Calculate the trigger efficiency by emulating the next trigger.
*/

void SimpleTriggerEfficiencyConsumer::Init(JetSettings const& settings) {
  JetConsumerBase::Init(settings);

  // Trigger path in order from looser to tighter paths.
  m_hltPaths = settings.GetHltPaths();

  for (size_t i = 0; i != m_hltPaths.size(); ++i) {
    m_allTriggerHists.push_back(new TH1F(("all_" + m_hltPaths[i]).c_str(), ("all_" + m_hltPaths[i]).c_str(),
                             settings.GetLogPtBinning().size() - 1, &settings.GetLogPtBinning()[0]));
    m_passTriggerHists.push_back(new TH1F(("pass_" + m_hltPaths[i]).c_str(), ("pass_" + m_hltPaths[i]).c_str(),
                             settings.GetLogPtBinning().size() - 1, &settings.GetLogPtBinning()[0]));
  }
}

void SimpleTriggerEfficiencyConsumer::ProcessFilteredEvent(JetEvent const& event,
    JetProduct const& product,
    JetSettings const& settings) 
{
  product.m_hltInfo.setLumiInfo(event.m_lumiInfo);
  // Quantity vs which the histograms are filled
  float triggerEffQuantity = product.m_dijet_ptavg;

  for (std::vector<std::string>::size_type i = 0; i != m_hltPaths.size(); i++) {
    // Fill Histograms for trigger which actually have fired
    std::string hltName = product.m_hltInfo.getHLTName(m_hltPaths[i]);
    // size_t hltPosition = product.m_hltInfo.getHLTPosition(m_hltPaths[i]);
    m_allTriggerHists[i]->Fill(triggerEffQuantity);

    if (event.m_eventInfo->hltFired(hltName, event.m_lumiInfo)) {
      LOG(DEBUG) << "Trigger " << hltName << "fired." << std::endl;
      m_passTriggerHists[i]->Fill(triggerEffQuantity);
    }
  }
}

void SimpleTriggerEfficiencyConsumer::Finish(setting_type const& settings) {
  // save histograms
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder() + "/TriggerEffs");
  for (size_t i = 0; i != (m_hltPaths.size()); ++i) {
    m_allTriggerHists[i]->Write();
    m_passTriggerHists[i]->Write();
  }
}

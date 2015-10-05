#include <TH1.h>
#include "TGraphAsymmErrors.h"
#include "TROOT.h"

#include "Artus/Utility/interface/RootFileHelper.h"
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

#include "JetAnalysis/DijetAna/interface/Consumers/TriggerEfficiencyHistogramConsumer.h"
#include "KappaTools/RootTools/HLTTools.h"
#include <boost/lexical_cast.hpp>

/**
   \brief Calculate the trigger efficiency by emulating the next trigger.
*/

void TriggerEfficiencyHistogramConsumer::Init(setting_type const& settings) {
  ConsumerBase<JetTypes>::Init(settings);

  // Trigger path in order from looser to tighter paths.
  m_hltPaths = settings.GetTriggerEffPaths();

  // Pattern how the L1 or HLT Filter is recognized.
  // i.e. (L1SingleJet)([0-9]+) or (PFJet)([0-9]+)
  m_patternL1Filter = boost::regex(settings.GetL1FilterPattern());
  m_patternHltFilter = boost::regex(settings.GetHltFilterPattern());

  for (size_t i = 0; i != m_hltPaths.size(); ++i) {
    m_triggerResultHists.push_back(new TH1F(m_hltPaths[i].c_str(),
                                            m_hltPaths[i].c_str(),
                                            settings.GetLogPtBinning().size() - 1,
                                            &settings.GetLogPtBinning()[0]));
    if (i < m_hltPaths.size() - 1) {
      m_triggerEmulatedHists.push_back(new TH1F(("emul_" + m_hltPaths[i + 1]).c_str(),
                                                ("emul_" + m_hltPaths[i + 1]).c_str(),
                                                settings.GetLogPtBinning().size() - 1,
                                                &settings.GetLogPtBinning()[0]));
    }
  }

  // Read the L1 and HLT filter thresholds from config instead of using the
  // pattern.
  assert(settings.GetL1FilterThresholds().size() == settings.GetTriggerEffPaths().size());
  assert(settings.GetHltFilterThresholds().size() == settings.GetTriggerEffPaths().size());

  for (size_t i = 0; i < settings.GetTriggerEffPaths().size(); i++) {
    m_filterThresholds[settings.GetTriggerEffPaths()[i]] =
        std::make_pair(settings.GetL1FilterThresholds()[i], settings.GetHltFilterThresholds()[i]);
  }
}

void TriggerEfficiencyHistogramConsumer::ProcessFilteredEvent(event_type const& event,
                                                              product_type const& product,
                                                              setting_type const& settings) {
  // auto const& specEvent = static_cast < JetEvent const&> (event);
  auto const& specProduct = static_cast<JetProduct const&>(product);
  // Set lumi metadata
  specProduct.m_hltInfo.setLumiInfo(event.m_lumiInfo);
  // Quantity for which the histograms are filled
  float triggerEffQuantity = specProduct.m_dijet_ptavg;

  // Threshold of the L1 and Hlt Filter
  double m_l1FilterThreshold = 0.;
  double m_hltFilterThreshold = 0.;

  for (std::vector<std::string>::size_type i = 0; i != m_hltPaths.size() - 1; i++) {
    // Fill Histograms for trigger which actually have fired
    // std::cout << "xyz " << m_hltFilterNames[i] << " " << std::endl;
    std::string hltName = product.m_hltInfo.getHLTName(m_hltPaths[i]);
    size_t hltPosition = product.m_hltInfo.getHLTPosition(m_hltPaths[i]);
    if (event.m_eventInfo->hltFired(hltName, event.m_lumiInfo)) {
      LOG(DEBUG) << "Trigger " << hltName << "fired." << std::endl;
      int l1objIdx = -1;
      int hltobjIdx = -1;
      // identify the l1 and hlt filter in all filters of the trigger path.
      LOG(DEBUG) << "Event contains the following filters.";
      for (size_t filterIndex = event.m_triggerObjectMetadata->getMinFilterIndex(hltPosition);
           filterIndex < event.m_triggerObjectMetadata->getMaxFilterIndex(hltPosition);
           filterIndex++) {
        std::string filterName = event.m_triggerObjectMetadata->toFilter[filterIndex];
        LOG(DEBUG) << "Filtername: " << filterName << " Index " << filterIndex;
        if (boost::regex_search(filterName, m_patternL1Filter)) {
          for (auto id : event.m_triggerObjects->toIdxFilter[filterIndex]) {
            LOG(DEBUG) << "L1 Triggerobject Pt: " << event.m_triggerObjects->trgObjects[id].p4.Pt();
          }
          // Sorted by Pt, we just use the first one
          l1objIdx = event.m_triggerObjects->toIdxFilter[filterIndex][0];
        } else if (boost::regex_search(filterName, m_patternHltFilter)) {
          for (auto id : event.m_triggerObjects->toIdxFilter[filterIndex]) {
            LOG(DEBUG) << "HLT Triggerobject Pt: " << event.m_triggerObjects->trgObjects[id].p4.Pt();
          }
          // Sorted by Pt, we just use the first one
          hltobjIdx = event.m_triggerObjects->toIdxFilter[filterIndex][0];
        }
      }
      if (l1objIdx < 0 || hltobjIdx < 0) {
        LOG(DEBUG) << "The L1 or HLT filter of the fired trigger in the event "
                      "were not found."
                   << "This can happen if the filter has saveTags=False set in "
                      "the trigger path. "
                   << "Event will be skipped.";
        return;
      }
      LOG(DEBUG) << "Trigger object pT: L1=" << event.m_triggerObjects->trgObjects[l1objIdx].p4.Pt() << " GeV "
                 << "HLT=" << event.m_triggerObjects->trgObjects[hltobjIdx].p4.Pt() << " GeV.";
      m_triggerResultHists[i]->Fill(triggerEffQuantity);
      if (i < m_hltPaths.size() - 1)
        m_l1FilterThreshold = m_filterThresholds[m_hltPaths[i + 1]].first;
      m_hltFilterThreshold = m_filterThresholds[m_hltPaths[i + 1]].second;

      if ((event.m_triggerObjects->trgObjects[l1objIdx].p4.Pt() > m_l1FilterThreshold) &&
          (event.m_triggerObjects->trgObjects[hltobjIdx].p4.Pt() > m_hltFilterThreshold)) {
        m_triggerEmulatedHists[i]->Fill(triggerEffQuantity);
      }
    }
  }
}

double TriggerEfficiencyHistogramConsumer::GetL1FilterThreshold(event_type const& event,
                                                                product_type const& product,
                                                                std::string path) {
  size_t hltPosition = product.m_hltInfo.getHLTPosition(path);
  // std::cout << "TriggerEfficiencyHistogramConsumer::GetL1FilterThreshold" <<
  // "hltposition" << hltPosition;
  for (size_t filterIndex = event.m_triggerObjectMetadata->getMinFilterIndex(hltPosition);
       filterIndex < event.m_triggerObjectMetadata->getMaxFilterIndex(hltPosition);
       filterIndex++) {
    std::string filterName = event.m_triggerObjectMetadata->toFilter[filterIndex];
    boost::smatch match;

    if (boost::regex_search(filterName, match, m_patternL1Filter)) {
      return boost::lexical_cast<double>(std::string(match[2].first, match[2].second));
    }
  }
  return 0.;
}

double TriggerEfficiencyHistogramConsumer::GetHltFilterThreshold(event_type const& event,
                                                                 product_type const& product,
                                                                 std::string path) {
  size_t hltPosition = product.m_hltInfo.getHLTPosition(path);
  for (size_t filterIndex = event.m_triggerObjectMetadata->getMinFilterIndex(hltPosition);
       filterIndex < event.m_triggerObjectMetadata->getMaxFilterIndex(hltPosition);
       filterIndex++) {
    std::string filterName = event.m_triggerObjectMetadata->toFilter[filterIndex];
    boost::smatch match;

    if (boost::regex_search(filterName, match, m_patternHltFilter)) {
      return boost::lexical_cast<double>(std::string(match[2].first, match[2].second));
    }
  }
  return 0.;
}

void TriggerEfficiencyHistogramConsumer::Finish(setting_type const& settings) {
  // save histograms
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder() + "/TriggerEffs");

  for (size_t i = 0; i != (m_hltPaths.size() - 1); ++i) {
    m_triggerResultHists[i]->Write(m_triggerResultHists[i]->GetName());
    m_triggerEmulatedHists[i]->Write(m_triggerEmulatedHists[i]->GetName());
  }
}

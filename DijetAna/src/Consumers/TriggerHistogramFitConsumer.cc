#include <TH1.h>
#include "TROOT.h"

#include "Artus/Utility/interface/RootFileHelper.h"

#include "JetAnalysis/DijetAna/interface/Consumers/TriggerHistogramFitConsumer.h"

void TriggerHistogramFitConsumer::Init(setting_type const& settings) {
  ConsumerBase<JetTypes>::Init(settings);

  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

  // Get Level 1 Pipelines
  for (auto pipeline : settings.GetPipelineInfos()) {
    if (pipeline.second == 1) {
      m_pipelineNames.push_back(pipeline.first);
    }
  }

  m_triggerPaths = settings.GetHltPaths();
  // m_pipelineNames = settings.GetPipelineNames();
  m_triggerEffHists.resize(m_pipelineNames.size());
  for (std::vector<std::string>::size_type i = 0; i < m_pipelineNames.size(); i++) {
    std::cout << m_pipelineNames[i] << std::endl;
    m_triggerEffHists[i].resize(m_triggerPaths.size() - 1);
  }
}

void TriggerHistogramFitConsumer::Process(setting_type const& settings) {
  for (std::vector<std::string>::size_type i = 0; i < m_pipelineNames.size(); i++) {
    RootFileHelper::SafeCd(settings.GetRootOutFile(), m_pipelineNames[i]);
    for (std::vector<std::string>::size_type j = 0; j < m_triggerPaths.size() - 1; j++) {
      TH1F* trgemul = (TH1F*)RootFileHelper::SafeGet<TH1F>(settings.GetRootOutFile(),
                                                           m_pipelineNames[i] + "/emul_" + m_triggerPaths[j + 1])
                          ->Clone(("trgeff_" + m_triggerPaths[j + 1]).c_str());
      TH1F* trg =
          (TH1F*)RootFileHelper::SafeGet<TH1F>(settings.GetRootOutFile(), m_pipelineNames[i] + "/" + m_triggerPaths[j]);

      // trgN->Scale(1. / settings.GetEffectiveLumiPerHLTPath()[j]);
      // trgNP1->Scale(1. / settings.GetEffectiveLumiPerHLTPath()[j+1]);

      m_triggerEffHists[i][j] = trgemul;
      m_triggerEffHists[i][j]->Divide(trg);
    }
  }
}

void TriggerHistogramFitConsumer::Finish(setting_type const& settings) {
  // save histograms
  // for (TH1F* hist : m_triggerEffHists) {
  // 	hist->Write(hist->GetName());
  // }
  for (std::vector<std::string>::size_type i = 0; i < m_pipelineNames.size(); i++) {
    RootFileHelper::SafeCd(settings.GetRootOutFile(), m_pipelineNames[i]);
    for (std::vector<std::string>::size_type j = 0; j < m_triggerPaths.size() - 1; j++) {
      m_triggerEffHists[i][j]->Write(m_triggerEffHists[i][j]->GetName());
    }
  }
}

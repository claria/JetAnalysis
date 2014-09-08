#include <TH1.h>
#include "TROOT.h"

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Utility/interface/RootFileHelper.h"

#include "JetAnalysis/DijetAna/interface/Consumers/TriggerHistogramFitConsumer.h"

void TriggerHistogramFitConsumer::Init(Pipeline<JetTypes> * pipeline)
{
	ConsumerBase<JetTypes>::Init(pipeline);

	RootFileHelper::SafeCd(this->GetPipelineSettings().GetRootOutFile(),
			this->GetPipelineSettings().GetRootFileFolder());

	// Get Level 1 Pipelines
	for ( auto pipeline : this->GetPipelineSettings().GetPipelineInfos())
	{
		if (pipeline.second == 1) {
			m_pipelineNames.push_back(pipeline.first);
		}
	}
	

	m_triggerPaths = this->GetPipelineSettings().GetHltPaths();
	//m_pipelineNames = this->GetPipelineSettings().GetPipelineNames();
	m_triggerEffHists.resize(m_pipelineNames.size());
	for (std::vector<std::string>::size_type i = 0; i < m_pipelineNames.size(); i++)
	{
		std::cout << m_pipelineNames[i] << std::endl;
		m_triggerEffHists[i].resize(m_triggerPaths.size() - 1);
	}
}

void TriggerHistogramFitConsumer::Process()
{
	for (std::vector<std::string>::size_type i = 0; i < m_pipelineNames.size(); i++)
	{
		RootFileHelper::SafeCd(this->GetPipelineSettings().GetRootOutFile(),
				m_pipelineNames[i]);
		for (std::vector<std::string>::size_type j = 0; j < m_triggerPaths.size() - 1; j++)
		{
			TH1F* trgN = (TH1F*)RootFileHelper::SafeGet<TH1F>(
					this->GetPipelineSettings().GetRootOutFile(),
					m_pipelineNames[i] + "/" + m_triggerPaths[j])->Clone(("trgeffs_" + m_triggerPaths[j]).c_str());
			TH1F* trgNP1 = (TH1F*)RootFileHelper::SafeGet<TH1F>(
					this->GetPipelineSettings().GetRootOutFile(),
					m_pipelineNames[i] + "/" + m_triggerPaths[j+1])->Clone(("trgeffs_" + m_triggerPaths[j+1]).c_str());

			trgN->Scale(1. / this->GetPipelineSettings().GetEffectiveLumiPerHLTPath()[j]);
			trgNP1->Scale(1. / this->GetPipelineSettings().GetEffectiveLumiPerHLTPath()[j+1]);

			m_triggerEffHists[i][j] = trgNP1;
			m_triggerEffHists[i][j]->SetName(("trgeffs_" + m_triggerPaths[j+1]).c_str());
			m_triggerEffHists[i][j]->SetTitle(("trgeffs_" + m_triggerPaths[j+1]).c_str());

			m_triggerEffHists[i][j]->Divide(trgN);

		}
	}

}

void TriggerHistogramFitConsumer::Finish()
{
	// save histograms
	// for (TH1F* hist : m_triggerEffHists) {
	// 	hist->Write(hist->GetName());
	// }
	for (std::vector<std::string>::size_type i = 0; i < m_pipelineNames.size(); i++)
	{
		RootFileHelper::SafeCd(this->GetPipelineSettings().GetRootOutFile(),
				m_pipelineNames[i]);
		for (std::vector<std::string>::size_type j = 0; j < m_triggerPaths.size() - 1; j++)
		{
			m_triggerEffHists[i][j]->Write(m_triggerEffHists[i][j]->GetName());
		}
	}
}

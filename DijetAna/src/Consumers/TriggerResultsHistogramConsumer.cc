#include <TH1.h>
#include "TROOT.h"

#include "Artus/Utility/interface/RootFileHelper.h"
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

#include "JetAnalysis/DijetAna/interface/Consumers/TriggerResultsHistogramConsumer.h"

#include "KappaTools/RootTools/HLTTools.h"

void TriggerResultsHistogramConsumer::Init(Pipeline<JetTypes> * pipeline)
{
	ConsumerBase<JetTypes>::Init( pipeline );

	m_hltPaths = this->GetPipelineSettings().GetHltPaths();

	m_l1ObjectsPaths = this->GetPipelineSettings().GetL1ObjectsPaths();
	m_hltObjectsPaths = this->GetPipelineSettings().GetHltObjectsPaths();
	m_l1SingleJetThresholds = this->GetPipelineSettings().GetL1SingleJetThresholds();
	m_hltSingleJetThresholds = this->GetPipelineSettings().GetHltSingleJetThresholds();

	RootFileHelper::SafeCd(this->GetPipelineSettings().GetRootOutFile(),
	                       this->GetPipelineSettings().GetRootFileFolder());


	for (size_t i = 0; i != m_hltObjectsPaths.size(); ++i)
	{
		m_triggerResultHists.push_back(new TH1F(m_hltPaths[i].c_str(), m_hltPaths[i].c_str(), 100., 0., 1000.));
		if (i < m_hltPaths.size() -1) {
			m_triggerEmulatedHists.push_back(new TH1F(("emul_" + m_hltPaths[i+1]).c_str(), m_hltPaths[i].c_str(), 100., 0., 1000.));
		}
	}
}

void TriggerResultsHistogramConsumer::ProcessFilteredEvent(event_type const& event,
                          product_type const& product)
{
	//std::cout << "Event " << std::endl;
	hltInfo = HLTTools(event.m_lumiMetadata);
	float triggerEffQuantity = (LambdaNtupleConsumer<JetTypes>::Quantities[this->GetPipelineSettings().GetTriggerEfficiencyQuantity()])(event, product);

	for(std::vector<std::string>::size_type i = 0; i != m_hltPaths.size() - 1; i++) {
		// Fill Histograms for trigger which actually have fired
		std::string hltName = hltInfo.getHLTName(m_hltObjectsPaths[i]);
		size_t hltPosition = hltInfo.getHLTPosition(m_hltObjectsPaths[i]);
		if (event.m_eventMetadata->hltFired(hltName, event.m_lumiMetadata)){
			m_triggerResultHists[i]->Fill(triggerEffQuantity);
			// Check if next trigger would have fired as well if it would not have been prescaled
			size_t l1objIdx = 0;
			size_t hltobjIdx = 0;
			for (size_t filterIndex = event.m_triggerInfos->getMinFilterIndex(hltPosition); filterIndex < event.m_triggerInfos->getMaxFilterIndex(hltPosition); filterIndex++)
			{
				// std::cout << "indexname " << event.m_triggerInfos->toFilter[filterIndex] << std::endl;;
				// std::cout << "l1name " << m_l1ObjectsPaths[i] << std::endl;;
				// std::cout << "hltname " << m_hltObjectsPaths[i] << std::endl;;

				//for (std::vector<int>::const_iterator triggerObjectIndex = event.m_triggerObjects->toIdxFilter[filterIndex].begin();
				//		(triggerObjectIndex != event.m_triggerObjects->toIdxFilter[filterIndex].end());
				//		++triggerObjectIndex)
				//{
					// std::cout << "trgobjidx " << *triggerObjectIndex << " " << event.m_triggerObjects->trgObjects[*triggerObjectIndex].p4.Pt() <<std::endl;
				//}

				if (event.m_triggerInfos->toFilter[filterIndex].find(m_l1ObjectsPaths[i]) != std::string::npos) {
					l1objIdx = event.m_triggerObjects->toIdxFilter[filterIndex][0];
				}
				if (event.m_triggerInfos->toFilter[filterIndex].find(m_hltObjectsPaths[i]) != std::string::npos) {
					hltobjIdx = event.m_triggerObjects->toIdxFilter[filterIndex][0];
				}
			}
			//std::cout << l1objIdx << " " << hltobjIdx << std::endl;
			// std::cout << "l1obj " << m_l1SingleJetThresholds[i] << " " <<  event.m_triggerObjects->trgObjects[l1objIdx].p4.Pt() << std::endl;
			// std::cout << "hltobj " << m_hltSingleJetThresholds[i] << " " <<  event.m_triggerObjects->trgObjects[hltobjIdx].p4.Pt() << std::endl;
			if ((event.m_triggerObjects->trgObjects[l1objIdx].p4.Pt() > m_l1SingleJetThresholds[i+1]) &&
				event.m_triggerObjects->trgObjects[hltobjIdx].p4.Pt() > m_hltSingleJetThresholds[i+1]) {
					m_triggerEmulatedHists[i]->Fill(triggerEffQuantity);
			}

		}

	}
}

void TriggerResultsHistogramConsumer::Finish()
{
	
	// save histograms
	RootFileHelper::SafeCd(this->GetPipelineSettings().GetRootOutFile(),
	                       this->GetPipelineSettings().GetRootFileFolder());
	
	for (size_t i = 0; i != m_hltPaths.size(); ++i)
	{
		m_triggerResultHists[i]->Write(m_triggerResultHists[i]->GetName());
		if (i < m_hltPaths.size() -1) {
			m_triggerEmulatedHists[i]->Write(m_triggerEmulatedHists[i]->GetName());
		}
	}
}

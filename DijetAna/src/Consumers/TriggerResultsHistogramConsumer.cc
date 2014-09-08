#include <TH1.h>
#include "TROOT.h"

#include "Artus/Utility/interface/RootFileHelper.h"
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

#include "JetAnalysis/DijetAna/interface/Consumers/TriggerResultsHistogramConsumer.h"

void TriggerResultsHistogramConsumer::Init(Pipeline<JetTypes> * pipeline)
{
	ConsumerBase<JetTypes>::Init( pipeline );
	m_triggerPaths = this->GetPipelineSettings().GetHltPaths();

	RootFileHelper::SafeCd(this->GetPipelineSettings().GetRootOutFile(),
	                       this->GetPipelineSettings().GetRootFileFolder());

	for (std::string hltPath : m_triggerPaths) {
		m_triggerResultHists.push_back(new TH1F(hltPath.c_str(), hltPath.c_str(), 100., 0., 1000.));
	}
}

void TriggerResultsHistogramConsumer::ProcessFilteredEvent(event_type const& event,
                          product_type const& product)
{
	for(std::vector<std::string>::size_type i = 0; i != m_triggerPaths.size(); i++) {
		if (event.m_eventMetadata->hltFired(product.m_hltInfo->getHLTName(m_triggerPaths[i]), event.m_lumiMetadata)){
			m_triggerResultHists[i]->Fill((LambdaNtupleConsumer<JetTypes>::Quantities[this->GetPipelineSettings().GetTriggerEfficiencyQuantity()])(event, product));
		}
	}
}

void TriggerResultsHistogramConsumer::Finish()
{
	
	// save histograms
	RootFileHelper::SafeCd(this->GetPipelineSettings().GetRootOutFile(),
	                       this->GetPipelineSettings().GetRootFileFolder());
	
	for (TH1F* hist : m_triggerResultHists) {
		hist->Write(hist->GetName());
	}
}


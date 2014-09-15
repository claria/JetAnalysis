#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class TriggerResultsHistogramConsumer: public ConsumerBase<JetTypes> {
public:

	virtual std::string GetConsumerId() const
	{
		return "TriggerResultsHistogramConsumer";
	}

	virtual void Init(Pipeline<JetTypes> * pipeline);
	virtual void ProcessFilteredEvent(event_type const& event,
	                          product_type const& product);
	virtual void Finish();

protected:
	std::vector<std::string> m_hltPaths;
	std::vector<std::string> m_hltObjectsPaths;
	std::vector<std::string> m_l1ObjectsPaths;
	std::vector<float> m_l1SingleJetThresholds;
	std::vector<float> m_hltSingleJetThresholds;

	std::vector<TH1F*> m_triggerResultHists;
	std::vector<TH1F*> m_triggerEmulatedHists;

	HLTTools hltInfo;
};

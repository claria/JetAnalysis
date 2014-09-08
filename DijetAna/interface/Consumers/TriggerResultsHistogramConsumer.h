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
	std::vector<std::string> m_triggerPaths;
	std::vector<TH1F*> m_triggerResultHists;

};

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include <boost/regex.hpp>

class TriggerResultsHistogramConsumer: public ConsumerBase<JetTypes> {
public:

	virtual std::string GetConsumerId() const
	{
		return "TriggerResultsHistogramConsumer";
	}

	virtual void Init(setting_type const& settings);
	virtual void ProcessFilteredEvent(event_type const& event,
	                          product_type const& product,
	                          setting_type const& settings);
	virtual void Finish(setting_type const& settings);
	double GetL1FilterThreshold(event_type const& event, product_type const& product, std::string path);
	double GetHltFilterThreshold(event_type const& event, product_type const& product, std::string path);

protected:
	std::vector<std::string> m_hltPaths;

	boost::regex m_patternL1Filter;
	boost::regex m_patternHltFilter;

	std::vector<TH1F*> m_triggerResultHists;
	std::vector<TH1F*> m_triggerEmulatedHists;


	// HLTTools hltInfo;
};


#pragma once

#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class JetResolutionConsumer: public ConsumerBase<JetTypes> {
public:

	virtual std::string GetConsumerId() const
	{
		return "JetResolutionConsumer";
	}

	virtual void Init(setting_type const& settings) ARTUS_CPP11_OVERRIDE;
	// virtual void Process(setting_type const& settings) ARTUS_CPP11_OVERRIDE;
	virtual void ProcessFilteredEvent(event_type const& event, product_type const& product, setting_type const& settings) override;

	virtual void Finish(setting_type const& settings) ARTUS_CPP11_OVERRIDE;

protected:
	TH2D* m_hGenVsRecoPt = NULL;
	TGraphErrors * graph_resolution = NULL;
};

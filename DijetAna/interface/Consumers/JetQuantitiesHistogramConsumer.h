#pragma once

#include "TH1.h"
#include "Artus/Utility/interface/RootFileHelper.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class JetQuantitiesHistogramConsumer: public ConsumerBase<JetTypes> {

public:

	virtual std::string GetConsumerId() const {
		return "JetQuantitiesHistogramConsumer";
	}
	virtual void Init(setting_type const& settings) override;
	virtual void ProcessFilteredEvent(event_type const& event,
	                                  product_type const& product,
	                                  setting_type const& settings) override;
	virtual void Finish(setting_type const& settings) override;

private:

	TH1F m_hjet1pt;
	TH1F m_hjet2pt;
	TH1F m_hincjetpt;

};

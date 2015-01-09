#pragma once

#include "TH1.h"
#include "Artus/Utility/interface/RootFileHelper.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class JetQuantitiesHistogramConsumer: public JetConsumerBase {

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

	TH1F m_h_jet1pt;
	TH1F m_h_jet2pt;
	TH1F m_h_incjetpt;
	TH2F m_h_jet12rap;

};

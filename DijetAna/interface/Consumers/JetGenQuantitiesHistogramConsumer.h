#pragma once

#include "TH1.h"
#include "Artus/Utility/interface/RootFileHelper.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class JetGenQuantitiesHistogramConsumer: public JetConsumerBase {

public:

	virtual std::string GetConsumerId() const {
		return "JetGenQuantitiesHistogramConsumer";
	}
	virtual void Init(setting_type const& settings) override;
	virtual void ProcessFilteredEvent(event_type const& event,
	                                  product_type const& product,
	                                  setting_type const& settings) override;
	virtual void Finish(setting_type const& settings) override;

private:

	// Gen vs. reco pt for each pt bin
	TH2D* m_h2GenVsRecoPt = NULL;

};

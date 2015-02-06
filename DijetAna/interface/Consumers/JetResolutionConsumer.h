
#pragma once

#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class JetResolutionConsumer: public ConsumerBase<JetTypes> {
public:

	virtual std::string GetConsumerId() const
	{
		return "JetResolutionConsumer";
	}

	virtual void Init(setting_type const& settings) ARTUS_CPP11_OVERRIDE;
	virtual void Process(setting_type const& settings) ARTUS_CPP11_OVERRIDE;
	virtual void Finish(setting_type const& settings) ARTUS_CPP11_OVERRIDE;

protected:
	std::vector<std::string> m_pipelineNames;
	std::vector<TH2D*> m_h2GenVsRecoPt;
	TGraphErrors * graph_resolution = NULL;
};

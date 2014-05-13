
#pragma once

#include "../JetTypes.h"
#include <typeinfo>

class EventWeightProducer: public JetProducerBase {
public:

	virtual std::string GetProducerId() const ARTUS_CPP11_OVERRIDE {
		int status;
		char *realname = abi::__cxa_demangle(typeid(*this).name(), 0, 0, &status);
		std::cout << realname;
		return std::string(realname);
		//return "EventWeightProducer";
	}
	
	EventWeightProducer() : JetProducerBase() {};

	void ProduceLocal(JetEvent const& event, JetProduct& product,
			JetPipelineSettings const& settings) const
	{
		double eventWeight = 1.0;

		// loop over all previously calculated weights and multiply them
		for(std::map<std::string, double>::const_iterator weight = product.m_weights.begin();
				weight != product.m_weights.end(); ++weight)
		{
			eventWeight *= weight->second;
		}

		product.m_weights["eventWeight"] = eventWeight;
	}
};

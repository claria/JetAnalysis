#pragma once

#include "Artus/Core/interface/Cpp11Support.h"

#include "Artus/KappaAnalysis/interface/Consumers/KappaLambdaNtupleConsumer.h"

#include "../JetTypes.h"


class JetLambdaNtupleConsumer: public KappaLambdaNtupleConsumer<JetTypes> {
public:

	typedef typename JetTypes::event_type event_type;
	typedef typename JetTypes::product_type product_type;
	typedef typename JetTypes::setting_type setting_type;
	
//	virtual std::string GetConsumerId() const ARTUS_CPP11_OVERRIDE;
	virtual void Init(setting_type const& settings) ARTUS_CPP11_OVERRIDE;
};

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "../JetTypes.h"

class GenJetQuantitiesProducer : public JetProducerBase {

public:
	virtual std::string GetProducerId() const override;
	virtual void Produce(JetEvent const& event, JetProduct & product, JetSettings const& settings) const override;
};

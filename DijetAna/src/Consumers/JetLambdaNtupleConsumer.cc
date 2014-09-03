#include <Math/VectorUtil.h>

#include "Artus/Utility/interface/SafeMap.h"
#include "Artus/Utility/interface/Utility.h"
#include "Artus/Utility/interface/DefaultValues.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"

void JetLambdaNtupleConsumer::Init(Pipeline<JetTypes> * pipeline)
{
	// add possible quantities for the lambda ntuples consumers
	
	// settings for synch ntuples
	LambdaNtupleConsumer<JetTypes>::Quantities["evt"] = [](event_type const& event, product_type const& product)
	{
		return event.m_eventMetadata->nEvent;
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["weight"] = [pipeline](event_type const& event, product_type const& product) {
		return SafeMap::GetWithDefault(product.m_weights, pipeline->GetSettings().GetEventWeight(), 1.0);
	};
	
	// Leading Jet Quantities
	LambdaNtupleConsumer<JetTypes>::Quantities["jet1_pt"] = [](event_type const& event, product_type const& product) {
		return product.m_validJets.size() >= 1 ? product.m_validJets.at(0)->p4.Pt() : DefaultValues::UndefinedDouble;
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["jet1_eta"] = [](event_type const& event, product_type const& product) {
		return product.m_validJets.size() >= 1 ? product.m_validJets.at(0)->p4.Eta() : DefaultValues::UndefinedDouble;
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["jet1_phi"] = [](event_type const& event, product_type const& product) {
		return product.m_validJets.size() >= 1 ? product.m_validJets.at(0)->p4.Phi() : DefaultValues::UndefinedDouble;
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["jet1_rap"] = [](event_type const& event, product_type const& product) {
		return product.m_validJets.size() >= 1 ? product.m_validJets.at(0)->p4.Rapidity() : DefaultValues::UndefinedDouble;
	};

	// Second Jet Quantities
	LambdaNtupleConsumer<JetTypes>::Quantities["jet2_pt"] = [](event_type const& event, product_type const& product) {
		return product.m_validJets.size() >= 2 ? product.m_validJets.at(1)->p4.Pt() : DefaultValues::UndefinedDouble;
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["jet2_eta"] = [](event_type const& event, product_type const& product) {
		return product.m_validJets.size() >= 2 ? product.m_validJets.at(1)->p4.Eta() : DefaultValues::UndefinedDouble;
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["jet2_phi"] = [](event_type const& event, product_type const& product) {
		return product.m_validJets.size() >= 2 ? product.m_validJets.at(1)->p4.Phi() : DefaultValues::UndefinedDouble;
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["jet2_rap"] = [](event_type const& event, product_type const& product) {
		return product.m_validJets.size() >= 2 ? product.m_validJets.at(1)->p4.Rapidity() : DefaultValues::UndefinedDouble;
	};

	// need to be called at last
	KappaLambdaNtupleConsumer<JetTypes>::Init(pipeline);
}

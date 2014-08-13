#include <Math/VectorUtil.h>

#include "Artus/Utility/interface/SafeMap.h"
#include "Artus/Utility/interface/Utility.h"
#include "Artus/Utility/interface/DefaultValues.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"


void JetLambdaNtupleConsumer::Init(Pipeline<JetTypes> * pipeline)
{
	// add possible quantities for the lambda ntuples consumers
	
	// settings for synch ntuples
	//LambdaNtupleConsumer<JetTypes>::Quantities["evt"] = [](event_type const& event, product_type const& product)
	//{
	//	return event.m_eventMetadata->nEvent;
	//};
	//LambdaNtupleConsumer<JetTypes>::Quantities["puweight"] = [](event_type const& event, product_type const& product) {
	//	return SafeMap::GetWithDefault(product.m_weights, std::string("puWeight"), 1.0);
	//};
	//LambdaNtupleConsumer<JetTypes>::Quantities["trigweight_1"] = [](event_type const& event, product_type const& product) {
	//	return SafeMap::GetWithDefault(product.m_weights, std::string("triggerWeight1"), 1.0);
	//};
	//LambdaNtupleConsumer<JetTypes>::Quantities["trigweight_2"] = [](event_type const& event, product_type const& product) {
	//	return SafeMap::GetWithDefault(product.m_weights, std::string("triggerWeight2"), 1.0);
	//};
	LambdaNtupleConsumer<JetTypes>::Quantities["weight"] = [pipeline](event_type const& event, product_type const& product) {
		return SafeMap::GetWithDefault(product.m_weights, pipeline->GetSettings().GetEventWeight(), 1.0);
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["jpt_1"] = LambdaNtupleConsumer<JetTypes>::Quantities["leadingJetPt"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jeta_1"] = LambdaNtupleConsumer<JetTypes>::Quantities["leadingJetEta"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jphi_1"] = LambdaNtupleConsumer<JetTypes>::Quantities["leadingJetPhi"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jpt_2"] = LambdaNtupleConsumer<JetTypes>::Quantities["trailingJetPt"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jeta_2"] = LambdaNtupleConsumer<JetTypes>::Quantities["trailingJetEta"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jphi_2"] = LambdaNtupleConsumer<JetTypes>::Quantities["trailingJetPhi"];
	
	// need to be called at last
	KappaLambdaNtupleConsumer<JetTypes>::Init(pipeline);
}

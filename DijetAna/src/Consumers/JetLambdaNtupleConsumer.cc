#include <Math/VectorUtil.h>

#include "Artus/Utility/interface/SafeMap.h"
#include "Artus/Utility/interface/Utility.h"
#include "Artus/Utility/interface/DefaultValues.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"

//std::string JetLambdaNtupleConsumer::GetConsumerId() const
//{
//	return "JetLambdaNtupleConsumer";
//}

void JetLambdaNtupleConsumer::Init(Pipeline<JetTypes> * pipeline)
{
	// add possible quantities for the lambda ntuples consumers
	
	// settings for synch ntuples
	LambdaNtupleConsumer<JetTypes>::Quantities["evt"] = [](event_type const& event, product_type const& product)
	{
		return event.m_eventMetadata->nEvent;
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["puweight"] = [](event_type const& event, product_type const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("puWeight"), 1.0);
	};
		LambdaNtupleConsumer<JetTypes>::Quantities["weight"] = [pipeline](event_type const& event, product_type const& product) {
		return SafeMap::GetWithDefault(product.m_weights, pipeline->GetSettings().GetEventWeight(), 1.0);
	};
	LambdaNtupleConsumer<JetTypes>::Quantities["jet1_pt"] = LambdaNtupleConsumer<JetTypes>::Quantities["leadingJetPt"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jet1_eta"] = LambdaNtupleConsumer<JetTypes>::Quantities["leadingJetEta"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jet1_phi"] = LambdaNtupleConsumer<JetTypes>::Quantities["leadingJetPhi"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jet2_pt"] = LambdaNtupleConsumer<JetTypes>::Quantities["trailingJetPt"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jet2_eta"] = LambdaNtupleConsumer<JetTypes>::Quantities["trailingJetEta"];
	LambdaNtupleConsumer<JetTypes>::Quantities["jet2_phi"] = LambdaNtupleConsumer<JetTypes>::Quantities["trailingJetPhi"];

	// need to be called at last
	KappaLambdaNtupleConsumer<JetTypes>::Init(pipeline);
}

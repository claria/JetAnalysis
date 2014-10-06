#include <Math/VectorUtil.h>

#include "Artus/Utility/interface/SafeMap.h"
#include "Artus/Utility/interface/Utility.h"
#include "Artus/Utility/interface/DefaultValues.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"

//std::string JetLambdaNtupleConsumer::GetConsumerId() const
//{
//	return "JetLambdaNtupleConsumer";
//}

void JetLambdaNtupleConsumer::Init( setting_type const& settings)
{
	// add possible quantities for the lambda ntuples consumers
	
	// settings for synch ntuples
	LambdaNtupleConsumer<JetTypes>::AddQuantity( "evt", [](event_type const& event, product_type const& product)
	{
		return event.m_eventMetadata->nEvent;
	} );
	//LambdaNtupleConsumer<JetTypes>::Quantities["puweight"] = [](event_type const& event, product_type const& product) {
	//	return SafeMap::GetWithDefault(product.m_weights, std::string("puWeight"), 1.0);
	//};
	LambdaNtupleConsumer<JetTypes>::AddQuantity ( "weight", [&settings](event_type const& event, product_type const& product) {
		return SafeMap::GetWithDefault(product.m_weights, settings.GetEventWeight(), 1.0);
	} );
	LambdaNtupleConsumer<JetTypes>::AddQuantity("jet1_pt", LambdaNtupleConsumer<JetTypes>::GetQuantities()["leadingJetPt"] );
	LambdaNtupleConsumer<JetTypes>::AddQuantity("jet1_eta", LambdaNtupleConsumer<JetTypes>::GetQuantities()["leadingJetEta"] );
	LambdaNtupleConsumer<JetTypes>::AddQuantity("jet1_phi", LambdaNtupleConsumer<JetTypes>::GetQuantities()["leadingJetPhi"] );
	LambdaNtupleConsumer<JetTypes>::AddQuantity("jet2_pt", LambdaNtupleConsumer<JetTypes>::GetQuantities()["trailingJetPt"] );
	LambdaNtupleConsumer<JetTypes>::AddQuantity("jet2_eta", LambdaNtupleConsumer<JetTypes>::GetQuantities()["trailingJetEta"] );
	LambdaNtupleConsumer<JetTypes>::AddQuantity("jet2_phi", LambdaNtupleConsumer<JetTypes>::GetQuantities()["trailingJetPhi"] );

	// need to be called at last
	KappaLambdaNtupleConsumer<JetTypes>::Init(settings);
}

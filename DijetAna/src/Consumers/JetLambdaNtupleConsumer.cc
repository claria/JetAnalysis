#include <Math/VectorUtil.h>
#include <cmath>
#include <algorithm>

#include "Artus/Utility/interface/SafeMap.h"
#include "Artus/Utility/interface/Utility.h"
#include "Artus/Utility/interface/DefaultValues.h"
#include "Artus/KappaAnalysis/interface/KappaTypes.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"

//std::string JetLambdaNtupleConsumer::GetConsumerId() const
//{
//	return "JetLambdaNtupleConsumer";
//}

void JetLambdaNtupleConsumer::Init(setting_type const& settings)
{
	// add possible quantities for the lambda ntuples consumers
	
	// settings for synch ntuples
	LambdaNtupleConsumer<KappaTypes>::AddIntQuantity("evt", [](KappaEvent const& event, KappaProduct const& product)
	{
		return event.m_eventMetadata->nEvent;
	} );
	//
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("puweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("puWeight"), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("trigweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("hltPrescaleWeight"), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("weight", [settings](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, settings.GetEventWeight(), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("generatorWeight"), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("xsweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("crossSectionPerEventWeight"), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("ngeneventsweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("numberGeneratedEventsWeight"), 1.0);
	});

	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("pathindex",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_selectedHltPosition;
	} );

	// MET
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("met",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_met->p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("sumet",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_met->sumEt;
	} );

	// Jets
	LambdaNtupleConsumer<KappaTypes>::AddIntQuantity("njets",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.size();
	} );
	// First jet
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet1_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genjet1_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet1_eta",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Eta();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet1_rap",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Rapidity();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet1_phi",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Phi();
	} );
	// Second jet
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet2_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet2_eta",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Eta();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet2_rap",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Rapidity();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet2_phi",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Phi();
	} );
	// Dijet observables
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_mass",[](KappaEvent const& event, KappaProduct const& product) {
		return (product.m_validJets.at(0)->p4 + product.m_validJets.at(1)->p4).mass();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_ymax",[](KappaEvent const& event, KappaProduct const& product) {
		return std::max(product.m_validJets.at(0)->p4.Rapidity(),product.m_validJets.at(1)->p4.Rapidity());
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_yboost",[](KappaEvent const& event, KappaProduct const& product) {
		return 0.5*(product.m_validJets.at(0)->p4.Rapidity() + product.m_validJets.at(1)->p4.Rapidity());
	} );

	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_ystar",[](KappaEvent const& event, KappaProduct const& product) {
		return 0.5*std::abs(product.m_validJets.at(0)->p4.Rapidity() - product.m_validJets.at(1)->p4.Rapidity());
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_chi",[](KappaEvent const& event, KappaProduct const& product) {
		return exp(abs(product.m_validJets.at(0)->p4.Rapidity() - product.m_validJets.at(1)->p4.Rapidity()));
	} );

	// need to be called at last
	KappaLambdaNtupleConsumer::Init(settings);
}

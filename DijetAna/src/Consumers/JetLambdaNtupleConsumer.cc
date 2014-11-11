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
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("evt", [](KappaEvent const& event, KappaProduct const& product)
	{
		return event.m_eventMetadata->nEvent;
	} );
	//
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("puweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("puWeight"), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("trigweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("hltPrescaleWeight"), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("weight", [settings](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, settings.GetEventWeight(), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("genweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("generatorWeight"), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("xsweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("crossSectionPerEventWeight"), 1.0);
	});
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("ngeneventsweight", [](KappaEvent const& event, KappaProduct const& product) {
		return SafeMap::GetWithDefault(product.m_weights, std::string("numberGeneratedEventsWeight"), 1.0);
	});

	LambdaNtupleConsumer<KappaTypes>::AddQuantity("pathindex",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_selectedHltPosition;
	} );

	// MET
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("met",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_met->p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("sumet",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_met->sumEt;
	} );

	// Jets
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("njets",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.size();
	} );
	// First jet
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("jet1_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("genjet1_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("jet1_eta",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Eta();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("jet1_rap",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Rapidity();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("jet1_phi",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Phi();
	} );
	// Second jet
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("jet2_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("jet2_eta",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Eta();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("jet2_rap",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Rapidity();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("jet2_phi",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Phi();
	} );
	// Dijet observables
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("dijet_mass",[](KappaEvent const& event, KappaProduct const& product) {
		return (product.m_validJets.at(1)->p4 + product.m_validJets.at(1)->p4).mass();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("dijet_ymax",[](KappaEvent const& event, KappaProduct const& product) {
		return std::max(product.m_validJets.at(0)->p4.Rapidity(),product.m_validJets.at(1)->p4.Rapidity());
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("dijet_yboost",[](KappaEvent const& event, KappaProduct const& product) {
		return 0.5*(product.m_validJets.at(0)->p4.Rapidity() + product.m_validJets.at(1)->p4.Rapidity());
	} );

	LambdaNtupleConsumer<KappaTypes>::AddQuantity("dijet_ystar",[](KappaEvent const& event, KappaProduct const& product) {
		return 0.5*std::abs(product.m_validJets.at(0)->p4.Rapidity() - product.m_validJets.at(1)->p4.Rapidity());
	} );
	LambdaNtupleConsumer<KappaTypes>::AddQuantity("dijet_chi",[](KappaEvent const& event, KappaProduct const& product) {
		return exp(abs(product.m_validJets.at(0)->p4.Rapidity() - product.m_validJets.at(1)->p4.Rapidity()));
	} );

	// need to be called at last
	KappaLambdaNtupleConsumer::Init(settings);
}

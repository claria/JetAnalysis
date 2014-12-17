#include <Math/VectorUtil.h>
#include <cmath>
#include <algorithm>

#include "Artus/Utility/interface/SafeMap.h"
#include "Artus/Utility/interface/Utility.h"
#include "Artus/Utility/interface/DefaultValues.h"
#include "Artus/KappaAnalysis/interface/KappaTypes.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetLambdaNtupleConsumer.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

//std::string JetLambdaNtupleConsumer::GetConsumerId() const
//{
//	return "JetLambdaNtupleConsumer";
//}

void JetLambdaNtupleConsumer::Init(setting_type const& settings)
{
	// add possible quantities for the lambda ntuples consumers
	
	//////////////
	// MetaData //
	//////////////
	LambdaNtupleConsumer<KappaTypes>::AddIntQuantity("evt", [](KappaEvent const& event, KappaProduct const& product)
	{
		return event.m_eventMetadata->nEvent;
	} );
	/////////////
	// Weights //
	/////////////
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

	// Kappa Trigger path index
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("pathindex",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_selectedHltPosition;
	} );

	// MET
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("met",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_met->p4.Pt();
	} );
	// SumEt
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("sumet",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_met->sumEt;
	} );

	/////////////
	// PF Jets //
	/////////////

	// Number of jets
	LambdaNtupleConsumer<KappaTypes>::AddIntQuantity("njets",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.size();
	} );
	// inclusive jet pts
	LambdaNtupleConsumer<KappaTypes>::AddVDoubleQuantity("incjets_pt",[](KappaEvent const& event, KappaProduct const& product) {
		auto const& specProduct = static_cast < JetProduct const&> (product);
		return specProduct.m_incJetsPt;
	} );
	LambdaNtupleConsumer<KappaTypes>::AddVDoubleQuantity("incjets_eta",[](KappaEvent const& event, KappaProduct const& product) {
		auto const& specProduct = static_cast < JetProduct const&> (product);
		return specProduct.m_incJetsEta;
	} );
	LambdaNtupleConsumer<KappaTypes>::AddVDoubleQuantity("incjets_rap",[](KappaEvent const& event, KappaProduct const& product) {
		auto const& specProduct = static_cast < JetProduct const&> (product);
		return specProduct.m_incJetsRap;
	} );
	LambdaNtupleConsumer<KappaTypes>::AddVDoubleQuantity("incjets_phi",[](KappaEvent const& event, KappaProduct const& product) {
		auto const& specProduct = static_cast < JetProduct const&> (product);
		return specProduct.m_incJetsPhi;
	} );

	// Leading jet pT
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet1_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Pt();
	} );
	// Leading jet eta
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet1_eta",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Eta();
	} );
	// Leading jet rapidity
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet1_rap",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Rapidity();
	} );
	// Leading jet phi
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet1_phi",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(0)->p4.Phi();
	} );
	// Second jet pT
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet2_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Pt();
	} );
	// Second jet eta
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet2_eta",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Eta();
	} );
	// Second jet rapidity
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet2_rap",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Rapidity();
	} );
	// Second jet phi
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("jet2_phi",[](KappaEvent const& event, KappaProduct const& product) {
		return product.m_validJets.at(1)->p4.Phi();
	} );

	/////////////
	// GenJets //
	/////////////

	// Dijet quantities
		LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("gendijet_mass",[](KappaEvent const& event, KappaProduct const& product) {
		return (event.m_genJets->at(0).p4 + event.m_genJets->at(1).p4).mass();
	} );
	// Leading GenJet
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genjet1_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_genJets->at(0).p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genjet1_eta",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_genJets->at(0).p4.Eta();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genjet1_rap",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_genJets->at(0).p4.Rapidity();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genjet1_phi",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_genJets->at(0).p4.Phi();
	} );
	// Second GenJet
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genjet2_pt",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_genJets->at(1).p4.Pt();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genjet2_eta",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_genJets->at(1).p4.Eta();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genjet2_rap",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_genJets->at(1).p4.Rapidity();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("genjet2_phi",[](KappaEvent const& event, KappaProduct const& product) {
		return event.m_genJets->at(1).p4.Phi();
	} );

	////////////
	// Dijets //
	////////////
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_mass",[](KappaEvent const& event, KappaProduct const& product) {
		return (product.m_validJets.at(0)->p4 + product.m_validJets.at(1)->p4).mass();
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_ymax",[](KappaEvent const& event, KappaProduct const& product) {
		return std::max(product.m_validJets.at(0)->p4.Rapidity(),product.m_validJets.at(1)->p4.Rapidity());
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_deltaphi",[](KappaEvent const& event, KappaProduct const& product) {
		return std::abs(product.m_validJets.at(0)->p4.Phi() - product.m_validJets.at(1)->p4.Phi());
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_costhetastar",[](KappaEvent const& event, KappaProduct const& product) {
		return std::tanh(product.m_validJets.at(0)->p4.Rapidity() - product.m_validJets.at(1)->p4.Rapidity());
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_yboost",[](KappaEvent const& event, KappaProduct const& product) {
		return 0.5*(product.m_validJets.at(0)->p4.Rapidity() + product.m_validJets.at(1)->p4.Rapidity());
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_ystar",[](KappaEvent const& event, KappaProduct const& product) {
		return 0.5*std::abs(product.m_validJets.at(0)->p4.Rapidity() - product.m_validJets.at(1)->p4.Rapidity());
	} );
	LambdaNtupleConsumer<KappaTypes>::AddFloatQuantity("dijet_chi",[](KappaEvent const& event, KappaProduct const& product) {
		return exp(std::abs(product.m_validJets.at(0)->p4.Rapidity() - product.m_validJets.at(1)->p4.Rapidity()));
	} );

	KappaLambdaNtupleConsumer::Init(settings);
}

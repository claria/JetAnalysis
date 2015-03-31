#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"
#include "TH3.h"

void JetUnfoldingResponseConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init(settings);

	// Define truth and reco binnings for response matrix
	TH3D meas_binning_jet12rap("meas_binning_jet12rap", "meas_binning_jet12rap", 
			settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0],
			settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0], 
			settings.GetTripleDiffPtBinning().size() -1, &settings.GetTripleDiffPtBinning()[0]);
	TH3D truth_binning_jet12rap("gen_binning_jet12rap", "gen_binning_jet12rap", 
			settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0],
			settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0], 
			settings.GetTripleDiffGenPtBinning().size() -1, &settings.GetTripleDiffGenPtBinning()[0]);
	TH3D meas_binning_jet12rapsign("meas_binning_jet12rapsign", "meas_binning_jet12rapsign", 
			settings.GetJet1RapidityBinning().size() - 1, &settings.GetJet1RapidityBinning()[0],
			settings.GetJet2RapidityBinning().size() - 1, &settings.GetJet2RapidityBinning()[0], 
			settings.GetTripleDiffPtBinning().size() -1, &settings.GetTripleDiffPtBinning()[0]);
	TH3D truth_binning_jet12rapsign("gen_binning_jet12rapsign", "gen_binning_jet12rapsign", 
			settings.GetJet1RapidityBinning().size() - 1, &settings.GetJet1RapidityBinning()[0],
			settings.GetJet2RapidityBinning().size() - 1, &settings.GetJet2RapidityBinning()[0], 
			settings.GetTripleDiffGenPtBinning().size() -1, &settings.GetTripleDiffGenPtBinning()[0]);

	TH1D meas_binning_pt("meas_binning_pt", "meas_binning_pt", 
			settings.GetPtBinning().size() -1, &settings.GetPtBinning()[0]);
	TH1D truth_binning_pt("truth_binning_pt", "truth_binning_pt", 
			settings.GetGenPtBinning().size() -1, &settings.GetGenPtBinning()[0]);

	m_unfoldResponse_jet12rap = new RooUnfoldResponse(&meas_binning_jet12rap, &truth_binning_jet12rap, "res_matrix_jet12rap", "res_matrix_jet12rap");
	m_unfoldResponse_jet12rapsign = new RooUnfoldResponse(&meas_binning_jet12rapsign, &truth_binning_jet12rapsign, "res_matrix_jet12rapsign", "res_matrix_jet12rapsign");
	m_unfoldResponse_pt = new RooUnfoldResponse(&meas_binning_pt, &truth_binning_pt, "res_matrix_pt", "res_matrix_pt");
}

void JetUnfoldingResponseConsumer::ProcessFilteredEvent(event_type const& event, product_type const& product, setting_type const& settings)
{
	double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

	if (product.m_matchedGenJets.at(0) != NULL &&
			product.m_matchedGenJets.at(1) != NULL) {
		m_unfoldResponse_jet12rap->Fill(product.m_validJets.at(0)->p4.Rapidity(), 
				product.m_validJets.at(1)->p4.Rapidity(),
				product.m_validJets.at(0)->p4.Pt(), 
				product.m_matchedGenJets.at(0)->p4.Rapidity(),
				product.m_matchedGenJets.at(1)->p4.Rapidity(),
				product.m_matchedGenJets.at(0)->p4.Pt(), 
				eventWeight);
		m_unfoldResponse_jet12rapsign->Fill(std::abs(product.m_validJets.at(0)->p4.Rapidity()), 
				boost::math::sign(product.m_validJets.at(0)->p4.Rapidity()*product.m_validJets.at(1)->p4.Rapidity())*product.m_validJets.at(1)->p4.Rapidity(),
				product.m_validJets.at(0)->p4.Pt(), 
				std::abs(product.m_matchedGenJets.at(0)->p4.Rapidity()),
				boost::math::sign(product.m_matchedGenJets.at(0)->p4.Rapidity()*product.m_matchedGenJets.at(1)->p4.Rapidity())*product.m_matchedGenJets.at(1)->p4.Rapidity(),
				product.m_matchedGenJets.at(0)->p4.Pt(), 
				eventWeight);

	}
	else {
		m_unfoldResponse_jet12rap->Fake(product.m_validJets.at(0)->p4.Rapidity(), 
				product.m_validJets.at(1)->p4.Rapidity(), 
				product.m_validJets.at(0)->p4.Pt(), 
				eventWeight);
		m_unfoldResponse_jet12rapsign->Fake(std::abs(product.m_validJets.at(0)->p4.Rapidity()), 
				boost::math::sign(product.m_validJets.at(0)->p4.Rapidity()*product.m_validJets.at(1)->p4.Rapidity())*product.m_validJets.at(1)->p4.Rapidity(), 
				product.m_validJets.at(0)->p4.Pt(), 
				eventWeight);

	}
	// Control Leading jet simple spectrum
	if (product.m_matchedGenJets.at(0)) {
		m_unfoldResponse_pt->Fill(product.m_validJets.at(0)->p4.Pt(), 
				product.m_matchedGenJets.at(0)->p4.Pt(), 
				eventWeight);

	}
	else {
		m_unfoldResponse_pt->Fake(product.m_validJets.at(0)->p4.Pt(), eventWeight);
	}
}

void JetUnfoldingResponseConsumer::Finish(setting_type const& settings)
{
	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
	m_unfoldResponse_jet12rap->Write(m_unfoldResponse_jet12rap->GetName());
	m_unfoldResponse_jet12rapsign->Write(m_unfoldResponse_jet12rapsign->GetName());
	m_unfoldResponse_pt->Write(m_unfoldResponse_pt->GetName());
}


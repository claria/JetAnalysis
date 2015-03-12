#include "JetAnalysis/DijetAna/interface/Consumers/JetQuantitiesHistogramConsumer.h"

void JetQuantitiesHistogramConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init(settings);
	// Jet Quantity histograms
	// TODO: Move to config
	// double incjet_pt_binning[42] = {48, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000}; 
	// double pt_binning[39] = {74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000}; 
	// double rap_binning[13] = {-3.0, -2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
	// double pt_binning2[7] = {74, 114, 196, 300, 468, 790, 3000}; 

	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

	// Leading Jet histograms
	m_h_jet1pt = new TH1D("h_jet1pt", "h_jet1pt", settings.GetPtBinning().size()-1, &settings.GetPtBinning()[0]);
	m_h_jet1pt->Sumw2();
	m_h_jet1rap = new TH1D("h_jet1rap", "h_jet1rap", 36, -3.0, 3.0);
	m_h_jet1rap->Sumw2();
	m_h_jet1phi = new TH1D("h_jet1phi", "h_jet1phi", 36, -3.2, 3.2);
	m_h_jet1phi->Sumw2();

	// Second Jet histograms
	m_h_jet2pt = new TH1D("h_jet2pt", "h_jet2pt", settings.GetPtBinning().size()-1, &settings.GetPtBinning()[0]);
	m_h_jet2pt->Sumw2();
	m_h_jet2rap = new TH1D("h_jet2rap", "h_jet2rap", 36, -3.0, 3.0);
	m_h_jet2rap->Sumw2();
	m_h_jet2phi = new TH1D("h_jet2phi", "h_jet2phi", 36, -3.2, 3.2);
	m_h_jet2phi->Sumw2();

	// Inclusive Jets
	m_h_incjetpt = new TH1D("h_incjetpt", "h_incjetpt", settings.GetPtBinning().size()-1, &settings.GetPtBinning()[0]);
	m_h_incjetpt->Sumw2();

	// Triple differential histogram
	m_h_jet12rap = new TH2D("h_jet12rap", "h_jet12rap", settings.GetRapidityBinning().size()-1, &settings.GetRapidityBinning()[0],
	                                                    settings.GetRapidityBinning().size()-1, &settings.GetRapidityBinning()[0]);
	m_h_jet12rap->Sumw2();
	// 3-dimensional histogram of rap1, rap2 and pt
	m_h3_jet12rap = new TH3D("h3_jet12rap", "h3_jet12rap", settings.GetRapidityBinning().size()-1, &settings.GetRapidityBinning()[0],
	                                                       settings.GetRapidityBinning().size()-1, &settings.GetRapidityBinning()[0],
	                                                       settings.GetTripleDiffPtBinning().size()-1, &settings.GetTripleDiffPtBinning()[0]);
	m_h3_jet12rap->Sumw2();
}

void JetQuantitiesHistogramConsumer::ProcessFilteredEvent(event_type const& event,
		product_type const& product,
		setting_type const& settings)
{
	// auto const& jetEvent = static_cast < JetEvent const&> (event);
	// auto const& jetProduct = static_cast < JetProduct const&> (product);

	double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

	// 1+ jet quantities
	if (product.m_validJets.size() > 0) {
		// std::cout << "Leading jet pt:" << product.m_validJets.at(0)->p4.Pt() << std::endl;
		m_h_jet1pt->Fill(product.m_validJets.at(0)->p4.Pt(), eventWeight);
		m_h_jet1rap->Fill(product.m_validJets.at(0)->p4.Rapidity(), eventWeight);
		m_h_jet1phi->Fill(product.m_validJets.at(0)->p4.Phi(), eventWeight);
	}
	// 2+ jet quantities
	if (product.m_validJets.size() > 1) {
		m_h_jet2pt->Fill(product.m_validJets.at(1)->p4.Pt(), eventWeight);
		m_h_jet2rap->Fill(product.m_validJets.at(1)->p4.Rapidity(), eventWeight);
		m_h_jet2phi->Fill(product.m_validJets.at(1)->p4.Phi(), eventWeight);

		m_h_jet12rap->Fill(product.m_validJets.at(0)->p4.Rapidity(), product.m_validJets.at(1)->p4.Rapidity(), eventWeight);
		m_h3_jet12rap->Fill(product.m_validJets.at(0)->p4.Rapidity(), product.m_validJets.at(1)->p4.Rapidity(), product.m_validJets.at(0)->p4.Pt(), eventWeight);
	}
	for ( auto jet = product.m_validJets.begin(); jet != product.m_validJets.end(); jet++ ) {
		m_h_incjetpt->Fill((*jet)->p4.Pt(), eventWeight);
	}
}

void JetQuantitiesHistogramConsumer::Finish(setting_type const& settings)
{
	// save histograms
	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

	m_h_jet1pt->Write(m_h_jet1pt->GetName());
	m_h_jet1rap->Write(m_h_jet1rap->GetName());
	m_h_jet1phi->Write(m_h_jet1phi->GetName());
	m_h_jet2pt->Write(m_h_jet2pt->GetName());
	m_h_jet2rap->Write(m_h_jet2rap->GetName());
	m_h_jet2phi->Write(m_h_jet2phi->GetName());
	m_h_incjetpt->Write(m_h_incjetpt->GetName());
	m_h_jet12rap->Write(m_h_jet12rap->GetName());
	m_h3_jet12rap->Write(m_h3_jet12rap->GetName());
}


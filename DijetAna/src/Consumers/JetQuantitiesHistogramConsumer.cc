#include "JetAnalysis/DijetAna/interface/Consumers/JetQuantitiesHistogramConsumer.h"

void JetQuantitiesHistogramConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init( settings );
	// Jet Quantity histograms
	double incjet_binning[42] = {48, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000}; 
	double jet_binning[39] = {74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000}; 
	m_hjet1pt = TH1F("h_jet1pt", "h_jet1pt", 38, jet_binning);
	m_hjet2pt = TH1F("h_jet2pt", "h_jet2pt", 38, jet_binning);
	m_hincjetpt = TH1F("h_incjetpt", "h_incjetpt", 41, incjet_binning);
}

void JetQuantitiesHistogramConsumer::ProcessFilteredEvent(event_type const& event,
		product_type const& product,
		setting_type const& settings)
{
	// auto const& jetEvent = static_cast < JetEvent const&> (event);
	// auto const& jetProduct = static_cast < JetProduct const&> (product);
	double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

	if (product.m_validJets.size() > 0) {
		m_hjet1pt.Fill(product.m_validJets.at(0)->p4.Pt(), eventWeight);
	}
	if (product.m_validJets.size() > 1) {
		m_hjet2pt.Fill(product.m_validJets.at(1)->p4.Pt(), eventWeight);
	}
	for ( auto jet = product.m_validJets.begin(); jet != product.m_validJets.end(); jet++ ) {
		m_hincjetpt.Fill((*jet)->p4.Pt(), eventWeight);
	}
}

void JetQuantitiesHistogramConsumer::Finish(setting_type const& settings)
{

	// save histograms
	RootFileHelper::SafeCd(settings.GetRootOutFile(),
			settings.GetRootFileFolder());

	m_hjet1pt.Write(m_hjet1pt.GetName());
	m_hjet2pt.Write(m_hjet2pt.GetName());
	m_hincjetpt.Write(m_hincjetpt.GetName());
}


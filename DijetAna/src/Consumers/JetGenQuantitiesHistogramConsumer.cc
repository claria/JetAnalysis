#include "JetAnalysis/DijetAna/interface/Consumers/JetGenQuantitiesHistogramConsumer.h"

void JetGenQuantitiesHistogramConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init( settings );
	// Jet Quantity histograms
	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
	m_h2GenVsRecoPt = new TH2D("h2GenVsRecoPt", "h2GenVsRecoPt", 50, 0.5, 1.5, settings.GetPtBinning().size()-1, &settings.GetPtBinning()[0]);
	m_h2GenVsRecoPt->Sumw2();
}

void JetGenQuantitiesHistogramConsumer::ProcessFilteredEvent(event_type const& event, product_type const& product, setting_type const& settings)
{
	double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;
	// KLV m_matchedGenJet = *product.m_matchedGenJets.at(0);
	// std::cout << "Reco jet pt: " << product.m_validJets.at(0)->p4.Pt() << " matched genjet pt " << m_matchedGenJet.p4.Pt() << std::endl;
	m_h2GenVsRecoPt->Fill(product.m_validJets.at(0)->p4.Pt()/(*product.m_matchedGenJets.at(0)).p4.Pt(), (*product.m_matchedGenJets.at(0)).p4.Pt(), eventWeight);
}

void JetGenQuantitiesHistogramConsumer::Finish(setting_type const& settings)
{
	// save histograms
	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

	m_h2GenVsRecoPt->Write(m_h2GenVsRecoPt->GetName());
}


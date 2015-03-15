#include "JetAnalysis/DijetAna/interface/Consumers/GenJetQuantitiesHistogramConsumer.h"

void GenJetQuantitiesHistogramConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init( settings );
	// Jet Quantity histograms
	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
	m_h2GenVsRecoPt = new TH2D("h2GenVsRecoPt", "h2GenVsRecoPt", 50, 0.5, 1.5, settings.GetPtBinning().size()-1, &settings.GetPtBinning()[0]);
	m_h2GenVsRecoPt->Sumw2();
	// Triple-differential distribution of y1, y2 and genjet pT
	m_h3_genjet12rap = new TH3D("h3_genjet12rap", "h3_genjet12rap", settings.GetRapidityBinning().size()-1, &settings.GetRapidityBinning()[0],
	                                                             settings.GetRapidityBinning().size()-1, &settings.GetRapidityBinning()[0],
	                                                             settings.GetTripleDiffPtBinning().size()-1, &settings.GetTripleDiffPtBinning()[0]);
	m_h3_genjet12rap->Sumw2();

}

void GenJetQuantitiesHistogramConsumer::ProcessFilteredEvent(event_type const& event, product_type const& product, setting_type const& settings)
{
	double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

	if (product.m_matchedRecoJets.at(0) != NULL) {
		m_h2GenVsRecoPt->Fill(product.m_matchedRecoJets.at(0)->p4.Pt()/event.m_genJets->at(0).p4.Pt(), event.m_genJets->at(0).p4.Pt(), eventWeight);
	}
	if (event.m_genJets->size() > 1) {
		m_h3_genjet12rap->Fill(event.m_genJets->at(0).p4.Rapidity(), 
		                       event.m_genJets->at(1).p4.Rapidity(), 
		                       event.m_genJets->at(0).p4.Pt(), 
		                       eventWeight);
	}

}

void GenJetQuantitiesHistogramConsumer::Finish(setting_type const& settings)
{
	// save histograms
	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
	m_h2GenVsRecoPt->Write(m_h2GenVsRecoPt->GetName());
	m_h3_genjet12rap->Write(m_h3_genjet12rap->GetName());
}


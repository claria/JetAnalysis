#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"

void JetUnfoldingResponseConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init( settings );

	double pt_binning[7] = {74, 114, 196, 300, 468, 790, 3000};
	double rap_binning[13] = {-3.0, -2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};

	TH2D* hist_binning = new TH2D("hist_binning", "hist_binning", 6, pt_binning, 12, rap_binning);
	m_unfoldResponse = new RooUnfoldResponse(hist_binning, hist_binning, "response_matrix", "response_matrix");
}

void JetUnfoldingResponseConsumer::ProcessEvent(event_type const& event, product_type const& product, setting_type const& settings, FilterResult& result)
{
	double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

	if (result.HasPassed()) 
	{
		m_unfoldResponse->Fill(product.m_validJets.at(0)->p4.Pt(),product.m_validJets.at(0)->p4.Rapidity(), event.m_genJets->at(0).p4.Pt(), event.m_genJets->at(0).p4.Rapidity(), eventWeight);
	}
	// TODO Implement correctly the missed events
	// else 
	// {
		// m_unfoldResponse->Miss(event.m_genJets->at(0).p4.Pt(), event.m_genJets->at(0).p4.Rapidity(), eventWeight);
	// }
}

void JetUnfoldingResponseConsumer::Finish(setting_type const& settings)
{

	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
	m_unfoldResponse->Write(m_unfoldResponse->GetName());
}


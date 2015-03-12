#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"
#include "TH3.h"

void JetUnfoldingResponseConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init(settings);

	// double pt_binning[7] = {74, 114, 196, 300, 468, 790, 3000};
	// double rap_binning[13] = {-3.0, -2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
	double* pt_binning = &settings.GetTripleDiffPtBinning()[0];
	double* rap_binning = &settings.GetRapidityBinning()[0];
	// std::cout << "ptsize " << settings.GetTripleDiffPtBinning().size() << std::endl;
	// std::cout << "rapsize " << settings.GetRapidityBinning().size() << std::endl;
	TH3D meas_binning("meas_binning", "meas_binning", settings.GetRapidityBinning().size() - 1, rap_binning,
	                                                  settings.GetRapidityBinning().size() - 1, rap_binning, 
	                                                  settings.GetTripleDiffPtBinning().size() -1, pt_binning);
	m_unfoldResponse = new RooUnfoldResponse(&meas_binning, &meas_binning, "response_matrix", "response_matrix");
}

void JetUnfoldingResponseConsumer::ProcessEvent(event_type const& event, product_type const& product, setting_type const& settings, FilterResult& result)
{
	double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

	if (result.HasPassed()) 
	{
		m_unfoldResponse->Fill(product.m_validJets.at(0)->p4.Rapidity(), product.m_validJets.at(1)->p4.Rapidity(),product.m_validJets.at(0)->p4.Pt(), (*product.m_matchedGenJets.at(0)).p4.Rapidity(), (*product.m_matchedGenJets.at(1)).p4.Rapidity(), (*product.m_matchedGenJets.at(0)).p4.Pt(), eventWeight);
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


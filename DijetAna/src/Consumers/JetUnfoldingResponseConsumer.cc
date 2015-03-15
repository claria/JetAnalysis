#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"
#include "TH3.h"

void JetUnfoldingResponseConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init(settings);

	// Define truth and reco binnings for response matrix
	TH3D meas_binning("meas_binning", "meas_binning", settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0],
	                                                  settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0], 
	                                                  settings.GetTripleDiffPtBinning().size() -1, &settings.GetTripleDiffPtBinning()[0]);
	// TH3D truth_binning("gen_binning", "gen_binning", settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0],
	//                                                settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0], 
	//                                                settings.GetTripleDiffGenPtBinning().size() -1, &settings.GetTripleDiffGenPtBinning()[0]);
	m_unfoldResponse = new RooUnfoldResponse(&meas_binning, &meas_binning, "response_matrix", "response_matrix");
}

void JetUnfoldingResponseConsumer::ProcessEvent(event_type const& event, product_type const& product, setting_type const& settings, FilterResult& result)
{
	double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

	if (result.HasPassed()) 
	{
		if (product.m_matchedRecoJets.at(0) != NULL && product.m_matchedRecoJets.at(1) != NULL) {
			m_unfoldResponse->Fill(product.m_matchedRecoJets.at(0)->p4.Rapidity(), 
					               product.m_matchedRecoJets.at(1)->p4.Rapidity(),
					               product.m_matchedRecoJets.at(0)->p4.Pt(), 
					               event.m_genJets->at(0).p4.Rapidity(), 
					               event.m_genJets->at(1).p4.Rapidity(), 
					               event.m_genJets->at(0).p4.Pt(), 
					               eventWeight);
		}
		else {
			m_unfoldResponse->Miss(event.m_genJets->at(0).p4.Rapidity(), 
					               event.m_genJets->at(1).p4.Rapidity(), 
					               event.m_genJets->at(0).p4.Pt(), 
					               eventWeight);
		}
	}
}

void JetUnfoldingResponseConsumer::Finish(setting_type const& settings)
{

	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
	m_unfoldResponse->Write(m_unfoldResponse->GetName());
}


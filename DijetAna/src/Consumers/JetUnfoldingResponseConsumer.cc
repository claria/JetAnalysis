#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"
#include "TH3.h"

void JetUnfoldingResponseConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init(settings);

	// Define truth and reco binnings for response matrix
	TH3D meas_binning("meas_binning", "meas_binning", settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0],
	                                                  settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0], 
	                                                  settings.GetTripleDiffPtBinning().size() -1, &settings.GetTripleDiffPtBinning()[0]);
	TH3D truth_binning("gen_binning", "gen_binning", settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0],
	                                                 settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0], 
	                                                 settings.GetTripleDiffGenPtBinning().size() -1, &settings.GetTripleDiffGenPtBinning()[0]);
	TH1D pt_binning("pt_binning", "pt_binning", settings.GetPtBinning().size() -1, &settings.GetPtBinning()[0]);
	TH1D genpt_binning("genpt_binning", "genpt_binning", settings.GetGenPtBinning().size() -1, &settings.GetGenPtBinning()[0]);

	m_unfoldResponse = new RooUnfoldResponse(&meas_binning, &truth_binning, "response_matrix", "response_matrix");
	m_unfoldResponseLeadJetPt = new RooUnfoldResponse(&pt_binning, &genpt_binning, "pt_response_matrix", "pt_response_matrix");
}

void JetUnfoldingResponseConsumer::ProcessFilteredEvent(event_type const& event, product_type const& product, setting_type const& settings)
{
	double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

		if (product.m_matchedGenJets.at(0) != NULL &&
			product.m_matchedGenJets.at(1) != NULL) {
			m_unfoldResponse->Fill(product.m_validJets.at(0)->p4.Rapidity(), 
			                       product.m_validJets.at(1)->p4.Rapidity(),
			                       product.m_validJets.at(0)->p4.Pt(), 
			                       product.m_matchedGenJets.at(0)->p4.Rapidity(),
			                       product.m_matchedGenJets.at(1)->p4.Rapidity(),
			                       product.m_matchedGenJets.at(0)->p4.Pt(), 
			                       eventWeight);
		}
		else {
			m_unfoldResponse->Fake(event.m_genJets->at(0).p4.Rapidity(), 
					               event.m_genJets->at(1).p4.Rapidity(), 
					               event.m_genJets->at(0).p4.Pt(), 
					               eventWeight);
		}
		// Control Leading jet simple spectrum
		if (product.m_matchedGenJets.at(0)) {
			m_unfoldResponseLeadJetPt->Fill(product.m_validJets.at(0)->p4.Pt(), 
			                                product.m_matchedGenJets.at(0)->p4.Pt(), 
			                                eventWeight);

		}
		else {
			m_unfoldResponseLeadJetPt->Fake(event.m_genJets->at(0).p4.Pt(), eventWeight);
		}
}

void JetUnfoldingResponseConsumer::Finish(setting_type const& settings)
{

	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
	m_unfoldResponse->Write(m_unfoldResponse->GetName());
	m_unfoldResponseLeadJetPt->Write(m_unfoldResponseLeadJetPt->GetName());
}


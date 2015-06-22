#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"
#include "TH3.h"

void JetUnfoldingResponseConsumer::Init(setting_type const& settings) {
  ConsumerBase<JetTypes>::Init(settings);

  // Define truth and reco binnings for response matrix
  TH3D meas_binning_jet12rap("meas_binning_jet12rap", "meas_binning_jet12rap", settings.GetRapidityBinning().size() - 1,
                             &settings.GetRapidityBinning()[0], settings.GetRapidityBinning().size() - 1,
                             &settings.GetRapidityBinning()[0], settings.GetTripleDiffPtBinning().size() - 1,
                             &settings.GetTripleDiffPtBinning()[0]);
  TH3D truth_binning_jet12rap("gen_binning_jet12rap", "gen_binning_jet12rap", settings.GetRapidityBinning().size() - 1,
                              &settings.GetRapidityBinning()[0], settings.GetRapidityBinning().size() - 1,
                              &settings.GetRapidityBinning()[0], settings.GetTripleDiffGenPtBinning().size() - 1,
                              &settings.GetTripleDiffGenPtBinning()[0]);
  TH3D meas_binning_jet12rapsign("meas_binning_jet12rapsign", "meas_binning_jet12rapsign",
                                 settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                                 settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0],
                                 settings.GetTripleDiffPtBinning().size() - 1, &settings.GetTripleDiffPtBinning()[0]);

  TH3D truth_binning_jet12rapsign(
      "gen_binning_jet12rapsign", "gen_binning_jet12rapsign", settings.GetRapidityAbsBinning().size() - 1,
      &settings.GetRapidityAbsBinning()[0], settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0],
      settings.GetTripleDiffGenPtBinning().size() - 1, &settings.GetTripleDiffGenPtBinning()[0]);

  TH3D meas_binning_ptavg_ysb("meas_binning_ptavg_ysb", "meas_binning_ptavg_ysb", settings.GetRapidityAbsBinning().size() - 1,
                          &settings.GetRapidityAbsBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
                          &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffPtBinning().size() - 1,
                          &settings.GetTripleDiffPtBinning()[0]);
  TH3D truth_binning_ptavg_ysb("gen_binning_ptavg_ysb", "gen_binning_ptavg_ysb", settings.GetRapidityAbsBinning().size() - 1,
                           &settings.GetRapidityAbsBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
                           &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffGenPtBinning().size() - 1,
                           &settings.GetTripleDiffGenPtBinning()[0]);

  TH1D meas_binning_pt("meas_binning_pt", "meas_binning_pt", settings.GetPtBinning().size() - 1,
                       &settings.GetPtBinning()[0]);
  TH1D truth_binning_pt("truth_binning_pt", "truth_binning_pt", settings.GetGenPtBinning().size() - 1,
                        &settings.GetGenPtBinning()[0]);

  m_unfoldResponse_jet12rap = new RooUnfoldResponse(&meas_binning_jet12rap, &truth_binning_jet12rap,
                                                    "res_matrix_jet12rap", "res_matrix_jet12rap");
  m_unfoldResponse_jet12rapsign = new RooUnfoldResponse(&meas_binning_jet12rapsign, &truth_binning_jet12rapsign,
                                                        "res_matrix_jet12rapsign", "res_matrix_jet12rapsign");

  m_unfoldResponse_ptavg_ysb = new RooUnfoldResponse(&meas_binning_ptavg_ysb, &truth_binning_ptavg_ysb, "res_matrix_ptavg_ysb", "res_matrix_ptavg_ysb");
  m_unfoldResponse_pt = new RooUnfoldResponse(&meas_binning_pt, &truth_binning_pt, "res_matrix_pt", "res_matrix_pt");
}

void JetUnfoldingResponseConsumer::ProcessEvent(event_type const& event, product_type const& product,
                                                setting_type const& settings, FilterResult& fres) {
  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  bool validGenEvent = false;
  bool validRecoEvent = false;

  if ((fres.GetDecisionEntry("NGenJetsFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (fres.GetDecisionEntry("LeadingGenJetPtFilter")->filterDecision == FilterResult::Decision::Passed)) 
  {
    validGenEvent = true;
  }

  if ((fres.GetDecisionEntry("NJetsFilter")->filterDecision == FilterResult::Decision::Passed) &&
     (fres.GetDecisionEntry("LeadingJetPtFilter")->filterDecision == FilterResult::Decision::Passed)) 
  {
    validRecoEvent = true;
  }

  if (validGenEvent && validRecoEvent)
  {
    m_unfoldResponse_ptavg_ysb->Fill(product.m_dijet_yboost, product.m_dijet_ystar, product.m_dijet_ptavg,
                                     product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg,
                                     eventWeight);
  }
  if (validGenEvent && !validRecoEvent) {
    std::cout << "Miss" << std::endl;
    m_unfoldResponse_ptavg_ysb->Miss(product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg, eventWeight);
  }
  if (! validGenEvent && validRecoEvent) {
    std::cout << "Fake" << std::endl;
    m_unfoldResponse_ptavg_ysb->Fake(product.m_dijet_yboost, product.m_dijet_ystar, product.m_dijet_ptavg, eventWeight);
  }


}
void JetUnfoldingResponseConsumer::ProcessFilteredEvent(event_type const& event, product_type const& product,
                                                        setting_type const& settings) 
{
  // double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  // if ((product.m_match_result_recojets[0] == 0 || product.m_match_result_recojets[0] == 1)
      // && (product.m_match_result_recojets[1] == 1 || product.m_match_result_recojets[1] == 0 )) 
  // {
    // m_unfoldResponse_ptavg_ysb->Fill(product.m_dijet_yboost, product.m_dijet_ystar, product.m_dijet_ptavg,
    //                                  product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg,
    //                                  eventWeight);
    // m_unfoldResponse_pt->Fill(product.m_jet1Pt, product.m_genjet1Pt, eventWeight);
  // }
    // m_unfoldResponse_ptavg_ysb->Fake(product.m_dijet_yboost, product.m_dijet_ystar, product.m_dijet_ptavg, eventWeight);
    // m_unfoldResponse_ptavg_ysb->Miss(product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg, eventWeight);
}

void JetUnfoldingResponseConsumer::Finish(setting_type const& settings) {
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
  m_unfoldResponse_jet12rap->Write(m_unfoldResponse_jet12rap->GetName());
  m_unfoldResponse_jet12rapsign->Write(m_unfoldResponse_jet12rapsign->GetName());
  m_unfoldResponse_ptavg_ysb->Write(m_unfoldResponse_ptavg_ysb->GetName());
  m_unfoldResponse_pt->Write(m_unfoldResponse_pt->GetName());
}

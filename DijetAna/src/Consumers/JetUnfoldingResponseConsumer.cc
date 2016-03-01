#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"
#include "TH3.h"

void JetUnfoldingResponseConsumer::Init(setting_type const& settings) {
  ConsumerBase<JetTypes>::Init(settings);

  // Define gen and reco binnings for response matrix
  TH3D reco_binning_ptavg_ysb("reco_binning_ptavg_ysb",
                              "reco_binning_ptavg_ysb",
                              settings.GetRapidityAbsBinning().size() - 1,
                              &settings.GetRapidityAbsBinning()[0],
                              settings.GetRapidityAbsBinning().size() - 1,
                              &settings.GetRapidityAbsBinning()[0],
                              settings.GetPtBinning().size() - 1,
                              &settings.GetPtBinning()[0]);
  TH3D gen_binning_ptavg_ysb("gen_binning_ptavg_ysb",
                             "gen_binning_ptavg_ysb",
                             settings.GetRapidityAbsBinning().size() - 1,
                             &settings.GetRapidityAbsBinning()[0],
                             settings.GetRapidityAbsBinning().size() - 1,
                             &settings.GetRapidityAbsBinning()[0],
                             settings.GetGenPtBinning().size() - 1,
                             &settings.GetGenPtBinning()[0]);

  TH1D reco_binning_ptavg("reco_binning_ptavg", "reco_binning_ptavg", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  TH1D gen_binning_ptavg("gen_binning_ptavg", "gen_binning_ptavg", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);

  m_unfoldResponse_ptavg_ysb = new RooUnfoldResponse(
      &reco_binning_ptavg_ysb, &gen_binning_ptavg_ysb, "res_matrix_ptavg_ysb", "res_matrix_ptavg_ysb");
  m_unfoldResponse_ptavg =
      new RooUnfoldResponse(&reco_binning_ptavg, &gen_binning_ptavg, "res_matrix_ptavg", "res_matrix_ptavg");

  m_h2_response_idx = new TH2D("h_response_idx", "h_response_idx",
                      (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6,
                      (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6);
  m_h2_response_idx->Sumw2();

  TH1D h1_reco_idx("h_reco_idx", "h_reco_idx", (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6);
  TH1D h1_gen_idx("h_gen_idx", "h_gen_idx", (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6);
  m_unfoldResponse_idx = new RooUnfoldResponse(&h1_reco_idx, &h1_gen_idx, "res_matrix_idx", "res_matrix_idx");
}

void JetUnfoldingResponseConsumer::ProcessFilteredEvent(event_type const& event,
    product_type const& product,
    setting_type const& settings) {
  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  bool validGenEvent = false;
  if ((product.fres.GetDecisionEntry("NGenJetsFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("GenPtAvgFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("GenYStarFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("GenYBoostFilter")->filterDecision == FilterResult::Decision::Passed)) {
    validGenEvent = true;
  }

  bool validRecoEvent = false;
  if ((product.fres.GetDecisionEntry("NJetsFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("PtAvgFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("YStarFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("YBoostFilter")->filterDecision == FilterResult::Decision::Passed)) {
    validRecoEvent = true;
  }

  if (validGenEvent && validRecoEvent) {
    m_unfoldResponse_ptavg->Fill(product.m_dijet_ptavg, product.m_gendijet_ptavg, eventWeight);
    m_unfoldResponse_idx->Fill(product.m_dijet_idx, product.m_gendijet_idx, eventWeight);
    m_unfoldResponse_ptavg_ysb->Fill(product.m_dijet_yboost,
        product.m_dijet_ystar,
        product.m_dijet_ptavg,
        product.m_gendijet_yboost,
        product.m_gendijet_ystar,
        product.m_gendijet_ptavg,
        eventWeight);
    m_h2_response_idx->Fill(product.m_dijet_idx, product.m_gendijet_idx);
  }
  if (validGenEvent && !validRecoEvent) {
    m_unfoldResponse_ptavg->Miss(product.m_gendijet_ptavg, eventWeight);
    m_unfoldResponse_idx->Miss(product.m_gendijet_idx, eventWeight);
    m_unfoldResponse_ptavg_ysb->Miss(product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg, eventWeight);
  }
  if (!validGenEvent && validRecoEvent) {
    m_unfoldResponse_ptavg->Fake(product.m_dijet_ptavg, eventWeight);
    m_unfoldResponse_idx->Fake(product.m_dijet_idx, eventWeight);
    m_unfoldResponse_ptavg_ysb->Fake(product.m_dijet_yboost, product.m_dijet_ystar, product.m_dijet_ptavg, eventWeight);
  }
}

void JetUnfoldingResponseConsumer::Finish(setting_type const& settings) {
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
  m_h2_response_idx->Write();
  m_unfoldResponse_idx->Write();

  m_unfoldResponse_ptavg_ysb->Write(m_unfoldResponse_ptavg_ysb->GetName());
  m_unfoldResponse_ptavg->Write(m_unfoldResponse_ptavg->GetName());
  m_unfoldResponse_ptavg->Hmeasured()->Write((std::string(m_unfoldResponse_ptavg->GetName()) + "_measured").c_str());
  m_unfoldResponse_ptavg->Hresponse()->Write((std::string(m_unfoldResponse_ptavg->GetName()) + "_response").c_str());
  m_unfoldResponse_ptavg->Htruth()->Write((std::string(m_unfoldResponse_ptavg->GetName()) + "_truth").c_str());
  m_unfoldResponse_ptavg->Hfakes()->Write((std::string(m_unfoldResponse_ptavg->GetName()) + "_fakes").c_str());
}

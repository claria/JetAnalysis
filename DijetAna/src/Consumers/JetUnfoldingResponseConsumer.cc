#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"
#include "TH3.h"

void JetUnfoldingResponseConsumer::Init(setting_type const& settings) {
  ConsumerBase<JetTypes>::Init(settings);

  m_h1_unf_gen_ptavg = new TH1D("h_unf_gen_ptavg", "h_unf_gen_ptavg", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h1_unf_gen_ptavg->Sumw2();

  m_h1_unf_fake_ptavg = new TH1D("h_unf_fake_ptavg", "h_unf_fake_ptavg", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h1_unf_fake_ptavg->Sumw2();

  m_h1_unf_miss_ptavg = new TH1D("h_unf_miss_ptavg", "h_unf_miss_ptavg", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h1_unf_miss_ptavg->Sumw2();

  m_h1_unf_reco_ptavg = new TH1D("h_unf_reco_ptavg", "h_unf_reco_ptavg", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h1_unf_reco_ptavg->Sumw2();
  m_h2_unf_response_ptavg = new TH2D("h2_unf_response_ptavg","h2_unf_response_ptavg", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0],
                                                                                      settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h2_unf_response_ptavg->Sumw2();


  m_h1_unf_gen_idx = new TH1D("h_unf_gen_idx", "h_unf_gen_idx", (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6);
  m_h1_unf_gen_idx->Sumw2();
  m_h1_unf_reco_idx = new TH1D("h_unf_reco_idx", "h_unf_reco_idx", (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6);
  m_h1_unf_reco_idx->Sumw2();
  m_h2_unf_response_idx = new TH2D("h2_unf_response_idx", "h2_unf_response_idx",
                      (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6,
                      (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6);
  m_h2_unf_response_idx->Sumw2();
  m_h2_unf_genvsreco_ptavg = new TH2D("h2_unf_genvsreco_ptavg",
                                      "h2_genvsreco_ptavg",
                                      50,
                                      0.5,
                                      1.5,
                                      settings.GetGenPtBinning().size() - 1,
                                      &settings.GetGenPtBinning()[0]);
  m_h2_unf_genvsreco_ptavg->Sumw2();

}

void JetUnfoldingResponseConsumer::ProcessFilteredEvent(event_type const& event,
    product_type const& product,
    setting_type const& settings) {
  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;


  // check if valid gen event using tagging filters
  bool validGenEvent = false;
  if ((product.fres.GetDecisionEntry("NGenJetsFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("GenPtAvgFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("GenDijetsRapFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("GenYStarFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (product.fres.GetDecisionEntry("GenYBoostFilter")->filterDecision == FilterResult::Decision::Passed)) 
  {
    validGenEvent = true;
  }

  // check if valid reco event using tagging filters
   bool validRecoEvent = false;
   if ((product.fres.GetDecisionEntry("NJetsFilter")->filterDecision == FilterResult::Decision::Passed) &&
       (product.fres.GetDecisionEntry("PtAvgFilter")->filterDecision == FilterResult::Decision::Passed) &&
       (product.fres.GetDecisionEntry("DijetsRapFilter")->filterDecision == FilterResult::Decision::Passed) &&
       (product.fres.GetDecisionEntry("YStarFilter")->filterDecision == FilterResult::Decision::Passed) &&
       (product.fres.GetDecisionEntry("GoodPrimaryVertexFilter")->filterDecision == FilterResult::Decision::Passed) &&
       (product.fres.GetDecisionEntry("METSumEtFilter")->filterDecision == FilterResult::Decision::Passed) &&
       (product.fres.GetDecisionEntry("YBoostFilter")->filterDecision == FilterResult::Decision::Passed)) 
   {
     validRecoEvent = true;
   }

  if (validGenEvent) {
    m_h1_unf_gen_ptavg->Fill(product.m_gendijet_ptavg, eventWeight);
    m_h1_unf_gen_idx->Fill(product.m_gendijet_idx, eventWeight);
  }
  if (validRecoEvent) {
    m_h1_unf_reco_ptavg->Fill(product.m_dijet_ptavg, eventWeight);
    m_h1_unf_reco_idx->Fill(product.m_dijet_idx, eventWeight);
  }
  if (validGenEvent && !validRecoEvent) {
    m_h1_unf_miss_ptavg->Fill(product.m_gendijet_ptavg, eventWeight);
  }
  if (validRecoEvent && !validGenEvent) {
    m_h1_unf_fake_ptavg->Fill(product.m_dijet_ptavg, eventWeight);
  }
  if (validRecoEvent && validGenEvent) {
    m_h2_unf_genvsreco_ptavg->Fill(product.m_dijet_ptavg/product.m_gendijet_ptavg, product.m_gendijet_ptavg, eventWeight);
    m_h2_unf_response_ptavg->Fill(product.m_dijet_ptavg, product.m_gendijet_ptavg, eventWeight);
    m_h2_unf_response_idx->Fill(product.m_dijet_idx, product.m_gendijet_idx, eventWeight);
  }
  // m_genmatchdijet_ptavg

}

void JetUnfoldingResponseConsumer::Finish(setting_type const& settings) {
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

  m_h1_unf_gen_ptavg->Write();
  m_h1_unf_reco_ptavg->Write();
  m_h1_unf_miss_ptavg->Write();
  m_h1_unf_fake_ptavg->Write();
  m_h2_unf_response_ptavg->Write();

  m_h1_unf_gen_idx->Write();
  m_h1_unf_reco_idx->Write();
  m_h2_unf_response_idx->Write();
  m_h2_unf_genvsreco_ptavg->Write();
  // m_unfoldResponse_idx->Write();

  // m_unfoldResponse_ptavg_ysb->Write(m_unfoldResponse_ptavg_ysb->GetName());
  // m_unfoldResponse_ptavg->Write(m_unfoldResponse_ptavg->GetName());
  // m_unfoldResponse_ptavg->Hmeasured()->Write((std::string(m_unfoldResponse_ptavg->GetName()) + "_measured").c_str());
  // m_unfoldResponse_ptavg->Hresponse()->Write((std::string(m_unfoldResponse_ptavg->GetName()) + "_response").c_str());
  // m_unfoldResponse_ptavg->Htruth()->Write((std::string(m_unfoldResponse_ptavg->GetName()) + "_truth").c_str());
  // m_unfoldResponse_ptavg->Hfakes()->Write((std::string(m_unfoldResponse_ptavg->GetName()) + "_fakes").c_str());
}

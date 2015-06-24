#include "JetAnalysis/DijetAna/interface/Consumers/JetUnfoldingResponseConsumer.h"
#include "TH3.h"

void JetUnfoldingResponseConsumer::Init(setting_type const &settings) {
  ConsumerBase<JetTypes>::Init(settings);

  // Define gen and reco binnings for response matrix
  TH3D reco_binning_jet12rap("reco_binning_jet12rap", "reco_binning_jet12rap",
                             settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                             settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                             settings.GetTripleDiffPtBinning().size() - 1, &settings.GetTripleDiffPtBinning()[0]);
  TH3D gen_binning_jet12rap("gen_binning_jet12rap", "gen_binning_jet12rap", settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffGenPtBinning().size() - 1,
                            &settings.GetTripleDiffGenPtBinning()[0]);

  TH3D reco_binning_ptavg_yio("reco_binning_ptavg_yio", "reco_binning_ptavg_yio",
                              settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                              settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                              settings.GetTripleDiffPtBinning().size() - 1, &settings.GetTripleDiffPtBinning()[0]);
  TH3D gen_binning_ptavg_yio("gen_binning_ptavg_yio", "gen_binning_ptavg_yio",
                             settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                             settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                             settings.GetTripleDiffGenPtBinning().size() - 1, &settings.GetTripleDiffGenPtBinning()[0]);

  TH3D reco_binning_ptavg_ysb("reco_binning_ptavg_ysb", "reco_binning_ptavg_ysb",
                              settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                              settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                              settings.GetTripleDiffPtBinning().size() - 1, &settings.GetTripleDiffPtBinning()[0]);
  TH3D gen_binning_ptavg_ysb("gen_binning_ptavg_ysb", "gen_binning_ptavg_ysb",
                             settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                             settings.GetRapidityAbsBinning().size() - 1, &settings.GetRapidityAbsBinning()[0],
                             settings.GetTripleDiffGenPtBinning().size() - 1, &settings.GetTripleDiffGenPtBinning()[0]);

  TH1D reco_binning_pt("reco_binning_pt", "reco_binning_pt", settings.GetPtBinning().size() - 1,
                       &settings.GetPtBinning()[0]);
  TH1D gen_binning_pt("gen_binning_pt", "gen_binning_pt", settings.GetGenPtBinning().size() - 1,
                      &settings.GetGenPtBinning()[0]);

  m_unfoldResponse_jet12rap = new RooUnfoldResponse(&reco_binning_jet12rap, &gen_binning_jet12rap,
                                                    "res_matrix_jet12rap", "res_matrix_jet12rap");
  m_unfoldResponse_ptavg_yio = new RooUnfoldResponse(&reco_binning_ptavg_yio, &gen_binning_ptavg_yio,
                                                     "res_matrix_ptavg_yio", "res_matrix_ptavg_yio");
  m_unfoldResponse_ptavg_ysb = new RooUnfoldResponse(&reco_binning_ptavg_ysb, &gen_binning_ptavg_ysb,
                                                     "res_matrix_ptavg_ysb", "res_matrix_ptavg_ysb");
  m_unfoldResponse_pt = new RooUnfoldResponse(&reco_binning_pt, &gen_binning_pt, "res_matrix_pt", "res_matrix_pt");
}

void JetUnfoldingResponseConsumer::ProcessEvent(event_type const &event, product_type const &product,
                                                setting_type const &settings, FilterResult &fres) {
  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  bool validGenEvent = false;
  bool validRecoEvent = false;

  if ((fres.GetDecisionEntry("NGenJetsFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (fres.GetDecisionEntry("LeadingGenJetPtFilter")->filterDecision == FilterResult::Decision::Passed)) {
    validGenEvent = true;
  }

  if ((fres.GetDecisionEntry("NJetsFilter")->filterDecision == FilterResult::Decision::Passed) &&
      (fres.GetDecisionEntry("LeadingJetPtFilter")->filterDecision == FilterResult::Decision::Passed)) {
    validRecoEvent = true;
  }

  if (validGenEvent && validRecoEvent) {
    m_unfoldResponse_ptavg_ysb->Fill(product.m_dijet_yboost, product.m_dijet_ystar, product.m_dijet_ptavg,
                                     product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg,
                                     eventWeight);
    m_unfoldResponse_ptavg_yio->Fill(boost::math::sign(product.m_dijet_yinner) * product.m_dijet_youter,
                                     std::abs(product.m_dijet_yinner), product.m_dijet_ptavg,
                                     boost::math::sign(product.m_gendijet_yinner) * product.m_gendijet_youter,
                                     std::abs(product.m_gendijet_yinner), product.m_gendijet_ptavg, eventWeight);
    m_unfoldResponse_jet12rap->Fill(boost::math::sign(product.m_jet1Rap) * product.m_jet2Rap,
                                    std::abs(product.m_jet1Rap), product.m_jet1Pt,
                                    boost::math::sign(product.m_genjet1Rap) * product.m_genjet2Rap,
                                    std::abs(product.m_genjet1Rap), product.m_genjet1Pt, eventWeight);
    m_unfoldResponse_jet12rap->Fill(boost::math::sign(product.m_jet2Rap) * product.m_jet1Rap,
                                    std::abs(product.m_jet2Rap), product.m_jet2Pt,
                                    boost::math::sign(product.m_genjet2Rap) * product.m_genjet1Rap,
                                    std::abs(product.m_genjet2Rap), product.m_genjet2Pt, eventWeight);
  }
  if (validGenEvent && !validRecoEvent) {
    m_unfoldResponse_ptavg_ysb->Miss(product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg,
                                     eventWeight);
    m_unfoldResponse_ptavg_yio->Miss(boost::math::sign(product.m_gendijet_yinner) * product.m_gendijet_youter,
                                     std::abs(product.m_gendijet_yinner), product.m_gendijet_ptavg, eventWeight);
    m_unfoldResponse_jet12rap->Miss(boost::math::sign(product.m_genjet1Rap) * product.m_genjet2Rap,
                                    std::abs(product.m_genjet1Rap), product.m_genjet1Pt, eventWeight);
    m_unfoldResponse_jet12rap->Miss(boost::math::sign(product.m_genjet2Rap) * product.m_genjet1Rap,
                                    std::abs(product.m_genjet2Rap), product.m_genjet2Pt, eventWeight);
  }
  if (!validGenEvent && validRecoEvent) {
    m_unfoldResponse_ptavg_ysb->Fake(product.m_dijet_yboost, product.m_dijet_ystar, product.m_dijet_ptavg, eventWeight);
    m_unfoldResponse_ptavg_yio->Fake(boost::math::sign(product.m_dijet_yinner) * product.m_dijet_youter,
                                     std::abs(product.m_dijet_yinner), product.m_dijet_ptavg, eventWeight);
    m_unfoldResponse_jet12rap->Fake(boost::math::sign(product.m_jet1Rap) * product.m_jet2Rap,
                                    std::abs(product.m_jet1Rap), product.m_jet1Pt, eventWeight);
    m_unfoldResponse_jet12rap->Fake(boost::math::sign(product.m_jet2Rap) * product.m_jet1Rap,
                                    std::abs(product.m_jet2Rap), product.m_jet2Pt, eventWeight);
  }
}
void JetUnfoldingResponseConsumer::ProcessFilteredEvent(event_type const &event, product_type const &product,
                                                        setting_type const &settings) {}

void JetUnfoldingResponseConsumer::Finish(setting_type const &settings) {
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
  m_unfoldResponse_jet12rap->Write(m_unfoldResponse_jet12rap->GetName());
  m_unfoldResponse_ptavg_ysb->Write(m_unfoldResponse_ptavg_ysb->GetName());
  m_unfoldResponse_ptavg_yio->Write(m_unfoldResponse_ptavg_yio->GetName());
  m_unfoldResponse_pt->Write(m_unfoldResponse_pt->GetName());
}

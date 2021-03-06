#include "JetAnalysis/DijetAna/interface/Consumers/JetQuantitiesHistogramConsumer.h"
#include "JetAnalysis/DijetAna/interface/Helpers.h"
// #include "Artus/Utility/interface/Utility.h"
// #include "Artus/Utility/interface/Utility.h"
#include <boost/math/special_functions/sign.hpp>

void JetQuantitiesHistogramConsumer::Init(setting_type const& settings) {
  ConsumerBase<JetTypes>::Init(settings);

  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

  m_h_nPV = new TH1D("h_nPV", "h_nPV", 61, -0.5, 60.5);
  m_h_nPV->Sumw2();
  m_h_hltPosition = new TH1D("h_hltPosition", "h_hltPosition", 11, -0.5, 10.5);
  m_h_hltPosition->Sumw2();

  // Leading Jet histograms
  m_h_jet1pt = new TH1D("h_jet1pt", "h_jet1pt", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h_jet1pt->Sumw2();
  m_h_jet1rap = new TH1D("h_jet1rap", "h_jet1rap", 50, -5.0, 5.0);
  m_h_jet1rap->Sumw2();
  m_h_jet1eta = new TH1D("h_jet1eta", "h_jet1eta", 50, -5.0, 5.0);
  m_h_jet1eta->Sumw2();
  m_h_jet1phi = new TH1D("h_jet1phi", "h_jet1phi", 36, -3.2, 3.2);
  m_h_jet1phi->Sumw2();

  // Second Jet histograms
  m_h_jet2pt = new TH1D("h_jet2pt", "h_jet2pt", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h_jet2pt->Sumw2();
  m_h_jet2rap = new TH1D("h_jet2rap", "h_jet2rap", 50, -5.0, 5.0);
  m_h_jet2rap->Sumw2();
  m_h_jet2eta = new TH1D("h_jet2eta", "h_jet2eta", 50, -5.0, 5.0);
  m_h_jet2eta->Sumw2();
  m_h_jet2phi = new TH1D("h_jet2phi", "h_jet2phi", 36, -3.2, 3.2);
  m_h_jet2phi->Sumw2();

  // MET
  m_h_METSumEtRatio = new TH1D("h_METSumEtRatio", "h_METSumEtRatio", 50, 0.0, 1.0);
  m_h_METSumEtRatio->Sumw2();

  // Inclusive Jets
  m_h_incjetpt = new TH1D("h_incjetpt", "h_incjetpt", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h_incjetpt->Sumw2();

  // Pt Avg
  m_h_ptavg = new TH1D("h_ptavg", "h_ptavg", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h_ptavg->Sumw2();

  m_h_idx = new TH1D("h_idx", "h_idx", (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6);
  m_h_idx->Sumw2();

  // dijet delta Phi
  std::vector<double> dijet_deltaphi_binning = linspace(0.0, 6.3, 100.0);
  m_h_dijet_deltaphi = new TH1D("h_dijet_deltaphi", "h_dijet_deltaphi", dijet_deltaphi_binning.size() -1, &dijet_deltaphi_binning[0]);
  m_h_dijet_deltaphi->Sumw2();
  // dijet costhetastar
  std::vector<double> dijet_costhetastar_binning = linspace(-1.0, 1.0, 100.0);
  m_h_dijet_costhetastar = new TH1D("h_dijet_costhetastar", "h_dijet_costhetastar", dijet_costhetastar_binning.size() -1, &dijet_costhetastar_binning[0]);
  m_h_dijet_costhetastar->Sumw2();
  // dijet delta DeltaR
  std::vector<double> dijet_deltar_binning = linspace(0.0, 6.3, 100.0);
  m_h_dijet_deltar = new TH1D("h_dijet_deltar", "h_dijet_deltar", dijet_deltar_binning.size() -1, &dijet_deltar_binning[0]);
  m_h_dijet_deltar->Sumw2();
  // dijet ystar
  std::vector<double> dijet_ystar_binning = linspace(-5.0, 5.0, 100);
  m_h_dijet_ystar = new TH1D("h_dijet_ystar", "h_dijet_ystar", dijet_ystar_binning.size() -1, &dijet_ystar_binning[0]);
  m_h_dijet_ystar->Sumw2();
  // dijet yboost
  std::vector<double> dijet_yboost_binning = linspace(-5.0, 5.0, 100);
  m_h_dijet_yboost = new TH1D("h_dijet_yboost", "h_dijet_yboost", dijet_yboost_binning.size() -1, &dijet_yboost_binning[0]);
  m_h_dijet_yboost->Sumw2();
  // dijet chi
  std::vector<double> dijet_chi_binning = logspace(0.0, 2, 100);
  m_h_dijet_chi = new TH1D("h_dijet_chi", "h_dijet_chi", dijet_chi_binning.size() -1, &dijet_chi_binning[0]);
  m_h_dijet_chi->Sumw2();
  // dijet mass
  std::vector<double> dijet_mass_binning = logspace(1.0, 4, 100);
  m_h_dijet_mass = new TH1D("h_dijet_mass", "h_dijet_mass", dijet_mass_binning.size() -1, &dijet_mass_binning[0]);
  m_h_dijet_mass->Sumw2();

  // Jet12 Pt
  m_h2_jet12Pt = new TH2D("h2_jet12pt", "h2_jet12pt",
                        settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0],
                        settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h2_jet12Pt->Sumw2();

  m_h2_jet12PtRVsPtavg = new TH2D("h2_jet12ptrvsptavg", "h2_jet12ptrvsptavg",
                        settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0],
                        50, 0.0, 1.0 );
  m_h2_jet12PtRVsPtavg->Sumw2();

  m_h2_jet1PtVsPtavg = new TH2D("h2_jet1ptvsptavg", "h2_jet1ptvsptavg",
                        settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0],
                        settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h2_jet1PtVsPtavg->Sumw2();


  // 2D histograms
  m_h2_yb_ys = new TH2D("h2_yb_ys",
                        "h2_yb_ys",
                        settings.GetRapidityAbsBinning().size() - 1,
                        &settings.GetRapidityAbsBinning()[0],
                        settings.GetRapidityAbsBinning().size() - 1,
                        &settings.GetRapidityAbsBinning()[0]);
  m_h2_yb_ys->Sumw2();
  // dijet rapidity distribution for two leading jets
  m_h2_y12 = new TH2D("h2_y12",
                      "h2_y12",
                      settings.GetFineRapidityBinning().size() - 1,
                      &settings.GetFineRapidityBinning()[0],
                      settings.GetFineRapidityBinning().size() - 1,
                      &settings.GetFineRapidityBinning()[0]);
  m_h2_y12->Sumw2();

  // Triple differential histogram
  m_h3_jet12rap = new TH3D("h3_jet12rap",
                           "h3_jet12rap",
                           settings.GetRapidityBinning().size() - 1,
                           &settings.GetRapidityBinning()[0],
                           settings.GetRapidityAbsBinning().size() - 1,
                           &settings.GetRapidityAbsBinning()[0],
                           settings.GetTripleDiffPtBinning().size() - 1,
                           &settings.GetTripleDiffPtBinning()[0]);
  m_h3_jet12rap->Sumw2();

  m_h3_ptavg_yio = new TH3D("h3_ptavg_yio",
                            "h3_ptavg_yio",
                            settings.GetRapidityBinning().size() - 1,
                            &settings.GetRapidityBinning()[0],
                            settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0],
                            settings.GetTripleDiffPtBinning().size() - 1,
                            &settings.GetTripleDiffPtBinning()[0]);
  m_h3_ptavg_yio->Sumw2();

  m_h3_ptavg_ysb = new TH3D("h3_ptavg_ysb",
                            "h3_ptavg_ysb",
                            settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0],
                            settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0],
                            settings.GetTripleDiffPtBinning().size() - 1,
                            &settings.GetTripleDiffPtBinning()[0]);
  m_h3_ptavg_ysb->Sumw2();

  m_h_neutralHadronFraction = new TH1D("h_neutralHadronFraction", "h_neutralHadronFraction", 50, 0., 1.);
  m_h_neutralHadronFraction->Sumw2();
  m_h_chargedHadronFraction = new TH1D("h_chargedHadronFraction", "h_chargedHadronFraction", 50, 0., 1.);
  m_h_chargedHadronFraction->Sumw2();
  m_h_neutralEMFraction = new TH1D("h_neutralEMFraction", "h_neutralEMFraction", 50, 0., 1.);
  m_h_neutralEMFraction->Sumw2();
  m_h_electronFraction = new TH1D("h_electronFraction", "h_electronFraction", 50, 0., 1.);
  m_h_electronFraction->Sumw2();
  m_h_muonFraction = new TH1D("h_muonFraction", "h_muonFraction", 50, 0., 1.);
  m_h_muonFraction->Sumw2();
  m_h_nConstituents = new TH1D("h_nConstituents", "h_nConstituents", 250, -0.5, 249.5);
  m_h_nConstituents->Sumw2();
  m_h_nCharged = new TH1D("h_nCharged", "h_nCharged", 250, -0.5, 249.5);
  m_h_nCharged->Sumw2();
}

void JetQuantitiesHistogramConsumer::ProcessFilteredEvent(event_type const& event,
                                                          product_type const& product,
                                                          setting_type const& settings) {
  // Event Weight
  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  // NPV Distribution
  m_h_nPV->Fill(event.m_vertexSummary->nVertices, eventWeight);
  m_h_hltPosition->Fill(product.m_selectedHltPosition);

  // MET over SumET
  m_h_METSumEtRatio->Fill(event.m_met->p4.Pt() / event.m_met->sumEt, eventWeight);

  // 1+ jet quantities
  if (product.m_validRecoJets.size() > 0) {
    m_h_jet1pt->Fill(product.m_jet1Pt, eventWeight);
    m_h_jet1rap->Fill(product.m_jet1Rap, eventWeight);
    m_h_jet1eta->Fill(product.m_jet1Eta, eventWeight);
    m_h_jet1phi->Fill(product.m_jet1Phi, eventWeight);
  }
  // 2+ jet quantities
  if (product.m_validRecoJets.size() > 1) {
    m_h_jet2pt->Fill(product.m_jet2Pt, eventWeight);
    m_h_jet2rap->Fill(product.m_jet2Rap, eventWeight);
    m_h_jet2eta->Fill(product.m_jet2Eta, eventWeight);
    m_h_jet2phi->Fill(product.m_jet2Phi, eventWeight);

    m_h_ptavg->Fill(product.m_dijet_ptavg, eventWeight);
    m_h_idx->Fill(product.m_dijet_idx, eventWeight);


    m_h_dijet_deltaphi->Fill(product.m_dijet_deltaPhi, eventWeight);
    m_h_dijet_deltar->Fill(product.m_dijet_deltaR, eventWeight);
    m_h_dijet_costhetastar->Fill(product.m_dijet_cosThetaStar, eventWeight);
    m_h_dijet_ystar->Fill(product.m_dijet_ystarnoabs, eventWeight);
    m_h_dijet_yboost->Fill(product.m_dijet_yboostnoabs, eventWeight);
    m_h_dijet_chi->Fill(product.m_dijet_chi, eventWeight);
    m_h_dijet_mass->Fill(product.m_dijet_mass, eventWeight);

    m_h2_yb_ys->Fill(product.m_dijet_yboost, product.m_dijet_ystar, eventWeight);
    m_h2_y12->Fill(product.m_jet1Rap, product.m_jet2Rap, eventWeight);

    m_h_dijet_deltaphi->Fill(product.m_dijet_deltaPhi, eventWeight);
    m_h2_jet12PtRVsPtavg->Fill(product.m_dijet_ptavg, product.m_dijet_jet12PtRatio, eventWeight);
    m_h2_jet1PtVsPtavg->Fill(product.m_dijet_ptavg, product.m_jet1Pt, eventWeight);
    m_h2_jet12Pt->Fill(product.m_jet1Pt, product.m_jet2Pt, eventWeight);

    // Fill double with inverted jet 1<->2
    m_h3_jet12rap->Fill(boost::math::sign(product.m_jet1Rap) * product.m_jet2Rap,
                        std::abs(product.m_jet1Rap),
                        product.m_jet1Pt,
                        eventWeight);
    m_h3_jet12rap->Fill(boost::math::sign(product.m_jet2Rap) * product.m_jet1Rap,
                        std::abs(product.m_jet2Rap),
                        product.m_jet2Pt,
                        eventWeight);

    m_h3_ptavg_yio->Fill(boost::math::sign(product.m_dijet_yinner) * product.m_dijet_youter,
                         std::abs(product.m_dijet_yinner),
                         product.m_dijet_ptavg,
                         eventWeight);
    m_h3_ptavg_ysb->Fill(product.m_dijet_yboost, product.m_dijet_ystar, product.m_dijet_ptavg, eventWeight);
  }

  for (auto& jet : product.m_validRecoJets) {
    m_h_incjetpt->Fill(jet.p4.Pt(), eventWeight);
    m_h_neutralHadronFraction->Fill(jet.neutralHadronFraction + jet.hfHadronFraction, eventWeight);
    m_h_chargedHadronFraction->Fill(jet.chargedHadronFraction, eventWeight);
    m_h_neutralEMFraction->Fill(jet.photonFraction + jet.hfEMFraction, eventWeight);
    m_h_electronFraction->Fill(jet.electronFraction, eventWeight);
    m_h_muonFraction->Fill(jet.muonFraction, eventWeight);
    m_h_nConstituents->Fill(jet.nConstituents, eventWeight);
    m_h_nCharged->Fill(jet.nCharged, eventWeight);
  }

  if (product.m_dijet_ptavg > myDijet_ptavg) {
    myRun = event.m_eventInfo->nRun;
    myLumi = event.m_eventInfo->nLumi;
    myEvent = event.m_eventInfo->nEvent;
    myDijet_ptavg = product.m_dijet_ptavg;
  }


}

void JetQuantitiesHistogramConsumer::Finish(setting_type const& settings) {
  // Save histograms

  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

  m_h_nPV->Write();
  m_h_METSumEtRatio->Write();
  m_h_hltPosition->Write();

  m_h_jet1pt->Write();
  m_h_jet1rap->Write();
  m_h_jet1eta->Write();
  m_h_jet1phi->Write();
  m_h_jet2pt->Write();
  m_h_jet2rap->Write();
  m_h_jet2eta->Write();
  m_h_jet2phi->Write();
  m_h_incjetpt->Write();

  m_h_ptavg->Write();
  m_h_idx->Write();

  m_h_dijet_deltaphi->Write();
  m_h_dijet_deltar->Write();
  m_h_dijet_costhetastar->Write();
  m_h_dijet_ystar->Write();
  m_h_dijet_yboost->Write();
  m_h_dijet_chi->Write();
  m_h_dijet_mass->Write();

  m_h2_jet12Pt->Write();
  m_h2_jet12PtRVsPtavg->Write();
  m_h2_jet1PtVsPtavg->Write();

  m_h2_yb_ys->Write();
  m_h2_y12->Write();

  m_h3_jet12rap->Write();
  m_h3_ptavg_yio->Write();
  m_h3_ptavg_ysb->Write();

  // Also need pt slices...
  m_h3_ptavg_ysb->GetZaxis()->SetRange(1, m_h3_ptavg_ysb->GetZaxis()->GetNbins());
  for (int i = 1; i < m_h3_ptavg_ysb->GetNbinsX() + 1; i++) {
    for (int j = 1; j < m_h3_ptavg_ysb->GetNbinsY() + 1; j++) {
      m_h3_ptavg_ysb->GetXaxis()->SetRange(i, i);
      m_h3_ptavg_ysb->GetYaxis()->SetRange(j, j);
      TH1D* m_h_ptavg_ysb = (TH1D*)m_h3_ptavg_ysb->Project3D("z");
      std::stringstream ss;
      ss << m_h3_ptavg_ysb->GetName() << "_ys_" << m_h3_ptavg_ysb->GetXaxis()->GetBinLowEdge(i) << "_"
         << m_h3_ptavg_ysb->GetXaxis()->GetBinUpEdge(i) << "_yb_" << m_h3_ptavg_ysb->GetYaxis()->GetBinLowEdge(j) << "_"
         << m_h3_ptavg_ysb->GetYaxis()->GetBinUpEdge(j);
      m_h_ptavg_ysb->SetName(ss.str().c_str());
      m_h_ptavg_ysb->Write(m_h_ptavg_ysb->GetName());
    }
  }

  // Jet property distributions
  m_h_neutralHadronFraction->Write();
  m_h_chargedHadronFraction->Write();
  m_h_neutralEMFraction->Write();
  m_h_electronFraction->Write();
  m_h_muonFraction->Write();
  m_h_nConstituents->Write();
  m_h_nCharged->Write();

  // LOG(INFO) << "My favourite dijet event is: " << settings.GetRootFileFolder() << " ptavg=" << myDijet_ptavg << " run=" << myRun << " lumi=" << myLumi << " event=" << myEvent << std::endl;
}

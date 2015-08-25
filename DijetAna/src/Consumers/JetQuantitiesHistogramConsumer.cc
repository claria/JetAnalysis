#include "JetAnalysis/DijetAna/interface/Consumers/JetQuantitiesHistogramConsumer.h"
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
  m_h_jet1rap = new TH1D("h_jet1rap", "h_jet1rap", 36, -3.0, 3.0);
  m_h_jet1rap->Sumw2();
  m_h_jet1phi = new TH1D("h_jet1phi", "h_jet1phi", 36, -3.2, 3.2);
  m_h_jet1phi->Sumw2();

  // Second Jet histograms
  m_h_jet2pt = new TH1D("h_jet2pt", "h_jet2pt", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h_jet2pt->Sumw2();
  m_h_jet2rap = new TH1D("h_jet2rap", "h_jet2rap", 36, -3.0, 3.0);
  m_h_jet2rap->Sumw2();
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
  // Jet12 Pt
  // m_h_pt12 = new TH1D("h_pt12", "h_jet12pt", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  // m_h_pt12->Sumw2();


  // 2D histograms
  m_h2_yb_ys = new TH2D("h2_yb_ys", "h2_yb_ys", settings.GetRapidityAbsBinning().size() - 1,
                           &settings.GetRapidityAbsBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
                           &settings.GetRapidityAbsBinning()[0]);
  m_h2_yb_ys->Sumw2();
  // Dijet rapidity distribution for two leading jets
  m_h2_y12 = new TH2D("h2_y12", "h2_y12", settings.GetFineRapidityBinning().size() -1,
                           &settings.GetFineRapidityBinning()[0], settings.GetFineRapidityBinning().size() - 1,
                           &settings.GetFineRapidityBinning()[0]);
  m_h2_y12->Sumw2();



  // Triple differential histogram
  m_h3_jet12rap = new TH3D("h3_jet12rap", "h3_jet12rap", settings.GetRapidityBinning().size() - 1,
                           &settings.GetRapidityBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
                           &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffPtBinning().size() - 1,
                           &settings.GetTripleDiffPtBinning()[0]);
  m_h3_jet12rap->Sumw2();

  m_h3_ptavg_yio = new TH3D("h3_ptavg_yio", "h3_ptavg_yio", settings.GetRapidityBinning().size() - 1,
                            &settings.GetRapidityBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffPtBinning().size() - 1,
                            &settings.GetTripleDiffPtBinning()[0]);
  m_h3_ptavg_yio->Sumw2();

  m_h3_ptavg_ysb = new TH3D("h3_ptavg_ysb", "h3_ptavg_ysb", settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffPtBinning().size() - 1,
                            &settings.GetTripleDiffPtBinning()[0]);
  m_h3_ptavg_ysb->Sumw2();

  m_h_neutralHadronFraction = new TH1D("h_neutralHadronFraction", "h_neutralHadronFraction", 50, 0., 2.);
  m_h_chargedHadronFraction = new TH1D("h_chargedHadronFraction", "h_chargedHadronFraction", 50, 0., 1.);
  m_h_photonFraction = new TH1D("h_photonFraction", "h_photonFraction", 50, 0., 2.);
  m_h_electronFraction = new TH1D("h_electronFraction", "h_electronFraction", 50, 0., 1.);
  m_h_muonFraction = new TH1D("h_muonFraction", "h_muonFraction", 50, 0., 1.);
  m_h_hfHadronFraction = new TH1D("h_hfHadronFraction", "h_hfHadronFraction", 50, 0., 1.);
  m_h_hfEMFraction = new TH1D("h_hfEMFraction", "h_hfEMFraction", 50, 0., 1.);
  m_h_nConstituents = new TH1D("h_nConstituents", "h_nConstituents", 250, -0.5, 249.5);
  m_h_nCharged = new TH1D("h_nCharged", "h_nCharged", 250, -0.5, 249.5);
}

void JetQuantitiesHistogramConsumer::ProcessFilteredEvent(event_type const& event, product_type const& product,
                                                          setting_type const& settings) {
  // Event Weight
  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  // NPV Distribution
  m_h_nPV->Fill(event.m_vertexSummary->nVertices);
  m_h_hltPosition->Fill(product.m_selectedHltPosition);

  // MET over SumET
  m_h_METSumEtRatio->Fill(event.m_met->p4.Pt() / event.m_met->sumEt, eventWeight);

  // 1+ jet quantities
  if (product.m_validJets.size() > 0) {
    m_h_jet1pt->Fill(product.m_jet1Pt, eventWeight);
    m_h_jet1rap->Fill(product.m_jet1Rap, eventWeight);
    m_h_jet1phi->Fill(product.m_jet1Phi, eventWeight);
  }
  // 2+ jet quantities
  if (product.m_validJets.size() > 1) {
    m_h_jet2pt->Fill(product.m_jet2Pt, eventWeight);
    m_h_jet2rap->Fill(product.m_jet2Rap, eventWeight);
    m_h_jet2phi->Fill(product.m_jet2Phi, eventWeight);

    m_h_ptavg->Fill(product.m_dijet_ptavg, eventWeight);

    m_h2_yb_ys->Fill(product.m_dijet_yboost, product.m_dijet_ystar, eventWeight);
    m_h2_y12->Fill(product.m_jet1Rap, product.m_jet2Rap, eventWeight);

    // Fill double with inverted jet 1<->2
    m_h3_jet12rap->Fill(boost::math::sign(product.m_jet1Rap) * product.m_jet2Rap, std::abs(product.m_jet1Rap),
                        product.m_jet1Pt, eventWeight);
    m_h3_jet12rap->Fill(boost::math::sign(product.m_jet2Rap) * product.m_jet1Rap, std::abs(product.m_jet2Rap),
                        product.m_jet2Pt, eventWeight);

    m_h3_ptavg_yio->Fill(boost::math::sign(product.m_dijet_yinner) * product.m_dijet_youter,
                         std::abs(product.m_dijet_yinner), product.m_dijet_ptavg, eventWeight);
    m_h3_ptavg_ysb->Fill(product.m_dijet_yboost, product.m_dijet_ystar, product.m_dijet_ptavg, eventWeight);
  }

  for (auto jet = product.m_validJets.begin(); jet != product.m_validJets.end(); jet++) {
    m_h_incjetpt->Fill((*jet)->p4.Pt(), eventWeight);

    m_h_neutralHadronFraction->Fill((*jet)->neutralHadronFraction + (*jet)->hfHadronFraction, eventWeight);
    m_h_chargedHadronFraction->Fill((*jet)->chargedHadronFraction, eventWeight);
    m_h_photonFraction->Fill((*jet)->photonFraction + (*jet)->hfEMFraction, eventWeight);
    m_h_electronFraction->Fill((*jet)->electronFraction, eventWeight);
    m_h_hfHadronFraction->Fill((*jet)->hfHadronFraction, eventWeight);
    m_h_hfEMFraction->Fill((*jet)->hfEMFraction, eventWeight);
    m_h_muonFraction->Fill((*jet)->muonFraction, eventWeight);
    m_h_nConstituents->Fill((*jet)->nConstituents, eventWeight);
    m_h_nCharged->Fill((*jet)->nCharged, eventWeight);
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
  m_h_jet1phi->Write();
  m_h_jet2pt->Write();
  m_h_jet2rap->Write();
  m_h_jet2phi->Write();
  m_h_incjetpt->Write();

  m_h_ptavg->Write();

  m_h2_yb_ys->Write();
  m_h2_y12->Write();

  m_h3_jet12rap->Write(m_h3_jet12rap->GetName());
  m_h3_ptavg_yio->Write(m_h3_ptavg_yio->GetName());
  m_h3_ptavg_ysb->Write(m_h3_ptavg_ysb->GetName());

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
  m_h_neutralHadronFraction->Write(m_h_neutralHadronFraction->GetName());
  m_h_chargedHadronFraction->Write(m_h_chargedHadronFraction->GetName());
  m_h_photonFraction->Write(m_h_photonFraction->GetName());
  m_h_electronFraction->Write(m_h_electronFraction->GetName());
  m_h_hfHadronFraction->Write(m_h_hfHadronFraction->GetName());
  m_h_hfEMFraction->Write(m_h_hfEMFraction->GetName());
  m_h_muonFraction->Write(m_h_muonFraction->GetName());
  m_h_nConstituents->Write(m_h_nConstituents->GetName());
  m_h_nCharged->Write(m_h_nCharged->GetName());
}

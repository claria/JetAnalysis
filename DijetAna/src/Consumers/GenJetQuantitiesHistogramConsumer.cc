#include "JetAnalysis/DijetAna/interface/Consumers/GenJetQuantitiesHistogramConsumer.h"

void GenJetQuantitiesHistogramConsumer::Init(setting_type const& settings) {
  ConsumerBase<JetTypes>::Init(settings);
  // Jet Quantity histograms
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

  // Leading Jet histogram
  m_h_genjet1pt =
      new TH1D("h_genjet1pt", "h_genjet1pt", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h_genjet1pt->Sumw2();
  m_h_genjet1rap = new TH1D("h_genjet1rap", "h_genjet1rap", 36, -3.0, 3.0);
  m_h_genjet1rap->Sumw2();
  m_h_genjet1phi = new TH1D("h_genjet1phi", "h_genjet1phi", 36, -3.2, 3.2);
  m_h_genjet1phi->Sumw2();

  // Second genjet histograms
  m_h_genjet2pt =
      new TH1D("h_genjet2pt", "h_genjet2pt", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h_genjet2pt->Sumw2();
  m_h_genjet2rap = new TH1D("h_genjet2rap", "h_genjet2rap", 36, -3.0, 3.0);
  m_h_genjet2rap->Sumw2();
  m_h_genjet2phi = new TH1D("h_genjet2phi", "h_genjet2phi", 36, -3.2, 3.2);
  m_h_genjet2phi->Sumw2();

  // Inclusive Jets
  m_h_incgenjetpt =
      new TH1D("h_incgenjetpt", "h_incgenjetpt", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h_incgenjetpt->Sumw2();

  // Gen Pt Avg
  m_h_genptavg = new TH1D("h_genptavg", "h_genptavg", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h_genptavg->Sumw2();

  // Jet flavours vs. ptavg
  m_h_dijet_flavour_qq = new TH1D("h_dijet_flavour_qq", "h_dijet_flavour_qq", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h_dijet_flavour_qq->Sumw2();
  m_h_dijet_flavour_qg = new TH1D("h_dijet_flavour_qg", "h_dijet_flavour_qg", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h_dijet_flavour_qg->Sumw2();
  m_h_dijet_flavour_gg = new TH1D("h_dijet_flavour_gg", "h_dijet_flavour_gg", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h_dijet_flavour_gg->Sumw2();


  m_h_genjet12dphi = new TH1D("h_genjet12dphi", "h_genjet12dphi", 63, 0, 6.3);
  m_h_genjet12dphi->Sumw2();
  // Jet12 Pt
  m_h2_genjet12PtRVsPtavg = new TH2D("h2_genjet12ptrvsptavg", "h2_genjet12ptrvsptavg",
                        settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0],
                        50, 0.0, 1.0);
  m_h2_genjet12PtRVsPtavg->Sumw2();


  // Triple differential histogram
  m_h_genjet12rap = new TH2D("h_genjet12rap",
                             "h_genjet12rap",
                             settings.GetRapidityBinning().size() - 1,
                             &settings.GetRapidityBinning()[0],
                             settings.GetRapidityBinning().size() - 1,
                             &settings.GetRapidityBinning()[0]);
  m_h_genjet12rap->Sumw2();

  m_h2_gen_yb_ys = new TH2D("h2_gen_yb_ys",
                            "h2_gen_yb_ys",
                            settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0],
                            settings.GetRapidityAbsBinning().size() - 1,
                            &settings.GetRapidityAbsBinning()[0]);
  m_h2_gen_yb_ys->Sumw2();

  m_h_jet1DeltaR = new TH1D("h_jet1DeltaR", "h_jet1DeltaR", 100, 0.0, 1.0);
  m_h_jet1DeltaR->Sumw2();
  m_h_jet2DeltaR = new TH1D("h_jet2DeltaR", "h_jet2DeltaR", 100, 0.0, 1.0);
  m_h_jet2DeltaR->Sumw2();

  m_h2_GenVsRecoPt = new TH2D("h2_GenVsRecoPt",
                              "h2_GenVsRecoPt",
                              50,
                              0.5,
                              1.5,
                              settings.GetGenPtBinning().size() - 1,
                              &settings.GetGenPtBinning()[0]);
  m_h2_GenVsRecoPt->Sumw2();

  m_h2_GenVsRecoPtAvg = new TH2D("h2_GenVsRecoPtAvg",
                                 "h2_GenVsRecoPtAvg",
                                 50,
                                 0.5,
                                 1.5,
                                 settings.GetGenPtBinning().size() - 1,
                                 &settings.GetGenPtBinning()[0]);
  m_h2_GenVsRecoPtAvg->Sumw2();

  m_h2_GenVsRecoYboost = new TH2D("h2_GenVsRecoYboost",
                                 "h2_GenVsRecoYboost",
                                 50,
                                 0.5,
                                 1.5,
                                 settings.GetGenPtBinning().size() - 1,
                                 &settings.GetGenPtBinning()[0]);
  m_h2_GenVsRecoYboost->Sumw2();

  m_h2_GenVsRecoYstar = new TH2D("h2_GenVsRecoYstar",
                                 "h2_GenVsRecoYstar",
                                 50,
                                 0.5,
                                 1.5,
                                 settings.GetGenPtBinning().size() - 1,
                                 &settings.GetGenPtBinning()[0]);
  m_h2_GenVsRecoYstar->Sumw2();

  m_h2_genreco_ptavg = new TH2D("h2_genreco_ptavg",
                                "h2_genreco_ptavg",
                                settings.GetPtBinning().size() - 1,
                                &settings.GetPtBinning()[0],
                                settings.GetGenPtBinning().size() - 1,
                                &settings.GetGenPtBinning()[0]);
  m_h2_genreco_ptavg->Sumw2();

  // Triple-differential distribution of y1, y2 and genjet pT
  m_h3_genptavg_ysb = new TH3D("h3_genptavg_ysb",
                               "h3_genptavg_ysb",
                               settings.GetRapidityAbsBinning().size() - 1,
                               &settings.GetRapidityAbsBinning()[0],
                               settings.GetRapidityAbsBinning().size() - 1,
                               &settings.GetRapidityAbsBinning()[0],
                               settings.GetTripleDiffGenPtBinning().size() - 1,
                               &settings.GetTripleDiffGenPtBinning()[0]);
  m_h3_genptavg_ysb->Sumw2();

  m_h3_genptavg_yio = new TH3D("h3_genptavg_yio",
                               "h3_genptavg_yio",
                               settings.GetRapidityBinning().size() - 1,
                               &settings.GetRapidityBinning()[0],
                               settings.GetRapidityAbsBinning().size() - 1,
                               &settings.GetRapidityAbsBinning()[0],
                               settings.GetTripleDiffGenPtBinning().size() - 1,
                               &settings.GetTripleDiffGenPtBinning()[0]);
  m_h3_genptavg_yio->Sumw2();

  m_h3_genjet12rap = new TH3D("h3_genjet12rap",
                              "h3_genjet12rap",
                              settings.GetRapidityBinning().size() - 1,
                              &settings.GetRapidityBinning()[0],
                              settings.GetRapidityAbsBinning().size() - 1,
                              &settings.GetRapidityAbsBinning()[0],
                              settings.GetTripleDiffGenPtBinning().size() - 1,
                              &settings.GetTripleDiffGenPtBinning()[0]);
  m_h3_genjet12rap->Sumw2();
}

void GenJetQuantitiesHistogramConsumer::ProcessFilteredEvent(event_type const& event,
                                                             product_type const& product,
                                                             setting_type const& settings) {
  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  if (product.m_validGenJets.size() > 0) {
    m_h_genjet1pt->Fill(product.m_genjet1Pt, eventWeight);
    m_h_genjet1rap->Fill(product.m_genjet1Rap, eventWeight);
    m_h_genjet1phi->Fill(product.m_genjet1Phi, eventWeight);

    if ((product.m_matchedRecoJets.size() > 0) && (product.m_matchedRecoJets.at(0) != NULL)) {
      m_h2_GenVsRecoPt->Fill(product.m_matchedRecoJets.at(0)->p4.Pt() / product.m_validGenJets.at(0).p4.Pt(),
                             product.m_validGenJets.at(0).p4.Pt(),
                             eventWeight);
      m_h_jet1DeltaR->Fill(
          ROOT::Math::VectorUtil::DeltaR(product.m_matchedRecoJets.at(0)->p4, product.m_validGenJets.at(0).p4),
          eventWeight);
    }
  }

  if (product.m_validGenJets.size() > 1) {
    m_h_genptavg->Fill(product.m_gendijet_ptavg, eventWeight);

    m_h_genjet2pt->Fill(product.m_validGenJets.at(1).p4.Pt(), eventWeight);
    m_h_genjet2rap->Fill(product.m_validGenJets.at(1).p4.Rapidity(), eventWeight);
    m_h_genjet2phi->Fill(product.m_validGenJets.at(1).p4.Phi(), eventWeight);

    m_h2_gen_yb_ys->Fill(product.m_gendijet_yboost, product.m_gendijet_ystar, eventWeight);

    m_h_genjet12dphi->Fill(product.m_gendijet_deltaPhi, eventWeight);
    m_h2_genjet12PtRVsPtavg->Fill(product.m_gendijet_ptavg, product.m_gendijet_jet12PtRatio, eventWeight);

    m_h_genjet12rap->Fill(
        product.m_validGenJets.at(0).p4.Rapidity(), product.m_validGenJets.at(1).p4.Rapidity(), eventWeight);
    m_h3_genjet12rap->Fill(
        boost::math::sign(product.m_validGenJets.at(0).p4.Rapidity()) * product.m_validGenJets.at(1).p4.Rapidity(),
        std::abs(product.m_validGenJets.at(0).p4.Rapidity()),
        product.m_validGenJets.at(0).p4.Pt(),
        eventWeight);
    m_h3_genjet12rap->Fill(
        boost::math::sign(product.m_validGenJets.at(1).p4.Rapidity()) * product.m_validGenJets.at(0).p4.Rapidity(),
        std::abs(product.m_validGenJets.at(1).p4.Rapidity()),
        product.m_validGenJets.at(1).p4.Pt(),
        eventWeight);

    m_h3_genptavg_ysb->Fill(product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg, eventWeight);

    m_h3_genptavg_yio->Fill(boost::math::sign(product.m_gendijet_yinner) * product.m_gendijet_youter,
                            std::abs(product.m_gendijet_yinner),
                            product.m_gendijet_ptavg,
                            eventWeight);

    if (product.m_matchedRecoJets.size() > 1 &&product.m_matchedRecoJets.at(0) != NULL && product.m_matchedRecoJets.at(1) != NULL) {
      double genmatch_ptavg = 0.5 * (product.m_matchedRecoJets.at(0)->p4.Pt() + product.m_matchedRecoJets.at(1)->p4.Pt());
      double genmatch_yboost = 0.5 * std::abs(product.m_matchedRecoJets.at(0)->p4.Rapidity() + product.m_matchedRecoJets.at(1)->p4.Rapidity());
      double genmatch_ystar = 0.5 * std::abs(product.m_matchedRecoJets.at(0)->p4.Rapidity() - product.m_matchedRecoJets.at(1)->p4.Rapidity());
      m_h2_GenVsRecoPtAvg->Fill(genmatch_ptavg / product.m_gendijet_ptavg, product.m_gendijet_ptavg, eventWeight);
      m_h2_GenVsRecoYboost->Fill(genmatch_yboost / product.m_gendijet_yboost, product.m_gendijet_ptavg, eventWeight);
      m_h2_GenVsRecoYstar->Fill(genmatch_ystar / product.m_gendijet_ystar, product.m_gendijet_ptavg, eventWeight);
      m_h2_genreco_ptavg->Fill(genmatch_ptavg, product.m_gendijet_ptavg, eventWeight);
    }
    if (product.m_matchedRecoJets.size() > 1 && product.m_matchedRecoJets.at(1) != NULL) {
      m_h_jet2DeltaR->Fill(
          ROOT::Math::VectorUtil::DeltaR(product.m_matchedRecoJets.at(1)->p4, product.m_validGenJets.at(1).p4),
          eventWeight);
    }
    // Dijet flavour
    if (product.m_matchedPartons.size() > 1) 
    {
      if (product.m_matchedPartons.at(0) != nullptr && product.m_matchedPartons.at(1) != nullptr) {
        // gg
        if ((product.m_matchedPartons.at(0)->pdgId() == 21) && (product.m_matchedPartons.at(1)->pdgId() == 21))
        {
          m_h_dijet_flavour_gg->Fill(product.m_dijet_ptavg, eventWeight);
        }
        // qq
        else if ((std::abs(product.m_matchedPartons.at(0)->pdgId()) < 6) 
            && (std::abs(product.m_matchedPartons.at(1)->pdgId()) < 6))
        {
          m_h_dijet_flavour_qq->Fill(product.m_dijet_ptavg, eventWeight);
        }
        // gq case
        else if (((std::abs(product.m_matchedPartons.at(0)->pdgId()) < 6) && (product.m_matchedPartons.at(1)->pdgId() == 21))
            || ((std::abs(product.m_matchedPartons.at(1)->pdgId()) < 6) && (product.m_matchedPartons.at(0)->pdgId() == 21)))
        {
          m_h_dijet_flavour_qg->Fill(product.m_dijet_ptavg, eventWeight);
        }
      }
    }
  }

  for (auto & jet : product.m_validGenJets) {
    m_h_incgenjetpt->Fill(jet.p4.Pt(), eventWeight);
  }
}

void GenJetQuantitiesHistogramConsumer::Finish(setting_type const& settings) {
  // save histograms
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
  m_h_genjet12dphi->Write();
  m_h2_genjet12PtRVsPtavg->Write();
  m_h2_GenVsRecoPt->Write();
  m_h2_GenVsRecoPtAvg->Write();
  m_h2_GenVsRecoYboost->Write();
  m_h2_GenVsRecoYstar->Write();
  m_h2_genreco_ptavg->Write();
  m_h_genjet1pt->Write();
  m_h_genjet1rap->Write();
  m_h_genjet1phi->Write();
  m_h_genjet2pt->Write();
  m_h_genjet2rap->Write();
  m_h_genjet2phi->Write();
  m_h_incgenjetpt->Write();
  m_h_genjet12rap->Write();
  m_h_genptavg->Write();

  m_h_dijet_flavour_qq->Write();
  m_h_dijet_flavour_qg->Write();
  m_h_dijet_flavour_gg->Write();

  m_h2_gen_yb_ys->Write();
  m_h3_genjet12rap->Write();
  // Also write 2d slices for easier handling in root files
  // for (int i=1; i < m_h3_genjet12rap->GetNbinsZ() + 1; i++){
  //   m_h3_genjet12rap->GetZaxis()->SetRange(i,i);
  //   TH2D* m_h2_genjet12rap = (TH2D*)m_h3_genjet12rap->Project3D("yx");
  //   std::stringstream ss;
  //   ss << m_h3_genjet12rap->GetName() << "_" <<  m_h3_genjet12rap->GetZaxis()->GetBinLowEdge(i) << "_" <<
  //   m_h3_genjet12rap->GetZaxis()->GetBinUpEdge(i);
  //   m_h2_genjet12rap->SetName(ss.str().c_str());
  //   m_h2_genjet12rap->Write(m_h2_genjet12rap->GetName());
  // }

  m_h3_genptavg_ysb->Write(m_h3_genptavg_ysb->GetName());
  // Also write 2d slices for easier handling in root files
  // for (int i=1; i < m_h3_genptavg_ysb->GetNbinsZ() + 1; i++){
  //   m_h3_genptavg_ysb->GetZaxis()->SetRange(i,i);
  //   TH2D* m_h2_genptavg_ysb = (TH2D*)m_h3_genptavg_ysb->Project3D("yx");
  //   std::stringstream ss;
  //   ss << m_h3_genptavg_ysb->GetName() << "_" <<  m_h3_genptavg_ysb->GetZaxis()->GetBinLowEdge(i) << "_" <<
  //   m_h3_genptavg_ysb->GetZaxis()->GetBinUpEdge(i);
  //   m_h2_genptavg_ysb->SetName(ss.str().c_str());
  //   m_h2_genptavg_ysb->Write(m_h2_genptavg_ysb->GetName());
  // }

  // Also need pt slices...
  m_h3_genptavg_ysb->GetZaxis()->SetRange(1, m_h3_genptavg_ysb->GetZaxis()->GetNbins());
  for (int i = 1; i < m_h3_genptavg_ysb->GetNbinsX() + 1; i++) {
    for (int j = 1; j < m_h3_genptavg_ysb->GetNbinsY() + 1; j++) {
      m_h3_genptavg_ysb->GetXaxis()->SetRange(i, i);
      m_h3_genptavg_ysb->GetYaxis()->SetRange(j, j);
      TH1D* m_h_genptavg_ysb = (TH1D*)m_h3_genptavg_ysb->Project3D("z");
      std::stringstream ss;
      ss << m_h3_genptavg_ysb->GetName() << "_ys_" << m_h3_genptavg_ysb->GetXaxis()->GetBinLowEdge(i) << "_"
         << m_h3_genptavg_ysb->GetXaxis()->GetBinUpEdge(i) << "_yb_" << m_h3_genptavg_ysb->GetYaxis()->GetBinLowEdge(j)
         << "_" << m_h3_genptavg_ysb->GetYaxis()->GetBinUpEdge(j);
      m_h_genptavg_ysb->SetName(ss.str().c_str());
      m_h_genptavg_ysb->Write(m_h_genptavg_ysb->GetName());
    }
  }

  m_h3_genptavg_yio->Write();
  // Also write 2d slices for easier handling in root files
  // for (int i=1; i < m_h3_genptavg_yio->GetNbinsZ() + 1; i++){
  //   m_h3_genptavg_yio->GetZaxis()->SetRange(i,i);
  //   TH2D* m_h2_ptavg_yio = (TH2D*)m_h3_genptavg_yio->Project3D("yx");
  //   std::stringstream ss;
  //   ss << m_h3_genptavg_yio->GetName() << "_" <<  m_h3_genptavg_yio->GetZaxis()->GetBinLowEdge(i) << "_" <<
  //   m_h3_genptavg_yio->GetZaxis()->GetBinUpEdge(i);
  //   m_h2_ptavg_yio->SetName(ss.str().c_str());
  //   m_h2_ptavg_yio->Write(m_h2_ptavg_yio->GetName());
  // }

  m_h_jet1DeltaR->Write();
  m_h_jet2DeltaR->Write();
}

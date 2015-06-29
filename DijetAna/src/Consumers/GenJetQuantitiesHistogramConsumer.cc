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

  // Triple differential histogram
  m_h_genjet12rap = new TH2D("h_genjet12rap", "h_genjet12rap", settings.GetRapidityBinning().size() - 1,
                             &settings.GetRapidityBinning()[0], settings.GetRapidityBinning().size() - 1,
                             &settings.GetRapidityBinning()[0]);
  m_h_genjet12rap->Sumw2();

  m_h_jet1DeltaR = new TH1D("h_jet1DeltaR", "h_jet1DeltaR", 100, 0.0, 1.0);
  m_h_jet1DeltaR->Sumw2();
  m_h_jet2DeltaR = new TH1D("h_jet2DeltaR", "h_jet2DeltaR", 100, 0.0, 1.0);
  m_h_jet2DeltaR->Sumw2();

  m_h2GenVsRecoPt = new TH2D("h2GenVsRecoPt", "h2GenVsRecoPt", 50, 0.5, 1.5, settings.GetGenPtBinning().size() - 1,
                             &settings.GetGenPtBinning()[0]);
  m_h2GenVsRecoPt->Sumw2();

  m_h2GenVsRecoPtAvg = new TH2D("h2GenVsRecoPtAvg", "h2GenVsRecoPtAvg", 50, 0.5, 1.5,
                                settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h2GenVsRecoPtAvg->Sumw2();

  // Triple-differential distribution of y1, y2 and genjet pT
  m_h3_genptavg_ysb = new TH3D("h3_genptavg_ysb", "h3_genptavg_ysb", settings.GetRapidityAbsBinning().size() - 1,
                               &settings.GetRapidityAbsBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
                               &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffGenPtBinning().size() - 1,
                               &settings.GetTripleDiffGenPtBinning()[0]);
  m_h3_genptavg_ysb->Sumw2();

  m_h3_genjet12rap = new TH3D(
      "h3_genjet12rap", "h3_genjet12rap", settings.GetRapidityAbsBinning().size() - 1,
      &settings.GetRapidityAbsBinning()[0], settings.GetRapidityBinning().size() - 1, &settings.GetRapidityBinning()[0],
      settings.GetTripleDiffGenPtBinning().size() - 1, &settings.GetTripleDiffGenPtBinning()[0]);
  m_h3_genjet12rap->Sumw2();

}

// void GenJetQuantitiesHistogramConsumer::ProcessEvent(event_type const& event, product_type const& product,
//                                                      setting_type const& settings, FilterResult& result) {
//   // if (result.HasPassed()) {
//   double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;
//   // std::cout << "event" << std::endl;
//   // std::cout << "genJet size: " <<  product.m_validGenJets.size() << std::endl;
//   // 1+ jet quantities
//   if (product.m_validGenJets.size() > 0) {
//     m_h_genjet1pt->Fill(product.m_validGenJets.at(0)->p4.Pt(), eventWeight);
//     m_h_genjet1rap->Fill(product.m_validGenJets.at(0)->p4.Rapidity(), eventWeight);
//     m_h_genjet1phi->Fill(product.m_validGenJets.at(0)->p4.Phi(), eventWeight);
//   }
//   // 2+ jet quantities
//   if (product.m_validGenJets.size() > 1) {
//     m_h_genjet2pt->Fill(product.m_validGenJets.at(1)->p4.Pt(), eventWeight);
//     m_h_genjet2rap->Fill(product.m_validGenJets.at(1)->p4.Rapidity(), eventWeight);
//     m_h_genjet2phi->Fill(product.m_validGenJets.at(1)->p4.Phi(), eventWeight);
//
//     m_h_genjet12rap->Fill(product.m_validGenJets.at(0)->p4.Rapidity(), product.m_validGenJets.at(1)->p4.Rapidity(), eventWeight);
//     m_h3_genjet12rap->Fill(product.m_validGenJets.at(0)->p4.Rapidity(), product.m_validGenJets.at(1)->p4.Rapidity(),
//                            product.m_validGenJets.at(0)->p4.Pt(), eventWeight);
//     m_h3_genjet12rap->Fill(
//         std::abs(product.m_validGenJets.at(0)->p4.Rapidity()),
//         boost::math::sign(product.m_validGenJets.at(0)->p4.Rapidity()) * product.m_validGenJets.at(1)->p4.Rapidity(),
//         product.m_validGenJets.at(0)->p4.Pt(), eventWeight);
//
//     m_h3_genptavg_ysb->Fill(product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg, eventWeight);
//   }
//
//   for (auto jet = event.m_genJets->begin(); jet != event.m_genJets->end(); jet++) {
//     m_h_incgenjetpt->Fill((*jet)->p4.Pt(), eventWeight);
//   }
//   // }
// }

void GenJetQuantitiesHistogramConsumer::ProcessFilteredEvent(event_type const& event, product_type const& product,
                                                             setting_type const& settings) {

  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  if (product.m_validGenJets.size() > 0) {
    m_h_genjet1pt->Fill(product.m_genjet1Pt, eventWeight);
    m_h_genjet1rap->Fill(product.m_genjet1Rap, eventWeight);
    m_h_genjet1phi->Fill(product.m_genjet1Phi, eventWeight);

    if ((product.m_matchedRecoJets.size() > 0) && (product.m_matchedRecoJets.at(0) != NULL)) {
      m_h2GenVsRecoPt->Fill(product.m_matchedRecoJets.at(0)->p4.Pt() / product.m_validGenJets.at(0)->p4.Pt(),
                            product.m_validGenJets.at(0)->p4.Pt(), eventWeight);
      m_h_jet1DeltaR->Fill(
          ROOT::Math::VectorUtil::DeltaR(product.m_matchedRecoJets.at(0)->p4, product.m_validGenJets.at(0)->p4), eventWeight);
    }
  }
  if (product.m_validGenJets.size() > 1) {
    m_h_genjet2pt->Fill(product.m_validGenJets.at(1)->p4.Pt(), eventWeight);
    m_h_genjet2rap->Fill(product.m_validGenJets.at(1)->p4.Rapidity(), eventWeight);
    m_h_genjet2phi->Fill(product.m_validGenJets.at(1)->p4.Phi(), eventWeight);

    m_h_genjet12rap->Fill(product.m_validGenJets.at(0)->p4.Rapidity(), product.m_validGenJets.at(1)->p4.Rapidity(), eventWeight);
    m_h3_genjet12rap->Fill(product.m_validGenJets.at(0)->p4.Rapidity(), product.m_validGenJets.at(1)->p4.Rapidity(),
                           product.m_validGenJets.at(0)->p4.Pt(), eventWeight);
    m_h3_genjet12rap->Fill(
        std::abs(product.m_validGenJets.at(0)->p4.Rapidity()),
        boost::math::sign(product.m_validGenJets.at(0)->p4.Rapidity()) * product.m_validGenJets.at(1)->p4.Rapidity(),
        product.m_validGenJets.at(0)->p4.Pt(), eventWeight);

    m_h3_genptavg_ysb->Fill(product.m_gendijet_yboost, product.m_gendijet_ystar, product.m_gendijet_ptavg, eventWeight);

    if (product.m_matchedRecoJets.size() > 1 && product.m_matchedRecoJets.at(0) != NULL && product.m_matchedRecoJets.at(1) != NULL) {
      double ptavg = 0.5 * (product.m_matchedRecoJets.at(0)->p4.Pt() + product.m_matchedRecoJets.at(1)->p4.Pt());
      m_h2GenVsRecoPtAvg->Fill(ptavg / product.m_gendijet_ptavg, product.m_gendijet_ptavg, eventWeight);
    }
    if (product.m_matchedRecoJets.size() > 1 && product.m_matchedRecoJets.at(1) != NULL) {
      m_h_jet2DeltaR->Fill(
          ROOT::Math::VectorUtil::DeltaR(product.m_matchedRecoJets.at(1)->p4, product.m_validGenJets.at(1)->p4), eventWeight);
    }

  }
  for (auto jet = product.m_validGenJets.begin(); jet != product.m_validGenJets.end(); jet++) {
    m_h_incgenjetpt->Fill((*jet)->p4.Pt(), eventWeight);
  }

}

void GenJetQuantitiesHistogramConsumer::Finish(setting_type const& settings) {
  // save histograms
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
  m_h2GenVsRecoPt->Write(m_h2GenVsRecoPt->GetName());
  m_h2GenVsRecoPtAvg->Write(m_h2GenVsRecoPtAvg->GetName());
  m_h_genjet1pt->Write(m_h_genjet1pt->GetName());
  m_h_genjet1rap->Write(m_h_genjet1rap->GetName());
  m_h_genjet1phi->Write(m_h_genjet1phi->GetName());
  m_h_genjet2pt->Write(m_h_genjet2pt->GetName());
  m_h_genjet2rap->Write(m_h_genjet2rap->GetName());
  m_h_genjet2phi->Write(m_h_genjet2phi->GetName());
  m_h_incgenjetpt->Write(m_h_incgenjetpt->GetName());
  m_h_genjet12rap->Write(m_h_genjet12rap->GetName());
  // m_h3_genjet12rap->Scale(1.0, "width");
  // m_h3_genjet12rap->Scale(1.0, "width");
  m_h3_genjet12rap->Write(m_h3_genjet12rap->GetName());

  // m_h3_genptavg_ysb->Scale(1.0, "width");
  m_h3_genptavg_ysb->Write(m_h3_genptavg_ysb->GetName());

  m_h_jet1DeltaR->Write(m_h_jet1DeltaR->GetName());
  m_h_jet2DeltaR->Write(m_h_jet2DeltaR->GetName());
}

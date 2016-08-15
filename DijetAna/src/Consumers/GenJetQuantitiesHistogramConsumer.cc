#include "Artus/Utility/interface/SafeMap.h"
#include "JetAnalysis/DijetAna/interface/Consumers/GenJetQuantitiesHistogramConsumer.h"

std::vector<double> createArray(double min, double max, double step )
{
    std::vector<double> array;
    for(double i = min; i < max; i += step )
        array.push_back(i);
    return array;
}

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

  // Reco Pt Avg but only fillled if gen_yb=reco_yb and gen_ys==reco_ys
  m_h_pure_ptavg = new TH1D("h_pure_ptavg", "h_pure_ptavg", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h_pure_ptavg->Sumw2();

  m_h_genidx = new TH1D("h_genidx", "h_genidx", (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6);
  m_h_genidx->Sumw2();

  m_h2_genrecoidx = new TH2D("h2_genrecoidx", "h2_genrecoidx", (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6,
                                                               (settings.GetPtBinning().size() - 1)*6,-0.5, -0.5+ (settings.GetPtBinning().size() - 1)*6);
  m_h2_genrecoidx->Sumw2();
  m_h2_genrecoptavg = new TH2D("h2_genrecoptavg", "h2_genrecoptavg", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0],
                                                                     settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h2_genrecoptavg->Sumw2();



  // Gen Match Pt Avg
  m_h_genmatchptavg = new TH1D("h_genmatchptavg", "h_genmatchptavg", settings.GetGenPtBinning().size() - 1, &settings.GetGenPtBinning()[0]);
  m_h_genmatchptavg->Sumw2();

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


  m_tp_GenVsRecoPtAvg = new TProfile("tp_GenVsRecoPtAvg",
                                     "tp_GenVsRecoPtAvg",
                                     settings.GetGenPtBinning().size() - 1,
                                     &settings.GetGenPtBinning()[0],
                                     0.0,
                                     10.0);
  m_tp_GenVsRecoPtAvg->Sumw2();
  m_tp_GenPtAvg = new TProfile("tp_GenPtAvg",
                               "tp_GenPtAvg",
                               settings.GetGenPtBinning().size() - 1,
                               &settings.GetGenPtBinning()[0],
                               0.0,
                               999999.0);
  m_tp_GenPtAvg->Sumw2();
  m_tp_RecoPtAvg = new TProfile("tp_RecoPtAvg",
                                "tp_RecoPtAvg",
                                settings.GetGenPtBinning().size() - 1,
                                &settings.GetGenPtBinning()[0],
                                0.0,
                                999999.0);
  m_tp_RecoPtAvg->Sumw2();


  m_h2_GenVsRecoYboost = new TH2D("h2_GenVsRecoYboost",
                                 "h2_GenVsRecoYboost",
                                 50,
                                 0.5,
                                 1.5,
                                 settings.GetGenPtBinning().size() - 1,
                                 &settings.GetGenPtBinning()[0]);
  m_h2_GenVsRecoYboost->Sumw2();

  m_h2_GenVsRecoY = new TH2D("h2_GenVsRecoY",
                                 "h2_GenVsRecoY",
                                 100, -5.0, 5.0,
                                 200, -1.0,1.0
                                 );
  m_h2_GenVsRecoY->Sumw2();
  m_h2_GenVsRecoEta = new TH2D("h2_GenVsRecoEta",
                               "h2_GenVsRecoEta",
                               100, -5.0, 5.0,
                               200, -1.0,1.0
                               );
  m_h2_GenVsRecoEta->Sumw2();



  std::vector<double> yBinning = createArray(-5.0, 5.0, 0.1);
  std::vector<double> resBinning = createArray(-1.0, 1.0, 0.02);
  m_h3_GenVsRecoY = new TH3D("h3_GenVsRecoY",
                                 "h3_GenVsRecoY",
                                 yBinning.size() - 1,
                                 &yBinning[0],
                                 settings.GetGenPtBinning().size() - 1,
                                 &settings.GetGenPtBinning()[0],
                                 resBinning.size() - 1,
                                 &resBinning[0]
                                 );
  m_h3_GenVsRecoY->Sumw2();
  m_h3_GenVsRecoEta = new TH3D("h3_GenVsRecoEta",
                                 "h3_GenVsRecoEta",
                                 yBinning.size() - 1,
                                 &yBinning[0],
                                 settings.GetGenPtBinning().size() - 1,
                                 &settings.GetGenPtBinning()[0],
                                 resBinning.size() - 1,
                                 &resBinning[0]
                                 );
  m_h3_GenVsRecoEta->Sumw2();



  std::vector<double> tp_genybinning = createArray(-5.0, 5.0, 0.1);
  m_tp_genyvsrecoy = new TProfile("tp_GenYVsRecoY",
                                  "tp_GenYVsRecoY",
                                  tp_genybinning.size() - 1,
                                  &tp_genybinning[0]);


  m_h2_GenVsRecoYstar = new TH2D("h2_GenVsRecoYstar",
                                 "h2_GenVsRecoYstar",
                                 50,
                                 0.5,
                                 1.5,
                                 settings.GetGenPtBinning().size() - 1,
                                 &settings.GetGenPtBinning()[0]);
  m_h2_GenVsRecoYstar->Sumw2();

  m_h2_GenYstarVsRecoYstar = new TH2D("h2_GenYstarVsRecoYstar",
                                 "h2_GenYstarVsRecoYstar",
                               settings.GetRapidityAbsBinning().size() - 1,
                               &settings.GetRapidityAbsBinning()[0],
                               settings.GetRapidityAbsBinning().size() - 1,
                               &settings.GetRapidityAbsBinning()[0]);

  m_h2_GenYstarVsRecoYstar->Sumw2();
  m_h2_GenYboostVsRecoYboost = new TH2D("h2_GenYboostVsRecoYboost",
                                 "h2_GenYboostVsRecoYboost",
                               settings.GetRapidityAbsBinning().size() - 1,
                               &settings.GetRapidityAbsBinning()[0],
                               settings.GetRapidityAbsBinning().size() - 1,
                               &settings.GetRapidityAbsBinning()[0]);
  m_h2_GenYboostVsRecoYboost->Sumw2();


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

    // std::cout << "have a single jet. yay" << std::endl;
    // std::cout << "validGenJets size " << product.m_validGenJets.size() << std::endl;
    // std::cout << "product.m_matchedRecoJets size " << product.m_matchedRecoJets.size() << std::endl;

    // std::cout << "GenJets" << std::endl;
//     for (auto & elem : product.m_validGenJets)
//       std::cout << &elem << "\n";
//     std::cout << "RecoJets" << std::endl;
//     for (auto & elem : product.m_validRecoJets)
//       std::cout << &elem << "\n";
//
//     for(auto elem : product.m_matchedRecoJets)
//       std::cout << elem.first << " : " << elem.second << "\n";



    if (product.m_validGenJets.size() > 0 &&
      product.m_matchedRecoJets.count(&product.m_validGenJets.at(0)) && product.m_matchedRecoJets.find(&product.m_validGenJets.at(0))->second)
    {
      // std::cout << "genreco deltar " << ROOT::Math::VectorUtil::DeltaR(product.m_matchedRecoJets.at(&product.m_validGenJets.at(0))->p4, product.m_validGenJets.at(0).p4) << std::endl;
      m_h2_GenVsRecoPt->Fill(product.m_genmatchjet1Pt / product.m_validGenJets.at(0).p4.Pt(),
                             product.m_validGenJets.at(0).p4.Pt(),
                             eventWeight);
      m_h_jet1DeltaR->Fill(
          ROOT::Math::VectorUtil::DeltaR(product.m_matchedRecoJets.at(&product.m_validGenJets.at(0))->p4, product.m_validGenJets.at(0).p4),
          eventWeight);
    }
  }

  if (product.m_validGenJets.size() > 1) {
    m_h_genptavg->Fill(product.m_gendijet_ptavg, eventWeight);
    m_h_genidx->Fill(product.m_gendijet_idx, eventWeight);
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

    if ( product.m_gendijet_ysbidx == product.m_dijet_ysbidx)
    {
      m_h_pure_ptavg->Fill(product.m_dijet_ptavg, eventWeight);
    }

    m_h2_GenYstarVsRecoYstar->Fill(product.m_dijet_ystar, product.m_gendijet_ystar, eventWeight);
    m_h2_GenYboostVsRecoYboost->Fill(product.m_dijet_yboost, product.m_gendijet_yboost, eventWeight);

    m_h2_genrecoidx->Fill(product.m_genmatchdijet_idx, product.m_gendijet_idx, eventWeight);
    m_h2_genrecoptavg->Fill(product.m_genmatchdijet_ptavg, product.m_gendijet_ptavg, eventWeight);

    m_h2_GenVsRecoPtAvg->Fill(product.m_dijet_ptavg / product.m_gendijet_ptavg, product.m_gendijet_ptavg, eventWeight);

    m_tp_GenVsRecoPtAvg->Fill(product.m_gendijet_ptavg, product.m_genmatchdijet_ptavg / product.m_gendijet_ptavg, eventWeight);
    m_tp_GenPtAvg->Fill(product.m_gendijet_ptavg, product.m_gendijet_ptavg, eventWeight);
    m_tp_RecoPtAvg->Fill(product.m_gendijet_ptavg, product.m_dijet_ptavg, eventWeight);

    m_h2_GenVsRecoYboost->Fill(product.m_genmatchdijet_yboost -product.m_gendijet_yboost, product.m_gendijet_ptavg, eventWeight);
    m_h2_GenVsRecoYstar->Fill(product.m_genmatchdijet_ystar - product.m_gendijet_ystar, product.m_gendijet_ptavg, eventWeight);

    m_h2_GenVsRecoY->Fill(product.m_validGenJets.at(0).p4.Rapidity(), product.m_genmatchjet1Rap - product.m_validGenJets.at(0).p4.Rapidity(), eventWeight);
    m_h2_GenVsRecoY->Fill(product.m_validGenJets.at(1).p4.Rapidity(), product.m_genmatchjet2Rap - product.m_validGenJets.at(1).p4.Rapidity(), eventWeight);

    m_h2_GenVsRecoEta->Fill(product.m_validGenJets.at(0).p4.Eta(), product.m_genmatchjet1Eta - product.m_validGenJets.at(0).p4.Eta(), eventWeight);
    m_h2_GenVsRecoEta->Fill(product.m_validGenJets.at(1).p4.Eta(), product.m_genmatchjet2Eta - product.m_validGenJets.at(1).p4.Eta(), eventWeight);

    m_h3_GenVsRecoY->Fill(product.m_validGenJets.at(0).p4.Rapidity(), product.m_validGenJets.at(0).p4.Pt(), product.m_genmatchjet1Rap - product.m_validGenJets.at(0).p4.Rapidity(), eventWeight);
    m_h3_GenVsRecoY->Fill(product.m_validGenJets.at(1).p4.Rapidity(), product.m_validGenJets.at(1).p4.Pt(), product.m_genmatchjet2Rap - product.m_validGenJets.at(1).p4.Rapidity(), eventWeight);
    m_h3_GenVsRecoEta->Fill(product.m_genmatchjet1Eta, product.m_genmatchjet1Pt, product.m_genmatchjet1Eta - product.m_validGenJets.at(0).p4.Eta(), eventWeight);
    m_h3_GenVsRecoEta->Fill(product.m_genmatchjet1Eta, product.m_genmatchjet2Pt, product.m_genmatchjet2Eta - product.m_validGenJets.at(1).p4.Eta(), eventWeight);

    m_tp_genyvsrecoy->Fill(product.m_validGenJets.at(0).p4.Rapidity(), product.m_genmatchjet1Rap - product.m_validGenJets.at(0).p4.Rapidity(), eventWeight);

    m_h2_genreco_ptavg->Fill(product.m_genmatchdijet_ptavg, product.m_gendijet_ptavg, eventWeight);
    m_h_genmatchptavg->Fill(product.m_genmatchdijet_ptavg, eventWeight);
  }
  if (product.m_validGenJets.size() > 1 &&
      SafeMap::GetWithDefault(product.m_matchedRecoJets, &product.m_validGenJets.at(1), static_cast<KBasicJet*>(nullptr)) != nullptr) {
    m_h_jet2DeltaR->Fill(
        ROOT::Math::VectorUtil::DeltaR(product.m_matchedRecoJets.at(&product.m_validGenJets.at(1))->p4, product.m_validGenJets.at(1).p4),
        eventWeight);
  }
  // Dijet flavour
  if (product.m_validRecoJets.size() > 1 &&
      SafeMap::GetWithDefault(product.m_matchedPartons, &product.m_validRecoJets.at(0), static_cast<KGenParticle*>(nullptr)) != nullptr && 
      SafeMap::GetWithDefault(product.m_matchedPartons, &product.m_validRecoJets.at(1), static_cast<KGenParticle*>(nullptr)) != nullptr) 
  {
    // gg
    if ((product.m_matchedPartons.at(&product.m_validRecoJets.at(0))->pdgId() == 21) && (product.m_matchedPartons.at(&product.m_validRecoJets.at(1))->pdgId() == 21))
    {
      m_h_dijet_flavour_gg->Fill(product.m_dijet_ptavg, eventWeight);
    }
    // qq
    else if ((std::abs(product.m_matchedPartons.at(&product.m_validRecoJets.at(0))->pdgId()) < 6) 
        && (std::abs(product.m_matchedPartons.at(&product.m_validRecoJets.at(1))->pdgId()) < 6))
    {
      m_h_dijet_flavour_qq->Fill(product.m_dijet_ptavg, eventWeight);
    }
    // gq case
    else if (((std::abs(product.m_matchedPartons.at(&product.m_validRecoJets.at(0))->pdgId()) < 6) && (product.m_matchedPartons.at(&product.m_validRecoJets.at(1))->pdgId() == 21))
        || ((std::abs(product.m_matchedPartons.at(&product.m_validRecoJets.at(1))->pdgId()) < 6) && (product.m_matchedPartons.at(&product.m_validRecoJets.at(0))->pdgId() == 21)))
    {
      m_h_dijet_flavour_qg->Fill(product.m_dijet_ptavg, eventWeight);
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
  m_tp_GenVsRecoPtAvg->Write();
  m_tp_GenPtAvg->Write();
  m_tp_RecoPtAvg->Write();
  m_h2_GenVsRecoYboost->Write();
  m_h2_GenVsRecoYstar->Write();
  m_h2_GenVsRecoY->Write();
  m_h3_GenVsRecoY->Write();
  m_h3_GenVsRecoEta->Write();
  m_h2_GenVsRecoEta->Write();

  m_tp_genyvsrecoy->Write();

  m_h_pure_ptavg->Write();

  m_h2_GenYstarVsRecoYstar->Write();
  m_h2_GenYboostVsRecoYboost->Write();
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
  m_h_genidx->Write();
  m_h2_genrecoidx->Write();
  m_h2_genrecoptavg->Write();
  m_h_genmatchptavg->Write();

  m_h_dijet_flavour_qq->Write();
  m_h_dijet_flavour_qg->Write();
  m_h_dijet_flavour_gg->Write();

  m_h2_gen_yb_ys->Write();
  m_h3_genjet12rap->Write();
  m_h3_genptavg_ysb->Write(m_h3_genptavg_ysb->GetName());
  m_h3_genptavg_yio->Write();

  m_h_jet1DeltaR->Write();
  m_h_jet2DeltaR->Write();
}

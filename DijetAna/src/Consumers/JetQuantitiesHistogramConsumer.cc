#include "JetAnalysis/DijetAna/interface/Consumers/JetQuantitiesHistogramConsumer.h"
// #include "Artus/Utility/interface/Utility.h"
#include <boost/math/special_functions/sign.hpp>

void JetQuantitiesHistogramConsumer::Init(setting_type const& settings) {
  ConsumerBase<JetTypes>::Init(settings);
  // Jet Quantity histograms
  // TODO: Move to config
  // double incjet_pt_binning[42] = {48, 56, 64, 74, 84, 97, 114, 133, 153, 174,
  // 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686,
  // 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588,
  // 1784, 2116, 2500, 3000};
  // double pt_binning[39] = {74, 84, 97, 114, 133, 153, 174, 196, 220, 245,
  // 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846,
  // 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500,
  // 3000};
  // double rap_binning[13] = {-3.0, -2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5,
  // 1.0, 1.5, 2.0, 2.5, 3.0};
  // double pt_binning2[7] = {74, 114, 196, 300, 468, 790, 3000};

  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

  m_h_nPV = new TH1D("h_nPV", "h_nPV", 61, -0.5, 60.5);

  // Leading Jet histograms
  m_h_jet1pt = new TH1D("h_jet1pt", "h_jet1pt", settings.GetPtBinning().size() - 1,
                        &settings.GetPtBinning()[0]);
  m_h_jet1pt->Sumw2();
  m_h_jet1rap = new TH1D("h_jet1rap", "h_jet1rap", 36, -3.0, 3.0);
  m_h_jet1rap->Sumw2();
  m_h_jet1phi = new TH1D("h_jet1phi", "h_jet1phi", 36, -3.2, 3.2);
  m_h_jet1phi->Sumw2();

  // Second Jet histograms
  m_h_jet2pt = new TH1D("h_jet2pt", "h_jet2pt", settings.GetPtBinning().size() - 1,
                        &settings.GetPtBinning()[0]);
  m_h_jet2pt->Sumw2();
  m_h_jet2rap = new TH1D("h_jet2rap", "h_jet2rap", 36, -3.0, 3.0);
  m_h_jet2rap->Sumw2();
  m_h_jet2phi = new TH1D("h_jet2phi", "h_jet2phi", 36, -3.2, 3.2);
  m_h_jet2phi->Sumw2();

  // Inclusive Jets
  m_h_incjetpt = new TH1D("h_incjetpt", "h_incjetpt", settings.GetPtBinning().size() - 1,
                          &settings.GetPtBinning()[0]);
  m_h_incjetpt->Sumw2();

  // Triple differential histogram
  m_h3_jet12rap =
      new TH3D("h3_jet12rap", "h3_jet12rap", settings.GetRapidityBinning().size() - 1,
               &settings.GetRapidityBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
               &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffPtBinning().size() - 1,
               &settings.GetTripleDiffPtBinning()[0]);
  m_h3_jet12rap->Sumw2();

  m_h3_ptavg_yio =
      new TH3D("h3_ptavg_yio", "h3_ptavg_yio", settings.GetRapidityBinning().size() - 1,
               &settings.GetRapidityBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
               &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffPtBinning().size() - 1,
               &settings.GetTripleDiffPtBinning()[0]);
  m_h3_ptavg_yio->Sumw2();

  m_h3_ptavg_ysb =
      new TH3D("h3_ptavg_ysb", "h3_ptavg_ysb", settings.GetRapidityAbsBinning().size() - 1,
               &settings.GetRapidityAbsBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
               &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffPtBinning().size() - 1,
               &settings.GetTripleDiffPtBinning()[0]);
  m_h3_ptavg_ysb->Sumw2();

  m_h3_ptavg_ysio =
      new TH3D("h3_ptavg_ysio", "h3_ptavg_ysio", settings.GetRapidityBinning().size() - 1,
               &settings.GetRapidityBinning()[0], settings.GetRapidityAbsBinning().size() - 1,
               &settings.GetRapidityAbsBinning()[0], settings.GetTripleDiffPtBinning().size() - 1,
               &settings.GetTripleDiffPtBinning()[0]);

  m_h3_ptavg_ysio->Sumw2();

  m_h_neutralHadronFraction =
      new TH1D("h_neutralHadronFraction", "h_neutralHadronFraction", 50, 0., 2.);
  m_h_chargedHadronFraction =
      new TH1D("h_chargedHadronFraction", "h_chargedHadronFraction", 50, 0., 1.);
  m_h_photonFraction = new TH1D("h_photonFraction", "h_photonFraction", 50, 0., 2.);
  m_h_electronFraction = new TH1D("h_electronFraction", "h_electronFraction", 50, 0., 1.);
  m_h_muonFraction = new TH1D("h_muonFraction", "h_muonFraction", 50, 0., 1.);
  m_h_hfHadronFraction = new TH1D("h_hfHadronFraction", "h_hfHadronFraction", 50, 0., 1.);
  m_h_hfEMFraction = new TH1D("h_hfEMFraction", "h_hfEMFraction", 50, 0., 1.);
  m_h_nConstituents = new TH1D("h_nConstituents", "h_nConstituents", 250, -0.5, 249.5);
  m_h_nCharged = new TH1D("h_nCharged", "h_nCharged", 250, -0.5, 249.5);
}

void JetQuantitiesHistogramConsumer::ProcessFilteredEvent(event_type const& event,
                                                          product_type const& product,
                                                          setting_type const& settings) {
  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  m_h_nPV->Fill(event.m_vertexSummary->nVertices);

  // 1+ jet quantities
  if (product.m_validJets.size() > 0) {
    // std::cout << "Leading jet pt:" << product.m_validJets.at(0)->p4.Pt() <<
    // std::endl;
    m_h_jet1pt->Fill(product.m_jet1Pt, eventWeight);
    m_h_jet1rap->Fill(product.m_jet1Rap, eventWeight);
    m_h_jet1phi->Fill(product.m_jet1Phi, eventWeight);
  }
  // 2+ jet quantities
  if (product.m_validJets.size() > 1) {
    m_h_jet2pt->Fill(product.m_jet2Pt, eventWeight);
    m_h_jet2rap->Fill(product.m_jet2Rap, eventWeight);
    m_h_jet2phi->Fill(product.m_jet2Phi, eventWeight);

    // Fill double with inverted jet 1<->2
    m_h3_jet12rap->Fill(boost::math::sign(product.m_jet1Rap) * product.m_jet2Rap,
                        std::abs(product.m_jet1Rap), product.m_jet1Pt, eventWeight);
    m_h3_jet12rap->Fill(boost::math::sign(product.m_jet1Rap) * product.m_jet2Rap,
                        std::abs(product.m_jet1Rap), product.m_jet1Pt, eventWeight);

    m_h3_ptavg_yio->Fill(boost::math::sign(product.m_dijet_yinner) * product.m_dijet_yinner,
                         std::abs(product.m_dijet_yinner), product.m_dijet_ptavg, eventWeight);
    m_h3_ptavg_ysb->Fill(std::abs(product.m_dijet_yboost), std::abs(product.m_dijet_ystar),
                         product.m_dijet_ptavg, eventWeight);
    m_h3_ptavg_ysio->Fill(product.m_dijet_yinner * boost::math::sign(product.m_dijet_youter),
                          std::abs(product.m_dijet_ystar), product.m_dijet_ptavg, eventWeight);
  }

  for (auto jet = product.m_validJets.begin(); jet != product.m_validJets.end(); jet++) {
    m_h_incjetpt->Fill((*jet)->p4.Pt(), eventWeight);

    m_h_neutralHadronFraction->Fill((*jet)->neutralHadronFraction + (*jet)->hfHadronFraction,
                                    eventWeight);
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
  // save histograms
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());

  m_h_nPV->Write(m_h_nPV->GetName());
  // hist->Scale(1.0 ,"width");
  m_h_jet1pt->Write(m_h_jet1pt->GetName());
  m_h_jet1rap->Write(m_h_jet1rap->GetName());
  m_h_jet1phi->Write(m_h_jet1phi->GetName());
  m_h_jet2pt->Write(m_h_jet2pt->GetName());
  m_h_jet2rap->Write(m_h_jet2rap->GetName());
  m_h_jet2phi->Write(m_h_jet2phi->GetName());
  m_h_incjetpt->Write(m_h_incjetpt->GetName());

  m_h3_jet12rap->Scale(0.5, "width");
  m_h3_jet12rap->Write(m_h3_jet12rap->GetName());

  m_h3_ptavg_yio->Scale(1.0, "width");
  m_h3_ptavg_yio->Write(m_h3_ptavg_yio->GetName());

  m_h3_ptavg_ysb->Scale(1.0, "width");
  m_h3_ptavg_ysb->Write(m_h3_ptavg_ysb->GetName());

  m_h3_ptavg_ysio->Scale(1.0, "width");
  m_h3_ptavg_ysio->Write(m_h3_ptavg_ysio->GetName());

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

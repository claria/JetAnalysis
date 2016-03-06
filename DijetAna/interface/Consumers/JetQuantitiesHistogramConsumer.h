#pragma once

#include "TH1.h"
#include "TH3.h"
#include "Artus/Utility/interface/RootFileHelper.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class JetQuantitiesHistogramConsumer : public JetConsumerBase {
 public:
  virtual std::string GetConsumerId() const { return "JetQuantitiesHistogramConsumer"; }
  virtual void Init(setting_type const& settings) override;
  virtual void ProcessFilteredEvent(event_type const& event,
                                    product_type const& product,
                                    setting_type const& settings) override;
  virtual void Finish(setting_type const& settings) override;

 private:
  TH1D* m_h_nPV = NULL;
  TH1D* m_h_METSumEtRatio = NULL;
  TH1D* m_h_hltPosition = NULL;
  TH1D* m_h_jet1pt = NULL;
  TH1D* m_h_jet1rap = NULL;
  TH1D* m_h_jet1eta = NULL;
  TH1D* m_h_jet1phi = NULL;
  TH1D* m_h_jet2pt = NULL;
  TH1D* m_h_jet2rap = NULL;
  TH1D* m_h_jet2eta = NULL;
  TH1D* m_h_jet2phi = NULL;
  TH1D* m_h_incjetpt = NULL;
  TH1D* m_h_ptavg = NULL;
  TH1D* m_h_idx = NULL;

  TH1D* m_h_dijet_deltaphi = NULL;
  TH1D* m_h_dijet_deltar = NULL;
  TH1D* m_h_dijet_costhetastar = NULL;
  TH1D* m_h_dijet_ystar = NULL;
  TH1D* m_h_dijet_yboost = NULL;
  TH1D* m_h_dijet_chi = NULL;
  TH1D* m_h_dijet_mass = NULL;

  TH2D* m_h_jet12rap = NULL;
  TH2D* m_h2_jet12PtRVsPtavg = NULL;
  TH2D* m_h2_jet1PtVsPtavg = NULL;
  TH2D* m_h2_jet12Pt = NULL;
  TH2D* m_h2_yb_ys = NULL;
  TH2D* m_h2_y12 = NULL;
  TH3D* m_h3_jet12rap = NULL;
  TH3D* m_h3_ptavg_yio = NULL;
  TH3D* m_h3_ptavg_ysb = NULL;
  TH3D* m_h3_ptavg_ysio = NULL;

  TH1D* m_h_neutralHadronFraction = NULL;
  TH1D* m_h_chargedHadronFraction = NULL;
  TH1D* m_h_neutralEMFraction = NULL;
  TH1D* m_h_electronFraction = NULL;
  TH1D* m_h_muonFraction = NULL;
  TH1D* m_h_nConstituents = NULL;
  TH1D* m_h_nCharged = NULL;
  TH1D* m_h_area = NULL;

  double myRun = 0;
  double myLumi = 0;
  double myEvent = 0;
  double myDijet_ptavg=0.;
};

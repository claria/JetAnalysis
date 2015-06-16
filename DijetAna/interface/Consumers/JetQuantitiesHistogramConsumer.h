#pragma once

#include "TH1.h"
#include "TH3.h"
#include "Artus/Utility/interface/RootFileHelper.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class JetQuantitiesHistogramConsumer : public JetConsumerBase {
 public:
  virtual std::string GetConsumerId() const { return "JetQuantitiesHistogramConsumer"; }
  virtual void Init(setting_type const &settings) override;
  virtual void ProcessFilteredEvent(event_type const &event, product_type const &product,
                                    setting_type const &settings) override;
  virtual void Finish(setting_type const &settings) override;

 private:
  TH1D *m_h_nPV = NULL;
  TH1D *m_h_jet1pt = NULL;
  TH1D *m_h_jet1rap = NULL;
  TH1D *m_h_jet1phi = NULL;
  TH1D *m_h_jet2pt = NULL;
  TH1D *m_h_jet2rap = NULL;
  TH1D *m_h_jet2phi = NULL;
  TH1D *m_h_incjetpt = NULL;
  TH2D *m_h_jet12rap = NULL;
  TH3D *m_h3_jet12rap = NULL;
  TH3D *m_h3_ptavg_yio = NULL;
  TH3D *m_h3_ptavg_ysb = NULL;
  TH3D *m_h3_ptavg_ysio = NULL;

  TH1D *m_h_neutralHadronFraction = NULL;
  TH1D *m_h_chargedHadronFraction = NULL;
  TH1D *m_h_photonFraction = NULL;
  TH1D *m_h_electronFraction = NULL;
  TH1D *m_h_muonFraction = NULL;
  TH1D *m_h_nConstituents = NULL;
  TH1D *m_h_nCharged = NULL;
  TH1D *m_h_hfHadronFraction = NULL;
  TH1D *m_h_hfEMFraction = NULL;
  TH1D *m_h_area = NULL;
};

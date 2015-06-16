#pragma once

#include "TH1.h"
#include "TH3.h"
#include "Artus/Utility/interface/RootFileHelper.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class GenJetQuantitiesHistogramConsumer : public JetConsumerBase {
 public:
  virtual std::string GetConsumerId() const { return "GenJetQuantitiesHistogramConsumer"; }
  virtual void Init(setting_type const &settings) override;
  virtual void ProcessEvent(event_type const &event, product_type const &product, setting_type const &setting,
                            FilterResult &result) override;
  virtual void ProcessFilteredEvent(event_type const &event, product_type const &product,
                                    setting_type const &settings) override;
  virtual void Finish(setting_type const &settings) override;

 private:
  // Gen vs. reco pt for each pt bin
  TH1D *m_h_genjet1pt = NULL;
  TH1D *m_h_genjet1rap = NULL;
  TH1D *m_h_genjet1phi = NULL;
  TH1D *m_h_genjet2pt = NULL;
  TH1D *m_h_genjet2rap = NULL;
  TH1D *m_h_genjet2phi = NULL;
  TH1D *m_h_incgenjetpt = NULL;
  TH2D *m_h_genjet12rap = NULL;
  TH3D *m_h3_genjet12rap = NULL;
  TH3D *m_h3_genptavg_ysb = NULL;

  TH2D *m_h2GenVsRecoPt = NULL;
  TH2D *m_h2GenVsRecoPtAvg = NULL;
  TH1D *m_h_jet1DeltaR = NULL;
  TH1D *m_h_jet2DeltaR = NULL;
};

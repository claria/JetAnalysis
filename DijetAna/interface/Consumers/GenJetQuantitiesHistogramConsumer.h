#pragma once

#include "TH1.h"
#include "TH3.h"
#include "Artus/Utility/interface/RootFileHelper.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

class GenJetQuantitiesHistogramConsumer : public JetConsumerBase {
 public:
  virtual std::string GetConsumerId() const { return "GenJetQuantitiesHistogramConsumer"; }
  virtual void Init(setting_type const& settings) override;
  // virtual void ProcessEvent(event_type const &event, product_type const &product, setting_type const &setting,
  //                           FilterResult &result) override;
  virtual void ProcessFilteredEvent(event_type const& event,
                                    product_type const& product,
                                    setting_type const& settings) override;
  virtual void Finish(setting_type const& settings) override;

 private:
  // Gen vs. reco pt for each pt bin
  TH1D* m_h_genjet1pt = nullptr;
  TH1D* m_h_genjet1rap = nullptr;
  TH1D* m_h_genjet1phi = nullptr;
  TH1D* m_h_genjet2pt = nullptr;
  TH1D* m_h_genjet2rap = nullptr;
  TH1D* m_h_genjet2phi = nullptr;
  TH1D* m_h_incgenjetpt = nullptr;
  TH1D* m_h_genptavg = nullptr;
  TH1D* m_h_genidx = nullptr;
  TH1D* m_h_genmatchptavg = nullptr;
  TH2D* m_h_genjet12rap = nullptr;
  TH2D* m_h2_genrecoidx = nullptr;
  TH2D* m_h2_genrecoptavg = nullptr;
  TH3D* m_h3_genjet12rap = nullptr;
  TH3D* m_h3_genptavg_yio = nullptr;
  TH3D* m_h3_genptavg_ysb = nullptr;
  TH1D* m_h_genjet12dphi = nullptr;
  TH2D* m_h2_genjet12PtRVsPtavg = nullptr;
  TH2D* m_h2_gen_yb_ys = nullptr;
  TH2D* m_h2_genreco_ptavg = nullptr;
  TH2D* m_h2_GenVsRecoPt = nullptr;
  TH2D* m_h2_GenVsRecoPtAvg = nullptr;
  TH2D* m_h2_GenVsRecoYboost = nullptr;
  TH2D* m_h2_GenVsRecoYstar = nullptr;
  TH2D* m_h2_GenVsRecoY = nullptr;
  TH2D* m_h2_GenVsRecoEta = nullptr;
  TH3D* m_h3_GenVsRecoY = nullptr;
  TH3D* m_h3_GenVsRecoEta = nullptr;
  TH2D* m_h2_GenYboostVsRecoYboost = nullptr;
  TH2D* m_h2_GenYstarVsRecoYstar = nullptr;
  TH1D* m_h_jet1DeltaR = nullptr;
  TH1D* m_h_jet2DeltaR = nullptr;

  TProfile* m_tp_genyvsrecoy = nullptr;

  TH1D* m_h_dijet_flavour_qq = nullptr;
  TH1D* m_h_dijet_flavour_qg = nullptr;
  TH1D* m_h_dijet_flavour_gg = nullptr;
};

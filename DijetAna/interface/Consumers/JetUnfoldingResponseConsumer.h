#pragma once

#include "TH1.h"
#include "Artus/Utility/interface/RootFileHelper.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

#include "RooUnfold/src/RooUnfoldResponse.h"
#include "RooUnfold/src/RooUnfoldBayes.h"
//#include "RooUnfoldSvd.h"
//#include "RooUnfoldTUnfold.h"

class JetUnfoldingResponseConsumer : public JetConsumerBase {
 public:
  virtual std::string GetConsumerId() const { return "JetUnfoldingResponseConsumer"; }
  virtual void Init(setting_type const& settings) override;
  virtual void ProcessFilteredEvent(event_type const& event,
                                    product_type const& product,
                                    setting_type const& settings) override;
  virtual void Finish(setting_type const& settings) override;

 private:
  RooUnfoldResponse* m_unfoldResponse_ptavg_ysb = nullptr;
  RooUnfoldResponse* m_unfoldResponse_ptavg = nullptr;
  TH2D* m_h2_response_idx = nullptr;
  RooUnfoldResponse* m_unfoldResponse_idx = nullptr;
};

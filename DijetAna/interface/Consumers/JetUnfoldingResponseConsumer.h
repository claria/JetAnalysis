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
  virtual void ProcessEvent(event_type const& event, product_type const& product,
                            setting_type const& setting, FilterResult& result) override;
  virtual void ProcessFilteredEvent(event_type const& event, product_type const& product,
                                    setting_type const& settings) override;
  virtual void Finish(setting_type const& settings) override;

 private:
  RooUnfoldResponse* m_unfoldResponse_jet12rap = NULL;
  RooUnfoldResponse* m_unfoldResponse_jet12rapsign = NULL;
  RooUnfoldResponse* m_unfoldResponse_ysba = NULL;
  RooUnfoldResponse* m_unfoldResponse_pt = NULL;
};

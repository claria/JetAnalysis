#pragma once

#include "Artus/Configuration/interface/SettingsBase.h"
#include "Artus/Configuration/interface/SettingMacros.h"
#include "Artus/Configuration/interface/PropertyTreeSupport.h"
#include "Artus/KappaAnalysis/interface/KappaSettings.h"
#include <limits>

class JetSettings : public KappaSettings {
 public:

  IMPL_SETTING(std::string, RapidityCorrectionFile);
  // Cuts for valid jet Producer to validate/invalidate a jet
  IMPL_SETTING_DEFAULT(double, MinValidJetPt, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MinValidJetAbsRap, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxValidJetAbsRap, std::numeric_limits<double>::max())

  IMPL_SETTING_DEFAULT(double, MinValidGenJetPt, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MinValidGenJetAbsRap, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxValidGenJetAbsRap, std::numeric_limits<double>::max())

  // Number of minimum required valid jets
  IMPL_SETTING_DEFAULT(int, MinValidJets, 0)
  IMPL_SETTING_DEFAULT(int, MinValidGenJets, 0)

  IMPL_SETTING_DEFAULT(double, MinPtAvg, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxPtAvg, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinGenPtAvg, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxGenPtAvg, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinGenMatchPtAvg, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxGenMatchPtAvg, std::numeric_limits<double>::max())

  IMPL_SETTING_DEFAULT(double, MinLeadingJetPt, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxLeadingJetPt, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinLeadingGenJetPt, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxLeadingGenJetPt, std::numeric_limits<double>::max())

  IMPL_SETTING_DEFAULT(double, MinYStar, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxYStar, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinYBoost, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxYBoost, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinGenYStar, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxGenYStar, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinGenYBoost, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxGenYBoost, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinYInner, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxYInner, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinYOuter, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxYOuter, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinGenYInner, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxGenYInner, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinGenYOuter, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxGenYOuter, std::numeric_limits<double>::max())

  IMPL_SETTING_DEFAULT(double, MinLeadingJetAbsRap, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxLeadingJetAbsRap, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinSecondJetAbsRap, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxSecondJetAbsRap, std::numeric_limits<double>::max())

  IMPL_SETTING_DEFAULT(double, MinDijetsAbsRap, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxDijetsAbsRap, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MinGenDijetsAbsRap, std::numeric_limits<double>::lowest())
  IMPL_SETTING_DEFAULT(double, MaxGenDijetsAbsRap, std::numeric_limits<double>::max())
  // MET Filter
  IMPL_SETTING_DEFAULT(double, MaxMETSumEtRatio, std::numeric_limits<double>::max())

  // GenJetMatcher
  IMPL_SETTING_DEFAULT(double, MaxDeltaR, std::numeric_limits<double>::max())
  IMPL_SETTING_DEFAULT(double, MaxDeltaPtRel, std::numeric_limits<double>::max())

  IMPL_SETTING_STRINGLIST(PipelineNames);
  IMPL_SETTING_DEFAULT(std::string, TriggerEfficiencyQuantity, "");
  IMPL_SETTING_DEFAULT(std::string, L1FilterPattern, "");
  IMPL_SETTING_DEFAULT(std::string, HltFilterPattern, "");

  IMPL_SETTING_STRINGLIST(TriggerEffPaths);
  IMPL_SETTING_DOUBLELIST(TriggerEffThresholds);
  IMPL_SETTING_STRINGLIST(HltPathsBlacklist);

  //JER Scaling Factors
  IMPL_SETTING_DOUBLELIST(JERScalingFactors);

  // IMPL_SETTING_STRINGLIST(L1FilterNames);
  // IMPL_SETTING_STRINGLIST(HltFilterNames);
  IMPL_SETTING_DOUBLELIST(L1FilterThresholds);
  IMPL_SETTING_DOUBLELIST(HltFilterThresholds);
  // Binnings
  IMPL_SETTING_DOUBLELIST(RapidityBinning);
  IMPL_SETTING_DOUBLELIST(RapidityAbsBinning);
  IMPL_SETTING_DOUBLELIST(FineRapidityBinning);
  IMPL_SETTING_DOUBLELIST(FineRapidityAbsBinning);

  IMPL_SETTING_DOUBLELIST(PtBinning);
  IMPL_SETTING_DOUBLELIST(LogPtBinning);
  IMPL_SETTING_DOUBLELIST(GenPtBinning);
  IMPL_SETTING_DOUBLELIST(TripleDiffPtBinning);
  IMPL_SETTING_DOUBLELIST(TripleDiffGenPtBinning);
};

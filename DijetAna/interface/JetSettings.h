#pragma once

#include "Artus/Configuration/interface/SettingsBase.h"
#include "Artus/Configuration/interface/SettingMacros.h"
#include "Artus/Configuration/interface/PropertyTreeSupport.h"
#include "Artus/KappaAnalysis/interface/KappaSettings.h"
#include <limits>

class JetSettings: public KappaSettings {
public:

	// Cuts for valid jet Producer to validate/invalidate a jet
	IMPL_SETTING_DEFAULT(double, MinValidJetPt, std::numeric_limits<double>::min())
	IMPL_SETTING_DEFAULT(double, MinValidJetAbsRap, std::numeric_limits<double>::min())
	IMPL_SETTING_DEFAULT(double, MaxValidJetAbsRap, std::numeric_limits<double>::max())

	// Number of minimum required valid jets
	IMPL_SETTING_DEFAULT(int, MinValidJets, 0)

	IMPL_SETTING_DEFAULT(double, MinLeadingJetPt, std::numeric_limits<double>::min())
	IMPL_SETTING_DEFAULT(double, MaxLeadingJetPt, std::numeric_limits<double>::max())

	IMPL_SETTING_DEFAULT(double, MinLeadingJetAbsRap, std::numeric_limits<double>::min())
	IMPL_SETTING_DEFAULT(double, MaxLeadingJetAbsRap, std::numeric_limits<double>::max())
	IMPL_SETTING_DEFAULT(double, MinSecondJetAbsRap, std::numeric_limits<double>::min())
	IMPL_SETTING_DEFAULT(double, MaxSecondJetAbsRap, std::numeric_limits<double>::max())

	IMPL_SETTING_DEFAULT(double, MinDijetsAbsRap, std::numeric_limits<double>::min())
	IMPL_SETTING_DEFAULT(double, MaxDijetsAbsRap, std::numeric_limits<double>::max())
        //MET Filter
	IMPL_SETTING_DEFAULT(double, MaxMETSumEtRatio, std::numeric_limits<double>::max())

	// GenJetMatcher
	IMPL_SETTING_DEFAULT(double, MaxDeltaR, std::numeric_limits<double>::max())

	IMPL_SETTING_STRINGLIST(PipelineNames);
	IMPL_SETTING_DEFAULT(std::string, TriggerEfficiencyQuantity, "");
	IMPL_SETTING_DEFAULT(std::string, L1FilterPattern, "");
	IMPL_SETTING_DEFAULT(std::string, HltFilterPattern, "");

	IMPL_SETTING_STRINGLIST(TriggerEffPaths);
	IMPL_SETTING_DOUBLELIST(TriggerEffThresholds);
	IMPL_SETTING_STRINGLIST(HltPathsBlacklist);

	// IMPL_SETTING_STRINGLIST(L1FilterNames);
	// IMPL_SETTING_STRINGLIST(HltFilterNames);
	IMPL_SETTING_DOUBLELIST(L1FilterThresholds);
	IMPL_SETTING_DOUBLELIST(HltFilterThresholds);
	//Binnings
	IMPL_SETTING_DOUBLELIST(RapidityBinning);
	IMPL_SETTING_DOUBLELIST(Jet1RapidityBinning);
	IMPL_SETTING_DOUBLELIST(Jet2RapidityBinning);
	IMPL_SETTING_DOUBLELIST(PtBinning);
	IMPL_SETTING_DOUBLELIST(GenPtBinning);
	IMPL_SETTING_DOUBLELIST(TripleDiffPtBinning);
	IMPL_SETTING_DOUBLELIST(TripleDiffGenPtBinning);
};

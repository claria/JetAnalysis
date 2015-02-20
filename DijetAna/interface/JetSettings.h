#pragma once

#include "Artus/Configuration/interface/SettingsBase.h"
#include "Artus/Configuration/interface/SettingMacros.h"
#include "Artus/Configuration/interface/PropertyTreeSupport.h"
#include "Artus/KappaAnalysis/interface/KappaSettings.h"
#include <limits>

class JetSettings: public KappaSettings {
public:

	// Cuts for valid jet Producer to validate/invalidate a jet
	IMPL_SETTING_DEFAULT(float, MinValidJetPt, std::numeric_limits<float>::min())
	IMPL_SETTING_DEFAULT(float, MinValidJetAbsRap, std::numeric_limits<float>::min())
	IMPL_SETTING_DEFAULT(float, MaxValidJetAbsRap, std::numeric_limits<float>::max())

	// Number of minimum required valid jets
	IMPL_SETTING_DEFAULT(int, MinValidJets, 0)

	IMPL_SETTING_DEFAULT(float, MinLeadingJetPt, std::numeric_limits<float>::min())
	IMPL_SETTING_DEFAULT(float, MaxLeadingJetPt, std::numeric_limits<float>::max())

	IMPL_SETTING_DEFAULT(float, MinLeadingJetAbsRap, std::numeric_limits<float>::min())
	IMPL_SETTING_DEFAULT(float, MaxLeadingJetAbsRap, std::numeric_limits<float>::max())
	IMPL_SETTING_DEFAULT(float, MinSecondJetAbsRap, std::numeric_limits<float>::min())
	IMPL_SETTING_DEFAULT(float, MaxSecondJetAbsRap, std::numeric_limits<float>::max())

	IMPL_SETTING_DEFAULT(float, MinDijetsAbsRap, std::numeric_limits<float>::min())
	IMPL_SETTING_DEFAULT(float, MaxDijetsAbsRap, std::numeric_limits<float>::max())
        //MET Filter
	IMPL_SETTING_DEFAULT(float, MaxMETSumEtRatio, std::numeric_limits<float>::max())


	IMPL_SETTING_STRINGLIST(PipelineNames);
	IMPL_SETTING_DEFAULT(std::string, TriggerEfficiencyQuantity, "");
	IMPL_SETTING_DEFAULT(std::string, L1FilterPattern, "");
	IMPL_SETTING_DEFAULT(std::string, HltFilterPattern, "");

	IMPL_SETTING_STRINGLIST(TriggerEffPaths);
	IMPL_SETTING_FLOATLIST(TriggerEffThresholds);
	IMPL_SETTING_STRINGLIST(HltPathsBlacklist);

	// IMPL_SETTING_STRINGLIST(L1FilterNames);
	// IMPL_SETTING_STRINGLIST(HltFilterNames);
	IMPL_SETTING_FLOATLIST(L1FilterThresholds);
	IMPL_SETTING_FLOATLIST(HltFilterThresholds);
	//Binnings
	IMPL_SETTING_DOUBLELIST(RapidityBinning);
	IMPL_SETTING_DOUBLELIST(PtBinning);
	IMPL_SETTING_DOUBLELIST(TripleDiffPtBinning);
};

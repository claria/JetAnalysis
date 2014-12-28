#include "JetAnalysis/DijetAna/interface/Filters/JetHltEfficiencyFilter.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

std::string JetHltEfficiencyFilter::GetFilterId() const {
	return "JetHltEfficiencyFilter";
}

void JetHltEfficiencyFilter::Init(JetSettings const& settings) {
	// KappaFilterBase:Init(settings);

	auto const& jetSettings = static_cast<JetSettings const&>(settings);
	// Fill trigger efficiency thresholds map
	assert(jetSettings.GetTriggerEffThresholds().size() == jetSettings.GetTriggerEffPaths().size() +1);
	for (size_t i=0; i< jetSettings.GetTriggerEffPaths().size(); i++) {
		triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]] = std::make_pair(jetSettings.GetTriggerEffThresholds()[i], jetSettings.GetTriggerEffThresholds()[i+1]);
	}
}

bool JetHltEfficiencyFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {

	// TODO: Fix this by somehow adding the analysis specific quantities in the beginning
	// double triggerEffQuantity = (LambdaNtupleConsumer<JetTypes>::GetFloatQuantities()[jetSettings.GetTriggerEfficiencyQuantity()])(jetEvent, jetProduct);
	double pass = false;
	double triggerEffQuantity = product.m_validJets.at(0)->p4.Pt();
	if ((triggerEffQuantity > triggerEffThresholds.at(product.m_selectedHltName).first) && 
		(triggerEffQuantity <= triggerEffThresholds.at(product.m_selectedHltName).second))
	{
		pass = true;
	}
	return pass;
}


#include "JetAnalysis/DijetAna/interface/Filters/JetHltEfficiencyFilter.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

std::string JetHltEfficiencyFilter::GetFilterId() const {
	return "JetHltEfficiencyFilter";
}

void JetHltEfficiencyFilter::Init(JetSettings const& settings) {
	// KappaFilterBase:Init(settings);

	auto const& jetSettings = static_cast<JetSettings const&>(settings);
	// Fill trigger efficiency thresholds map
	assert(jetSettings.GetTriggerEffThresholds().size() == jetSettings.GetTriggerEffPaths().size());
	for (size_t i=0; i< jetSettings.GetTriggerEffThresholds().size(); i++) {
		double triggerEffThresholdNext = std::numeric_limits<double>::max();
		if (i < jetSettings.GetHltPaths().size()) {
			triggerEffThresholdNext = jetSettings.GetTriggerEffThresholds().at(i+1);
		}
		triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]] = std::make_pair(jetSettings.GetTriggerEffThresholds()[i], triggerEffThresholdNext);
		// std::cout << jetSettings.GetTriggerEffPaths()[i] << std::endl;
		// std::cout << "chacka1 " << triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]].first << std::endl;
		// std::cout << "chacka2 " << triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]].second << std::endl;
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


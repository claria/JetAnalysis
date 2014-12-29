#include "JetAnalysis/DijetAna/interface/Filters/JetHltEfficiencyFilter.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

std::string JetHltEfficiencyFilter::GetFilterId() const {
	return "JetHltEfficiencyFilter";
}

void JetHltEfficiencyFilter::Init(JetSettings const& settings) {
	JetFilterBase::Init(settings);

	assert(settings.GetTriggerEffThresholds().size() == settings.GetTriggerEffPaths().size() +1);
	for (size_t i=0; i< settings.GetTriggerEffPaths().size(); i++) {
		triggerEffThresholds[settings.GetTriggerEffPaths()[i]] = std::make_pair(settings.GetTriggerEffThresholds()[i], settings.GetTriggerEffThresholds()[i+1]);
		std::cout << "Trigger eff. thresholds for path " << settings.GetTriggerEffPaths()[i] << " is: (" << triggerEffThresholds[settings.GetTriggerEffPaths()[i]].first << ", " << triggerEffThresholds[settings.GetTriggerEffPaths()[i]].second << ")." << std::endl;
	}
}

bool JetHltEfficiencyFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {

	// TODO: Fix this by somehow adding the analysis specific quantities in the beginning
	// double triggerEffQuantity = (LambdaNtupleConsumer<JetTypes>::GetFloatQuantities()[settings.GetTriggerEfficiencyQuantity()])(jetEvent, jetProduct);
	double pass = false;
	double triggerEffQuantity = product.m_validJets.at(0)->p4.Pt();
	std::cout << "Selected trigger:" << product.m_selectedHltName << " in Range (" << triggerEffThresholds.at(product.m_selectedHltName).first << ", " << triggerEffThresholds.at(product.m_selectedHltName).second << "). :" << product.m_validJets.at(0)->p4.Pt() << std::endl;
	if ((triggerEffQuantity > triggerEffThresholds.at(product.m_selectedHltName).first) && 
		(triggerEffQuantity <= triggerEffThresholds.at(product.m_selectedHltName).second))
	{
		pass = true;
	}
	return pass;
}


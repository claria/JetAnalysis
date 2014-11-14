#include "JetAnalysis/DijetAna/interface/Producers/JetHltProducer.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

std::string JetHltProducer::GetProducerId() const {
	return "JetHltProducer";
}

void JetHltProducer::Init(KappaSettings const& settings) {
	KappaProducerBase::Init(settings);
	auto const& jetSettings = static_cast<JetSettings const&>(settings);

	if (settings.GetHltPaths().empty()) 
		LOG(FATAL) << "No Hlt Trigger path list (tag \"HltPaths\") configured!";

	// Fill trigger efficiency thresholds map
	assert(jetSettings.GetTriggerEffThresholds().size() == jetSettings.GetTriggerEffPaths().size());
	for (size_t i=0; i< jetSettings.GetTriggerEffThresholds().size(); i++) {
		double triggerEffThresholdNext = std::numeric_limits<double>::max();
		if (i < jetSettings.GetHltPaths().size()) {
			triggerEffThresholdNext = jetSettings.GetTriggerEffThresholds().at(i+1);
		}
		triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]] = std::make_pair(jetSettings.GetTriggerEffThresholds()[i], triggerEffThresholdNext);
	}
}

void JetHltProducer::Produce(KappaEvent const& event, KappaProduct& product,
                     KappaSettings const& settings) const
{
	// auto const& jetEvent = static_cast <JetEvent const&> (event);
	// auto const& jetProduct = static_cast <JetProduct const&> (product);
	auto const& jetSettings = static_cast<JetSettings const&>(settings);
	// std::cout << "New Event: " << event.m_eventMetadata->nRun << " " << event.m_eventMetadata->nLumi << " " << event.m_eventMetadata->nEvent << std::endl;
	assert(event.m_lumiMetadata);
	assert(event.m_eventMetadata);
	
	// set LumiMetadat, needs to be done here for the case running over multiple files
	product.m_hltInfo.setLumiMetadata(event.m_lumiMetadata);

	// Selected trigger Names
	std::string selectedHltName;
	int prescaleSelectedHlt = std::numeric_limits<int>::max();

	// Check if any of the blacklisted triggers fired
	bool blacklistHltFired = false;
	for (std::vector<std::string>::const_iterator blacklistHltPath = jetSettings.GetHltPathsBlacklist().begin(); blacklistHltPath != jetSettings.GetHltPathsBlacklist().end(); ++blacklistHltPath) 
	{
		std::string blacklistHltFilter = product.m_hltInfo.getHLTName(*blacklistHltPath);
		if (event.m_eventMetadata->hltFired(blacklistHltFilter, event.m_lumiMetadata)) {
			blacklistHltFired = true;
		}
	}

	if (!blacklistHltFired) {
		for (std::vector<std::string>::const_iterator hltPath = jetSettings.GetHltPaths().begin(); hltPath != jetSettings.GetHltPaths().end(); ++hltPath)
		{
			std::string hltFilter = product.m_hltInfo.getHLTName(*hltPath);
			if (!hltFilter.empty()){
				if (event.m_eventMetadata->hltFired(hltFilter, event.m_lumiMetadata)) 
				{
					// std::cout << "Trigger fired: " << hltFilter << std::endl;
					if (product.m_hltInfo.getPrescale(hltFilter) < prescaleSelectedHlt) {
						// Check if trigger is efficient
						// TODO: Fix this by somehow adding the analysis specific quantities in the beginning
						// double triggerEffQuantity = (LambdaNtupleConsumer<JetTypes>::GetFloatQuantities()[jetSettings.GetTriggerEfficiencyQuantity()])(jetEvent, jetProduct);
						double triggerEffQuantity = product.m_validJets.at(0)->p4.Pt();
						if ((triggerEffQuantity > triggerEffThresholds.at(*hltPath).first) && 
							(triggerEffQuantity <= triggerEffThresholds.at(*hltPath).second))
						{
							prescaleSelectedHlt = product.m_hltInfo.getPrescale(hltFilter);
							selectedHltName = hltFilter;
						}
					}
				}
			}
		}
	}

	if (! selectedHltName.empty()) 
	{
		product.m_selectedHltName = selectedHltName;
		product.m_weights["hltPrescaleWeight"] = prescaleSelectedHlt;
		product.m_selectedHltPosition = (int) product.m_hltInfo.getHLTPosition(selectedHltName);
	}
	else
	{
		product.m_selectedHltPosition = DefaultValues::UndefinedInt;
	}
}



#include "JetAnalysis/DijetAna/interface/Producers/JetHltProducer.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

std::string JetHltProducer::GetProducerId() const {
	return "JetHltProducer";
}

void JetHltProducer::Init(KappaSettings const& settings) {
	KappaProducerBase::Init(settings);
	// auto const& jetSettings = static_cast<JetSettings const&>(settings);

	if (settings.GetHltPaths().empty()) 
		LOG(FATAL) << "No Hlt Trigger path list (tag \"HltPaths\") configured!";

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
						prescaleSelectedHlt = product.m_hltInfo.getPrescale(hltFilter);
						selectedHltName = *hltPath;
					}
				}
			}
		}
	}

	if (! selectedHltName.empty()) 
	{
		product.m_selectedHltName = selectedHltName;
		product.m_weights["hltPrescaleWeight"] = prescaleSelectedHlt;
		product.m_selectedHltPosition = (int) product.m_hltInfo.getHLTPosition(product.m_hltInfo.getHLTName(selectedHltName));
	}
	else
	{
		product.m_selectedHltPosition = DefaultValues::UndefinedInt;
	}
}



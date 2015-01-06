#include "JetAnalysis/DijetAna/interface/Producers/JetHltProducer.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

std::string JetHltProducer::GetProducerId() const
{
  return "JetHltProducer";
}

void JetHltProducer::Init(KappaSettings const &settings)
{
  KappaProducerBase::Init(settings);
  auto const &jetSettings = static_cast<JetSettings const &>(settings);

  if (settings.GetHltPaths().empty())
	LOG(FATAL) << "No Hlt Trigger path list (tag \"HltPaths\") configured!";

  assert(jetSettings.GetTriggerEffThresholds().size() ==
         jetSettings.GetTriggerEffPaths().size() + 1);
  for (size_t i = 0; i < jetSettings.GetTriggerEffPaths().size(); i++)
  {
	triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]] = std::make_pair(
	    jetSettings.GetTriggerEffThresholds()[i], jetSettings.GetTriggerEffThresholds()[i + 1]);
	std::cout << "Trigger eff. thresholds for path " << jetSettings.GetTriggerEffPaths()[i]
	          << " is: (" << triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]].first << ", "
	          << triggerEffThresholds[jetSettings.GetTriggerEffPaths()[i]].second << ")."
	          << std::endl;
  }
}

void JetHltProducer::Produce(KappaEvent const &event, KappaProduct &product,
                             KappaSettings const &settings) const
{
  // auto const& jetEvent = static_cast <JetEvent const&> (event);
  // auto const& jetProduct = static_cast <JetProduct const&> (product);
  auto const &jetSettings = static_cast<JetSettings const &>(settings);

  LOG(DEBUG) << "Process: "
             << "run = " << event.m_eventMetadata->nRun << ", "
             << "lumi = " << event.m_eventMetadata->nLumi << ", "
             << "event = " << event.m_eventMetadata->nEvent;

  assert(event.m_lumiMetadata);
  assert(event.m_eventMetadata);
  assert(product.m_validJets.size() > 0);

  product.m_hltInfo.setLumiMetadata(event.m_lumiMetadata);

  double triggerEffQuantity = product.m_validJets.at(0)->p4.Pt();
  for (std::vector<std::string>::const_iterator hltPath = jetSettings.GetHltPaths().begin();
       hltPath != jetSettings.GetHltPaths().end(); ++hltPath)
  {
	std::string hltName = product.m_hltInfo.getHLTName(*hltPath);
	if (!hltName.empty())
	{
	  if (event.m_eventMetadata->hltFired(hltName, event.m_lumiMetadata))
	  {
		// std::cout << "Trigger " << *hltPath << " fired." << std::endl;
		if ((triggerEffQuantity >= triggerEffThresholds.at(*hltPath).first) &&
		    (triggerEffQuantity < triggerEffThresholds.at(*hltPath).second))
		{
		  // std::cout << "Trigger " << *hltPath << " in selected pt range." << std::endl;
		  // std::cout << "Trigger " << *hltPath << " prescale: " <<
		  // product.m_hltInfo.getPrescale(hltName) << std::endl;
		  // std::cout << "Trigger " << *hltPath << " leading Jet Pt: " << triggerEffQuantity <<
		  // std::endl;
		  product.m_selectedHltName = *hltPath;
		  product.m_weights["hltPrescaleWeight"] = product.m_hltInfo.getPrescale(hltName);
		  product.m_selectedHltPosition = (int)product.m_hltInfo.getHLTPosition(hltName);
		  break;
		}
		else
		{
		  product.m_selectedHltName = "";
		  product.m_weights["hltPrescaleWeight"] = 0.;
		  product.m_selectedHltPosition = DefaultValues::UndefinedInt;
		}
	  }
	}
	else
	{
	  std::cout << "Trigger " << *hltPath << " could not be resolved." << std::endl;
	}
  }
}

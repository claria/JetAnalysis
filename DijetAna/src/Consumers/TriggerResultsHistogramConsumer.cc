#include <TH1.h>
#include "TROOT.h"

#include "Artus/Utility/interface/RootFileHelper.h"
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

#include "JetAnalysis/DijetAna/interface/Consumers/TriggerResultsHistogramConsumer.h"
#include "KappaTools/RootTools/HLTTools.h"
#include <boost/lexical_cast.hpp>


void TriggerResultsHistogramConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init( settings );

	m_hltPaths = settings.GetTriggerEffPaths();

	m_patternL1Filter = boost::regex(settings.GetL1FilterPattern());
	m_patternHltFilter = boost::regex(settings.GetHltFilterPattern());

	for (size_t i = 0; i != m_hltPaths.size(); ++i)
	{
		m_triggerResultHists.push_back(new TH1F(m_hltPaths[i].c_str(), m_hltPaths[i].c_str(), 500., 0., 1000.));
		if (i < m_hltPaths.size() -1) {
			m_triggerEmulatedHists.push_back(new TH1F(("emul_" + m_hltPaths[i+1]).c_str(), ("emul_" +m_hltPaths[i+1]).c_str(), 500., 0., 1000.));
		}
	}

	assert(settings.GetL1FilterThresholds().size() == settings.GetTriggerEffPaths().size());
	assert(settings.GetHltFilterThresholds().size() == settings.GetTriggerEffPaths().size());
	for (size_t i=0; i< settings.GetTriggerEffPaths().size(); i++) {
		m_filterThresholds[settings.GetTriggerEffPaths()[i]] = std::make_pair(settings.GetL1FilterThresholds()[i], settings.GetHltFilterThresholds()[i]);
	}



}

void TriggerResultsHistogramConsumer::ProcessFilteredEvent(event_type const& event,
		product_type const& product,
		setting_type const& settings)
{
	// auto const& specEvent = static_cast < JetEvent const&> (event);
	auto const& specProduct = static_cast < JetProduct const&> (product);

	LOG(DEBUG) << "Process: " <<
	              "run = " << event.m_eventMetadata->nRun << ", " <<
	              "lumi = " << event.m_eventMetadata->nLumi << ", " <<
	              "event = " << event.m_eventMetadata->nEvent;

	// Quantity for which the histograms are filled
	float triggerEffQuantity = specProduct.m_validJets.at(0)->p4.Pt();

	// Threshold of the L1 and Hlt Filter
	double m_l1FilterThreshold = 0.;
	double m_hltFilterThreshold = 0.;

	for(std::vector<std::string>::size_type i = 0; i != m_hltPaths.size() - 1; i++) {
		// Fill Histograms for trigger which actually have fired
		// std::cout << "xyz " << m_hltFilterNames[i] << " " << std::endl;
		std::string hltName = product.m_hltInfo.getHLTName(m_hltPaths[i]);
		size_t hltPosition = product.m_hltInfo.getHLTPosition(m_hltPaths[i]);
		// std::cout << "Trigger " << hltName << " " << std::endl;
		if (event.m_eventMetadata->hltFired(hltName, event.m_lumiMetadata)){
			// std::cout << "Trigger " << hltName << " fired." << std::endl;
			// Check if next trigger would have fired as well if it would not have been prescaled
			int l1objIdx = -1;
			int hltobjIdx = -1;
			for (size_t filterIndex = event.m_triggerInfos->getMinFilterIndex(hltPosition); filterIndex < event.m_triggerInfos->getMaxFilterIndex(hltPosition); filterIndex++)
			{
				std::string filterName = event.m_triggerInfos->toFilter[filterIndex];
				// std::cout << "Filtername " << filterIndex << " "  << filterName << std::endl;
				if (boost::regex_search(filterName, m_patternL1Filter)) {
					// Sorted by Pt, we just use the first one
					l1objIdx = event.m_triggerObjects->toIdxFilter[filterIndex][0];
				}
				else if (boost::regex_search(filterName,  m_patternHltFilter)) {
					// Sorted by Pt, we just use the first one
					hltobjIdx = event.m_triggerObjects->toIdxFilter[filterIndex][0];
				}
			}
			// std::cout << "l1objIdx" << l1objIdx << std::endl;
			if (l1objIdx < 0 || hltobjIdx < 0) 
			{
				LOG(WARNING) << "The L1 or HLT filter of the fired trigger were not found. "
					         << "Event will be skipped." <<std::endl;
				return;
			}


			// std::cout << "Get trigger thresholds for path "  << m_hltPaths[i+1] << std::endl;
			// m_l1FilterThreshold = GetL1FilterThreshold(event, product, m_hltPaths[i+1]);
			// m_hltFilterThreshold = GetHltFilterThreshold(event, product, m_hltPaths[i+1]);
			m_l1FilterThreshold = m_filterThresholds[m_hltPaths[i+1]].first;
			m_hltFilterThreshold = m_filterThresholds[m_hltPaths[i+1]].second;

			// std::cout << "L1obj Pt: " << event.m_triggerObjects->trgObjects[l1objIdx].p4.Pt() << " thr: " << m_l1FilterThreshold << std::endl;
			// std::cout << "Hltobj Pt: " << event.m_triggerObjects->trgObjects[hltobjIdx].p4.Pt() << " thr: " << m_hltFilterThreshold << std::endl;

			// Fill pass histogram for path i
			m_triggerResultHists[i]->Fill(triggerEffQuantity);
			// Fill pass histogram for path i+1
			if ((event.m_triggerObjects->trgObjects[l1objIdx].p4.Pt() > m_l1FilterThreshold) &&
				(event.m_triggerObjects->trgObjects[hltobjIdx].p4.Pt() > m_hltFilterThreshold)) {
					m_triggerEmulatedHists[i]->Fill(triggerEffQuantity);
			}
		}
	}
}

double TriggerResultsHistogramConsumer::GetL1FilterThreshold(event_type const& event, product_type const& product, std::string path)
{
	size_t hltPosition = product.m_hltInfo.getHLTPosition(path);
	// std::cout << "TriggerResultsHistogramConsumer::GetL1FilterThreshold" << "hltposition" << hltPosition;
	for (size_t filterIndex = event.m_triggerInfos->getMinFilterIndex(hltPosition); filterIndex < event.m_triggerInfos->getMaxFilterIndex(hltPosition); filterIndex++)
	{
		std::string filterName = event.m_triggerInfos->toFilter[filterIndex];
		boost::smatch match;

		if (boost::regex_search(filterName, match, m_patternL1Filter)) {
			return boost::lexical_cast<double>(std::string(match[2].first, match[2].second));
		}
	}
	return 0.;
}

double TriggerResultsHistogramConsumer::GetHltFilterThreshold(event_type const& event, product_type const& product, std::string path)
{
	size_t hltPosition = product.m_hltInfo.getHLTPosition(path);
	for (size_t filterIndex = event.m_triggerInfos->getMinFilterIndex(hltPosition); filterIndex < event.m_triggerInfos->getMaxFilterIndex(hltPosition); filterIndex++)
	{
		std::string filterName = event.m_triggerInfos->toFilter[filterIndex];
		// std::cout << "filtername" << filterName << std::endl;
		boost::smatch match;

		if (boost::regex_search(filterName, match, m_patternHltFilter)) {
			// std::cout << "filtername" << filterName << std::endl;
			return boost::lexical_cast<double>(std::string(match[2].first, match[2].second));
		}
	}
	return 0.;
}

void TriggerResultsHistogramConsumer::Finish(setting_type const& settings)
{

	// save histograms
	RootFileHelper::SafeCd(settings.GetRootOutFile(),
			settings.GetRootFileFolder() + "/triggerResults");

	for (size_t i = 0; i != m_hltPaths.size(); ++i)
	{
		m_triggerResultHists[i]->Write(m_triggerResultHists[i]->GetName());
		if (i < m_hltPaths.size() -1) {
			m_triggerEmulatedHists[i]->Write(m_triggerEmulatedHists[i]->GetName());
		}
	}
}


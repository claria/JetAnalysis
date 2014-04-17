/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "../JetTypes.h"

class JetHltProducer : public JetProducerBase {
public:

	virtual std::string GetProducerId() const {
		return "JetHltProducer";
	}

	virtual void ProduceGlobal(JetEvent const& event,
			JetProduct & product,
			JetGlobalSettings const& globalSettings) const
	{
		std:: cout << "EEEvent" << std::endl;
		for (size_t i = 0; i < event.m_lumiMetadata->hltNames.size(); ++i){
			product.m_HltTrigger[event.m_lumiMetadata->hltNames[i]] = false;
			if (event.m_eventMetadata->bitsHLT & (1ul << i)){
				product.m_HltTrigger[event.m_lumiMetadata->hltNames[i]] = true;
			}
		}

		std::cout << "recojetpt ";
		for (size_t i = 0; i < event.m_jets->size(); ++i){
			std::cout << event.m_jets->at(i).p4.Pt() << " ";
		}
		std::cout << std::endl;

		
		
		std::cout << "trgobjpt ";
		for( std::vector<KDataLV>::const_iterator i = event.m_triggerObjects->trgObjects.begin(); 
				i != event.m_triggerObjects->trgObjects.end(); ++i)
			std::cout << (*i).p4.Pt() << ' ';
		std::cout << std::endl;


		std::cout << "l1 ";
		for ( std::vector<std::vector<unsigned long>>::size_type i = 0; i < event.m_triggerObjects->toIdxL1L2.size(); i++ )
		{
			for ( std::vector<int>::size_type j = 0; j < event.m_triggerObjects->toIdxL1L2[i].size(); j++ )
			{
				std::cout << event.m_triggerObjects->toIdxL1L2[i][j] << ' ';
			}
			std::cout << std::endl;
		}

		std::cout << "hlt ";
		for ( std::vector<std::vector<unsigned long>>::size_type i = 0; i < event.m_triggerObjects->toIdxHLT.size(); i++ )
		{
			for ( std::vector<int>::size_type j = 0; j < event.m_triggerObjects->toIdxHLT[i].size(); j++ )
			{
				std::cout << event.m_triggerObjects->toIdxHLT[i][j] << ' ';
			}
			std::cout << std::endl;
		}

		std::cout << "fired ";
		for (size_t i = 0; i < event.m_lumiMetadata->hltNames.size(); ++i){
			product.m_HltTrigger[event.m_lumiMetadata->hltNames[i]] = false;
			if (event.m_eventMetadata->bitsHLT & (1ul << i)){
				std::cout << event.m_lumiMetadata->hltNames[i] << " " ;
			}
		}
		std::cout << std::endl;
		//for (auto& kv : product.m_HltTrigger) {
		//	std::cout << kv.first << " has value " << std::boolalpha << kv.second << std::endl;
		//}
	}

};

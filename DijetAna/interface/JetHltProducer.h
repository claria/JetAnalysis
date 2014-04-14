/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "JetTypes.h"

class JetHltProducer : public JetProducerBase {
public:

	virtual std::string GetProducerId() const {
		return "JetHltProducer";
	}

	virtual void ProduceGlobal(JetEvent const& event,
			JetProduct & product,
			JetGlobalSettings const& globalSettings) const
	{
		for (size_t i = 0; i < event.m_lumiMetadata->hltNames.size(); ++i){
			product.m_HltTrigger[event.m_lumiMetadata->hltNames[i]] = false;
			if (event.m_eventMetadata->bitsHLT & (1ul << i)){
				product.m_HltTrigger[event.m_lumiMetadata->hltNames[i]] = true;
			}
		}
		//for (auto& kv : product.m_HltTrigger) {
		//	std::cout << kv.first << " has value " << std::boolalpha << kv.second << std::endl;
		//}
	}

};

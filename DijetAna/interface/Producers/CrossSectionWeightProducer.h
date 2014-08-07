/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "../JetTypes.h"

class CrossSectionWeightProducer : public JetProducerBase {
public:

	virtual std::string GetProducerId() const {
		return "CrossSectionWeightProducer";
	}

	virtual void Produce(JetEvent const& event,
			JetProduct & product,
			JetSettings const& settings) const
		{
			if (settings.GetXSection() > 0.)
				product.m_weights["CrossSectionPerEventWeight"] = settings.GetXSection();
			else if (event.m_genLumiMetadata->xSectionExt > 0.)
				product.m_weights["CrossSectionPerEventWeight"] = event.m_genLumiMetadata->xSectionExt;
			else if (event.m_genLumiMetadata->xSectionInt > 0.)
				product.m_weights["CrossSectionPerEventWeight"] = event.m_genLumiMetadata->xSectionInt;
			else
				LOG(INFO) << "No CrossSection information found.";

		}

};

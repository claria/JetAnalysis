/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Consumer/interface/ValueModifier.h"

#include "Artus/Consumer/interface/DrawHist1dConsumer.h"
#include "Artus/Consumer/interface/ProfileConsumerBase.h"

#include "JetTypes.h"

#include "JetPipelineSettings.h"
#include "JetEvent.h"
#include "JetProduct.h"

// filter
//#include "PtFilter.h"

// consumer
#include "JetNtupleConsumer.h"
//#include "JetObservables.h"

class JetPipelineInitializer: public PipelineInitilizerBase<JetTypes> {
	public:

		virtual void InitPipeline(JetPipeline * pLine,
				JetPipelineSettings const& pset) const ARTUS_CPP11_OVERRIDE
		{

			auto extractPt =
				[]( JetEvent const& event, JetProduct const & product )
				-> std::vector<float> {
					std::vector<float> ptjets(product.m_validJets.size());
					transform(product.m_validJets.begin(),product.m_validJets.end(), ptjets.begin(),
							[](KDataPFJet* pfjet){ return (float)pfjet->p4.Pt(); });
					return ptjets;
				};

			auto PtValue = std::make_pair(extractPt,
					DefaultModifiers::getGenericModifier(0., 2000., 50));

			BOOST_FOREACH(std::string id, pset.GetConsumer())
			{
				if (id == "quantities_all")
				{
					//pLine->AddConsumer(new DrawHist1dConsumerBase<JetTypes>("npv", NPVValue));
					pLine->AddConsumer(new DrawHist1dConsumerBase<JetTypes>("pT", PtValue));
					//pLine->AddConsumer(new DrawHist1dConsumerBase<JetTypes>("nJet", NJetValue));
				}
			}
		}
};

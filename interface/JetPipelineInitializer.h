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
#include "PreselectionFilter.h"
#include "JetObservables.h"

class JetPipelineInitializer: public PipelineInitilizerBase<JetTypes> {
public:

	virtual void InitPipeline(JetPipeline * pLine,
			JetPipelineSettings const& pset) const ARTUS_CPP11_OVERRIDE
			{

		// define how to extract Pt and the range
		//auto extractNPV =
		//		[]( JetEvent const& ev, JetProduct const & prod )
		//		-> std::vector<float> {return {(float)ev.m_vertexsummary->nVertices};};
		//auto extractNJet =
		//		[]( JetEvent const& ev, JetProduct const & prod )
		//		-> std::vector<float> {return {(float) ev.m_ak5pfJets->size()};};
		//auto extractPt =
		//		[]( JetEvent const& ev, JetProduct const & prod )
		//		-> std::vector<float> {
		//			std::vector<float> ptjets(ev.m_ak5pfJets->size());
		//			transform(ev.m_ak5pfJets->begin(),ev.m_ak5pfJets->end(), ptjets.begin(),
		//					[](KDataPFJet pfjet){ return (float)pfjet.p4.Pt(); });
		//			return ptjets;
		//		};

		//auto NPVValue = std::make_pair(extractNPV,
		//		DefaultModifiers::getGenericModifier(0., 50., 50));
		//auto NJetValue = std::make_pair(extractNJet,
		//		DefaultModifiers::getGenericModifier(-0.5, 20.5, 21));
		//auto PtValue = std::make_pair(extractPt,
		//		DefaultModifiers::getGenericModifier(0., 1000., 50));



		BOOST_FOREACH(std::string filterId, pset.GetFilters())
		{
			if(filterId == PreselectionFilter().GetFilterId()) {
				pLine->AddFilter(new PreselectionFilter());
			}
			else {
				LOG_FATAL("Filter \"" << filterId << "\" not found.");
			}
		}


		BOOST_FOREACH(std::string producerId, pset.GetLocalProducers())
		{
			if(producerId == JetObservables().GetProducerId()) {
				pLine->AddProducer(new JetObservables());
			}
			else {
				LOG_FATAL("Producer \"" << producerId << "\" not found.");
			}
		}

		BOOST_FOREACH(std::string id, pset.GetConsumer())
		{
			if (id == "quantities_all")
			{
				//pLine->AddConsumer(new DrawHist1dConsumerBase<JetTypes>("npv", NPVValue));
				//pLine->AddConsumer(new DrawHist1dConsumerBase<JetTypes>("pT", PtValue));
				//pLine->AddConsumer(new DrawHist1dConsumerBase<JetTypes>("nJet", NJetValue));
			}
			else if (id == "ntuple")
				pLine->AddConsumer(new JetNtupleConsumer);
		}

	}
};

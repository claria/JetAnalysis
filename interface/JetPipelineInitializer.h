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
//#include "MeanPtConsumer.h"
//#include "JetNtupleConsumer.h"

class JetPipelineInitializer: public PipelineInitilizerBase<JetTypes> {
public:

	virtual void InitPipeline(JetPipeline * pLine,
			JetPipelineSettings const& pset) const ARTUS_CPP11_OVERRIDE
			{

		// define how to extract Pt and the range
		auto extractNPV =
				[]( JetEvent const& ev, JetGlobalProduct const & gm, JetLocalProduct const & lm )
				-> std::vector<float> {return {(float)ev.m_vertexsummary->nVertices};};
		auto extractPT =
				[]( JetEvent const& ev, JetGlobalProduct const & gm, JetLocalProduct const & lm )
				-> std::vector<float> {return {(float)ev.m_vertexsummary->nVertices};};

		auto NPVValue = std::make_pair(extractNPV,
				DefaultModifiers::getGenericModifier(0., 50., 50));

		pLine->AddConsumer(
						new DrawHist1dConsumerBase<JetTypes>("npv", NPVValue));

/*
		//pLine->AddFilter(new PtFilter());
		//pLine->AddConsumer(new MeanPtConsumer());

		//typedef std::function<
				//std::vector<float>(event_type const&, global_product_type const&,
						//local_product_type const&)> ValueExtractLambda;
		//typedef std::pair<ValueExtractLambda, ValueModifiers> ValueDesc;

		// define how to extract Pt and the range
		auto extractPtSim =
				[]( JetEvent const& ev, JetGlobalProduct const & gm, JetLocalProduct const & lm )
				-> std::vector<float> {return {ev.m_floatPtSim};};
		auto PtSimValue = std::make_pair(extractPtSim,
				DefaultModifiers::getPtModifier(0.7, 1.3f));

		// extracts the value which has been corrected by a globalProducer
		auto extractPtSimCorrected =
				[]( JetEvent const& ev, JetGlobalProduct const & gm, JetLocalProduct const & lm )
				-> std::vector<float> {return {gm.m_floatPtSim_corrected};};
		auto PtSimCorrectedValue = std::make_pair(extractPtSimCorrected,
				DefaultModifiers::getPtModifier(0.7, 1.3f));

		// define how to extract Theta and the range
		auto extractThetaSim =
				[]( JetEvent const& ev, JetGlobalProduct const & gm, JetLocalProduct const & lm )
				-> std::vector<float> {return {ev.m_floatTheSim};};

		auto ThetaSimValue = std::make_pair(extractThetaSim,
				DefaultModifiers::getThetaModifier());


		BOOST_FOREACH(std::string id, pset.GetConsumer())
		{
			if (id == "quantities_all")
			{
				// plot Pt
				pLine->AddConsumer(
						new DrawHist1dConsumerBase<JetTypes>("pt", PtSimValue));

				// plot Pt - corrected, from the global product
				pLine->AddConsumer(
						new DrawHist1dConsumerBase<JetTypes>("pt_corr", PtSimCorrectedValue));


				// plot Theta
				pLine->AddConsumer(
						new DrawHist1dConsumerBase<JetTypes>("theta", ThetaSimValue));

				// profile Pt over Theta
				pLine->AddConsumer(
						new ProfileConsumerBase<JetTypes>("pt_over_theta",
							ThetaSimValue, PtSimValue));
			}
			else if (id == "ntuple")
				pLine->AddConsumer(new JetNtupleConsumer);
		}*/

	}
};

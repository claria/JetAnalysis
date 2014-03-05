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
#include "JetObservables.h"

class JetPipelineInitializer: public PipelineInitilizerBase<JetTypes> {
public:

	virtual void InitPipeline(JetPipeline * pLine,
			JetPipelineSettings const& pset) const ARTUS_CPP11_OVERRIDE
			{}
};

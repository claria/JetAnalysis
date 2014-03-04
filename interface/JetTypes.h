/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/ProducerBase.h"
#include "Artus/Core/interface/PipelineRunner.h"

#include "Artus/KappaAnalysis/interface/KappaPipelineRunner.h"


#include "JetEvent.h"
#include "JetProduct.h"
#include "JetPipelineSettings.h"

// all data types which are used for this analysis
struct JetTypes {
	typedef JetEvent event_type;
	typedef JetProduct product_type;
	typedef JetPipelineSettings setting_type;
	typedef JetGlobalSettings global_setting_type;
};

// Pass the template parameters for the Producers
typedef ProducerBase<JetTypes> JetProducerBase;

// Pass the template parameters for the Consumer
typedef ConsumerBase<JetTypes> JetConsumerBase;

// Pass the template parameters for the Filter
typedef FilterBase<JetTypes> JetFilterBase;

//Pass the template parameters for the Pipeline
typedef Pipeline<JetTypes> JetPipeline;

//Setup our custom pipeline runner
typedef KappaPipelineRunner<JetTypes, JetPipeline, JetProducerBase> JetPipelineRunner;


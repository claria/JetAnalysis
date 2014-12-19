#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/ProducerBase.h"
#include "Artus/Core/interface/Pipeline.h"
#include "Artus/Core/interface/PipelineRunner.h"

#include "Artus/KappaAnalysis/interface/KappaPipelineInitializer.h"


#include "JetEvent.h"
#include "JetProduct.h"
#include "JetSettings.h"

// all data types which are used for this analysis
struct JetTypes {
	typedef JetEvent event_type;
	typedef JetProduct product_type;
	typedef JetSettings setting_type;
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
typedef PipelineRunner<JetPipeline, JetTypes> JetPipelineRunner;
typedef KappaPipelineInitializer<JetTypes> JetPipelineInitializer;


#pragma once

#include "Artus/Consumer/interface/ValueModifier.h"

#include "Artus/Consumer/interface/DrawHist1dConsumer.h"
#include "Artus/Consumer/interface/ProfileConsumerBase.h"

#include "JetTypes.h"

#include "JetSettings.h"
#include "JetEvent.h"
#include "JetProduct.h"

// filter
//#include "PtFilter.h"

// consumer
#include "JetNtupleConsumer.h"
//#include "JetObservables.h"

class JetPipelineInitializer : public PipelineInitilizerBase<JetTypes> {
 public:
  virtual void InitPipeline(JetPipeline *pLine, JetPipelineSettings const &pset) const override {}
};

#pragma once

#include "Artus/KappaAnalysis/interface/KappaFactory.h"

#include "JetTypes.h"

#include "JetSettings.h"
#include "JetEvent.h"
#include "JetProduct.h"

class JetFactory : public KappaFactory /*<JetTypes> */ {
 public:
  JetFactory();
  ~JetFactory();
  virtual ProducerBaseUntemplated* createProducer(std::string const& id) override;
  virtual ConsumerBaseUntemplated* createConsumer(std::string const& id) override;
  virtual FilterBaseUntemplated* createFilter(std::string const& id) override;
};

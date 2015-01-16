#pragma once

#include "Artus/KappaAnalysis/interface/Producers/ValidJetsProducer.h"
#include "Artus/KappaAnalysis/interface/KappaTypes.h"

/**
   \brief Producer for valid jets (simple PF jets).
   
*/
class JetValidJetsProducer: public ValidJetsProducer
{
private:
	float minValidJetPt;
	float minValidJetAbsRap;
	float maxValidJetAbsRap;
protected:
	virtual bool AdditionalCriteria(KBasicJet* jet, KappaEvent const& event, KappaProduct & product, KappaSettings const& settings) const override;
	virtual void Init(KappaSettings const& settings) override;
};

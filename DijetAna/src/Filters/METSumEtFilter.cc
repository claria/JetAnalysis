#include "JetAnalysis/DijetAna/interface/Filters/METSumEtFilter.h"


void METSumEtFilter::Init(JetSettings const& settings) {
	JetFilterBase::Init(settings);
	maxMETSumEtRatio = settings.GetMaxMETSumEtRatio();
}

bool METSumEtFilter::DoesEventPass(JetEvent const& event, 
		JetProduct const& product, 
		JetSettings const& settings) const {
	bool pass = false;
	if ((event.m_met->p4.Pt() / event.m_met->sumEt) < maxMETSumEtRatio) 
	{
		pass = true;
	}
	return pass;
}

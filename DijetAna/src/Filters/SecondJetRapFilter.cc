#include "JetAnalysis/DijetAna/interface/Filters/SecondJetRapFilter.h"

void SecondJetRapFilter::Init(JetSettings const& settings) {
	minSecondJetRap = settings.GetMinSecondJetRap();
	maxSecondJetRap = settings.GetMaxSecondJetRap();
}

bool SecondJetRapFilter::DoesEventPass(JetEvent const& event, 
		JetProduct const& product, JetSettings const& settings) const {

	bool pass = false;
	if ((abs(product.m_validJets.at(1)->p4.Rapidity()) >= minSecondJetRap) &&
	     abs(product.m_validJets.at(1)->p4.Rapidity()) <  maxSecondJetRap) {
		pass = true;
	}
	return pass;
}

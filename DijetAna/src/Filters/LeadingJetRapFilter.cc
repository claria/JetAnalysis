#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetRapFilter.h"

void LeadingJetRapFilter::Init(JetSettings const& settings) {
	minLeadingJetRap = settings.GetMinLeadingJetRap();
	maxLeadingJetRap = settings.GetMaxLeadingJetRap();
}

bool LeadingJetRapFilter::DoesEventPass(JetEvent const& event, 
		JetProduct const& product, JetSettings const& settings) const {

	bool pass = false;
	if ((abs(product.m_validJets.at(0)->p4.Rapidity()) >= minLeadingJetRap) &&
			abs(product.m_validJets.at(0)->p4.Rapidity()) <  maxLeadingJetRap) {
		pass = true;
	}
	return pass;
}

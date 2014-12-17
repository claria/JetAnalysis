#include "JetAnalysis/DijetAna/interface/Filters/LeadingJetPtFilter.h"

void LeadingJetPtFilter::Init(JetSettings const& settings) {
	minLeadingJetPt = settings.GetMinLeadingJetPt();
	maxLeadingJetPt = settings.GetMaxLeadingJetPt();
}

bool LeadingJetPtFilter::DoesEventPass(JetEvent const& event, 
		JetProduct const& product, JetSettings const& settings) const {
	bool pass = false;
	if ((std::abs(product.m_validJets.at(0)->p4.Pt()) >= minLeadingJetPt) &&
		 std::abs(product.m_validJets.at(0)->p4.Pt()) < maxLeadingJetPt) {
		pass = true;
	}
	return pass;
}

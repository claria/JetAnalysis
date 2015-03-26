#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/Producers/GenJetQuantitiesProducer.h"

std::string GenJetQuantitiesProducer::GetProducerId() const {
	return "GenJetQuantitiesProducer";
}

void GenJetQuantitiesProducer::Produce(JetEvent const& event, JetProduct & product, JetSettings const& settings) const
{
	// auto & jetProduct = static_cast <JetProduct &> (product);

	if (event.m_genJets->size() > 1) {
		product.genobs1 = event.m_genJets->at(0).p4.Pt();
		product.genobs2 = event.m_genJets->at(0).p4.Rapidity();
		product.genobs3 = event.m_genJets->at(1).p4.Rapidity();
	}

// 	for (auto i = product.m_validJets.begin(); i != product.m_validJets.end(); i++) {
// 		jetProduct.m_incJetsPt.push_back((*i)->p4.Pt());
// 		jetProduct.m_incJetsEta.push_back((*i)->p4.Eta());
// 		jetProduct.m_incJetsRap.push_back((*i)->p4.Rapidity());
// 		jetProduct.m_incJetsPhi.push_back((*i)->p4.Phi());
// 	}
//
}



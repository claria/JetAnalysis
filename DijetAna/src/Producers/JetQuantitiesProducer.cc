#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/Producers/JetQuantitiesProducer.h"

std::string JetQuantitiesProducer::GetProducerId() const {
	return "JetQuantitiesProducer";
}

void JetQuantitiesProducer::Produce(JetEvent const& event, JetProduct & product, JetSettings const& settings) const
{
	auto & jetProduct = static_cast <JetProduct &> (product);

	if (product.m_validJets.size() > 1) {
		product.obs1 = product.m_validJets.at(0)->p4.Pt();
		product.obs2 = product.m_validJets.at(0)->p4.Rapidity();
		product.obs3 = product.m_validJets.at(1)->p4.Rapidity();
	}

	for (auto i = product.m_validJets.begin(); i != product.m_validJets.end(); i++) {
		jetProduct.m_incJetsPt.push_back((*i)->p4.Pt());
		jetProduct.m_incJetsEta.push_back((*i)->p4.Eta());
		jetProduct.m_incJetsRap.push_back((*i)->p4.Rapidity());
		jetProduct.m_incJetsPhi.push_back((*i)->p4.Phi());
	}

}



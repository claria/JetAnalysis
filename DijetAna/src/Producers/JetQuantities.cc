#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/Producers/JetQuantities.h"

std::string JetQuantities::GetProducerId() const {
	return "JetQuantities";
}

void JetQuantities::Produce(JetEvent const& event, JetProduct & product, JetSettings const& settings) const
{
	auto & jetProduct = static_cast <JetProduct &> (product);

	for ( auto i = product.m_validJets.begin(); i != product.m_validJets.end(); i++ ) {
		jetProduct.m_incJetsPt.push_back((*i)->p4.Pt());
		jetProduct.m_incJetsEta.push_back((*i)->p4.Eta());
		jetProduct.m_incJetsRap.push_back((*i)->p4.Rapidity());
		jetProduct.m_incJetsPhi.push_back((*i)->p4.Phi());
	}

}



#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/Producers/GenJetMatchingProducer.h"

std::string GenJetMatchingProducer::GetProducerId() const {
	return "GenJetMatchingProducer";
}

void GenJetMatchingProducer::Produce(JetEvent const& event, JetProduct & product, JetSettings const& settings) const
{
	product.m_matchedGenJets.resize(product.m_validJets.size());
	// auto & jetProduct = static_cast <JetProduct &> (product)

	for (std::vector<KBasicJet*>::iterator validJet = product.m_validJets.begin();
			validJet != product.m_validJets.end(); ++validJet)
	{
		double minDeltaR = 999.;
		product.m_matchedGenJets[validJet - product.m_validJets.begin()] = NULL;
		for (std::vector<KLV>::iterator genJet = event.m_genJets->begin();
				genJet != event.m_genJets->end(); ++genJet) {
			double deltaR = ROOT::Math::VectorUtil::DeltaR((*validJet)->p4, genJet->p4);
			// TODO: Make deltaR configurable
			if (deltaR < 0.3) {
				if (deltaR < minDeltaR) {
					minDeltaR = deltaR;
					product.m_matchedGenJets[validJet - product.m_validJets.begin()] = &(*genJet);
				}
			}
		}
	}
}



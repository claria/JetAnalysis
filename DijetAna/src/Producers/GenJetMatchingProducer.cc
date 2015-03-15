#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/Producers/GenJetMatchingProducer.h"

std::string GenJetMatchingProducer::GetProducerId() const {
	return "GenJetMatchingProducer";
}

void GenJetMatchingProducer::Init(JetSettings const& settings) {
	maxDeltaR = settings.GetMaxDeltaR();
}

void GenJetMatchingProducer::Produce(JetEvent const& event, JetProduct & product, JetSettings const& settings) const
{
	product.m_matchedGenJets.resize(product.m_validJets.size());
	// auto & jetProduct = static_cast <JetProduct &> (product)
	// TODO Poor/slow code but works...
	// Produce matched genjets to vector of valid reco jets
	for (std::vector<KBasicJet*>::iterator validJet = product.m_validJets.begin();
			validJet != product.m_validJets.end(); ++validJet)
	{
		double minDeltaR = 999.;
		product.m_matchedGenJets[validJet - product.m_validJets.begin()] = NULL;
		for (std::vector<KLV>::iterator genJet = event.m_genJets->begin();
				genJet != event.m_genJets->end(); ++genJet) 
		{
			double deltaR = ROOT::Math::VectorUtil::DeltaR((*validJet)->p4, genJet->p4);
			// TODO: Make deltaR configurable
			if (deltaR < maxDeltaR) {
				if (deltaR < minDeltaR) {
					minDeltaR = deltaR;
					product.m_matchedGenJets[validJet - product.m_validJets.begin()] = &(*genJet);
				}
			}
		}
	}
	product.m_matchedRecoJets.resize(event.m_genJets->size());
	// Produce matched recojets to vector of gen jets
	for (std::vector<KLV>::iterator genJet = event.m_genJets->begin();
				genJet != event.m_genJets->end(); ++genJet)
	{
		double minDeltaR = 999.;
		product.m_matchedRecoJets[genJet - event.m_genJets->begin()] = NULL;
		for (std::vector<KBasicJet*>::iterator validJet = product.m_validJets.begin();
				validJet != product.m_validJets.end(); ++validJet)
		{
			double deltaR = ROOT::Math::VectorUtil::DeltaR((*validJet)->p4, genJet->p4);
			// TODO: Make deltaR configurable
			if (deltaR < maxDeltaR) {
				if (deltaR < minDeltaR) 
				{
					minDeltaR = deltaR;
					product.m_matchedRecoJets[genJet - event.m_genJets->begin()] = *validJet;
				}
			}
		}
	}

}



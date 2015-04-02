#include "Artus/Core/interface/Cpp11Support.h"
#include "JetAnalysis/DijetAna/interface/Producers/GenJetMatchingProducer.h"
#include <KappaTools/RootTools/Matching.h>

std::string GenJetMatchingProducer::GetProducerId() const {
	return "GenJetMatchingProducer";
}

void GenJetMatchingProducer::Init(JetSettings const& settings) {
	maxDeltaR = settings.GetMaxDeltaR();
}

void GenJetMatchingProducer::Produce(JetEvent const& event, JetProduct & product, JetSettings const& settings) const
{
	// auto & jetProduct = static_cast <JetProduct &> (product)
	// TODO Poor/slow code but works...
	// Produce matched genjets to vector of valid reco jets
	//
	std::vector<KBasicJet> validJets;
	for (auto jet : product.m_validJets)
		validJets.push_back(*jet);

	std::vector<int> match_result_genjets = matchSort_Matrix<KLV, KBasicJet>(*(event.m_genJets), event.m_genJets->size(), validJets, validJets.size(), maxDeltaR);
	std::vector<int> match_result_recojets = matchSort_Matrix<KBasicJet, KLV>(validJets, validJets.size(), *(event.m_genJets), event.m_genJets->size(), maxDeltaR);

	product.m_matchedGenJets.resize(product.m_validJets.size());
	product.m_matchedRecoJets.resize(event.m_genJets->size());

	// std::cout << "recojets size " << validJets.size() << std::endl;
	// std::cout << "genjets size " << event.m_genJets->size() << std::endl;
	// std::cout << "matchresultgenjets size " << match_result_genjets.size() << std::endl;
	// std::cout << "matchresultrecojets size " << match_result_recojets.size() << std::endl;

	for (std::vector<KBasicJet*>::iterator validJet = product.m_validJets.begin(); validJet != product.m_validJets.end(); ++validJet)
	{
		size_t idx = validJet - product.m_validJets.begin();
		if (match_result_genjets[idx] < 0)
			product.m_matchedGenJets.at(idx) = NULL;
		else
			product.m_matchedGenJets.at(idx) = &(event.m_genJets->at(match_result_genjets[idx]));
	}

	for (std::vector<KLV>::iterator genJet = event.m_genJets->begin(); genJet != event.m_genJets->end(); ++genJet)
	{
		size_t idx = genJet - event.m_genJets->begin();
		if (match_result_recojets[idx] < 0)
			product.m_matchedRecoJets.at(idx) = NULL;
		else
			product.m_matchedRecoJets.at(idx) = product.m_validJets.at(match_result_recojets[idx]);
	}

	// for (size_t i=0;i<match_result_recojets.size();i++){
	// 	std::cout << i << " " << match_result_recojets[i] << std::endl;;
	// }

	
	// for (std::vector<KBasicJet*>::iterator validJet = product.m_validJets.begin();
	// 		validJet != product.m_validJets.end(); ++validJet)
	// {
	// 	double minDeltaR = maxDeltaR;
	// 	product.m_matchedGenJets[validJet - product.m_validJets.begin()] = NULL;
	// 	for (std::vector<KLV>::iterator genJet = event.m_genJets->begin();
	// 			genJet != event.m_genJets->end(); ++genJet) 
	// 	{
	// 		double deltaR = ROOT::Math::VectorUtil::DeltaR((*validJet)->p4, genJet->p4);
	// 		if (deltaR < minDeltaR) {
	// 			if(std::find(product.m_matchedGenJets.begin(), product.m_matchedGenJets.end(), &(*genJet)) != product.m_matchedGenJets.end()) 
	// 			{
	// 				continue;
	// 			}
	// 			minDeltaR = deltaR;
	// 			product.m_matchedGenJets[validJet - product.m_validJets.begin()] = &(*genJet);
	// 		}
	// 	}
	// }
	// product.m_matchedRecoJets.resize(event.m_genJets->size());
	// // Produce matched recojets to vector of gen jets
	// for (std::vector<KLV>::iterator genJet = event.m_genJets->begin();
	// 			genJet != event.m_genJets->end(); ++genJet)
	// {
	// 	double minDeltaR = maxDeltaR;
	// 	product.m_matchedRecoJets[genJet - event.m_genJets->begin()] = NULL;
	// 	for (std::vector<KBasicJet*>::iterator validJet = product.m_validJets.begin();
	// 			validJet != product.m_validJets.end(); ++validJet)
	// 	{
	// 		double deltaR = ROOT::Math::VectorUtil::DeltaR((*validJet)->p4, genJet->p4);
	// 		if (deltaR < minDeltaR) 
	// 		{
	// 			if(std::find(product.m_matchedRecoJets.begin(), product.m_matchedRecoJets.end(), *validJet) != product.m_matchedRecoJets.end()) 
	// 			{
	// 				continue;
	// 			}
	// 			minDeltaR = deltaR;
	// 			product.m_matchedRecoJets[genJet - event.m_genJets->begin()] = *validJet;
	// 		}
	// 	}
	// }

}



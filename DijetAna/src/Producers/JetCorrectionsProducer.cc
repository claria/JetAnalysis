#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include "JetAnalysis/DijetAna/interface/Producers/JetCorrectionsProducer.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#define USE_JEC
#include "KappaTools/RootTools/JECTools.h"

std::string JetCorrectionsProducer::GetProducerId() const {
  return "JetCorrectionsProducer";
}

void JetCorrectionsProducer::Init(JetSettings const& settings) {
  LOG(WARNING) << "Using the jetana JEC corrections producer.";
  LOG(DEBUG) << "\tLoading JEC correction parameters from files...";
  assert(!settings.GetJetEnergyCorrectionParameters().empty());

  std::vector<JetCorrectorParameters> jecParameters;

  for (std::vector<std::string>::const_iterator jecParametersFile = settings.GetJetEnergyCorrectionParameters().begin();
       jecParametersFile != settings.GetJetEnergyCorrectionParameters().end();
       ++jecParametersFile) {
    jecParameters.push_back(JetCorrectorParameters(*jecParametersFile));
  }
  factorizedJetCorrector = new FactorizedJetCorrector(jecParameters);
}

void JetCorrectionsProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  // Copy jets from event.
  for (auto jet : *event.m_basicJets) {
    product.m_corrJets.push_back(jet);
  }

  // Apply correction in place
  for (auto & jet : product.m_corrJets) {
    factorizedJetCorrector->setRho(static_cast<float>(event.m_pileupDensity->rho));
    factorizedJetCorrector->setNPV(event.m_vertexSummary->nVertices);
    correctSingleJet(jet, factorizedJetCorrector);
  }
  // Sort jets after pT
  std::sort(product.m_corrJets.begin(),
            product.m_corrJets.end(),
            [](KBasicJet jet1, KBasicJet jet2)
                -> bool { return jet1.p4.Pt() > jet2.p4.Pt(); });
}

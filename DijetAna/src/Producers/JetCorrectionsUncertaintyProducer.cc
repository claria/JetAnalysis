#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include "JetAnalysis/DijetAna/interface/Producers/JetCorrectionsUncertaintyProducer.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#define USE_JEC
#include "KappaTools/RootTools/JECTools.h"

std::string JetCorrectionsUncertaintyProducer::GetProducerId() const { 
  return "JetCorrectionsUncertaintyProducer"; 
}

void JetCorrectionsUncertaintyProducer::Init(JetSettings const& settings) 
{
  LOG(WARNING) << "Using the new JEC Uncertainty producer.";
  LOG(DEBUG) << "\tLoading JetCorrectionUncertainty from files...";
  assert(! settings.GetJetEnergyCorrectionUncertaintyParameters().empty());

  JetCorrectorParameters * jecUncertaintyParameters = nullptr;

  if (!settings.GetJetEnergyCorrectionUncertaintySource().empty()) {
    jecUncertaintyParameters = new JetCorrectorParameters(settings.GetJetEnergyCorrectionUncertaintyParameters(), settings.GetJetEnergyCorrectionUncertaintySource());
  }
  else {
    jecUncertaintyParameters = new JetCorrectorParameters(settings.GetJetEnergyCorrectionUncertaintyParameters());
  }
  if ((!jecUncertaintyParameters->isValid()) || (jecUncertaintyParameters->size() == 0)) {
    LOG(FATAL) << "Invalid definition " << settings.GetJetEnergyCorrectionUncertaintySource() 
               << " in file " << settings.GetJetEnergyCorrectionUncertaintyParameters();
  }
  jetCorrectionUncertainty = new JetCorrectionUncertainty(*jecUncertaintyParameters);
  jetCorrectionUncertaintyShift = settings.GetJetEnergyCorrectionUncertaintyShift();

  LOG(DEBUG) << "\t\t" << settings.GetJetEnergyCorrectionUncertaintySource();
  LOG(DEBUG) << "\t\t" << settings.GetJetEnergyCorrectionUncertaintyParameters();


}

void JetCorrectionsUncertaintyProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const 
{

  for (auto jet : product.m_correctedJets) {
    applyUncertainty(*jet, jetCorrectionUncertainty, jetCorrectionUncertaintyShift);
  }

  std::sort(product.m_correctedJets.begin(), product.m_correctedJets.end(),
          [](std::shared_ptr<KBasicJet> jet1, std::shared_ptr<KBasicJet> jet2) -> bool
          { return jet1->p4.Pt() > jet2->p4.Pt(); });

}

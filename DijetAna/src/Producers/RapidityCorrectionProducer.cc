#include "JetAnalysis/DijetAna/interface/Producers/RapidityCorrectionProducer.h"

std::string RapidityCorrectionProducer::GetProducerId() const {
  return "RapidityCorrectionProducer";
}

void RapidityCorrectionProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);


  const std::string objectName = "res";
  LOG(DEBUG) << "\tLoading rapidity correction TGraph from files...";
  LOG(DEBUG) << "\t\t" << settings.GetRapidityCorrectionFile() << "/" << objectName;
  TFile file(settings.GetRapidityCorrectionFile().c_str(), "READONLY");
  m_rapidityCorrectionGraph = dynamic_cast<TGraphErrors*>(file.Get(objectName.c_str()));
  file.Close();
}

void RapidityCorrectionProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  for (auto & recojet : product.m_corrJets) {
      float corrRap = m_rapidityCorrectionGraph->Eval(recojet.p4.Rapidity());
      float corrPz = recojet.p4.E() * (exp(2*corrRap) -1)/(exp(2*corrRap) +1);
      recojet.p4.SetPxPyPzE(recojet.p4.Px(),recojet.p4.Py(),corrPz,recojet.p4.E());
    }
}

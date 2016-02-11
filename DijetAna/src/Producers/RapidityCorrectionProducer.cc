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
  m_rapidityCorrection = *(dynamic_cast<TProfile2D*>(file.Get(objectName.c_str())));
  file.Close();
}

void RapidityCorrectionProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  for (auto & recojet : product.m_corrJets) {
    if ((recojet.p4.Pt() > m_rapidityCorrection.GetXaxis()->GetXmin()) &&
        (recojet.p4.Pt() < m_rapidityCorrection.GetXaxis()->GetXmax()) &&
        (recojet.p4.Eta() < m_rapidityCorrection.GetYaxis()->GetXmax()) &&
        (recojet.p4.Eta() < m_rapidityCorrection.GetYaxis()->GetXmax()))
    {
      // double corrEta = recojet.p4.Eta() - m_rapidityCorrection.Interpolate(recojet.p4.Pt(), recojet.p4.Eta());
      double corrEta = recojet.p4.Eta() - m_rapidityCorrection.GetBinContent(m_rapidityCorrection.FindBin(recojet.p4.Pt(), recojet.p4.Eta()));
      // std::cout << "Etal will be corrected from " << recojet.p4.Eta() << " " << recojet.p4.Pt() << " to " << corrEta << std::endl;
      // float corrPz = recojet.p4.E() * (exp(2*corrRap) -1)/(exp(2*corrRap) +1);
      // recojet.p4.SetPxPyPzE(recojet.p4.Px(),recojet.p4.Py(),corrPz,recojet.p4.E());
      recojet.p4.SetEta(corrEta);
    }
  }
}

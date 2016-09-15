#include "JetAnalysis/DijetAna/interface/Producers/RapidityCorrectionProducer.h"

std::string RapidityCorrectionProducer::GetProducerId() const {
  return "RapidityCorrectionProducer";
}

void RapidityCorrectionProducer::Init(JetSettings const& settings) {
  JetProducerBase::Init(settings);


  const std::string objectName = "default/tp2_DeltaEta";
  LOG(DEBUG) << "\tLoading rapidity correction TGraph from files...";
  LOG(DEBUG) << "\t\t" << settings.GetRapidityCorrectionFile() << "/" << objectName;
  TDirectory *savedir(gDirectory);
  TFile *savefile(gFile);
  TFile file(settings.GetRapidityCorrectionFile().c_str(), "READONLY");
  m_rapidityCorrection = (TProfile2D*)(file.Get(objectName.c_str()));
  m_rapidityCorrection->SetDirectory(0);
  file.Close();
  gDirectory = savedir;
  gFile = savefile;
}

void RapidityCorrectionProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {
  for (auto & recojet : product.m_corrJets) {
    int ibin = m_rapidityCorrection->FindBin(recojet.p4.Pt(), recojet.p4.Rapidity());
    if (m_rapidityCorrection->IsBinOverflow(ibin))
      continue;
    // std::cout << "findbin " << m_rapidityCorrection->FindBin(recojet.p4.Pt(), recojet.p4.Rapidity()) << std::endl;
    double corrEta = recojet.p4.Eta() + m_rapidityCorrection->GetBinContent(ibin);
    // double corrRap = recojet.p4.Rapidity() + m_rapidityCorrection->GetBinContent(ibin);
    // std::cout << "corrRap" << corrRap << " deltay " << m_rapidityCorrection->GetBinContent(ibin) << std::endl;
    // std::cout << "Etal will be corrected from " << recojet.p4.Eta() << " " << recojet.p4.Pt() << " to " << corrEta << std::endl;
    // double corrPz = (recojet.p4.E() * (std::exp(2*corrRap) -1))/(std::exp(2*corrRap) +1);
    // recojet.p4.SetPxPyPzE(recojet.p4.Px(),recojet.p4.Py(),corrPz,recojet.p4.E());
    // double corrEta = recojet.p4.Eta() - m_rapidityCorrection->GetBinContent(m_rapidityCorrection->FindBin(recojet.p4.Pt(), recojet.p4.Rapidity()));
    // double corrM = std::sqrt(std::pow(corrE,2)-std::pow(recojet.p4.P(),2));
    // std::cout << "RapidityCorrectionProducer: Before correction: pT=" << recojet.p4.Pt() 
    //                                                         << " Eta=" << recojet.p4.Eta() 
    //                                                         << " Rap=" << recojet.p4.Rapidity() 
    //                                                         << " Phi=" << recojet.p4.Phi() 
    //                                                         << " E=" << recojet.p4.E() 
    //                                                         << " M=" << recojet.p4.M() 
    //                                                         << std::endl;
    recojet.p4.SetEta(corrEta);
    // recojet.p4.SetPxPyPzE(recojet.p4.Px(),recojet.p4.Py(),corrPz,recojet.p4.E());
    // recojet.p4.SetPxPyPzE(recojet.p4.Px(),recojet.p4.Py(),corrPz,recojet.p4.E());
    // 
    // std::cout << "RapidityCorrectionProducer: After correction: pT=" << recojet.p4.Pt() 
    //                                                         << " Eta=" << recojet.p4.Eta() 
    //                                                         << " Rap=" << recojet.p4.Rapidity() 
    //                                                         << " Phi=" << recojet.p4.Phi() 
    //                                                         << " E=" << recojet.p4.E() 
    //                                                         << " M=" << recojet.p4.M() 
    //                                                         << std::endl;
  }
}

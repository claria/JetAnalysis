#include "JetAnalysis/DijetAna/interface/Producers/JetQuantitiesProducer.h"

std::string JetQuantitiesProducer::GetProducerId() const {
  return "JetQuantitiesProducer";
}

void JetQuantitiesProducer::Init(setting_type const& settings) {
  ProducerBase<JetTypes>::Init(settings);
  // dummy histogram used to find bin index
  m_h_ptavg = new TH1D("h_ptavg", "h_ptavg", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h_ptavg->Sumw2();
}

void JetQuantitiesProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {

  // Inclusive Jets
  for (auto & jet : product.m_validRecoJets) {
    product.m_incJetsPt.push_back(jet.p4.Pt());
    product.m_incJetsEta.push_back(jet.p4.Eta());
    product.m_incJetsRap.push_back(jet.p4.Rapidity());
    product.m_incJetsPhi.push_back(jet.p4.Phi());
  }

  product.m_jet1Pt = (product.m_validRecoJets.size() > 0) ? product.m_validRecoJets.at(0).p4.Pt() : -999.;
  product.m_jet1Eta = (product.m_validRecoJets.size() > 0) ? product.m_validRecoJets.at(0).p4.Eta() : -999.;
  product.m_jet1Rap = (product.m_validRecoJets.size() > 0) ? product.m_validRecoJets.at(0).p4.Rapidity() : -999.;
  product.m_jet1Phi = (product.m_validRecoJets.size() > 0) ? product.m_validRecoJets.at(0).p4.Phi() : -999.;

  product.m_jet2Pt = (product.m_validRecoJets.size() > 1) ? product.m_validRecoJets.at(1).p4.Pt() : -999.;
  product.m_jet2Eta = (product.m_validRecoJets.size() > 1) ? product.m_validRecoJets.at(1).p4.Eta() : -999.;
  product.m_jet2Rap = (product.m_validRecoJets.size() > 1) ? product.m_validRecoJets.at(1).p4.Rapidity() : -999.;
  product.m_jet2Phi = (product.m_validRecoJets.size() > 1) ? product.m_validRecoJets.at(1).p4.Phi() : -999.;

  product.m_dijet_mass = (product.m_validRecoJets.size() > 1)
                                ? (product.m_validRecoJets.at(0).p4 + product.m_validRecoJets.at(1).p4).mass()
                                : -999.;
  product.m_dijet_ptavg = (product.m_validRecoJets.size() > 1)
                                 ? 0.5 * (product.m_validRecoJets.at(0).p4.Pt() + product.m_validRecoJets.at(1).p4.Pt())
                                 : -999.;
  product.m_dijet_ymax = (product.m_validRecoJets.size() > 1) ? std::max(product.m_validRecoJets.at(0).p4.Rapidity(),
                                                                           product.m_validRecoJets.at(1).p4.Rapidity())
                                                                : -999.;
  product.m_dijet_yinner = (product.m_validRecoJets.size() > 1)
                                  ? ((std::abs(product.m_validRecoJets.at(0).p4.Rapidity()) <=
                                      std::abs(product.m_validRecoJets.at(1).p4.Rapidity()))
                                         ? product.m_validRecoJets.at(0).p4.Rapidity()
                                         : product.m_validRecoJets.at(1).p4.Rapidity())
                                  : -999.;
  product.m_dijet_youter = (product.m_validRecoJets.size() > 1)
                                  ? ((std::abs(product.m_validRecoJets.at(0).p4.Rapidity()) >
                                      std::abs(product.m_validRecoJets.at(1).p4.Rapidity()))
                                         ? product.m_validRecoJets.at(0).p4.Rapidity()
                                         : product.m_validRecoJets.at(1).p4.Rapidity())
                                  : -999.;
  product.m_dijet_deltaPhi =
      (product.m_validRecoJets.size() > 1)
          ? std::abs(product.m_validRecoJets.at(0).p4.Phi() - product.m_validRecoJets.at(1).p4.Phi())
          : -999.;
  product.m_dijet_deltaR =
      (product.m_validRecoJets.size() > 1)
          ? ROOT::Math::VectorUtil::DeltaR(product.m_validRecoJets.at(0).p4, product.m_validRecoJets.at(1).p4)
          : -999.;
  product.m_dijet_jet12PtRatio =
      (product.m_validRecoJets.size() > 1)
          ? product.m_validRecoJets.at(1).p4.Pt()/product.m_validRecoJets.at(0).p4.Pt()
          : -999.;
  product.m_dijet_cosThetaStar =
      (product.m_validRecoJets.size() > 1)
          ? std::tanh(product.m_validRecoJets.at(0).p4.Rapidity() - product.m_validRecoJets.at(1).p4.Rapidity())
          : -999.;
  product.m_dijet_yboost =
      (product.m_validRecoJets.size() > 1)
          ? 0.5 * std::abs(product.m_validRecoJets.at(0).p4.Rapidity() + product.m_validRecoJets.at(1).p4.Rapidity())
          : -999.;
  product.m_dijet_yboostnoabs =
      (product.m_validRecoJets.size() > 1)
          ? 0.5 * (product.m_validRecoJets.at(0).p4.Rapidity() + product.m_validRecoJets.at(1).p4.Rapidity())
          : -999.;
  product.m_dijet_ystar =
      (product.m_validRecoJets.size() > 1)
          ? 0.5 * std::abs(product.m_validRecoJets.at(0).p4.Rapidity() - product.m_validRecoJets.at(1).p4.Rapidity())
          : -999.;
  product.m_dijet_ystarnoabs =
      (product.m_validRecoJets.size() > 1)
          ? 0.5 * (product.m_validRecoJets.at(0).p4.Rapidity() - product.m_validRecoJets.at(1).p4.Rapidity())
          : -999.;
  product.m_dijet_chi =
      (product.m_validRecoJets.size() > 1)
          ? exp(std::abs(product.m_validRecoJets.at(0).p4.Rapidity() - product.m_validRecoJets.at(1).p4.Rapidity()))
          : -999.;

  size_t i = (size_t)product.m_dijet_yboost;
  size_t j = (size_t)product.m_dijet_ystar;
  // index of histogram to be filled
  // yb0ys0 --> 0 yb0ys1 --> 1 ...
  product.m_dijet_ysbidx = j + 3*i - i*(i-1)/2;
  product.m_dijet_ptavgidx = m_h_ptavg->FindBin(product.m_dijet_ptavg)-1;
  product.m_dijet_idx = (m_h_ptavg->GetNbinsX() * product.m_dijet_ysbidx) + m_h_ptavg->FindBin(product.m_dijet_ptavg)-1; 
}

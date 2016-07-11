#include "Artus/Utility/interface/SafeMap.h"
#include "JetAnalysis/DijetAna/interface/Producers/GenJetQuantitiesProducer.h"

std::string GenJetQuantitiesProducer::GetProducerId() const {
  return "GenJetQuantitiesProducer";
}

void GenJetQuantitiesProducer::Init(setting_type const& settings) {
  ProducerBase<JetTypes>::Init(settings);
  // dummy histogram used to find bin index
  m_h_ptavg = new TH1D("h_ptavg_dummy", "h_ptavg_dummy", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h_ptavg->Sumw2();
}

void GenJetQuantitiesProducer::Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const {

  product.m_genjet1Pt = (product.m_validGenJets.size() > 0) ? product.m_validGenJets.at(0).p4.Pt() : -999.;
  product.m_genjet1Eta = (product.m_validGenJets.size() > 0) ? product.m_validGenJets.at(0).p4.Eta() : -999.;
  product.m_genjet1Rap = (product.m_validGenJets.size() > 0) ? product.m_validGenJets.at(0).p4.Rapidity() : -999.;
  product.m_genjet1Phi = (product.m_validGenJets.size() > 0) ? product.m_validGenJets.at(0).p4.Phi() : -999.;

  product.m_genjet2Pt = (product.m_validGenJets.size() > 1) ? product.m_validGenJets.at(1).p4.Pt() : -999.;
  product.m_genjet2Eta = (product.m_validGenJets.size() > 1) ? product.m_validGenJets.at(1).p4.Eta() : -999.;
  product.m_genjet2Rap = (product.m_validGenJets.size() > 1) ? product.m_validGenJets.at(1).p4.Rapidity() : -999.;
  product.m_genjet2Phi = (product.m_validGenJets.size() > 1) ? product.m_validGenJets.at(1).p4.Phi() : -999.;

  product.m_gendijet_mass = (product.m_validGenJets.size() > 1)
                                   ? (product.m_validGenJets.at(0).p4 + product.m_validGenJets.at(1).p4).mass()
                                   : -999.;
  product.m_gendijet_ptavg =
      (product.m_validGenJets.size() > 1)
          ? 0.5 * (product.m_validGenJets.at(0).p4.Pt() + product.m_validGenJets.at(1).p4.Pt())
          : -999.;
  product.m_gendijet_jet12PtRatio =
      (product.m_validGenJets.size() > 1)
          ? (product.m_validGenJets.at(1).p4.Pt()/product.m_validGenJets.at(0).p4.Pt())
          : -999.;

  product.m_gendijet_ymax =
      (product.m_validGenJets.size() > 1)
          ? std::max(product.m_validGenJets.at(0).p4.Rapidity(), product.m_validGenJets.at(1).p4.Rapidity())
          : -999.;
  product.m_gendijet_yinner = (product.m_validGenJets.size() > 1)
                                     ? ((std::abs(product.m_validGenJets.at(0).p4.Rapidity()) <=
                                         std::abs(product.m_validGenJets.at(1).p4.Rapidity()))
                                            ? product.m_validGenJets.at(0).p4.Rapidity()
                                            : product.m_validGenJets.at(1).p4.Rapidity())
                                     : -999.;
  product.m_gendijet_youter = (product.m_validGenJets.size() > 1)
                                     ? ((std::abs(product.m_validGenJets.at(0).p4.Rapidity()) >
                                         std::abs(product.m_validGenJets.at(1).p4.Rapidity()))
                                            ? product.m_validGenJets.at(0).p4.Rapidity()
                                            : product.m_validGenJets.at(1).p4.Rapidity())
                                     : -999.;
  product.m_gendijet_deltaPhi =
      (product.m_validGenJets.size() > 1)
          ? std::abs(product.m_validGenJets.at(0).p4.Phi() - product.m_validGenJets.at(1).p4.Phi())
          : -999.;
  product.m_gendijet_cosThetaStar =
      (product.m_validGenJets.size() > 1)
          ? std::tanh(product.m_validGenJets.at(0).p4.Rapidity() - product.m_validGenJets.at(1).p4.Rapidity())
          : -999.;
  product.m_gendijet_yboost =
      (product.m_validGenJets.size() > 1)
          ? 0.5 * std::abs(product.m_validGenJets.at(0).p4.Rapidity() + product.m_validGenJets.at(1).p4.Rapidity())
          : -999.;
  product.m_gendijet_ystar =
      (product.m_validGenJets.size() > 1)
          ? 0.5 * std::abs(product.m_validGenJets.at(0).p4.Rapidity() - product.m_validGenJets.at(1).p4.Rapidity())
          : -999.;
  product.m_gendijet_chi =
      (product.m_validGenJets.size() > 1)
          ? exp(std::abs(product.m_validGenJets.at(0).p4.Rapidity() - product.m_validGenJets.at(1).p4.Rapidity()))
          : -999.;

  size_t i = (size_t)product.m_gendijet_yboost;
  size_t j = (size_t)product.m_gendijet_ystar;
  // index of histogram to be filled
  // yb0ys0 --> 0 yb0ys1 --> 1 ...
  product.m_gendijet_ysbidx = j + 3*i - i*(i-1)/2;
  product.m_gendijet_ptavgidx = m_h_ptavg->FindBin(product.m_gendijet_ptavg)-1;
  product.m_gendijet_idx = (m_h_ptavg->GetNbinsX() * product.m_gendijet_ysbidx) + product.m_gendijet_ptavgidx; 

  if (product.m_validGenJets.size() > 1 &&
      product.m_matchedRecoJets.count(&product.m_validGenJets.at(0)) && product.m_matchedRecoJets.find(&product.m_validGenJets.at(0))->second &&
      product.m_matchedRecoJets.count(&product.m_validGenJets.at(1)) && product.m_matchedRecoJets.find(&product.m_validGenJets.at(1))->second )
  {
    product.m_genmatchdijet_ptavg = 0.5 * (product.m_matchedRecoJets.at(&product.m_validGenJets.at(0))->p4.Pt() + product.m_matchedRecoJets.at(&product.m_validGenJets.at(1))->p4.Pt());
    product.m_genmatchdijet_yboost = 0.5 * std::abs(product.m_matchedRecoJets.at(&product.m_validGenJets.at(0))->p4.Rapidity() + product.m_matchedRecoJets.at(&product.m_validGenJets.at(1))->p4.Rapidity());
    product.m_genmatchdijet_ystar = 0.5 * std::abs(product.m_matchedRecoJets.at(&product.m_validGenJets.at(0))->p4.Rapidity() - product.m_matchedRecoJets.at(&product.m_validGenJets.at(1))->p4.Rapidity());

    i = (size_t)product.m_genmatchdijet_yboost;
    j = (size_t)product.m_genmatchdijet_ystar;
    // index of histogram to be filled
    // yb0ys0 --> 0 yb0ys1 --> 1 ...
    product.m_genmatchdijet_ysbidx = j + 3*i - i*(i-1)/2;
    product.m_genmatchdijet_ptavgidx = m_h_ptavg->FindBin(product.m_genmatchdijet_ptavg)-1;
    product.m_genmatchdijet_idx = (m_h_ptavg->GetNbinsX() * product.m_genmatchdijet_ysbidx) + product.m_genmatchdijet_ptavgidx; 
  }
  if (product.m_validGenJets.size() > 0 && product.m_matchedRecoJets.count(&product.m_validGenJets.at(0)) && product.m_matchedRecoJets.find(&product.m_validGenJets.at(0))->second)
  {
    product.m_genmatchjet1Pt = product.m_matchedRecoJets.at(&product.m_validGenJets.at(0))->p4.Pt();
    product.m_genmatchjet1Eta = product.m_matchedRecoJets.at(&product.m_validGenJets.at(0))->p4.Eta();
    product.m_genmatchjet1Rap = product.m_matchedRecoJets.at(&product.m_validGenJets.at(0))->p4.Rapidity();
    product.m_genmatchjet1Phi = product.m_matchedRecoJets.at(&product.m_validGenJets.at(0))->p4.Phi();
  }
  if (product.m_validGenJets.size() > 1 && product.m_matchedRecoJets.count(&product.m_validGenJets.at(1)) && product.m_matchedRecoJets.find(&product.m_validGenJets.at(1))->second)
  {
    product.m_genmatchjet2Pt = product.m_matchedRecoJets.at(&product.m_validGenJets.at(1))->p4.Pt();
    product.m_genmatchjet2Eta = product.m_matchedRecoJets.at(&product.m_validGenJets.at(1))->p4.Eta();
    product.m_genmatchjet2Rap = product.m_matchedRecoJets.at(&product.m_validGenJets.at(1))->p4.Rapidity();
    product.m_genmatchjet2Phi = product.m_matchedRecoJets.at(&product.m_validGenJets.at(1))->p4.Phi();
  }

}

#include <TH1.h>
#include <TF1.h>
#include "TROOT.h"
#include "TMinuit.h"

#include "Artus/Utility/interface/RootFileHelper.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetResolutionConsumer.h"

void JetResolutionConsumer::Init(setting_type const& settings) {
  ConsumerBase<JetTypes>::Init(settings);
  RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
  m_hGenVsRecoPt = new TH2D("hgenvsrecopt", "hgenvsrecopt", 100, 0.5, 1.5, settings.GetPtBinning().size() - 1,
                            &settings.GetPtBinning()[0]);
}

void JetResolutionConsumer::ProcessFilteredEvent(event_type const& event, product_type const& product,
                                                 setting_type const& settings) {
  double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;

  m_hGenVsRecoPt->Fill(product.m_validJets.at(0)->p4.Pt() / event.m_genJets->at(0).p4.Pt(),
                       event.m_genJets->at(0).p4.Pt(), eventWeight);
}

void JetResolutionConsumer::Finish(setting_type const& settings) {
  size_t nObsBins = m_hGenVsRecoPt->GetNbinsY();
  std::cout << nObsBins << std::endl;
  graph_resolution = new TGraphErrors(nObsBins);
  for (size_t i = 1; i < nObsBins; ++i) {
    double obsbin_center = m_hGenVsRecoPt->GetYaxis()->GetBinCenter(i);
    std::cout << "Obs bin " << i << std::endl;
    std::cout << m_hGenVsRecoPt->GetYaxis()->GetBinCenter(i) << std::endl;

    TH1D* proj = m_hGenVsRecoPt->ProjectionX("test", i, i);
    proj->Draw();
    proj->Fit("gaus");
    std::cout << "failtest 0" << std::endl;
    TF1* gaussFunction = proj->GetFunction("gaus");
    if (gaussFunction == NULL) continue;
    std::cout << "failtest 1" << std::endl;
    // double mu = gaussFunction->GetParameter(0);
    double sigma = gaussFunction->GetParameter(2);
    std::cout << "failtest 2" << std::endl;
    // double sigma_err = gaussFunction->GetParError(1);
    std::cout << "failtest 3" << std::endl;
    graph_resolution->SetPoint(i, obsbin_center, sigma);
    graph_resolution->SetPointError(i, m_hGenVsRecoPt->GetYaxis()->GetBinWidth(i), gaussFunction->GetParError(1));
  }
  std::cout << "failtest 4" << std::endl;
  RootFileHelper::SafeCd(settings.GetRootOutFile(), "default/");
  graph_resolution->Write("resolution");
}

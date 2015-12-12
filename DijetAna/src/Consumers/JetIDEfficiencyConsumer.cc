#include <TH1.h>
#include "TGraphAsymmErrors.h"
#include "TROOT.h"

#include "Artus/Utility/interface/RootFileHelper.h"
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetIDEfficiencyConsumer.h"

/**
   \brief Calculates the jet id efficiency using a tag-and-probe method.
*/

void JetIDEfficiencyConsumer::Init(JetSettings const& settings) {
  ConsumerBase<JetTypes>::Init(settings);

  m_h_doPassJetID = new TH1D("h_probepassjetid", "h_probepassjetid", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h_doPassJetID->Sumw2();
  m_h_allPassJetID = new TH1D("h_allpassjetid", "h_allpassjetid", settings.GetPtBinning().size() - 1, &settings.GetPtBinning()[0]);
  m_h_allPassJetID->Sumw2();
}

void JetIDEfficiencyConsumer::ProcessFilteredEvent(JetEvent const& event, JetProduct const& product, JetSettings const& settings) {
  // Event Weight
  // double eventWeight = product.m_weights.find(settings.GetEventWeight())->second;
  if (product.m_dijet_deltaPhi >= 2.8) {
    if (product.m_doPassID.at(&product.m_validRecoJets.at(0)) == true) {
      m_h_allPassJetID->Fill(product.m_dijet_ptavg);
      if (product.m_doPassID.at(&product.m_validRecoJets.at(1)) == true) {
        m_h_doPassJetID->Fill(product.m_dijet_ptavg);
      }
      if (product.m_doPassID.at(&product.m_validRecoJets.at(1)) == true) {
        m_h_allPassJetID->Fill(product.m_dijet_ptavg);
        if (product.m_doPassID.at(&product.m_validRecoJets.at(0)) == true) {
          m_h_doPassJetID->Fill(product.m_dijet_ptavg);
        }
      }

    }
  }
}

void JetIDEfficiencyConsumer::Finish(JetSettings const& settings) {
  m_h_doPassJetID->Write();
  m_h_allPassJetID->Write();
}

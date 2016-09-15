#pragma once

#include "../JetTypes.h"

class JERScalingProducer : public JetProducerBase {
 public:
  virtual std::string GetProducerId() const override;
  virtual void Produce(JetEvent const& event, JetProduct& product, JetSettings const& settings) const override;
  virtual void Init(JetSettings const& settings) override;
  double GetScalingFactor(double eta) const;
 private:
  std::vector<double> m_jerScalingFactors;
  double maxDeltaR;
};


double get_sigma_rel(double pt, double eta);

struct matchSort_deltaRdeltaPtVar
{
  matchSort_deltaRdeltaPtVar(const double dR = 0.3) : maxDeltaR(dR) {};

  template<typename T1, typename T2>
  double operator()(const T1 &obj1, const T2 &obj2) const
  {
    const double val1 = ROOT::Math::VectorUtil::DeltaR(obj1.p4, obj2.p4);
    const double val2 = std::abs(obj1.p4.pt() - obj2.p4.pt()) / obj2.p4.pt();
    // std::cout << "pt eta sigmarel delta " << obj2.p4.pt() << " " << obj2.p4.eta() << " " << get_sigma_rel(obj2.p4.pt(), obj2.p4.eta())<< " " << val2 << std::endl;
    return (val1 < maxDeltaR && val2 < 1.5*get_sigma_rel(obj2.p4.pt(), obj2.p4.eta())) ? val1 : std::numeric_limits<double>::quiet_NaN();
  }
  const double maxDeltaR;
};


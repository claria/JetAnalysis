#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class DiJetsRapFilter: public JetFilterBase {

	public:
		virtual ~DiJetsRapFilter() {
		}

		virtual void InitGlobal(JetGlobalSettings const& globalSettings) {
			maxJetRapCut = globalSettings.GetMaxJetRapCut();
		}

		virtual std::string GetFilterId() const {
			return "DiJetsRapFilter";
		}

		virtual bool DoesEventPassGlobal(JetEvent const& event,
				JetProduct const& product,
				JetGlobalSettings const& settings) const {

			bool pass = true;

			if (abs(product.m_validJets.at(0)->p4.Rapidity()) >= maxJetRapCut ||
				abs(product.m_validJets.at(1)->p4.Rapidity()) >= maxJetRapCut) {
				pass = false;
			}
			return pass;
		}
	private:
		double maxJetRapCut;
};

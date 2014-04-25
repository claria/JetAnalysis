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

		virtual void InitLocal(JetPipelineSettings const& pipelineSettings) {
			maxJetRapCut = pipelineSettings.GetMaxJetRapCut();
		}

		virtual std::string GetFilterId() const {
			return "DiJetsRapFilter";
		}

		virtual bool DoesEventPassGlobal(JetEvent const& event,
				JetProduct const& product,
				JetGlobalSettings const& settings) const {

			return DoesEventPass(event, product);
		}

		virtual bool DoesEventPassLocal(JetEvent const& event, 
				JetProduct const& product,
				JetGlobalSettings const& settings) const {

			return DoesEventPass(event, product);
		}

		virtual bool DoesEventPass(JetEvent const& event, 
				JetProduct const& product) const {

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

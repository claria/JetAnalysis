#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class DiJetsPtFilter: public JetFilterBase {
	public:

		virtual ~DiJetsPtFilter() {
		}

		virtual std::string GetFilterId() const {
			return "DiJetsPtFilter";
		}

		virtual void InitGlobal(JetGlobalSettings const& globalSettings) {
			minJetPtCut = globalSettings.GetMinJetPtCut();
		}

		virtual bool DoesEventPassGlobal(JetEvent const& event,
				JetProduct const& product,
				JetGlobalSettings const& globalSettings) const {

			bool pass = true;

			if (product.m_validJets.at(0)->p4.Pt() < minJetPtCut ||
				product.m_validJets.at(1)->p4.Pt() < minJetPtCut) {
				pass = false;
			}
			return pass;
		}

	private:
		double minJetPtCut;
};


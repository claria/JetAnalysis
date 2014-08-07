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

		virtual void Init(JetSettings const& settings) {
			minJetPtCut = settings.GetMinJetPtCut();
		}

		virtual bool DoesEventPass(JetEvent const& event,
				JetProduct const& product,
				JetSettings const& settings) const {

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


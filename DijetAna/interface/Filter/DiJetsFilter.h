#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class DiJetsFilter: public JetFilterBase {
	public:

		virtual ~DiJetsFilter() {
		}

		virtual std::string GetFilterId() const {
			return "DiJetsFilter";
		}

		virtual bool DoesEventPassGlobal(JetEvent const& event,
				JetProduct const& product,
				JetGlobalSettings const& settings) const {

			bool pass = true;

			if (product.m_validJets.size() < 2) {
				pass = false;
			}
			return pass;
		}
};

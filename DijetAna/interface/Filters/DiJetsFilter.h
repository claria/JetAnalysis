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

		virtual bool DoesEventPass(JetEvent const& event,
				JetProduct const& product,
				JetSettings const& settings) const {

			bool pass = true;

			if (product.m_validJets.size() < 2) {
				pass = false;
			}
			return pass;
		}
};
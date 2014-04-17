#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class JetHltFilter: public JetFilterBase {
	public:

		virtual ~JetHltFilter() {
		}

		virtual std::string GetFilterId() const {
			return "JetHltFilter";
		}

		virtual bool DoesEventPassGlobal(JetEvent const& event,
				JetProduct const& product,
				JetGlobalSettings const& settings) const {

			bool pass = false;
			for (size_t i = 0; i < event.m_lumiMetadata->hltNames.size(); ++i)
				if (event.m_eventMetadata->bitsHLT & (1ul << i))
					pass = true;

			return pass;
		}
};

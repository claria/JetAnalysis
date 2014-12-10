#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Core/interface/FilterBase.h"

#include "../JetTypes.h"

class JetHltFilter: public JetFilterBase {
	public:

		virtual std::string GetFilterId() const;
		virtual bool DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const;
};

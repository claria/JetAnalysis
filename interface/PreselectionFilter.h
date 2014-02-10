
#pragma once

#include "Artus/Core/interface/Cpp11Support.h"

#include "JetTypes.h"

class PreselectionFilter: public JetFilterBase {
public:

	virtual ~PreselectionFilter() {
	}

	virtual std::string GetFilterId() {
		return "filter_preselection";
	}

	virtual bool DoesEventPass(JetEvent const& event,
							JetProduct const& product,
							JetPipelineSettings const& settings ) {

		bool passPreselection = true;
		//if(event.m_ak5pfJets->size() < 2)
		//	passPreselection = false;

		return passPreselection;
	}
};



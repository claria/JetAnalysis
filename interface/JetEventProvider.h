/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/KappaAnalysis/interface/KappaEventProviderBase.h"
#include "KappaTools/Toolbox/ProgressMonitor.h"

#include "JetTypes.h"
/*
 * Will load the corresponding ntuple from a root file
 * The memory locations are passed to ROOT one time, in the
 * WireEvent() method call.
 */
class JetEventProvider: public KappaEventProviderBase<JetTypes> {
public:
	JetEventProvider(FileInterface2& fi, InputTypeEnum inpType) :
			KappaEventProviderBase<JetTypes>(fi,inpType) {

	//virtual void WireEvent(global_setting_type const&);
	}

	virtual void WireEvent(global_setting_type const& globalSettings)
		ARTUS_CPP11_OVERRIDE
	{
		m_event.m_vertexsummary = m_fi.Get<KVertexSummary> ("offlinePrimaryVerticesSummary", false);
		m_event.m_pfJets = m_fi.Get<KDataPFJets>(globalSettings.GetJets(), true);
	}



};

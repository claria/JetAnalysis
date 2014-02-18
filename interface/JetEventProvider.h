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

		m_event.m_pfJets = m_fi.Get<KDataPFJets>(globalSettings.GetJets(), true);

		m_event.m_vertexSummary = m_fi.Get<KVertexSummary>("offlinePrimaryVerticesSummary", false);
		//m_event.m_beamSpot = m_fi.Get<KDataBeamSpot>("offlineBeamSpot");
		m_event.m_jetArea = m_fi.Get<KJetArea>("KT6Area");
		m_event.m_pfMet = m_fi.Get<KDataPFMET>("PFMET");
		//m_event.m_filtermetadata = m_fi.GetMeta<KFilterMetadata>("KFilterMetadata");
		//m_event.m_filter = m_fi.Get<KFilterSummary>("filterSummary");

	}



};

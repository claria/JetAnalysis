/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Provider/interface/KappaEventProvider.h"
#include "JetAnalysis/DijetAna/interface/JetEvent.h"
#include "KappaTools/Toolbox/ProgressMonitor.h"

/*
 * Will load the corresponding ntuple from a root file
 * The memory locations are passed to ROOT one time, in the
 * WireEvent() method call.
 */
class JetEventProvider: public KappaEventProvider<JetEvent> {
public:
	JetEventProvider(FileInterface2& fi, InputTypeEnum inpType) :
			KappaEventProvider<JetEvent>(fi,inpType) {

		WireEvent();

	}

	void WireEvent() {
		m_event.m_vertexsummary = m_fi.Get<KVertexSummary> ("offlinePrimaryVerticesSummary", false);
	}



};

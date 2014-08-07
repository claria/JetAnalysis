/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/KappaAnalysis/interface/KappaEventProvider.h"

#include "JetTypes.h"
/*
 * Will load the corresponding ntuple from a root file
 * The memory locations are passed to ROOT one time, in the
 * WireEvent() method call.
 */
class JetEventProvider: public KappaEventProvider<JetTypes> {
public:

	JetEventProvider(FileInterface2& fi, InputTypeEnum inpType) :
			KappaEventProvider<JetTypes>(fi,inpType) {}

	virtual void WireEvent(JetSettings const& settings)
		ARTUS_CPP11_OVERRIDE
	{
		KappaEventProvider::WireEvent(settings);
	}



};

/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */
#include "JetAnalysis/DijetAna/interface/JetEventProvider.h"

/*
 * Will load the corresponding ntuple from a root file
 * The memory locations are passed to ROOT one time, in the
 * WireEvent() method call.
 */

JetEventProvider::JetEventProvider(FileInterface2& fi, InputTypeEnum inpType) :
	KappaEventProvider<JetTypes>(fi,inpType) {}

void JetEventProvider::WireEvent(JetSettings const& settings)
{
	KappaEventProvider::WireEvent(settings);
}

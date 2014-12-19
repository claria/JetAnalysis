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

	JetEventProvider(FileInterface2& fi, InputTypeEnum inpType);

	virtual void WireEvent(JetSettings const& settings) ARTUS_CPP11_OVERRIDE;



};

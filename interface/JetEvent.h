#pragma once

#include <sstream>
#include <Kappa/DataFormats/interface/Kappa.h>
#include <KappaTools/RootTools/FileInterface2.h>
#include <Kappa/DataFormats/interface/KDebug.h>

#include "Artus/KappaAnalysis/interface/KappaEventBase.h"

class JetEvent : public KappaEventBase
{

public:
	JetEvent() : KappaEventBase() {};

	KGenEventMetadata* m_geneventmetadata;
	KEventMetadata* m_eventmetadata;
	KLumiMetadata* m_lumimetadata;
	KGenLumiMetadata* m_genlumimetadata;
	KFilterMetadata* m_filtermetadata;
	KVertexSummary* m_vertexsummary;
	KFilterSummary* m_filter;


	KDataPFJets* m_ak5pfJets;
	KDataPFJets* m_ak7pfJets;

	KGenLumiMetadata* GetGenLumiMetadata() const
	{
		return (KGenLumiMetadata*) m_lumimetadata;
	}

	KDataLumiMetadata* GetDataLumiMetadata() const
	{
		return (KDataLumiMetadata*) m_lumimetadata;
	}

};

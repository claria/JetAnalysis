#pragma once

#include <sstream>
#include <Kappa/DataFormats/interface/Kappa.h>
#include <KappaTools/RootTools/FileInterface2.h>
#include <Kappa/DataFormats/interface/KDebug.h>

class JetEvent
{

public:
	KDataPFJets* PF_jets; // = fi.Get<KDataPFJets>(names[0]);
	KGenEventMetadata* m_geneventmetadata;
	KEventMetadata* m_eventmetadata;
	KLumiMetadata* m_lumimetadata;
	KGenLumiMetadata* m_genlumimetadata;
	KFilterMetadata* m_filtermetadata;
	KVertexSummary* m_vertexsummary;
	KFilterSummary* m_filter;


	KGenLumiMetadata* GetGenLumiMetadata() const
	{
		return (KGenLumiMetadata*) m_lumimetadata;
	}

	KDataLumiMetadata* GetDataLumiMetadata() const
	{
		return (KDataLumiMetadata*) m_lumimetadata;
	}

};

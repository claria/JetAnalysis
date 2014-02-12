#pragma once

#include "Artus/KappaAnalysis/interface/KappaEvent.h"

class JetEvent : public KappaEvent
{
public:
	JetEvent() : KappaEvent() {};

	//KGenEventMetadata* m_geneventmetadata;
	//KEventMetadata* m_eventmetadata;
	//KLumiMetadata* m_lumimetadata;
	//KGenLumiMetadata* m_genlumimetadata;
	//KFilterMetadata* m_filtermetadata;
	//KVertexSummary* m_vertexsummary;
	//KFilterSummary* m_filter;


	//KDataPFJets* m_ak5pfJets;
	//KDataPFJets* m_ak7pfJets;

	//KGenLumiMetadata* GetGenLumiMetadata() const
	//{
	//	return (KGenLumiMetadata*) m_lumimetadata;
	//}

	//KDataLumiMetadata* GetDataLumiMetadata() const
	//{
	//	return (KDataLumiMetadata*) m_lumimetadata;
	//}

};

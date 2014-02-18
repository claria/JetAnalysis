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


	// Jets
	KDataPFJets* m_pfJets = 0;
	KJetArea* m_jetArea = 0;


	KDataPFMET* m_pfMet;
	KGenParticles* m_particles;

	//Metadata
	KGenEventMetadata* m_geneventmetadata;
	KEventMetadata* m_eventmetadata;
	KLumiMetadata* m_lumimetadata;
	KGenLumiMetadata* m_genlumimetadata;
	//KFilterMetadata* m_filtermetadata;
	KVertexSummary* m_vertexSummary;
	//KDataBeamSpot* m_beamSpot;
	//KFilterSummary* m_filter;


	KGenLumiMetadata* GetGenLumiMetadata() const
	{
		return (KGenLumiMetadata*) m_lumimetadata;
	}

	KDataLumiMetadata* GetDataLumiMetadata() const
	{
		return (KDataLumiMetadata*) m_lumimetadata;
	}

};

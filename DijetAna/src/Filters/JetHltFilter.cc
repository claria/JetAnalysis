#include "JetAnalysis/DijetAna/interface/Filters/JetHltFilter.h"
#include "JetAnalysis/DijetAna/interface/JetTypes.h"

std::string JetHltFilter::GetFilterId() const {
	return "JetHltFilter";
}

bool JetHltFilter::DoesEventPass(JetEvent const& event, JetProduct const& product, JetSettings const& settings) const {

	if (product.m_selectedHltName.empty())
	{
		return false;
	}
	else
	{
		std::string hltPath = product.m_hltInfo.getHLTName(product.m_selectedHltName);
		return event.m_eventMetadata->hltFired(hltPath, event.m_lumiMetadata);;
	}

}


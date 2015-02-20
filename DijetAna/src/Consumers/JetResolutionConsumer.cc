#include <TH1.h>
#include <TF1.h>
#include "TROOT.h"
#include "TMinuit.h"

#include "Artus/Core/interface/Cpp11Support.h"
#include "Artus/Utility/interface/RootFileHelper.h"

#include "JetAnalysis/DijetAna/interface/Consumers/JetResolutionConsumer.h"

void JetResolutionConsumer::Init(setting_type const& settings)
{
	ConsumerBase<JetTypes>::Init(settings);
	RootFileHelper::SafeCd(settings.GetRootOutFile(), settings.GetRootFileFolder());
	// Get Level 1 Pipelines
	// for ( auto pipeline : settings.GetPipelineInfos())
	// {
	// 	if (pipeline.second == 1) {
	// 		m_pipelineNames.push_back(pipeline.first);
	// 		TH2D* histo = (TH2D*)RootFileHelper::SafeGet<TH2D>(settings.GetRootOutFile(),
	// 				(pipeline.first + "/h2GenVsRecoPt").c_str());
	// 		m_h2GenVsRecoPt.push_back(histo);
	// 	}
	// }
	// get th2d 
	TH2D* histo = (TH2D*)RootFileHelper::SafeGet<TH2D>(settings.GetRootOutFile(),
			"default/h2GenVsRecoPt");
	m_h2GenVsRecoPt.push_back(histo);
}

void JetResolutionConsumer::Process(setting_type const& settings)
{
	for (std::vector<TH2D*>::iterator histo=m_h2GenVsRecoPt.begin(); histo!=m_h2GenVsRecoPt.end(); ++histo)
	{
		size_t nObsBins = (*histo)->GetNbinsY();
		std::cout << nObsBins << std::endl;
		// graph_resolution = new TGraphErrors(nObsBins);
		for (size_t i=1; i<nObsBins; ++i) {
			double obsbin_center = (*histo)->GetYaxis()->GetBinCenter(i);
			std::cout << "Obs bin " << i << std::endl;
			std::cout << (*histo)->GetYaxis()->GetBinCenter(i) << std::endl;

			TH1D* proj = (*histo)->ProjectionX("test", i, i);
			proj->Fit("gaus");
			int status = gMinuit->GetStatus();
			if (status == 0) {
				std::cout << status << std::endl;
				TF1* gaussFunction = proj->GetFunction("gaus");
				// double mu = gaussFunction->GetParameter(0);
				double sigma = gaussFunction->GetParameter(2);
				// double sigma_err = gaussFunction->GetParError(1);
				graph_resolution->SetPoint(i, obsbin_center, sigma);
				graph_resolution->SetPointError(i,(*histo)->GetYaxis()->GetBinWidth(i), gaussFunction->GetParError(1));
			}
		}
	}

}

void JetResolutionConsumer::Finish(setting_type const& settings)
{
	//for (std::vector<std::string>::size_type i = 0; i < m_pipelineNames.size(); i++)
	//{
	//	RootFileHelper::SafeCd(settings.GetRootOutFile(), m_pipelineNames[i]);
	//}
	RootFileHelper::SafeCd(settings.GetRootOutFile(), "default/");
	graph_resolution->Write("resolution");

}

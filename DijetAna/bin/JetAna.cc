#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Artus/Configuration/interface/ArtusConfig.h"
#include "Artus/Configuration/interface/RootEnvironment.h"

#include "JetAnalysis/DijetAna/interface/JetTypes.h"
#include "JetAnalysis/DijetAna/interface/JetEventProvider.h"
//#include "JetAnalysis/DijetAna/interface/JetPipelineInitializer.h"
//#include "JetAnalysis/DijetAna/interface/ValidDiJetsProducer.h"
#include "JetAnalysis/DijetAna/interface/JetFactory.h"

//#include "Artus/KappaAnalysis/interface/Producers/ValidJetsProducer.h"
//#include "JetAnalysis/DijetAna/interface/ValidDiJetsProducer.h"
/*
        This example implements a simple dummy anaylsis which
        reads entries from a root file and produces various pt plots

        It can be run with the config file data/exampleConfig.json
*/

int main(int argc, char** argv) {
  // parse the command line and load the
  ArtusConfig myConfig(argc, argv);

  // load the settings from the config file
  JetSettings settings = myConfig.GetSettings<JetSettings>();
  // create the output root file
  boost::scoped_ptr<TFile> rootOutputFile(new TFile(myConfig.GetOutputPath().c_str(), "RECREATE"));

  // will load the Ntuples from the root file
  // this must be modified if you want to load more/new quantities

  FileInterface2 finterface(myConfig.GetInputFiles());
  JetEventProvider evtProvider(finterface, (settings.GetInputIsData() ? DataInput : McInput));
  evtProvider.WireEvent(settings);

  // the pipeline initializer will setup the pipeline, with
  // all the attached Producer, Filer and Consumer
  JetPipelineInitializer pInit;
  JetFactory factory;

  JetPipelineRunner runner;

  // load the pipeline with their configuration from the config file
  myConfig.LoadConfiguration(pInit, runner, factory, rootOutputFile.get());

  // run all the configured pipelines and all their attached
  // consumers
  runner.RunPipelines(evtProvider, settings);

  // close output root file, pointer will be automatically
  // deleted
  rootOutputFile->Close();

  return 0;
}

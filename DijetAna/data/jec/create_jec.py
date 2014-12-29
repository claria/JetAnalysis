import FWCore.ParameterSet.Config as cms
process = cms.Process("jectxt")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
# define your favorite global tag
process.GlobalTag.globaltag = 'FT53_V21A_AN6::All'
# process.GlobalTag.globaltag = 'START53_V26::All'

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptySource")
process.readAK7PF    = cms.EDAnalyzer('JetCorrectorDBReader',  
      # below is the communication to the database 
      payloadName    = cms.untracked.string('AK7PF'),
      # this is used ONLY for the name of the printed txt files. You can use any name that you like, 
      # but it is recommended to use the GT name that you retrieved the files from.
      globalTag      = cms.untracked.string('FT53_V21A_AN6'),  
      printScreen    = cms.untracked.bool(False),
      createTextFile = cms.untracked.bool(True)
)
process.readAK7Calo = process.readAK7PF.clone(payloadName = 'AK7Calo')
process.readAK7JPT = process.readAK7PF.clone(payloadName = 'AK7JPT')
process.p = cms.Path(process.readAK7PF * process.readAK7Calo * process.readAK7JPT)

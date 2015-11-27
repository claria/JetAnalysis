import FWCore.ParameterSet.Config as cms

gt = 'START53_V27'

process = cms.Process("jectxt")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
# define your favorite global tag
process.GlobalTag.globaltag = '{}::All'.format(gt)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptySource")
process.readAK7PF    = cms.EDAnalyzer('JetCorrectorDBReader',  
      # below is the communication to the database 
      payloadName    = cms.untracked.string('AK7PF'),
      # this is used ONLY for the name of the printed txt files. You can use any name that you like, 
      # but it is recommended to use the GT name that you retrieved the files from.
      globalTag      = cms.untracked.string(gt),  
      printScreen    = cms.untracked.bool(False),
      createTextFile = cms.untracked.bool(True)
)
process.readAK5PF    = cms.EDAnalyzer('JetCorrectorDBReader',  
      # below is the communication to the database 
      payloadName    = cms.untracked.string('AK5PF'),
      # this is used ONLY for the name of the printed txt files. You can use any name that you like, 
      # but it is recommended to use the GT name that you retrieved the files from.
      globalTag      = cms.untracked.string(gt),  
      printScreen    = cms.untracked.bool(False),
      createTextFile = cms.untracked.bool(True)
)
process.readAK5Calo = process.readAK5PF.clone(payloadName = 'AK5Calo')
process.readAK5PFCHS = process.readAK7PF.clone(payloadName = 'AK5PFchs')
process.readAK5JPT = process.readAK5PF.clone(payloadName = 'AK5JPT')

process.readAK7Calo = process.readAK7PF.clone(payloadName = 'AK7Calo')
process.readAK7PFCHS = process.readAK7PF.clone(payloadName = 'AK7PFchs')
process.readAK7JPT = process.readAK7PF.clone(payloadName = 'AK7JPT')

process.p = cms.Path(process.readAK5PF*
                     process.readAK5JPT*
                     process.readAK5PFCHS*
                     process.readAK5Calo*
                     process.readAK7PF*
                     process.readAK7PFCHS*
                     process.readAK7Calo*
                     process.readAK7JPT)

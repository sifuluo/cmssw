import FWCore.ParameterSet.Config as cms

omtfStage2Raw = cms.EDProducer("OmtfPacker",
  cscInputLabel = cms.InputTag('omtfStage2Digis'),
  dtPhInputLabel = cms.InputTag('omtfStage2Digis'),
  dtThInputLabel = cms.InputTag('omtfStage2Digis'),
  rpcInputLabel = cms.InputTag('omtfStage2Digis'),
)

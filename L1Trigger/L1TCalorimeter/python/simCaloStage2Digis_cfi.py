import FWCore.ParameterSet.Config as cms

simCaloStage2Digis = cms.EDProducer(
    "L1TStage2Layer2Producer",
    towerToken = cms.InputTag("caloStage2Layer1Digis"),
    firmware = cms.int32(1)
)

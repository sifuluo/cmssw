import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

def L1TTestHLTSeeds(process):    
    process.load("L1Trigger.L1TGlobal.hackConditions_cff")

    process.hltGtStage2Digis = cms.EDProducer(
        "L1TRawToDigi",
        Setup           = cms.string("stage2::GTSetup"),
        FedIds          = cms.vint32( 1404 ),
        )
    process.hltCaloStage2Digis = cms.EDProducer(
        "L1TRawToDigi",
        Setup           = cms.string("stage2::CaloSetup"),
        FedIds          = cms.vint32( 1360, 1366 ),
        )
    process.hltGmtStage2Digis = cms.EDProducer(
        "L1TRawToDigi",
        Setup = cms.string("stage2::GMTSetup"),
        FedIds = cms.vint32(1402),
        )

    process.hltGtStage2ObjectMap = cms.EDProducer("L1TGlobalProducer",
                                                  MuonInputTag = cms.InputTag("hltGmtStage2Digis", "Muon"),
                                                  ExtInputTag = cms.InputTag("hltGtStage2Digis"), # (external conditions are not emulated, use unpacked)
                                                  EtSumInputTag = cms.InputTag("hltCaloStage2Digis", "EtSum"),
                                                  EGammaInputTag = cms.InputTag("hltCaloStage2Digis", "EGamma"),
                                                  TauInputTag = cms.InputTag("hltCaloStage2Digis", "Tau"),
                                                  JetInputTag = cms.InputTag("hltCaloStage2Digis", "Jet"),
                                                  AlgorithmTriggersUnprescaled = cms.bool(True),
                                                  AlgorithmTriggersUnmasked = cms.bool(True),
                                                  )

    process.HLTL1UnpackerSequence = cms.Sequence(
        process.hltGtStage2Digis +
        process.hltCaloStage2Digis +
        process.hltGmtStage2Digis +
        process.hltGtStage2ObjectMap
        )

    process.hltL1TSeed = cms.EDFilter( "HLTL1TSeed",
                                       L1SeedsLogicalExpression = cms.string( "L1_SingleEG10" ),
                                       L1ObjectMapInputTag  = cms.InputTag("hltGtStage2ObjectMap"),
                                       L1GlobalInputTag     = cms.InputTag("hltGtStage2Digis"),
                                       L1MuonInputTag       = cms.InputTag("hltGmtStage2Digis","Muon"),
                                       L1EGammaInputTag     = cms.InputTag("hltCaloStage2Digis","EGamma"),
                                       L1JetInputTag        = cms.InputTag("hltCaloStage2Digis","Jet"),
                                       L1TauInputTag        = cms.InputTag("hltCaloStage2Digis","Tau"),
                                       L1EtSumInputTag      = cms.InputTag("hltCaloStage2Digis","EtSum"),
                                       )

    process.L1THltTesting = cms.Sequence(
        process.hltGtStage2Digis +
        process.hltCaloStage2Digis +
        process.hltGmtStage2Digis +
        process.hltGtStage2ObjectMap +
        process.hltL1TSeed 
        )
    process.L1THltPath = cms.Path(process.L1THltTesting)    
    process.schedule.append(process.L1THltPath)

    return process


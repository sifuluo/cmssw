# to test the communication with DBS and produce the csctf configuration
import FWCore.ParameterSet.Config as cms

process = cms.Process("L1ConfigWritePayloadDummy")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cout.placeholder = cms.untracked.bool(False)
process.MessageLogger.cout.threshold = cms.untracked.string('INFO')
process.MessageLogger.debugModules = cms.untracked.vstring('*')

# Generate dummy L1TriggerKeyList
process.load("CondTools.L1Trigger.L1TriggerKeyListDummy_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.l1GtTriggerMenuOnline_cfi")

# Generate dummy L1TriggerKeyList
process.load("CondTools.L1Trigger.L1TriggerKeyDummy_cff")
process.L1TriggerKeyDummy.objectKeys = cms.VPSet(
    cms.PSet(
        record = cms.string('L1GtTriggerMenuRcd'),
        type = cms.string('L1GtTriggerMenu'),
        key = cms.string('L1Menu_Collisions2015_lowPU_25nsStage1_v5/L1T_Scales_20141121/Imp0/0x1039')
#        key = cms.string('L1Menu_Collisions2015_25nsStage1_v4/L1T_Scales_20141121/Imp0/0x1038')
#        key = cms.string('L1Menu_Collisions2015_lowPU_v4/L1T_Scales_20141121/Imp0/0x1037')
#        key = cms.string('L1Menu_Collisions2015_50nsGct_v4/L1T_Scales_20141121/Imp0/0x1036')
#        key = cms.string('L1Menu_Collisions2015_50nsGct_v4/L1T_Scales_20141121/Imp0/0x1036')
#        key = cms.string('L1Menu_Collisions2015_lowPU_v3/L1T_Scales_20141121/Imp0/0x1034')
#        key = cms.string('L1Menu_Collisions2015_50nsGct_v3/L1T_Scales_20141121/Imp0/0x1035')
#        key = cms.string('L1Menu_Collisions2015_25nsStage1_v3/L1T_Scales_20141121/Imp0/0x1031')
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
process.source = cms.Source("EmptySource")

process.getter = cms.EDAnalyzer("EventSetupRecordDataGetter",
   toGet = cms.VPSet(cms.PSet(
       record = cms.string('L1GtTriggerMenuRcd'),
       data   = cms.vstring('L1GtTriggerMenu')
   )),
   verbose = cms.untracked.bool(True)
)

process.l1mr = cms.EDAnalyzer("L1MenuReader")
process.l1mw = cms.EDAnalyzer("L1MenuWriter")

from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
outputDB = cms.Service("PoolDBOutputService",
    CondDBSetup,
    connect = cms.string('sqlite_file:l1config.db'),
    toPut   = cms.VPSet(
        cms.PSet(
            record = cms.string('L1GtTriggerMenuRcd'),
            tag = cms.string("L1GtTriggerMenu_lowPU_25nsStage1_v5")
        ),
        cms.PSet(
            record = cms.string("L1TriggerKeyListRcd"),
            tag = cms.string("L1TriggerKeyList_CRAFT09_hlt")
        )
    )
)
outputDB.DBParameters.authenticationPath = '.'
process.add_(outputDB)

process.p = cms.Path(process.getter + process.l1mw)


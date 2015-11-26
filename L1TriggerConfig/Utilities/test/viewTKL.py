import FWCore.ParameterSet.Config as cms

process = cms.Process("L1ConfigWritePayloadDummy")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cout.placeholder = cms.untracked.bool(False)
process.MessageLogger.cout.threshold = cms.untracked.string('DEBUG')
process.MessageLogger.debugModules = cms.untracked.vstring('*')

process.source = cms.Source("EmptySource", firstRun = cms.untracked.uint32(262318)) #91
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

#from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
#process.l1conddb = cms.ESSource("PoolDBESSource",
#       CondDBSetup,
#       connect = cms.string('sqlite_file:o2o/l1config.db'),
#       toGet   = cms.VPSet(
#            cms.PSet(
#                 record = cms.string('L1TriggerKeyExtRcd'),
#                 tag = cms.string('L1TriggerKeyExt_CRAFT09_hlt')
#            )
#       )
#)
#process.prefer("l1conddb")

# Following block of lines accesses CondDB
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_HLT_v1', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_74_V9', '')
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")

process.GlobalTag.toGet = cms.VPSet(
 cms.PSet(
           record  = cms.string("L1TriggerKeyListExtRcd"),
           tag     = cms.string("L1TriggerKeyListExt_CRAFT09_hlt"),
           connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
#           tag     = cms.string("L1TCaloParams_CRAFT09_hlt"),
#           connect = cms.untracked.string("frontier://FrontierPrep/CMS_CONDITIONS")
          )
)



process.l1tkr = cms.EDAnalyzer("L1TriggerKeyListViewer",
    TSC_KEY = cms.string("TSC_20151120_003624_collisions_BASE")
)

process.p = cms.Path(process.l1tkr)


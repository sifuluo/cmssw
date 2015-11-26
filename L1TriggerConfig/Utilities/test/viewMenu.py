import FWCore.ParameterSet.Config as cms

process = cms.Process("tester")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cout.placeholder = cms.untracked.bool(False)
process.MessageLogger.cout.threshold = cms.untracked.string('DEBUG')
process.MessageLogger.debugModules = cms.untracked.vstring('*')

process.source = cms.Source("EmptySource", firstRun = cms.untracked.uint32(262544)) #91
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
process.l1conddb = cms.ESSource("PoolDBESSource",
       CondDBSetup,
#       connect = cms.string('sqlite_file:/afs/cern.ch/user/t/tmatsush/public/L1Menu/L1Menu_Collisions2015_5TeV_pp_reference_v5/sqlFile/l1config.db'),
       connect = cms.string('sqlite_file:l1config.db'),
       toGet   = cms.VPSet(
            cms.PSet(
                 record = cms.string('L1GtTriggerMenuRcd'),
#                 tag = cms.string('L1Menu_Collisions2015_5TeV_pp_reference_v5')
                 tag = cms.string('L1GtTriggerMenu_CRAFT09_hlt')
            )
       )
)
process.prefer("l1conddb")

## Following block of lines accesses CondDB
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
##process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_HLT_v1', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_74_V9', '')
#process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
#
#process.GlobalTag.toGet = cms.VPSet(
# cms.PSet(
#           record  = cms.string("L1GtTriggerMenuRcd"),
#           tag     = cms.string("L1GtTriggerMenu_CRAFT09_hlt"),
#           connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
#          )
#)

process.l1cr = cms.EDAnalyzer("L1MenuViewer")

process.p = cms.Path(process.l1cr)


import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("L1TMuonEmulation", eras.Run2_2016)
import os
import sys
import commands

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(50)
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))

process.source = cms.Source('PoolSource',
 fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/g/gflouris/public/SingleMuPt6180_noanti_10k_eta1.root')
	                    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10))

# PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

#### Sim L1 Emulator Sequence:
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.dumpED = cms.EDAnalyzer("EventContentAnalyzer")
process.dumpES = cms.EDAnalyzer("PrintEventSetupContent")

process.l1tSummary = cms.EDAnalyzer("L1TSummary")
process.l1tSummary.tag   = cms.string("debugging L1T");
process.l1tSummary.egCheck   = cms.bool(True);
process.l1tSummary.tauCheck  = cms.bool(True);
process.l1tSummary.jetCheck  = cms.bool(True);
process.l1tSummary.sumCheck  = cms.bool(True);
process.l1tSummary.muonCheck = cms.bool(True);
process.l1tSummary.egToken   = cms.InputTag("simCaloStage2Digis");
process.l1tSummary.tauToken  = cms.InputTag("simCaloStage2Digis");
process.l1tSummary.jetToken  = cms.InputTag("simCaloStage2Digis");
process.l1tSummary.sumToken  = cms.InputTag("simCaloStage2Digis");
process.l1tSummary.muonToken = cms.InputTag("simGmtStage2Digis","");
#process.l1tSummary.muonToken = cms.InputTag("simGmtStage2Digis","imdMuonsBMTF");

process.load('L1Trigger.L1TCalorimeter.simCaloStage2Layer1Digis_cfi')
process.simCaloStage2Layer1Digis.ecalToken = cms.InputTag("simEcalTriggerPrimitiveDigis")
process.simCaloStage2Layer1Digis.hcalToken = cms.InputTag("simHcalTriggerPrimitiveDigis")
process.load('L1Trigger.L1TCalorimeter.simCaloStage2Digis_cfi')

# Additional output definition
# TTree output file
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName = cms.string('l1t_debug.root')

# enable debug message logging for our modules
process.MessageLogger.categories.append('L1TCaloEvents')
process.MessageLogger.categories.append('L1TGlobalEvents')
process.MessageLogger.categories.append('l1t|Global')
process.MessageLogger.suppressInfo = cms.untracked.vstring('Geometry', 'AfterSource')


process.bxVector = cms.EDProducer("L1TBXVectorTester")
process.bxVector.muonToken = cms.InputTag("simGmtStage2Digis","");

process.L1TSeq = cms.Sequence(   process.SimL1Emulator
#                                   + process.dumpED
#                                   + process.dumpES
                                   + process.bxVector
                                   + process.dumpED
#                                   + process.l1tSummary
)

process.L1TPath = cms.Path(process.L1TSeq)

process.out = cms.OutputModule("PoolOutputModule", 
   fileName = cms.untracked.string("l1t.root")
)

process.output_step = cms.EndPath(process.out)
process.schedule = cms.Schedule(process.L1TPath)
process.schedule.extend([process.output_step])

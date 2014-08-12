import FWCore.ParameterSet.Config as cms
##################################################################

MaxEvents=-1
OutputFileName='file:SimL1Emulator_Stage1_PP.root'
InputFiles=["file:/eos/uscms/store/user/lpctrig/HATS/DYJetsToLL_M-50_13TeV-pythia6_Fall13dr-tsg_PU40bx25_POSTLS162_V2-v1/skim_150_1_6UN.root","file:/eos/uscms/store/user/lpctrig/HATS/DYJetsToLL_M-50_13TeV-pythia6_Fall13dr-tsg_PU40bx25_POSTLS162_V2-v1/skim_49_1_tOR.root"]

##################################################################
process = cms.Process('L1TEMULATION')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryIdeal_cff')

# Select the Message Logger output you would like to see:
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(MaxEvents)
    )

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames= cms.untracked.vstring(InputFiles)
    )


process.output = cms.OutputModule(
    "PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
#    outputCommands = cms.untracked.vstring('keep *',
#                                           'drop FEDRawDataCollection_rawDataRepacker_*_*',
#                                           'drop FEDRawDataCollection_virginRawDataRepacker_*_*'),
    outputCommands = cms.untracked.vstring('drop *',
                                           'keep *_ak5PFJets*__*',
                                           'keep *_pfMet*__*',
                                           'keep *_muons__*',
                                           'keep *_gsfElectrons__*',
                                           'keep *_gtDigis__*',
                                           'keep *_l1extraParticles_*_*',
                                           'keep *_*_*_L1TEMULATION'),
    fileName = cms.untracked.string(OutputFileName),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
                                           )
process.options = cms.untracked.PSet()

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.connect = cms.string('frontier://FrontierProd/CMS_COND_31X_GLOBALTAG')
process.GlobalTag.globaltag = cms.string('POSTLS162_V2::All')

process.load('L1Trigger.L1TCalorimeter.L1TCaloStage1_PPFromRaw_cff')


process.p1 = cms.Path(
    process.L1TCaloStage1_PPFromRaw
    )

process.output_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(
    process.p1, process.output_step
    )

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

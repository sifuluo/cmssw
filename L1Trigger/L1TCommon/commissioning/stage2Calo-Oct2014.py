# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: SingleElectronPt10_cfi.py -s GEN,SIM,DIGI,L1 --pileup=NoPileUp --geometry DB --conditions=auto:startup -n 1 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('L1')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("EmptySource",
)

process.options = cms.untracked.PSet(
)


# Output definition
process.output = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring("keep *"),
    fileName = cms.untracked.string('L1T_EDM.root')
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

# enable debug message logging for our modules
process.MessageLogger = cms.Service(
    "MessageLogger",
    destinations   = cms.untracked.vstring(
        'detailedInfo',
        'critical'
    ),
    detailedInfo   = cms.untracked.PSet(
        threshold  = cms.untracked.string('DEBUG') 
    ),
    debugModules = cms.untracked.vstring(
#        'mp7BufferDumpToRaw',
        'l1tDigis',
        #'caloStage2TowerDigis',
        'caloStage2Digis'
    )
)

# TTree output file
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName = cms.string('l1t.root')


# user stuff

# raw data from MP card
import EventFilter.L1TRawToDigi.mp7BufferDumpToRaw_cfi
process.stage2Layer2Raw = EventFilter.L1TRawToDigi.mp7BufferDumpToRaw_cfi.mp7BufferDumpToRaw.clone()
process.stage2Layer2Raw.fedId           = cms.untracked.int32(2)
process.stage2Layer2Raw.rxFile          = cms.untracked.string("MPCaloPattSingEle/rx_summary.txt")
process.stage2Layer2Raw.txFile          = cms.untracked.string("MPCaloPattSingEle/tx_summary.txt")
process.stage2Layer2Raw.nEventsToSkip = cms.untracked.int32(0)
process.stage2Layer2Raw.nFramesPerEvent = cms.untracked.int32(40)
#process.stage2Layer2Raw.txLatency       = cms.untracked.int32(57) # For the pi0 half eta
#process.stage2Layer2Raw.txLatency       = cms.untracked.int32(92) # For the ttbar
#process.stage2Layer2Raw.txLatency       = cms.untracked.int32(55) # For Brian's pattern
process.stage2Layer2Raw.txLatency       = cms.untracked.int32(100) # For Brian's single electron (2nd event because 1st event is not picked up in rx)
process.stage2Layer2Raw.nRxEventHeaders = cms.untracked.int32(0)
process.stage2Layer2Raw.nTxEventHeaders = cms.untracked.int32(0)

process.stage2Layer2Raw.rxBlockLength   = cms.untracked.vint32( 
    40,40,40,40,40,40,40,40,40,
    40,40,40,40,40,40,40,40,40,
    40,40,40,40,40,40,40,40,40,
    40,40,40,40,40,40,40,40,40,
    40,40,40,40,40,40,40,40,40,
    40,40,40,40,40,40,40,40,40,
    40,40,40,40,40,40,40,40,40,
    40,40,40,40,40,40,40,40,40)

# For patterns taken with half detector in eta
#    40,0,40,0,40,0,40,0,40,
#    0,40,0,40,0,40,0,40,0,
#    40,0,40,0,40,0,40,0,40,
#    0,40,0,40,0,40,0,40,0,
#    40,0,40,0,40,0,40,0,40,
#    0,40,0,40,0,40,0,40,0,
#    40,0,40,0,40,0,40,0,40,
#    0,40,0,40,0,40,0,40,0)

process.stage2Layer2Raw.txBlockLength   = cms.untracked.vint32(
    39,39,39,39,39,39,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0)

# raw data from Demux
process.stage2DemuxRaw = EventFilter.L1TRawToDigi.mp7BufferDumpToRaw_cfi.mp7BufferDumpToRaw.clone()
process.stage2DemuxRaw.fedId = cms.untracked.int32(1)
process.stage2DemuxRaw.rxFile = cms.untracked.string("CaloSingleElectronDemux2Quad/rx_summary.txt")
process.stage2DemuxRaw.txFile = cms.untracked.string("CaloSingleElectronDemux2Quad/tx_summary.txt")
process.stage2DemuxRaw.fedId            = cms.untracked.int32(1)
process.stage2DemuxRaw.packetisedData   = cms.untracked.bool(True)
process.stage2DemuxRaw.nRxLinks         = cms.untracked.int32(8)
process.stage2DemuxRaw.nTxLinks         = cms.untracked.int32(8)
process.stage2DemuxRaw.nFramesPerEvent  = cms.untracked.int32(40)
process.stage2DemuxRaw.txLatency        = cms.untracked.int32(257) #second event as above
process.stage2DemuxRaw.nRxEventHeaders  = cms.untracked.int32(0)
process.stage2DemuxRaw.nTxEventHeaders  = cms.untracked.int32(0)
process.stage2DemuxRaw.rxBlockLength    = cms.untracked.vint32(
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0)
process.stage2DemuxRaw.txBlockLength    = cms.untracked.vint32(
    6,6,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0)


# merge raw data
import EventFilter.RawDataCollector.rawDataCollector_cfi
process.rawData = EventFilter.RawDataCollector.rawDataCollector_cfi.rawDataCollector.clone()
process.rawData.RawCollectionList = cms.VInputTag(
    cms.InputTag('stage2Layer2Raw'),
    cms.InputTag('stage2DemuxRaw')
)


# MP raw to digi
import EventFilter.L1TRawToDigi.l1tRawToDigi_cfi
process.mpDigis = EventFilter.L1TRawToDigi.l1tRawToDigi_cfi.l1tRawToDigi.clone()
process.mpDigis.FedId = cms.int32(2)
process.mpDigis.InputLabel = cms.InputTag("stage2Layer2Raw")
process.mpDigis.Unpackers = cms.vstring([ "l1t::CaloTowerUnpackerFactory",
                                           "l1t::MPUnpackerFactory"])

# Demux Raw to digi
process.demuxDigis = EventFilter.L1TRawToDigi.l1tRawToDigi_cfi.l1tRawToDigi.clone()
process.demuxDigis.FedId = cms.int32(1)
process.demuxDigis.InputLabel = cms.InputTag("stage2DemuxRaw")
process.demuxDigis.Unpackers = cms.vstring([ "l1t::EGammaUnpackerFactory",
                                             "l1t::EtSumUnpackerFactory",
                                             "l1t::JetUnpackerFactory",
                                             "l1t::TauUnpackerFactory"])

# Single unpacker
process.l1tDigis = EventFilter.L1TRawToDigi.l1tRawToDigi_cfi.l1tRawToDigi.clone()
process.l1tDigis.FedId = cms.int32(2)  # what does this do when the RAW has multiple FEDs ????
process.l1tDigis.InputLabel = cms.InputTag("rawData")

process.l1tDigis.Unpackers = cms.vstring([ "l1t::CaloTowerUnpackerFactory",
                                           "l1t::EGammaUnpackerFactory",
                                           "l1t::EtSumUnpackerFactory",
                                           "l1t::JetUnpackerFactory",
                                           "l1t::TauUnpackerFactory",
                                           "l1t::MPUnpackerFactory"])

### emulator ###

# upgrade calo stage 2
process.load('L1Trigger.L1TCalorimeter.L1TCaloStage2_cff')
process.caloStage2Digis.towerToken = cms.InputTag("mpDigis")

process.load("L1Trigger.L1TCalorimeter.caloStage2Params_cfi")
process.caloStage2Params.etSumEtThreshold = cms.vdouble(0.,  0.,   16.,   16.)
process.caloStage2Params.jetPUSType = cms.string("ChunkyDonut")


### diagnostics ###

# dump raw data
process.dumpRaw = cms.EDAnalyzer( 
    "DumpFEDRawDataProduct",
    label = cms.untracked.string("rawData"),
    feds = cms.untracked.vint32 ( 1, 2 ),
    dumpPayload = cms.untracked.bool ( True )
)

# plots from unpacker
import L1Trigger.L1TCalorimeter.l1tStage2CaloAnalyzer_cfi
process.mpPlots = L1Trigger.L1TCalorimeter.l1tStage2CaloAnalyzer_cfi.l1tStage2CaloAnalyzer.clone()
process.mpPlots.towerToken = cms.InputTag("mpDigis")
process.mpPlots.clusterToken = cms.InputTag("None")
process.mpPlots.egToken = cms.InputTag("None")
process.mpPlots.tauToken = cms.InputTag("None")
process.mpPlots.jetToken = cms.InputTag("mpDigis:MP")
process.mpPlots.etSumToken = cms.InputTag("mpDigis:MP")

process.demuxPlots = L1Trigger.L1TCalorimeter.l1tStage2CaloAnalyzer_cfi.l1tStage2CaloAnalyzer.clone()
process.demuxPlots.towerToken = cms.InputTag("None")
process.demuxPlots.clusterToken = cms.InputTag("None")
process.demuxPlots.egToken = cms.InputTag("None")
process.demuxPlots.tauToken = cms.InputTag("None")
process.demuxPlots.jetToken = cms.InputTag("demuxDigis")
process.demuxPlots.etSumToken = cms.InputTag("demuxDigis")

# plots from emulator
process.simPlots = L1Trigger.L1TCalorimeter.l1tStage2CaloAnalyzer_cfi.l1tStage2CaloAnalyzer.clone()
process.simPlots.towerToken = cms.InputTag("None")
process.simPlots.clusterToken = cms.InputTag("None")
process.simPlots.egToken = cms.InputTag("None")
process.simPlots.tauToken = cms.InputTag("None")
process.simPlots.jetToken = cms.InputTag("caloStage2Digis")
process.simPlots.etSumToken = cms.InputTag("caloStage2Digis")


# Path and EndPath definitions
process.path = cms.Path(

    # produce RAW
    process.stage2Layer2Raw
    +process.stage2DemuxRaw
    +process.rawData

    # unpack
    +process.mpDigis
    +process.demuxDigis
#    +process.l1tDigis

    # emulator
    +process.caloStage2Digis

    # diagnostics
#    +process.dumpRaw
    +process.mpPlots
    +process.demuxPlots
    +process.simPlots
)

process.out = cms.EndPath(
    process.output
)

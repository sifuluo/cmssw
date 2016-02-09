# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: L1TEST --python_filename=l1t_test.py --conditions=80X_dataRun2_v13 -s RAW2DIGI -n 100 --era=Run2_2016 --data --filein=/store/data/Run2016A/ZeroBias1/RAW/v1/000/271/336/00000/00963A5A-BF0A-E611-A657-02163E0141FB.root --no_output --no_exec --customise=L1Trigger/Configuration/customiseUtils.L1TStage2DigisSummary --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAW
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RAW2DIGI',eras.Run2_2016)

process.options = cms.untracked.PSet(
    numberOfThreads = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    multiProcesses = cms.untracked.PSet(
        maxChildProcesses = cms.untracked.int32(0)
    )
)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source( "FedRawDataInputSource",
    fileNames = cms.untracked.vstring(
#'/afs/cern.ch/user/f/fwyzard/public/MWGR2_run265627/run265627_ls0001_index000000_fu-c2d31-12-01_pid63079.raw'     
'/afs/cern.ch/work/m/mulhearn/data/run000006_ls0003_index000019.raw',
    ),
    fileListMode = cms.untracked.bool( True ),
    numBuffers = cms.untracked.uint32( 2 ),
    useL1EventID = cms.untracked.bool( True ),
    eventChunkSize = cms.untracked.uint32( 32 ),
    verifyChecksum = cms.untracked.bool( True ),
    eventChunkBlock = cms.untracked.uint32( 32 ),
    maxBufferedFiles = cms.untracked.uint32( 2 ),
    verifyAdler32 = cms.untracked.bool( True )
)

process.FastMonitoringService = cms.Service( "FastMonitoringService",
    fastMonIntervals = cms.untracked.uint32( 2 ),
    sleepTime = cms.untracked.int32( 1 )
)

process.EvFDaqDirector = cms.Service( "EvFDaqDirector",
    buBaseDir = cms.untracked.string( "." ),
    baseDir = cms.untracked.string( "." ),
    fuLockPollInterval = cms.untracked.uint32( 2000 ),
    runNumber = cms.untracked.uint32( 6 ),
    outputAdler32Recheck = cms.untracked.bool( False ),
    selectedTransferMode = cms.untracked.string( "" ),
    requireTransfersPSet = cms.untracked.bool( False ),
    emptyLumisectionMode = cms.untracked.bool( True )
)



process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('L1TEST nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_v13', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.L1TRawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step)

# customisation of the process.

from L1Trigger.Configuration.customiseUtils import L1TGlobalDigisSummary, L1TDumpEventData
process = L1TGlobalDigisSummary(process)
process = L1TDumpEventData(process)

# End of customisation functions


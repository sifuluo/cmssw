import FWCore.ParameterSet.Config as cms

def updatel1ExtraReEmulTag(process,inputTag):

    l1ExtraReEmul = getattr(process,'l1ExtraReEmul') 

    l1ExtraReEmul.isolatedEmSource    = cms.InputTag(inputTag,"isoEm")
    l1ExtraReEmul.nonIsolatedEmSource = cms.InputTag(inputTag,"nonIsoEm")

    l1ExtraReEmul.forwardJetSource = cms.InputTag(inputTag,"forJets")
    l1ExtraReEmul.centralJetSource = cms.InputTag(inputTag,"cenJets")
    l1ExtraReEmul.tauJetSource     = cms.InputTag(inputTag,"tauJets")
        
    l1ExtraReEmul.etTotalSource = cms.InputTag(inputTag)
    l1ExtraReEmul.etHadSource   = cms.InputTag(inputTag)
    l1ExtraReEmul.etMissSource  = cms.InputTag(inputTag)
    l1ExtraReEmul.htMissSource  = cms.InputTag(inputTag)
    
    l1ExtraReEmul.hfRingEtSumsSource    = cms.InputTag(inputTag)
    l1ExtraReEmul.hfRingBitCountsSource = cms.InputTag(inputTag)

def updategtReEmulTag(process,inputTag):

    getattr(process,'gtReEmulDigis').GctInputTag = cms.InputTag(inputTag)
    getattr(process,'gtReEmulDigis').EmulateBxInEvent = cms.int32(1)

def updatel1ntupleTag(process,inputTag):

    ntuple = getattr(process,'l1NtupleProducer')
    ntuple.gctCentralJetsSource = cms.InputTag(inputTag,"cenJets")
    ntuple.gctNonIsoEmSource    = cms.InputTag(inputTag,"nonIsoEm")
    ntuple.gctForwardJetsSource = cms.InputTag(inputTag,"forJets")
    ntuple.gctIsoEmSource       = cms.InputTag(inputTag,"isoEm")
    ntuple.gctTauJetsSource     = cms.InputTag(inputTag,"tauJets")
    ntuple.gctEnergySumsSource  = cms.InputTag(inputTag,"")
    ntuple.rctSource            = cms.InputTag("none")

def customiseUCT2015(process, runOnMC, runOnPostLS1, whichPU ):

    if hasattr(process,'reEmulCaloChain') :
        print "[L1Menu]: Customising calo chain with UCT2015"

        if runOnMC and runOnPostLS1 :
            print "[L1Menu]:\tUsing MC configuration for post LS1"
            process.load("L1Trigger.UCT2015.emulationMC_cfi")
            from L1Trigger.UCT2015.regionSF_cfi import *
            if whichPU == 20 :
                process.CorrectedDigis.regionSubtraction = regionSubtraction_PU20_MC13TeV
        elif not runOnMC : 
            print "[L1Menu]:\tUsing DATA configuration"
            process.load("L1Trigger.UCT2015.emulation_cfi") # For running on data
        else :
            # If run on MC and on 53X needs a patch different w.r.t. the UCT "standard" one
            # in this case there are no hcal digis stored so need to run on unpacked (Zero Suppressed) ones
            print "[L1Menu]:\tUsing MC configuration for 53X"

            process.load("L1Trigger.UCT2015.emulation_cfi") # For running on data patch by hand
            
            process.load("SimCalorimetry.HcalSimProducers.hcalUnsuppressedDigis_cfi")
            process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")

            process.hcalReEmulDigis = process.simHcalTriggerPrimitiveDigis.clone()
            
            process.hcalReEmulDigis.inputLabel = cms.VInputTag(cms.InputTag('hcalDigis'), cms.InputTag('hcalDigis'))
            process.HcalTPGCoderULUT.LUTGenerationMode = cms.bool(True)

            process.hackHCALMIPs.src = cms.InputTag("hcalReEmulDigis")


        process.uctGctDigis =cms.EDProducer("UCT2015GctCandsProducer",
                                           egRelaxed = cms.InputTag("UCT2015Producer","RelaxedEGUnpacked"),
                                           egIsolated = cms.InputTag("UCT2015Producer","IsolatedEGUnpacked"),
                                           tauRelaxed = cms.InputTag("UCT2015Producer","RelaxedTauUnpacked"), # this collection is ignored in the final output, GT constraints
                                           tauIsolated = cms.InputTag("UCT2015Producer","IsolatedTauUnpacked"),
                                           jetSource = cms.InputTag("UCT2015Producer","CorrJetUnpacked"), # default are corrected jets
                                           #jetSource = cms.InputTag("UCT2015Producer","JetUnpacked"),
                                           setSource = cms.InputTag("UCT2015Producer","SETUnpacked"),
                                           metSource = cms.InputTag("UCT2015Producer","METUnpacked"),
                                           shtSource = cms.InputTag("UCT2015Producer","SHTUnpacked"),
                                           mhtSource = cms.InputTag("UCT2015Producer","MHTUnpacked")
        )

        if runOnMC and not runOnPostLS1 :
            # If run on MC and on 53X needs a patch different w.r.t. the UCT "standard" one
            # in this case there are no hcal digis stored so need to run on unpacked (Zero Suppressed) ones
            
            process.reEmulUctChain = cms.Sequence(process.hcalReEmulDigis + process.emulationSequence + process.uctGctDigis)
        else :
            process.reEmulUctChain = cms.Sequence(process.emulationSequence + process.uctGctDigis)


        getattr(process,'reEmul').replace(process.reEmulCaloChain, process.reEmulUctChain)

        l1ExtraReEmul = getattr(process,'l1ExtraReEmul') 

        updatel1ExtraReEmulTag(process,"uctGctDigis")
        updategtReEmulTag(process,"uctGctDigis")

    else :
       print "[L1Menu]: ERROR: Can't customise calo chain with UCT2015, reEmulCaloChain is missing!"

    if hasattr(process,'l1NtupleProducer') and hasattr(process,'l1ExtraTreeProducer') :
        print "[L1Menu]:\tConfiguring Ntuple to use UCT2015 information"
 
        updatel1ntupleTag(process,"uctGctDigis")


def customiseL1Calos(process, customGCT=True):

    print "[L1Menu]: Customising legacy calo chain with possible 2015 improvements"

    if customGCT and hasattr(process,"gctReEmulDigis") :

        print "[L1Menu]:\tCustomising GCT configuration to use 10 GeV jet Seeds"

        process.load("L1TriggerConfig.GctConfigProducers.L1GctConfig_cff")
    
        process.L1GctConfigProducers.JetFinderCentralJetSeed = 10.0
        process.L1GctConfigProducers.JetFinderForwardJetSeed = 10.0
    
        process.es_prefer_gct = cms.ESPrefer("L1GctConfigProducers")
        

def customiseStage1(process, runOnMC, runOnPostLS1, whichPU ):

    if hasattr(process,'reEmulCaloChain') :
        print "[L1Menu]: Customising calo chain with new L1 Stage1 Emulator"

        if runOnMC and runOnPostLS1 :
            ## print "[L1Menu]:\tUsing MC configuration for post LS1"
            process.load('L1Trigger/L1TCalorimeter/caloStage1Params_cfi')
            from L1Trigger.L1TCalorimeter.caloStage1RegionSF_cfi import *
            if whichPU == 20 :
                process.CorrectedDigis.regionSubtraction = regionSubtraction_PU20_MC13TeV
        elif not runOnMC : 
            print "[L1Menu]:\tUsing DATA configuration"
            process.load("L1Trigger.UCT2015.emulation_cfi") # For running on data
        else :
            print "Illegal option(s) for Stage1 Emulator"
            sys.exit(1)


        ## process.gctDigis = cms.EDProducer(
        ##     "GctRawToDigi",
        ##     unpackSharedRegions = cms.bool(False),
        ##     numberOfGctSamplesToUnpack = cms.uint32(1),
        ##     verbose = cms.untracked.bool(False),
        ##     numberOfRctSamplesToUnpack = cms.uint32(1),
        ##     inputLabel = cms.InputTag("rawDataCollector"),
        ##     unpackerVersion = cms.uint32(0),
        ##     gctFedId = cms.untracked.int32(745),
        ##     hltMode = cms.bool(False)
        ##     )

        process.rctLayer2Format = cms.EDProducer(
            "l1t::L1TCaloRCTToUpgradeConverter",
            regionTag = cms.InputTag("gctDigis"),
            emTag = cms.InputTag("gctDigis"))

        process.Layer2HW = cms.EDProducer(
            "l1t::Stage1Layer2Producer",
            CaloRegions = cms.InputTag("rctLayer2Format"),
            CaloEmCands = cms.InputTag("rctLayer2Format"),
            FirmwareVersion = cms.uint32(2),  ## 1=HI algo, 2= pp algo
            egRelativeJetIsolationCut = cms.double(0.5), ## eg isolation cut
            tauRelativeJetIsolationCut = cms.double(1.), ## tau isolation cut
            regionETCutForHT = cms.uint32(7),
            regionETCutForMET = cms.uint32(0),
            minGctEtaForSums = cms.int32(4),
            maxGctEtaForSums = cms.int32(17)
            )

        process.Layer2Phys = cms.EDProducer(
            "l1t::PhysicalEtAdder",
            InputCollection = cms.InputTag("Layer2HW")
            )

        process.Layer2gctFormat = cms.EDProducer(
            "l1t::L1TCaloUpgradeToGCTConverter",
            InputCollection = cms.InputTag("Layer2Phys")
            )

        process.load("L1Trigger.UCT2015.emulationMC_cfi")
        process.rctLayer2Format.regionTag = "uctDigis"
        process.rctLayer2Format.emTag = "uctDigis"

        process.Stage1GctDigis = cms.Sequence(
            process.rctLayer2Format
            *process.Layer2HW
            *process.Layer2Phys
            *process.Layer2gctFormat
            ## *process.L1Packer
            ## *process.L1Unpacker
            )

        if runOnMC and not runOnPostLS1 :
            # If run on MC and on 53X needs a patch different w.r.t. the UCT "standard" one
            # in this case there are no hcal digis stored so need to run on unpacked (Zero Suppressed) ones
            
            process.reEmulStage1Chain = cms.Sequence(process.hcalReEmulDigis + process.emulationSequence + process.Stage1GctDigis)
        else :
            process.reEmulStage1Chain = cms.Sequence(process.emulationSequence + process.Stage1GctDigis)


        getattr(process,'reEmul').replace(process.reEmulCaloChain, process.reEmulStage1Chain)

        l1ExtraReEmul = getattr(process,'l1ExtraReEmul') 

        updatel1ExtraReEmulTag(process,"Layer2gctFormat")
        updategtReEmulTag(process,"Layer2gctFormat")

    else :
       print "[L1Menu]: ERROR: Can't customise calo chain with Stage1 emulator, reEmulCaloChain is missing!"

    if hasattr(process,'l1NtupleProducer') and hasattr(process,'l1ExtraTreeProducer') :
        print "[L1Menu]:\tConfiguring Ntuple to use Stage1 emulator information"
 
        updatel1ntupleTag(process,"Layer2gctFormat")

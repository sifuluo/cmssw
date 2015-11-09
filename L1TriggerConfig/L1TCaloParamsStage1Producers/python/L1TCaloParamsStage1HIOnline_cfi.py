import FWCore.ParameterSet.Config as cms

#from L1Trigger.L1TCalorimeter.caloStage1Params_cfi import caloStage1Params

L1TCaloParamsStage1HIOnlineProd = cms.ESProducer("L1TCaloParamsStage1HIOnlineProd",
    onlineAuthentication = cms.string('.'),
    forceGeneration = cms.bool(False),
    onlineDB = cms.string('oracle://CMS_OMDS_LB/CMS_TRG_R') #,

#    etSumEtaMin      = caloStage1Params.etSumEtaMin,
#    etSumEtaMax      = caloStage1Params.etSumEtaMax,
#    etSumEtThreshold = caloStage1Params.etSumEtThreshold,
#    towerLsbSum      = caloStage1Params.towerLsbSum,
#    regionLsb        = caloStage1Params.regionLsb,
#    jetLsb           = caloStage1Params.jetLsb,
#    egLsb            = caloStage1Params.egLsb
)

#es_prefer = cms.ESPrefer("L1TCaloParamsStage1HIOnlineProd","")

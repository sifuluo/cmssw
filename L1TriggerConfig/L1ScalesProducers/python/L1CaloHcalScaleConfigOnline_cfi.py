import FWCore.ParameterSet.Config as cms
L1CaloHcalScaleConfigOnline = cms.ESProducer("L1CaloHcalScaleConfigOnlineProd",
    onlineAuthentication = cms.string('.'),
    forceGeneration = cms.bool(False),
    onlineDB = cms.string('oracle://CMS_OMDS_LB/CMS_TRG_R')
)

from Configuration.Geometry.GeometryExtended2015Reco_cff import *
HcalTrigTowerGeometryESProducer = cms.ESProducer("HcalTrigTowerGeometryESProducer")

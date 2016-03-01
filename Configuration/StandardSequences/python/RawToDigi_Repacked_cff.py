import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.RawToDigi_cff import *

scalersRawToDigi.scalersInputTag = 'rawDataRepacker'
siPixelDigis.InputLabel = 'rawDataRepacker'
siStripDigis.ProductLabel = 'rawDataRepacker'
#False by default ecalDigis.DoRegional = False
ecalDigis.InputLabel = 'rawDataRepacker'
ecalPreshowerDigis.sourceTag = 'rawDataRepacker'
hcalDigis.InputLabel = 'rawDataRepacker'
muonCSCDigis.InputObjects = 'rawDataRepacker'
muonDTDigis.inputLabel = 'rawDataRepacker'
muonRPCDigis.InputLabel = 'rawDataRepacker'
castorDigis.InputLabel = 'rawDataRepacker'

RawToDigi = cms.Sequence(L1TRawToDigi+siPixelDigis+siStripDigis+ecalDigis+ecalPreshowerDigis+hcalDigis+muonCSCDigis+muonDTDigis+muonRPCDigis+castorDigis+scalersRawToDigi)

# Can we deprecate this please?
RawToDigi_woGCT = cms.Sequence(L1TRawToDigi_woGCT+dttfDigis+gtDigis+gtEvmDigis+siPixelDigis+siStripDigis+ecalDigis+ecalPreshowerDigis+hcalDigis+muonCSCDigis+muonDTDigis+muonRPCDigis+castorDigis+scalersRawToDigi)


siStripVRDigis = siStripDigis.clone(ProductLabel = 'virginRawDataRepacker')
RawToDigi_withVR = cms.Sequence(RawToDigi + siStripVRDigis)

l1t_override_raw_data_location('rawDataRepacker')

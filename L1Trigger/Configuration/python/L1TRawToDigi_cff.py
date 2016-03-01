#
#  L1TRawToDigi:  Defines
#
#     L1TRawToDigi = cms.Sequence(...)
#
# which contains all packers needed for the current era.
#

import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras


def unpack_legacy():
    global L1TRawToDigi_Legacy
    global csctfDigis, dttfDigis, gctDigis, gtDigis, gtEvmDigis
    import EventFilter.CSCTFRawToDigi.csctfunpacker_cfi
    csctfDigis = EventFilter.CSCTFRawToDigi.csctfunpacker_cfi.csctfunpacker.clone()
    import EventFilter.DTTFRawToDigi.dttfunpacker_cfi
    dttfDigis = EventFilter.DTTFRawToDigi.dttfunpacker_cfi.dttfunpacker.clone()
    import EventFilter.GctRawToDigi.l1GctHwDigis_cfi
    gctDigis = EventFilter.GctRawToDigi.l1GctHwDigis_cfi.l1GctHwDigis.clone()
    import EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi
    gtDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi.l1GtUnpack.clone()
    import EventFilter.L1GlobalTriggerRawToDigi.l1GtEvmUnpack_cfi
    gtEvmDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtEvmUnpack_cfi.l1GtEvmUnpack.clone()
    #
    csctfDigis.producer = 'rawDataCollector'
    dttfDigis.DTTF_FED_Source = 'rawDataCollector'
    gctDigis.inputLabel = 'rawDataCollector'
    gtDigis.DaqGtInputTag = 'rawDataCollector'
    gtEvmDigis.EvmGtInputTag = 'rawDataCollector'
    L1TRawToDigi_Legacy = cms.Sequence(csctfDigis+dttfDigis+gctDigis+gtDigis+gtEvmDigis)
    # this is used in RawToDigi_Repacked_cff ... will try to deprecate:
    L1TRawToDigi_Legacy_woGCT = cms.Sequence(csctfDigis+dttfDigis+gtDigis+gtEvmDigis)
    
def unpack_stage1():
    global csctfDigis, dttfDigis, gtDigis,caloStage1Digis,caloStage1FinalDigis,gctDigis
    global caloStage1LegacyFormatDigis
    global L1TRawToDigi_Stage1    
    import EventFilter.CSCTFRawToDigi.csctfunpacker_cfi
    csctfDigis = EventFilter.CSCTFRawToDigi.csctfunpacker_cfi.csctfunpacker.clone()
    import EventFilter.DTTFRawToDigi.dttfunpacker_cfi
    dttfDigis = EventFilter.DTTFRawToDigi.dttfunpacker_cfi.dttfunpacker.clone()
    import EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi
    gtDigis = EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi.l1GtUnpack.clone()
    from EventFilter.L1TRawToDigi.caloStage1Digis_cfi import caloStage1Digis
    # this adds the physical ET to unpacked data
    from L1Trigger.L1TCalorimeter.caloStage1LegacyFormatDigis_cfi import caloStage1LegacyFormatDigis
    from L1Trigger.L1TCalorimeter.caloStage1FinalDigis_cfi import caloStage1FinalDigis
    csctfDigis.producer = 'rawDataCollector'
    dttfDigis.DTTF_FED_Source = 'rawDataCollector'
    gtDigis.DaqGtInputTag = 'rawDataCollector'
    # unpack GCT digis too, so DQM offline doesn't crash:
    import EventFilter.GctRawToDigi.l1GctHwDigis_cfi
    gctDigis = EventFilter.GctRawToDigi.l1GctHwDigis_cfi.l1GctHwDigis.clone()
    gctDigis.inputLabel = 'rawDataCollector'
    L1TRawToDigi_Stage1 = cms.Sequence(csctfDigis+dttfDigis+gtDigis+caloStage1Digis+caloStage1FinalDigis+caloStage1LegacyFormatDigis+gctDigis)    

def unpack_stage2():
    global L1TRawToDigi_Stage2
    global caloStage2Digis, gmtStage2Digis, gtStage2Digis,L1TRawToDigi_Stage2    
    from EventFilter.L1TRawToDigi.caloStage2Digis_cfi import caloStage2Digis
    from EventFilter.L1TRawToDigi.gmtStage2Digis_cfi import gmtStage2Digis
    from EventFilter.L1TRawToDigi.gtStage2Digis_cfi import gtStage2Digis
    L1TRawToDigi_Stage2 = cms.Sequence(caloStage2Digis + gmtStage2Digis + gtStage2Digis)
    
#
# Legacy Trigger:
#
if not (eras.stage1L1Trigger.isChosen() or eras.stage2L1Trigger.isChosen()):
    print "L1TRawToDigi Sequence configured for Run1 (Legacy) trigger. "
    unpack_legacy()
    L1TRawToDigi = cms.Sequence(L1TRawToDigi_Legacy);
    # this is used in RawToDigi_Repacked_cff ... will try to deprecate:
    L1TRawToDigi_woGCT = cms.Sequence(L1TRawToDigi_Legacy_woGCT);

#
# Stage-1 Trigger
#
if eras.stage1L1Trigger.isChosen() and not eras.stage2L1Trigger.isChosen():
    print "L1TRawToDigi Sequence configured for Stage-1 (2015) trigger. "    
    unpack_stage1()
    L1TRawToDigi = cms.Sequence(L1TRawToDigi_Stage1);
    # this is used in RawToDigi_Repacked_cff ... will try to deprecate:
    L1TRawToDigi_woGCT = cms.Sequence(L1TRawToDigi);

#
# Stage-2 Trigger:  fow now, unpack Stage 1 and Stage 2 (in case both available)
#
if eras.stage2L1Trigger.isChosen():
    print "L1TRawToDigi Sequence configured for Stage-2 (2016) trigger. "    
    unpack_stage1()
    unpack_stage2()
    L1TRawToDigi = cms.Sequence(L1TRawToDigi_Stage1+L1TRawToDigi_Stage2);
    # this is used in RawToDigi_Repacked_cff ... will try to deprecate:
    L1TRawToDigi_woGCT = cms.Sequence(L1TRawToDigi);


def l1t_override_raw_data_location(raw_data):
    print "L1T INFO:  configuring L1T unpacking to use raw data at ", raw_data
    csctfDigis.producer = raw_data
    dttfDigis.DTTF_FED_Source = raw_data
    gtDigis.DaqGtInputTag = raw_data

    if not eras.stage1L1Trigger.isChosen() and not eras.stage2L1Trigger.isChosen():
        gctDigis.inputLabel = raw_data
        gtEvmDigis.EvmGtInputTag = raw_data
    
    if eras.stage1L1Trigger.isChosen() or eras.stage2L1Trigger.isChosen():
        caloStage1Digis.InputLabel = cms.InputTag(raw_data)

    if eras.stage2L1Trigger.isChosen():
        caloStage2Digis.InputLabel = cms.InputTag(raw_data)
        gmtStage2Digis.InputLabel = cms.InputTag(raw_data)
        gtStage2Digis.InputLabel = cms.InputTag(raw_data)

def initL1O2OTagsExt():

    import FWCore.ParameterSet.Config as cms
    from CondTools.L1TriggerExt.L1CondEnumExt_cfi import L1CondEnumExt

    initL1O2OTagsExt.tagBaseVec = [None] * L1CondEnumExt.NumL1Cond

    initL1O2OTagsExt.tagBaseVec[ L1CondEnumExt.L1TriggerKeyListExt ] = "CRAFT09_hlt"
    initL1O2OTagsExt.tagBaseVec[ L1CondEnumExt.L1TriggerKeyExt ] = "CRAFT09_hlt"
    initL1O2OTagsExt.tagBaseVec[ L1CondEnumExt.L1TCaloParams ] = "CRAFT09_hlt"
    
#    for i in range( 0, L1CondEnumExt.NumL1Cond ):
#        print i, initL1O2OTagsExt.tagBaseVec[ i ]

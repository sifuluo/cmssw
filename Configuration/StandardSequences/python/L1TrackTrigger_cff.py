import FWCore.ParameterSet.Config as cms

### put L1 track trigger configs here

from L1Trigger.TrackTrigger.TrackTrigger_cff import *


L1TrackTrigger=cms.Sequence(TrackTriggerClustersStubs)

TTStubAlgorithm_official_Phase2TrackerDigi_.zMatchingPS = cms.bool(True)

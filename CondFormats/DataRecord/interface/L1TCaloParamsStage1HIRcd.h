///
/// \class L1TCaloParamsRcd
///
/// Description: Record for CaloParams
///
/// Implementation:
///    
///
/// \author: Jim Brooke, University of Bristol
///  \modified by Khristian Kotov
///
#ifndef CondFormatsDataRecord_L1TCaloParamsStage1HIRcd_h
#define CondFormatsDataRecord_L1TCaloParamsStage1HIRcd_h

//#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"

//class L1TCaloParamsStage1HIRcd : public edm::eventsetup::EventSetupRecordImplementation<L1TCaloParamsStage1HIRcd> {};

// Dependent record implmentation:
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "CondFormats/DataRecord/interface/L1TriggerKeyListExtRcd.h"
#include "CondFormats/DataRecord/interface/L1TriggerKeyExtRcd.h"
class L1TCaloParamsStage1HIRcd : public edm::eventsetup::DependentRecordImplementation<L1TCaloParamsStage1HIRcd, boost::mpl::vector<L1TriggerKeyListExtRcd,L1TriggerKeyExtRcd> > {};

#endif

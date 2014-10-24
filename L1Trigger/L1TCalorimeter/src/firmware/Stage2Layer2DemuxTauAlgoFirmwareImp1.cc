///
/// \class l1t::Stage2Layer2TauAlgorithmFirmwareImp1
///
/// \author: 
///
/// Description: 

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2DemuxTauAlgoFirmware.h"

#include "CondFormats/L1TObjects/interface/CaloParams.h"
#include "L1Trigger/L1TCalorimeter/interface/BitonicSort.h"

#include <vector>
#include <algorithm>

bool operator > ( l1t::Tau& a, l1t::Tau b )
{
  if ( a.hwPt() > b.hwPt() ){ 
    return true;
  } else {
    return false;
  }
}

namespace{
  bool sortbypt(const l1t::Tau &a, const l1t::Tau &b) { return a.hwPt() > b.hwPt(); };
  bool sortbyeta(const l1t::Tau &a, const l1t::Tau &b) { return a.hwEta() < b.hwEta(); };
}

l1t::Stage2Layer2DemuxTauAlgoFirmwareImp1::Stage2Layer2DemuxTauAlgoFirmwareImp1(CaloParams* params) :
  params_(params)
{


}


l1t::Stage2Layer2DemuxTauAlgoFirmwareImp1::~Stage2Layer2DemuxTauAlgoFirmwareImp1() {


}


void l1t::Stage2Layer2DemuxTauAlgoFirmwareImp1::processEvent(const std::vector<l1t::Tau> & inputTaus,
                                                             std::vector<l1t::Tau> & outputTaus) {

  // Set the output Taus to the input Taus
  outputTaus = inputTaus;

  // Sort the jets by pT
  std::vector<l1t::Tau>::iterator start(outputTaus.begin());
  std::vector<l1t::Tau>::iterator end(outputTaus.end());

  //BitonicSort< l1t::Tau >(down,start,end);
  std::sort(outputTaus.begin(), outputTaus.end(), sortbypt);

  // Truncate for now to allow GT testing
  if (outputTaus.size()>8) outputTaus.resize(8);

}


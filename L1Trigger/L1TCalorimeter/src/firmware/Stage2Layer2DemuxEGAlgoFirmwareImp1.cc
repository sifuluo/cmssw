///
/// \class l1t::Stage2Layer2EGAlgorithmFirmwareImp1
///
/// \author: 
///
/// Description: 

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2DemuxEGAlgoFirmware.h"

#include "CondFormats/L1TObjects/interface/CaloParams.h"
#include "L1Trigger/L1TCalorimeter/interface/BitonicSort.h"

#include <vector>
#include <algorithm>

bool operator > ( l1t::EGamma& a, l1t::EGamma& b )
{
  if ( a.hwPt() > b.hwPt() ){ 
    return true;
  } else {
    return false;
  }
}

namespace{
  bool sortbypt(const l1t::EGamma &a, const l1t::EGamma &b) { return a.hwPt() > b.hwPt(); };
  bool sortbyeta(const l1t::EGamma &a, const l1t::EGamma &b) { return a.hwEta() < b.hwEta(); };
}

l1t::Stage2Layer2DemuxEGAlgoFirmwareImp1::Stage2Layer2DemuxEGAlgoFirmwareImp1(CaloParams* params) :
  params_(params)
{


}


l1t::Stage2Layer2DemuxEGAlgoFirmwareImp1::~Stage2Layer2DemuxEGAlgoFirmwareImp1() {


}


void l1t::Stage2Layer2DemuxEGAlgoFirmwareImp1::processEvent(const std::vector<l1t::EGamma> & inputEGammas,
                                                            std::vector<l1t::EGamma> & outputEGammas) {

  // Set the output jets to the input jets
  outputEGammas = inputEGammas;

  // Sort the jets by pT
  std::vector<l1t::EGamma>::iterator start(outputEGammas.begin());
  std::vector<l1t::EGamma>::iterator end(outputEGammas.end());

  //  BitonicSort< l1t::EGamma >(down,start,end);
  std::sort(outputEGammas.begin(), outputEGammas.end(), sortbypt);

  // Truncate for now to allow GT testing
  if (outputEGammas.size()>12) outputEGammas.resize(12);

}


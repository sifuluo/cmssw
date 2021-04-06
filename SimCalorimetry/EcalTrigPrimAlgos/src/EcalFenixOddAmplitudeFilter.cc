#include "CondFormats/EcalObjects/interface/EcalTPGGroups.h"
#include "CondFormats/EcalObjects/interface/EcalTPGOddWeightGroup.h"
#include "CondFormats/EcalObjects/interface/EcalTPGOddWeightIdMap.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <SimCalorimetry/EcalTrigPrimAlgos/interface/EcalFenixOddAmplitudeFilter.h>
#include <iostream>
#include <string>
#include <fstream>

EcalFenixOddAmplitudeFilter::EcalFenixOddAmplitudeFilter(bool TPinfoPrintout)
    : inputsAlreadyIn_(0), stripid_{0}, shift_(6), TPinfoPrintout_(TPinfoPrintout) {}

EcalFenixOddAmplitudeFilter::~EcalFenixOddAmplitudeFilter() {}

int EcalFenixOddAmplitudeFilter::setInput(int input) {
  if (input > 0X3FFFF) {
    std::cout << "ERROR IN INPUT OF AMPLITUDE FILTER" << std::endl;
    return -1;
  }
  if (inputsAlreadyIn_ < 5) {
    buffer_[inputsAlreadyIn_] = input;
    inputsAlreadyIn_++;
  } else {
    for (int i = 0; i < 4; i++) {
      buffer_[i] = buffer_[i + 1];
    }
    buffer_[4] = input;
  }
  return 1;
}

void EcalFenixOddAmplitudeFilter::process(std::vector<int> &addout, std::vector<int> &output) {
  // test
  inputsAlreadyIn_ = 0;
  for (unsigned int i = 0; i < 5; i++) {
    buffer_[i] = 0;  // FIXME: 5
  }
  // test end

  for (unsigned int i = 0; i < addout.size(); i++) {
    // Only save TP info for Clock i >= 4 (from 0-9) because first 5 digis required to produce first ET value
    if (i >= 4) {
      if (TPinfoPrintout_)
        std::cout << i << std::dec;
    }
    setInput(addout[i]);
    process();  // This should probably be renamed to something meaningful and not the same as the very function it's being called in...
    output[i] = processedOutput_;
  }
  // shift the result by 1!
  for (unsigned int i = 0; i < (output.size()); i++) {
    if (i != output.size() - 1) {
      output[i] = output[i + 1];
    } else {
      output[i] = 0;
    }
  }
  return;
}

void EcalFenixOddAmplitudeFilter::process() {
  // UB FIXME: 5
  processedOutput_ = 0;
  if (inputsAlreadyIn_ < 5)
    return;
  int output = 0;

  for (int i = 0; i < 5; i++) {
    output += (weights_[i] * buffer_[i]) >> shift_;
  }

  if (output < 0)
    output = 0;
  if (output > 0X3FFFF)
    output = 0X3FFFF;
  processedOutput_ = output;

  // by RK
  if (TPinfoPrintout_) {
    std::cout << " " << stripid_;
    for (int i = 0; i < 5; i++) {
      std::cout << " " << weights_[i] << std::dec;
    }
    for (int i = 0; i < 5; i++) {
      std::cout << " " << weights_[i] / 64.0 << std::dec;
    }
    for (int i = 0; i < 5; i++) {
      std::cout << " " << buffer_[i] << std::dec;
    }  // Digis
    std::cout << " --> output: " << output;
    std::cout << " ODD";
    std::cout << std::endl;
  }
}

void EcalFenixOddAmplitudeFilter::setParameters(uint32_t raw,
                                                const EcalTPGOddWeightIdMap *ecaltpgOddWeightMap,
                                                const EcalTPGOddWeightGroup *ecaltpgOddWeightGroup) {
  stripid_ = raw;  // by RK
  uint32_t params_[5];
  const EcalTPGGroups::EcalTPGGroupsMap &groupmap = ecaltpgOddWeightGroup->getMap();
  EcalTPGGroups::EcalTPGGroupsMapItr it = groupmap.find(raw);
  if (it != groupmap.end()) {
    uint32_t weightid = (*it).second;
    const EcalTPGOddWeightIdMap::EcalTPGWeightMap &weightmap = ecaltpgOddWeightMap->getMap();
    EcalTPGOddWeightIdMap::EcalTPGWeightMapItr itw = weightmap.find(weightid);
    (*itw).second.getValues(params_[0], params_[1], params_[2], params_[3], params_[4]);

    // we have to transform negative coded in 7 bits into negative coded in 32
    // bits maybe this should go into the getValue method??
    // std::cout << "peak flag settings" << std::endl;

    for (int i = 0; i < 5; ++i) {
      weights_[i] = (params_[i] & 0x40) ? (int)(params_[i] | 0xffffffc0) : (int)(params_[i]);
      //std::cout << "ODD weight: "<<  std::dec << params_[i] << " --> " << std::dec << weights_[i] << std::endl;
      // Construct the peakFlag for sFGVB processing
      // peakFlag_[i] = ((params_[i] & 0x80) > 0x0) ? 1 : 0;
      // std::cout << " " << params_[i] << std::endl;
      // std::cout << " " << peakFlag_[i] << std::endl;
    }
    // std::cout << std::endl;
  } else
    edm::LogWarning("EcalTPG") << " could not find EcalTPGGroupsMap entry for " << raw;
}

#include "L1Trigger/L1TGlobal/interface/PrescalesVetosHelper.h"

using namespace l1t;


const PrescalesVetosHelper *  PrescalesVetosHelper::readFromEventSetup(const L1TGlobalPrescalesVetos * es){
  return new PrescalesVetosHelper(es);
}

PrescalesVetosHelper *  PrescalesVetosHelper::readAndWriteFromEventSetup(const L1TGlobalPrescalesVetos * es){
  PrescalesVetosHelper * x = new PrescalesVetosHelper(es);
  x->useCopy();
  return x;
}

PrescalesVetosHelper::PrescalesVetosHelper() {write_.version_ = VERSION; read_ = &write_; }

PrescalesVetosHelper::PrescalesVetosHelper(const L1TGlobalPrescalesVetos * es) {read_ = es; }

void PrescalesVetosHelper::useCopy(){
  write_ = (*read_);
  read_  = &write_;
}

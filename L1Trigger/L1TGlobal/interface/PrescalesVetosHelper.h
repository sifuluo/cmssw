#ifndef PRESCALESVETOSHELPERS_H__
#define PRESCALESVETOSHELPERS_H__

#include "CondFormats/L1TObjects/interface/L1TGlobalPrescalesVetos.h"

// If you want to just create a new object that you can read and write, use default ctor:
//
//   l1t::PrescalesVetosHelper x;
//
// If you wish to read the table from the EventSetup, and will only read, use this:
//
//   const PrescalesVetosHelper * x = PrescalesVetosHelper::readFromEventSetup(...)
//   //...
//   delete x;
//
// If you wish to read the table from the EventSetup, but then be able to edit the values locally, use this:
//
//   PrescalesVetorsHelper * x = PrescalesVetosHelper::readAndWriteFromEventSetup(...)
//   //...
///  delete x;
//
// but there's a performance penalty as a copy is made.

namespace l1t {

  class PrescalesVetosHelper {
  public:
    enum {VERSION = 1};
    
    //ctor if creating a new table (e.g. from XML or python file)
    PrescalesVetosHelper();
    //create for reading only, from the EventSetup:
    static const PrescalesVetosHelper * readFromEventSetup(const L1TGlobalPrescalesVetos * es);
    // create for reading and writing, starting from the EventSetup:
    static PrescalesVetosHelper * readAndWriteFromEventSetup(const L1TGlobalPrescalesVetos * es);

    int bxMaskDefault() const { return read_->bxmask_default_; };
    void setBxMaskDefault(int value) { write_.bxmask_default_ = value; };
    



       
  private:
    PrescalesVetosHelper(const L1TGlobalPrescalesVetos * es);
    void useCopy();

    // separating read from write allows for a high-performance read-only mode (as no copy is made):
    const L1TGlobalPrescalesVetos * read_;  // when reading/getting, use this.
    L1TGlobalPrescalesVetos write_; // when writing/setting, use this.     
  };

}
#endif

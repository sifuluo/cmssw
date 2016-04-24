//
// L1TMuonEndCapParams: parameters needed to calculte the EMTF algorithm
//

#ifndef l1t_L1TMuonendCapParams_h
#define l1t_L1TMuonendCapParams_h

#include <memory>
#include <iostream>
#include <vector>
#include <map>

#include "CondFormats/Serialization/interface/Serializable.h"

class L1TMuonEndCapParams {
 public:

  class DTreeNode {
  public:
    DTreeNode(){splitVar = ileft = iright = 0; splitVal = fitVal = 0.0;}
    int splitVar;
    double splitVal;
    double fitVal;
    unsigned ileft;
    unsigned iright;

    COND_SERIALIZABLE;
  };
  typedef std::vector<DTreeNode> DTree;
  typedef std::vector<DTree> DForest;
  typedef std::vector<DForest> DForestColl;
  typedef std::map<int,int> DForestMap;

  DForestColl forest_coll_;
  DForestMap forest_map_;
  		
  L1TMuonEndCapParams() { PtAssignVersion_=1; }
  ~L1TMuonEndCapParams() {}

  // this requires cleanup too, but leaving as is for now:
  unsigned PtAssignVersion_;
  int PhiMatchWindowSt1_, PhiMatchWindowSt2_, PhiMatchWindowSt3_, PhiMatchWindowSt4_;
  
  COND_SERIALIZABLE;
};
#endif

#ifndef HcalSimAlgos_HcalShapes_h
#define HcalSimAlgos_HcalShapes_h

/** A class which decides which shape to return,
   based on the DetId
*/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloShapes.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalShape.h"
#include "SimCalorimetry/HcalSimAlgos/interface/ZDCShape.h"
#include <map>
class CaloVShape;
class DetId;
class HcalMCParams;
class HcalTopology;

class HcalShapes : public CaloShapes
{
public:
  enum {HPD=101, LONG=102, ZECOTEK=201, HAMAMATSU=202, HE2017=203, HF=301, ZDC=401};
  HcalShapes();
  ~HcalShapes();

  void beginRun(edm::EventSetup const & es);
  void endRun();

  virtual const CaloVShape * shape(const DetId & detId, bool precise=false) const;

private:
  typedef std::map<int, const CaloVShape *> ShapeMap;
  // hardcoded, if we can't figure it out from the DB
  const CaloVShape * defaultShape(const DetId & detId, bool precise=false) const;
  const ShapeMap& getShapeMap(bool precise) const;
  HcalMCParams * theMCParams;
  const HcalTopology * theTopology;
  ShapeMap theShapes;
  ShapeMap theShapesPrecise;
  ZDCShape theZDCShape;
  //   list of vShapes.
  HcalShape theHcalShape101;
  HcalShape theHcalShape102;
  HcalShape theHcalShape103;
  HcalShape theHcalShape104;
  HcalShape theHcalShape105;
  HcalShape theHcalShape123;
  HcalShape theHcalShape124;
  HcalShape theHcalShape125;
  HcalShape theHcalShape201;
  HcalShape theHcalShape202;
  HcalShape theHcalShape203;
  HcalShape theHcalShape205;
  HcalShape theHcalShape301;
  HcalShape theHcalShape401;

};

#endif


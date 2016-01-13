//
// NOTE:  This file was automatically generated from UTM library via import_utm.pl
// DIRECT EDITS MIGHT BE LOST.
//
/**
 * @author      Takashi Matsushita
 * Created:     12 Mar 2015
 */

/** @todo nope */

#ifndef tmEventSetup_esCutValue_hh
#define tmEventSetup_esCutValue_hh

/*====================================================================*
 * declarations
 *====================================================================*/
/*-----------------------------------------------------------------*
 * headers
 *-----------------------------------------------------------------*/
#include <limits>
#include "CondFormats/Serialization/interface/Serializable.h"


/*-----------------------------------------------------------------*
 * constants
 *-----------------------------------------------------------------*/
/* nope */



/**
 *  This class implements data structure for CutValue
 */
struct L1TUtmCutValue
{
  // ctor
  L1TUtmCutValue()
    : value(std::numeric_limits<double>::max()),
      index(std::numeric_limits<unsigned int>::max()) { };

  double value;               /**< cut value */
  unsigned int index;         /**< HW index for the cut value */
  unsigned int version;
  COND_SERIALIZABLE;
};

#endif // tmEventSetup_esCutValue_hh
/* eof */

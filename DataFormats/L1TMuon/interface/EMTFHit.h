// Class for input trigger primitives to EMTF - AWB 04.01.16
// Based on L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitive.h
// In particular, see struct CSCData

#ifndef __l1t_EMTFHit_h__
#define __l1t_EMTFHit_h__

#include <vector>
#include <boost/cstdint.hpp> 

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"

namespace l1t {
  class EMTFHit {
  public:
    
  EMTFHit() :
    
    // Using -999 instead of -99 b/c this seems most common in the emulator.  Unfortunate. - AWB 17.03.16
    endcap(-999), station(-999), ring(-999), sector(-999), sector_GMT(-999), subsector(-999), chamber(-999), 
      layer(-999), csc_ID(-999), mpc_link(-999), wire(-999), strip(-999), zone_hit(-999), track_num(-999), 
      phi_hit(-999), phi_z_val(-999), phi_loc_int(-999), phi_loc_deg(-999), phi_loc_rad(-999), 
      phi_glob_deg(-999), phi_glob_rad(-999), phi_geom_rad(-999),
      theta_int(-999), theta_loc(-999), theta_deg(-999), theta_rad(-999), eta(-999), 
      quality(-999), pattern(-999), bend(-999), 
      valid(-999), sync_err(-999), bx0(-999), bx(-999),
      is_CSC_hit(false), is_RPC_hit(false)
      {};
    
    virtual ~EMTFHit() {};

    // Functions defined in src/EMTFHit.cc
    void ImportCSCDetId ( const CSCDetId& _detId);
    void ImportCSCCorrelatedLCTDigi ( const CSCCorrelatedLCTDigi& _digi);

    void SetZoneContribution (std::vector<int> vect_ints)  { zone_contribution = vect_ints; };
    void SetCSCDetId         (CSCDetId id)                 { csc_DetId         = id;        };
    
    const std::vector<int> Zone_contribution () const { return zone_contribution; };
    const CSCDetId CSC_DetId                 () const { return csc_DetId;         };
    
    void set_endcap         (int  bits) { endcap        = bits; };
    void set_station        (int  bits) { station       = bits; };
    void set_ring           (int  bits) { ring          = bits; };
    void set_sector         (int  bits) { sector        = bits; };
    void set_sector_GMT     (int  bits) { sector_GMT    = bits; };
    void set_subsector      (int  bits) { subsector     = bits; };
    void set_chamber        (int  bits) { chamber       = bits; };
    void set_layer          (int  bits) { layer         = bits; };
    void set_csc_ID         (int  bits) { csc_ID        = bits; };
    void set_mpc_link       (int  bits) { mpc_link      = bits; };
    void set_wire           (int  bits) { wire          = bits; };
    void set_strip          (int  bits) { strip         = bits; };
    void set_zone_hit       (int  bits) { zone_hit      = bits; };
    void set_track_num      (int  bits) { track_num     = bits; };
    void set_phi_hit        (int  bits) { phi_hit       = bits; };
    void set_phi_z_val      (int  bits) { phi_z_val     = bits; };
    void set_phi_loc_int    (int  bits) { phi_loc_int   = bits; };
    void set_phi_loc_deg    (float val) { phi_loc_deg    = val; };
    void set_phi_loc_rad    (float val) { phi_loc_rad    = val; };
    void set_phi_glob_deg   (float val) { phi_glob_deg   = val; };
    void set_phi_glob_rad   (float val) { phi_glob_rad   = val; };
    void set_phi_geom_rad   (float val) { phi_geom_rad   = val; };
    void set_theta_int      (int  bits) { theta_int     = bits; };
    void set_theta_loc      (float val) { theta_loc      = val; };
    void set_theta_deg      (float val) { theta_deg      = val; };
    void set_theta_rad      (float val) { theta_rad      = val; };
    void set_eta            (float val) { eta            = val; };
    void set_quality        (int  bits) { quality       = bits; };
    void set_pattern        (int  bits) { pattern       = bits; };
    void set_bend           (int  bits) { bend          = bits; };
    void set_valid          (int  bits) { valid         = bits; };
    void set_sync_err       (int  bits) { sync_err      = bits; };
    void set_bx0            (int  bits) { bx0           = bits; };
    void set_bx             (int  bits) { bx            = bits; };
    void set_is_CSC_hit     (bool expr) { is_CSC_hit    = expr; };
    void set_is_RPC_hit     (bool expr) { is_RPC_hit    = expr; };

    const int   Endcap         ()  const { return endcap   ;      };
    const int   Station        ()  const { return station  ;      };
    const int   Ring           ()  const { return ring     ;      };
    const int   Sector         ()  const { return sector   ;      };
    const int   Sector_GMT     ()  const { return sector_GMT;     };
    const int   Subsector      ()  const { return subsector;      };
    const int   Chamber        ()  const { return chamber  ;      };
    const int   Layer          ()  const { return layer    ;      };
    const int   CSC_ID         ()  const { return csc_ID   ;      };
    const int   MPC_link       ()  const { return mpc_link ;      };
    const int   Wire           ()  const { return wire     ;      };
    const int   Strip          ()  const { return strip    ;      };
    const int   Zone_hit       ()  const { return zone_hit ;      };
    const int   Track_num      ()  const { return track_num;      };
    const int   Phi_hit        ()  const { return phi_hit;        };
    const int   Phi_Z_val      ()  const { return phi_z_val;      };
    const int   Phi_loc_int    ()  const { return phi_loc_int;    };
    const float Phi_loc_deg    ()  const { return phi_loc_deg;    };
    const float Phi_loc_rad    ()  const { return phi_loc_rad;    };
    const float Phi_glob_deg   ()  const { return phi_glob_deg;   };
    const float Phi_glob_rad   ()  const { return phi_glob_rad;   };
    const float Phi_geom_rad   ()  const { return phi_geom_rad;   };
    const int   Theta_int      ()  const { return theta_int;      };
    const float Theta_loc      ()  const { return theta_loc;      };
    const float Theta_deg      ()  const { return theta_deg;      };
    const float Theta_rad      ()  const { return theta_rad;      };
    const float Eta            ()  const { return eta      ;      };
    const int   Quality        ()  const { return quality  ;      };
    const int   Pattern        ()  const { return pattern  ;      };
    const int   Bend           ()  const { return bend     ;      };
    const int   Valid          ()  const { return valid    ;      };
    const int   Sync_err       ()  const { return sync_err ;      };
    const int   BX0            ()  const { return bx0      ;      };
    const int   BX             ()  const { return bx       ;      };
    const bool  Is_CSC_hit     ()  const { return is_CSC_hit;     };
    const bool  Is_RPC_hit     ()  const { return is_RPC_hit;     };


  private:
    
    std::vector<int> zone_contribution;
    CSCDetId csc_DetId;
    
    int   endcap;
    int   station;
    int   ring;
    int   sector;
    int   sector_GMT;
    int   subsector;
    int   chamber;
    int   layer;
    int   csc_ID;
    int   mpc_link;
    int   wire;
    int   strip;
    int   zone_hit;
    int   track_num;
    int   phi_hit;
    int   phi_z_val;
    int   phi_loc_int;
    float phi_loc_deg;
    float phi_loc_rad;
    float phi_glob_deg;
    float phi_glob_rad;
    float phi_geom_rad; // The global phi value returned by L1Trigger/L1TMuon/interface/deprecate/GeometryTranslator.h
    int   theta_int;
    float theta_loc; // This is some bizzare local definition of theta
    float theta_deg; // This is the true global theta in degrees
    float theta_rad; // This is the true global theta in radians
    float eta;
    int   quality;
    int   pattern;
    int   bend;
    int   valid;
    int   sync_err;
    int   bx0;
    int   bx;
    bool  is_CSC_hit;
    bool  is_RPC_hit;

  }; // End of class EMTFHit
  
  // Define a vector of EMTFHit
  typedef std::vector<EMTFHit> EMTFHitCollection;
  
} // End of namespace l1t

#endif /* define __l1t_EMTFHit_h__ */

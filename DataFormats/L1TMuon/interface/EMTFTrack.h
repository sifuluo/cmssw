// Class for muon tracks in EMTF - AWB 04.01.16
// Mostly copied from L1Trigger/L1TMuonEndCap/interface/MuonInternalTrack.h

#ifndef __l1t_EMTFTrack_h__
#define __l1t_EMTFTrack_h__

#include <vector>
#include <boost/cstdint.hpp> 

#include "DataFormats/L1TMuon/interface/EMTFHit.h"

namespace l1t {
  class EMTFTrack {
  public:
    
  EMTFTrack() :
    // Using -999 instead of -99 b/c this seems most common in the emulator.  Unfortunate. - AWB 17.03.16
    endcap(-999), sector(-999), sector_GMT(-999), type(-999), mode(-999), quality(-999), rank(-999), 
      straightness(-999), first_bx(-999), second_bx(-999), pt_int(-999), pt(-999), pt_GMT(-999), 
      theta_int(-999), theta_deg(-999), theta_rad(-999), eta_int(-999), eta(-999), eta_GMT(-999), 
      phi_loc_int(-999), phi_loc_deg(-999), phi_loc_rad(-999), phi_glob_deg(-999), phi_glob_rad(-999), 
      phi_GMT(-999), charge(-999), isGMT(0),
      numHits(0)
	{};
    
    virtual ~EMTFTrack() {};
    
    void push_Hit(EMTFHit bits)  { _Hits.push_back(bits); numHits += 1; };
    const int NumHits() const { return numHits; };
    EMTFHitCollection Hits()  { return _Hits; };
    const EMTFHitCollection * PtrHits() const { return &_Hits; };
    
    /* // Can't have a vector of vectors of vectors in ROOT files */
    /* void set_deltas (std::vector< std::vector<int> > _deltas) { deltas = _deltas; } */
    void set_phis   (std::vector<int> _phis)   { phis   = _phis; }
    void set_thetas (std::vector<int> _thetas) { thetas = _thetas; }
    
    void set_endcap        (int  bits) { endcap       = bits; };
    void set_sector        (int  bits) { sector       = bits; };
    void set_sector_GMT    (int  bits) { sector_GMT   = bits; };
    void set_type          (int  bits) { type         = bits; };
    void set_mode          (int  bits) { mode         = bits; };
    void set_quality       (int  bits) { quality      = bits; };
    void set_rank          (int  bits) { rank         = bits; };
    void set_straightness  (int  bits) { straightness = bits; };
    void set_first_bx      (int  bits) { first_bx     = bits; };
    void set_second_bx     (int  bits) { second_bx    = bits; };
    void set_pt_int        (int  bits) { pt_int       = bits; };
    void set_pt            (float val) { pt           = val;  };
    void set_pt_GMT        (int  bits) { pt_GMT       = bits; };
    void set_theta_int     (int  bits) { theta_int    = bits; };
    void set_theta_deg     (float val) { theta_deg    = val;  };
    void set_theta_rad     (float val) { theta_rad    = val;  };
    void set_eta_int       (int  bits) { eta_int      = bits; };
    void set_eta           (float val) { eta          = val;  };
    void set_eta_GMT       (int  bits) { eta_GMT      = bits; };
    void set_phi_loc_int   (int  bits) { phi_loc_int  = bits; };
    void set_phi_loc_deg   (float val) { phi_loc_deg  = val;  };
    void set_phi_loc_rad   (float val) { phi_loc_rad  = val;  };
    void set_phi_glob_deg  (float val) { phi_glob_deg = val;  };
    void set_phi_glob_rad  (float val) { phi_glob_rad = val;  };
    void set_phi_GMT       (int  bits) { phi_GMT      = bits; };
    void set_charge        (int  bits) { charge       = bits; }
    void set_isGMT         (int  bits) { isGMT        = bits; }
    
    const int   Endcap()        const { return  endcap;       };
    const int   Sector()        const { return  sector;       };
    const int   Sector_GMT()    const { return  sector_GMT;   };
    const int   Type()          const { return  type;         };
    const int   Mode()          const { return  mode;         };
    const int   Quality()       const { return  quality;      };
    const int   Rank()          const { return  rank;         };
    const int   Straightness()  const { return  straightness; };
    const int   First_BX()      const { return  first_bx;     };
    const int   Second_BX()     const { return  second_bx;    };
    const int   Pt_int()        const { return  pt_int;       };
    const float Pt()            const { return  pt;           };
    const int   Pt_GMT()        const { return  pt_GMT;       };
    const int   Theta_int()     const { return  theta_int;    };
    const float Theta_deg()     const { return  theta_deg;    };
    const float Theta_rad()     const { return  theta_rad;    };
    const int   Eta_int()       const { return  eta_int;      };
    const float Eta()           const { return  eta;          };
    const int   Eta_GMT()       const { return  eta_GMT;      };
    const int   Phi_loc_int()   const { return  phi_loc_int;  };
    const float Phi_loc_deg()   const { return  phi_loc_deg;  };
    const float Phi_loc_rad()   const { return  phi_loc_rad;  };
    const float Phi_glob_deg()  const { return  phi_glob_deg; };
    const float Phi_glob_rad()  const { return  phi_glob_rad; };
    const int   Phi_GMT()       const { return  phi_GMT;      };
    const int   Charge()        const { return  charge;       };
    const int   IsGMT()         const { return  isGMT;        }
    
    
  private:
    
    EMTFHitCollection _Hits;
    
    /* // Can't have a vector of vectors of vectors in ROOT files */
    /* std::vector< std::vector<int> > deltas; */
    std::vector<int> phis;
    std::vector<int> thetas;
    
    int   endcap; 
    int   sector; 
    int   sector_GMT; 
    int   type; 
    int   mode; 
    int   quality; 
    int   rank;
    int   straightness;
    int   first_bx;
    int   second_bx;
    int   pt_int;
    float pt;
    int   pt_GMT;
    int   theta_int; // This is some bizzare local definition of theta
    float theta_deg; // This is the true global theta in degrees
    float theta_rad; // This is the true global theta in radians
    int   eta_int;
    float eta;
    int   eta_GMT;
    int   phi_loc_int;
    float phi_loc_deg;
    float phi_loc_rad;
    float phi_glob_deg;
    float phi_glob_rad;
    int   phi_GMT;
    int   charge;
    int   isGMT;
    int   numHits;
    
  }; // End of class EMTFTrack
  
  // Define a vector of EMTFTrack
  typedef std::vector<EMTFTrack> EMTFTrackCollection;
  
} // End of namespace l1t

#endif /* define __l1t_EMTFTrack_h__ */

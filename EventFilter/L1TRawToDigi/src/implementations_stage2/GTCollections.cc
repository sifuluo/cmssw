#include "FWCore/Framework/interface/Event.h"

#include "GTCollections.h"

#include <iostream>
using namespace std;

namespace l1t {
   namespace stage2 {
      GTCollections::~GTCollections()
      {

	cout << "DEBUG GTCollections:  Muon Size at BX=0" << muons_->size(0) << "\n"; 
        event_.put(muons_, "Muon");
	event_.put(egammas_, "EGamma");
	event_.put(etsums_, "EtSum");
	event_.put(jets_, "Jet");
	event_.put(taus_, "Tau");
	
	event_.put(algBlk_);
	event_.put(extBlk_); 

      }
   }
}

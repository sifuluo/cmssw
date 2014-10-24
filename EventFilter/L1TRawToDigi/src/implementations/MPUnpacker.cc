#include "FWCore/Framework/interface/MakerMacros.h"

#include "EventFilter/L1TRawToDigi/interface/Unpacker.h"

#include "CaloCollections.h"

namespace l1t {
   class MPUnpacker : public Unpacker {
      public:
         virtual bool unpack(const unsigned block_id, const unsigned size, const unsigned char *data, UnpackerCollections *coll) override;
   };
}

// Implementation

namespace l1t {
   bool
   MPUnpacker::unpack(const unsigned block_id, const unsigned size, const unsigned char *data, UnpackerCollections *coll)
   {

     LogDebug("L1T") << "Block ID  = " << block_id << " size = " << size;

     auto res1_ = static_cast<CaloCollections*>(coll)->getMPJets();
     auto res2_ = static_cast<CaloCollections*>(coll)->getMPEtSums();

     // Only one BX readout makes sense from each MP
     res1_->setBXRange(0,0);
     res2_->setBXRange(0,0);

     // Initialise index
     int unsigned i = 0;

     // ET / MET(x) / MET (y)

     uint32_t raw_data = pop(data,i); // pop advances the index i internally

     l1t::EtSum et = l1t::EtSum();
    
     et.setHwPt(raw_data & 0xFFFFF);

     switch (block_id){

     case 1:
       et.setType(l1t::EtSum::kTotalEt);              
       break;

     case 7:
       et.setType(l1t::EtSum::kTotalEt);
       break;

     case 3:
       et.setType(l1t::EtSum::kTotalEtx);
       break;

     case 9:
       et.setType(l1t::EtSum::kTotalEtx);
       break;

     case 5:
       et.setType(l1t::EtSum::kTotalEty);
       break;

     case 11:
       et.setType(l1t::EtSum::kTotalEty);
       break;

     }

     LogDebug("L1T") << "ET/METx/METy: pT " << et.hwPt();

     res2_->push_back(0,et);

     // Skip 10 empty frames
     for (int j=0; j<10; j++) raw_data=pop(data,i); 

     // HT / MHT(x)/ MHT (y)

     raw_data = pop(data,i); // pop advances the index i internally

     l1t::EtSum ht = l1t::EtSum();
    
     ht.setHwPt(raw_data & 0xFFFFF);

     switch (block_id){

     case 1:
       ht.setType(l1t::EtSum::kTotalHt);
       break;

     case 7:
       ht.setType(l1t::EtSum::kTotalHt);
       break;

     case 3:
       ht.setType(l1t::EtSum::kTotalHtx);
       break;

     case 9:
       ht.setType(l1t::EtSum::kTotalHtx);
       break;

     case 5:
       ht.setType(l1t::EtSum::kTotalHty);
       break;

     case 11:
       ht.setType(l1t::EtSum::kTotalHty);
       break;

     }

     LogDebug("L1T") << "HT/MHTx/MHTy: pT " << ht.hwPt();

     res2_->push_back(0,ht);

     // Skip 25 empty frames                                                                                                                                             
     for (int j=0; j<25; j++) raw_data=pop(data,i);

     // Two jets
     for (unsigned nJet=0; nJet < 2; nJet++){
       raw_data = pop(data,i); // pop advances the index i internally

       if (raw_data == 0)
            continue;

       l1t::Jet jet = l1t::Jet();

       int etasign = 1;
       if ((block_id == 7) ||
           (block_id == 9) ||
           (block_id == 11)) {
         etasign = -1;
       }

       LogDebug("L1") << "block ID=" << block_id << " etasign=" << etasign;

       //       jet.setHwEta(etasign*(raw_data & 0x3F));
       jet.setHwEta(raw_data & 0x3F);
       jet.setHwPhi((raw_data >> 6) & 0x7F);
       jet.setHwPt((raw_data >> 13) & 0xFFFF);
         
       LogDebug("L1T") << "Jet: eta " << jet.hwEta() << " phi " << jet.hwPhi() << " pT " << jet.hwPt() << " qual " << jet.hwQual();

       res1_->push_back(0,jet);
     }

     return true;
   }
}

DEFINE_L1T_UNPACKER(l1t::MPUnpacker);

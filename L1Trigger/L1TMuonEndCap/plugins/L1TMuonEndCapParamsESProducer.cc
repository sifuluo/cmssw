#include <iostream>
#include <memory>
#include <iostream>
#include "boost/shared_ptr.hpp"

#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESProducts.h"

#include "CondFormats/L1TObjects/interface/L1TMuonEndCapParams.h"
#include "CondFormats/DataRecord/interface/L1TMuonEndCapParamsRcd.h"
#include "L1Trigger/L1TMuonEndCap/interface/EndCapParamsHelper.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "TXMLEngine.h"

#include "L1Trigger/L1TMuonEndCap/interface/Tree.h"

using namespace std;

// class declaration

class L1TMuonEndCapParamsESProducer : public edm::ESProducer {
public:
  L1TMuonEndCapParamsESProducer(const edm::ParameterSet&);
  ~L1TMuonEndCapParamsESProducer();
  
  typedef boost::shared_ptr<L1TMuonEndCapParams> ReturnType;

  ReturnType produce(const L1TMuonEndCapParamsRcd&);
private:
  l1t::EndCapParamsHelper data_;
};

L1TMuonEndCapParamsESProducer::L1TMuonEndCapParamsESProducer(const edm::ParameterSet& iConfig) :
  data_(new L1TMuonEndCapParams())
{
   //the following line is needed to tell the framework what
   // data is being produced
   setWhatProduced(this);

   data_.SetPtAssignVersion(iConfig.getParameter<int>("PtAssignVersion"));
   data_.SetSt1PhiMatchWindow(iConfig.getParameter<int>("St1MatchWindow"));
   data_.SetSt2PhiMatchWindow(iConfig.getParameter<int>("St2MatchWindow"));
   data_.SetSt3PhiMatchWindow(iConfig.getParameter<int>("St3MatchWindow"));
   data_.SetSt4PhiMatchWindow(iConfig.getParameter<int>("St4MatchWindow"));
      
   int allowedModes[11] = {3,5,9,6,10,12,7,11,13,14,15};
   for(int modes=10;modes<11;modes++){  // switch back to 11...

     std::stringstream ss;
     ss << "L1Trigger/L1TMuon/data/emtf_luts/ModeVariables/trees/" << allowedModes[modes];
     std::string directory;
     ss >> directory;

     int numTrees = 64;
     double sum = 0;
     for(int i=0; i < numTrees; i++) 
       {   
	 std::stringstream ss;
	 ss << directory << "/" << i << ".xml";
	 std::string filename;
	 ss >> filename;
	 cout << "INFO: loading tree " << filename << "\n";
	 //DEBUG: mode:  15 dir L1Trigger/L1TMuon/data/emtf_luts/ModeVariables/trees/15 num trees 64
	 //DEBUG: pred val:  0.072506
	 //DEBUG: data = 1, 1.375, 27, 16, 4, 1, 

         //DEBUG: mode:  15 dir L1Trigger/L1TMuon/data/emtf_luts/ModeVariables/trees/15 num trees 64
	 //DEBUG: pred val:  0.021769
	 //DEBUG: data = 1, 1.325, 7, 0, 0, 0, 
	 
	 data_.loadTreeFromXML(edm::FileInPath(ss.str().c_str()).fullPath().c_str());
	 //std::vector<double> x = {1, 1.375, 27, 16, 4, 1};
	 std::vector<double> x = {1, 1.325, 7, 0, 0, 0}; 
	 double y = data_.evalTree(x, data_.dtree_test);
	 cout << "INFO: predicted value:  " << y << "\n";
	 sum += y;
       }
     cout << "TOTAL PRED VALS:  " << sum << "\n";
   }
}


L1TMuonEndCapParamsESProducer::~L1TMuonEndCapParamsESProducer()
{
}



//
// member functions
//

// ------------ method called to produce the data  ------------
L1TMuonEndCapParamsESProducer::ReturnType
L1TMuonEndCapParamsESProducer::produce(const L1TMuonEndCapParamsRcd& iRecord)
{
   using namespace edm::es;
   boost::shared_ptr<L1TMuonEndCapParams> pEMTFParams;

   pEMTFParams = boost::shared_ptr<L1TMuonEndCapParams>(data_.getWriteInstance());
   return pEMTFParams;
   
}

//define this as a plug-in
DEFINE_FWK_EVENTSETUP_MODULE(L1TMuonEndCapParamsESProducer);

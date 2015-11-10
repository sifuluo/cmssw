////
/// \class l1t::L1TGlobalUtil.cc
///
/// Description: Dump Accessors for L1 GT Result.
///
/// Implementation:
///    
///
/// \author: Brian Winer Ohio State
///
/// 
#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"

#include "CondFormats/DataRecord/interface/L1TGlobalTriggerMenuRcd.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"


#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageLogger/interface/MessageDrop.h"

// constructor
l1t::L1TGlobalUtil::L1TGlobalUtil() 
{

    // initialize cached IDs
    m_l1GtMenuCacheID = 0ULL;

}

// destructor
l1t::L1TGlobalUtil::~L1TGlobalUtil() {
 
}


void l1t::L1TGlobalUtil::retrieveL1(const edm::Event& iEvent, const edm::EventSetup& evSetup,
                                    edm::EDGetToken gtAlgToken) {

// get / update the trigger menu from the EventSetup
// local cache & check on cacheIdentifier
    unsigned long long l1GtMenuCacheID = evSetup.get<L1TGlobalTriggerMenuRcd>().cacheIdentifier();

    if (m_l1GtMenuCacheID != l1GtMenuCacheID) {

        //std::cout << "Attempting to get the Menu " << std::endl;
        edm::ESHandle< TriggerMenu> l1GtMenu;
        evSetup.get< L1TGlobalTriggerMenuRcd>().get(l1GtMenu) ;
        m_l1GtMenu =  l1GtMenu.product();
       //(const_cast<TriggerMenu*>(m_l1GtMenu))->buildGtConditionMap();

        //std::cout << "Attempting to fill the map " << std::endl;
        m_algorithmMap = &(m_l1GtMenu->gtAlgorithmMap());

	//reset vectors since we have new menu
	resetDecisionVectors();
	
	m_l1GtMenuCacheID = l1GtMenuCacheID;
    }

// Get the Global Trigger Output Algorithm block
     iEvent.getByToken(gtAlgToken,m_uGtAlgBlk);

     // get the GlabalAlgBlk (Stupid find better way)
     std::vector<GlobalAlgBlk>::const_iterator algBlk = m_uGtAlgBlk->begin(0);     

     // Make a map of the trigger name and whether it passed various stages (initial,prescale,final)
     // Note: might be able to improve performance by not full remaking map with names each time
     for (CItAlgo itAlgo = m_algorithmMap->begin(); itAlgo != m_algorithmMap->end(); itAlgo++) {

       // Get the algorithm name
       std::string algName = itAlgo->first;
       int algBit = (itAlgo->second).algoBitNumber();

       bool decisionInitial   = algBlk->getAlgoDecisionInitial(algBit);
       (m_decisionsInitial[algBit]).first  = algName;
       (m_decisionsInitial[algBit]).second = decisionInitial;

       bool decisionPrescaled = algBlk->getAlgoDecisionPreScaled(algBit);
       (m_decisionsPrescaled[algBit]).first  = algName;
       (m_decisionsPrescaled[algBit]).second = decisionPrescaled;

       bool decisionFinal     = algBlk->getAlgoDecisionFinal(algBit);
       (m_decisionsFinal[algBit]).first  = algName;
       (m_decisionsFinal[algBit]).second = decisionFinal;
      
     }  
}

void l1t::L1TGlobalUtil::resetDecisionVectors() {

  // Reset all the vector contents with null information
  int maxTriggers = 128; //Get this from a standard location
  m_decisionsInitial.clear();
  m_decisionsInitial.resize(maxTriggers);
  m_decisionsPrescaled.clear();
  m_decisionsPrescaled.resize(maxTriggers);
  m_decisionsFinal.clear();
  m_decisionsFinal.resize(maxTriggers);

  for(int algBit = 0; algBit< maxTriggers; algBit++) {

    (m_decisionsInitial.at(algBit)).first = "NULL";
    (m_decisionsInitial.at(algBit)).second = false;

    (m_decisionsPrescaled.at(algBit)).first = "NULL";
    (m_decisionsPrescaled.at(algBit)).second = false;
    
    (m_decisionsFinal.at(algBit)).first = "NULL";
    (m_decisionsFinal.at(algBit)).second = false;    

  }


}

const bool l1t::L1TGlobalUtil::getAlgBitFromName(const std::string& algName, int& bit) const {
  
    CItAlgo itAlgo = m_algorithmMap->find(algName);
    if(itAlgo != m_algorithmMap->end()) {
        bit = (itAlgo->second).algoBitNumber();
	return true;
    }
        
    return false; //did not find anything by that name
}

const bool l1t::L1TGlobalUtil::getAlgNameFromBit(int& bit, std::string& algName) const {

  // since we are just looking up the name, doesn't matter which vector we get it from
  if((m_decisionsInitial.at(bit)).first != "NULL") {
    algName = (m_decisionsInitial.at(bit)).first;
    return true;
  }
  return false; //No name associated with this bit
  
}

const bool l1t::L1TGlobalUtil::getInitialDecisionByBit(int& bit, bool& decision) const {

  /*
    for(std::vector<GlobalAlgBlk>::const_iterator algBlk = m_uGtAlgBlk->begin(0); algBlk != m_uGtAlgBlk->end(0); ++algBlk) { 
      decision = algBlk->getAlgoDecisionFinal(bit);            
    } 
  */
  // Need some check that this is a valid bit
  if((m_decisionsInitial.at(bit)).first != "NULL") {
    decision = (m_decisionsInitial.at(bit)).second;
    return true;
  }
  
  return false;  //couldn't get the information requested. 
}
const bool l1t::L1TGlobalUtil::getPrescaledDecisionByBit(int& bit, bool& decision) const {

  // Need some check that this is a valid bit
  if((m_decisionsPrescaled.at(bit)).first != "NULL") {
    decision = (m_decisionsPrescaled.at(bit)).second;
    return true;
  }
  
  return false;  //couldn't get the information requested. 
}
const bool l1t::L1TGlobalUtil::getFinalDecisionByBit(int& bit, bool& decision) const {

  // Need some check that this is a valid bit
  if((m_decisionsFinal.at(bit)).first != "NULL") {
    decision = (m_decisionsFinal.at(bit)).second;
    return true;
  }
  
  return false;  //couldn't get the information requested. 
}

const bool l1t::L1TGlobalUtil::getInitialDecisionByName(const std::string& algName, bool& decision) const {

  int bit = -1;
  if(getAlgBitFromName(algName,bit)) {
    decision = (m_decisionsInitial.at(bit)).second;
    return true;
  }
  
  return false;  //trigger name was not the menu. 
}

const bool l1t::L1TGlobalUtil::getPrescaledDecisionByName(const std::string& algName, bool& decision) const {

  int bit = -1;
  if(getAlgBitFromName(algName,bit)) {
    decision = (m_decisionsPrescaled.at(bit)).second;
    return true;
  }
  
  return false;  //trigger name was not the menu. 
}
  
const bool l1t::L1TGlobalUtil::getFinalDecisionByName(const std::string& algName, bool& decision) const {

  int bit = -1;
  if(getAlgBitFromName(algName,bit)) {
    decision = (m_decisionsFinal.at(bit)).second;
    return true;
  }
  
  return false;  //trigger name was not the menu. 
}

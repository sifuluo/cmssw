#ifndef L1TGlobalUtil_h
#define L1TGlobalUtil_h

/**
 * \class L1TGlobalUtil
 *
 *
 * Description: Accessor Class for uGT Result
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *
 */

// system include files
#include <vector>

#include "L1Trigger/L1TGlobal/interface/TriggerMenu.h"

// Objects to produce for the output record.
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "DataFormats/L1TGlobal/interface/GlobalExtBlk.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/EventSetup.h"

// forward declarations
//class TriggerMenu;


// class declaration

namespace l1t {

class L1TGlobalUtil
{

public:

    // constructors
    L1TGlobalUtil();

    // destructor
    virtual ~L1TGlobalUtil();

public:

    /// initialize the class (mainly reserve)
    void retrieveL1(const edm::Event& iEvent, const edm::EventSetup& evSetup,
                    edm::EDGetToken gtAlgToken);


public:

    inline void setVerbosity(const int verbosity) {
        m_verbosity = verbosity;
    }

    // get the trigger bit from the name
    const bool getAlgBitFromName(const std::string& AlgName, int& bit) const;

    // get the name from the trigger bit
    const bool getAlgNameFromBit(int& bit, std::string& AlgName) const;
				 
    // Access results for particular trigger bit 
    const bool getInitialDecisionByBit(int& bit,   bool& decision) const;
    const bool getPrescaledDecisionByBit(int& bit, bool& decision) const;
    const bool getFinalDecisionByBit(int& bit,     bool& decision) const;
    const bool getPrescaleByBit(int& bit,           int& prescale) const;
    const bool getMaskByBit(int& bit,              bool&     mask) const;

    // Access results for particular trigger name
    const bool getInitialDecisionByName(const std::string& algName,   bool& decision) const;
    const bool getPrescaledDecisionByName(const std::string& algName, bool& decision) const;
    const bool getFinalDecisionByName(const std::string& algName,     bool& decision) const;
    const bool getPrescaleByName(const std::string& algName,           int& prescale) const;
    const bool getMaskByName(const std::string& algName,              bool&     mask) const;

    // Some inline commands to return the full vectors
    inline const std::vector<std::pair<std::string, bool> >& decisionsInitial()   { return m_decisionsInitial; }
    inline const std::vector<std::pair<std::string, bool> >& decisionsPrescaled() { return m_decisionsPrescaled; }
    inline const std::vector<std::pair<std::string, bool> >& decisionsFinal()     { return m_decisionsFinal; }
    inline const std::vector<std::pair<std::string, int> >&  prescales()          { return m_prescales; }
    inline const std::vector<std::pair<std::string, bool> >& masks()              { return m_masks; }
    
private:

    /// clear decision vectors on a menu change
    void resetDecisionVectors();
    void resetPrescaleVectors();
    void resetMaskVectors();

    // trigger menu
    const TriggerMenu* m_l1GtMenu;
    unsigned long long m_l1GtMenuCacheID;

    // prescales and masks
    bool m_filledPrescales;

    // algorithm maps
    const AlgorithmMap* m_algorithmMap;
    
private:

    // access to the results block from uGT 
    edm::Handle<BXVector<GlobalAlgBlk>>  m_uGtAlgBlk;
    

    // Vectors containing the trigger name and information about that trigger
    std::vector<std::pair<std::string, bool> > m_decisionsInitial;
    std::vector<std::pair<std::string, bool> > m_decisionsPrescaled;
    std::vector<std::pair<std::string, bool> > m_decisionsFinal;
    std::vector<std::pair<std::string, int> >  m_prescales;
    std::vector<std::pair<std::string, bool> > m_masks;
    
    /// verbosity level
    int m_verbosity;
    

};

}
#endif

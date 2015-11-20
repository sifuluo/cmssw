/**
 * \class CorrCondition
 *
 *
 * Description: evaluation of a correlation condition.
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *
 *
 */

// this class header
#include "L1Trigger/L1TGlobal/interface/CorrCondition.h"

// system include files
#include <iostream>
#include <iomanip>

#include <string>
#include <vector>
#include <algorithm>

// user include files
//   base classes
#include "L1Trigger/L1TGlobal/interface/CorrelationTemplate.h"
#include "L1Trigger/L1TGlobal/interface/ConditionEvaluation.h"

#include "L1Trigger/L1TGlobal/interface/MuCondition.h"
#include "L1Trigger/L1TGlobal/interface/CaloCondition.h"
#include "L1Trigger/L1TGlobal/interface/EnergySumCondition.h"
#include "L1Trigger/L1TGlobal/interface/MuonTemplate.h"
#include "L1Trigger/L1TGlobal/interface/CaloTemplate.h"
#include "L1Trigger/L1TGlobal/interface/EnergySumTemplate.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"

#include "DataFormats/L1Trigger/interface/L1Candidate.h"
/*#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
*/

#include "CondFormats/L1TObjects/interface/GlobalStableParameters.h"
#include "CondFormats/DataRecord/interface/L1TGlobalStableParametersRcd.h"

#include "L1Trigger/GlobalTrigger/interface/L1GlobalTriggerFunctions.h"
#include "L1Trigger/L1TGlobal/interface/GtBoard.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageLogger/interface/MessageDrop.h"

// constructors
//     default
l1t::CorrCondition::CorrCondition() :
    ConditionEvaluation() {

/*  //BLW comment out for now
    m_ifCaloEtaNumberBits = -1;
    m_corrParDeltaPhiNrBins = 0;
*/
}

//     from base template condition (from event setup usually)
l1t::CorrCondition::CorrCondition(const GtCondition* corrTemplate, 
                                  const GtCondition* cond0Condition,
				  const GtCondition* cond1Condition,
				  const GtBoard* ptrGTB
        ) :
    ConditionEvaluation(),
    m_gtCorrelationTemplate(static_cast<const CorrelationTemplate*>(corrTemplate)),
    m_gtCond0(cond0Condition), m_gtCond1(cond1Condition),
    m_uGtB(ptrGTB)
{



}

// copy constructor
void l1t::CorrCondition::copy(const l1t::CorrCondition& cp) {

    m_gtCorrelationTemplate = cp.gtCorrelationTemplate();
    m_uGtB = cp.getuGtB();

    m_condMaxNumberObjects = cp.condMaxNumberObjects();
    m_condLastResult = cp.condLastResult();
    m_combinationsInCond = cp.getCombinationsInCond();

    m_verbosity = cp.m_verbosity;

}

l1t::CorrCondition::CorrCondition(const l1t::CorrCondition& cp) :
    ConditionEvaluation() {

    copy(cp);

}

// destructor
l1t::CorrCondition::~CorrCondition() {

    // empty

}

// equal operator
l1t::CorrCondition& l1t::CorrCondition::operator=(const l1t::CorrCondition& cp) {
    copy(cp);
    return *this;
}

// methods
void l1t::CorrCondition::setGtCorrelationTemplate(const CorrelationTemplate* caloTempl) {

    m_gtCorrelationTemplate = caloTempl;

}

///   set the pointer to uGT GtBoard
void l1t::CorrCondition::setuGtB(const GtBoard* ptrGTB) {

    m_uGtB = ptrGTB;

}

/* //BLW COmment out for now
//   set the number of bits for eta of calorimeter objects
void l1t::CorrCondition::setGtIfCaloEtaNumberBits(const int& ifCaloEtaNumberBitsValue) {

    m_ifCaloEtaNumberBits = ifCaloEtaNumberBitsValue;

}

//   set the maximum number of bins for the delta phi scales
void l1t::CorrCondition::setGtCorrParDeltaPhiNrBins(
        const int& corrParDeltaPhiNrBins) {

    m_corrParDeltaPhiNrBins = corrParDeltaPhiNrBins;

}
*/


// try all object permutations and check spatial correlations, if required
const bool l1t::CorrCondition::evaluateCondition(const int bxEval) const {

    // std::cout << "m_isDebugEnabled = " << m_isDebugEnabled << std::endl;
    // std::cout << "m_verbosity = " << m_verbosity << std::endl;

    std::ostringstream myCout;
    m_gtCorrelationTemplate->print(myCout);
    LogDebug("L1TGlobal") << "Correlation Condition Evaluation \n" << myCout.str() << std::endl;

    bool condResult = false;
    bool reqObjResult = false;

    // number of objects in condition (it is 2, no need to retrieve from
    // condition template) and their type
    int nObjInCond = 2;
    std::vector<L1GtObject> cndObjTypeVec(nObjInCond);

    // evaluate first the two sub-conditions (Type1s)

    const GtConditionCategory cond0Categ = m_gtCorrelationTemplate->cond0Category();
    const GtConditionCategory cond1Categ = m_gtCorrelationTemplate->cond1Category();

    const MuonTemplate* corrMuon = 0;
//    const CaloTemplate* corrCalo = 0;
//    const EnergySumTemplate* corrEnergySum = 0;

    // FIXME copying is slow...
    CombinationsInCond cond0Comb;
    CombinationsInCond cond1Comb;
    
    switch (cond0Categ) {
        case CondMuon: {
            corrMuon = static_cast<const MuonTemplate*>(m_gtCond0);
            MuCondition muCondition(corrMuon, m_uGtB,
                    0,0); //BLW these are counts that don't seem to be used...perhaps remove

            muCondition.evaluateConditionStoreResult(bxEval);
            reqObjResult = muCondition.condLastResult();

            cond0Comb = (muCondition.getCombinationsInCond());
            cndObjTypeVec[0] = (corrMuon->objectType())[0];

            if (m_verbosity ) {
                std::ostringstream myCout;
                muCondition.print(myCout);

                LogDebug("L1TGlobal") << myCout.str() << std::endl;
            }
        }
            break;
        case CondCalo: {
/*            corrCalo = static_cast<const CaloTemplate*>(m_gtCond0);

            CaloCondition caloCondition(corrCalo, m_gtPSB,
                    m_cond0NrL1Objects, m_cond0NrL1Objects, m_cond0NrL1Objects,
                    m_cond0NrL1Objects, m_cond0NrL1Objects, m_cond0EtaBits);

            caloCondition.evaluateConditionStoreResult();
            reqObjResult = caloCondition.condLastResult();

            cond0Comb = (caloCondition.getCombinationsInCond());
            cndObjTypeVec[0] = (corrCalo->objectType())[0];

            if (m_verbosity) {
                std::ostringstream myCout;
                caloCondition.print(myCout);

                LogTrace("L1GlobalTrigger") << myCout.str() << std::endl;
            }
*/        }
            break;
        case CondEnergySum: {
/*            corrEnergySum = static_cast<const EnergySumTemplate*>(m_gtCond0);
            EnergySumCondition eSumCondition(corrEnergySum, m_gtPSB);

            eSumCondition.evaluateConditionStoreResult();
            reqObjResult = eSumCondition.condLastResult();

            cond0Comb = (eSumCondition.getCombinationsInCond());
            cndObjTypeVec[0] = (corrEnergySum->objectType())[0];

            if (m_verbosity ) {
                std::ostringstream myCout;
                eSumCondition.print(myCout);

                LogTrace("L1GlobalTrigger") << myCout.str() << std::endl;
            }
*/        }
            break;
        default: {
            // should not arrive here, there are no correlation conditions defined for this object
            return false;
        }
            break;
    }

    // return if first subcondition is false
    if (!reqObjResult) {
            LogDebug("L1TGlobal")
                    << "\n  First sub-condition false, second sub-condition not evaluated and not printed."
                    << std::endl;
        return false;
    }

    // second object
    reqObjResult = false;

    switch (cond1Categ) {
        case CondMuon: {
            corrMuon = static_cast<const MuonTemplate*>(m_gtCond1);
            MuCondition muCondition(corrMuon, m_uGtB,
                    0,0); //BLW these are counts that don't seem to be used...perhaps remove

            muCondition.evaluateConditionStoreResult(bxEval);
            reqObjResult = muCondition.condLastResult();

            cond1Comb = (muCondition.getCombinationsInCond());
            cndObjTypeVec[1] = (corrMuon->objectType())[0];

            if (m_verbosity) {
                std::ostringstream myCout;
                muCondition.print(myCout);

               LogDebug("L1TGlobal") << myCout.str() << std::endl;
            }
        }
            break;
        case CondCalo: {
/*            corrCalo = static_cast<const CaloTemplate*>(m_gtCond1);
            CaloCondition caloCondition(corrCalo, m_gtPSB,
                    m_cond1NrL1Objects, m_cond1NrL1Objects, m_cond1NrL1Objects,
                    m_cond1NrL1Objects, m_cond1NrL1Objects, m_cond1EtaBits);

            caloCondition.evaluateConditionStoreResult();
            reqObjResult = caloCondition.condLastResult();

            cond1Comb = (caloCondition.getCombinationsInCond());
            cndObjTypeVec[1] = (corrCalo->objectType())[0];

            if (m_verbosity ) {
                std::ostringstream myCout;
                caloCondition.print(myCout);

                LogTrace("L1GlobalTrigger") << myCout.str() << std::endl;
            }

*/        }
            break;
        case CondEnergySum: {
/*            corrEnergySum = static_cast<const EnergySumTemplate*>(m_gtCond1);
            EnergySumCondition eSumCondition(corrEnergySum, m_gtPSB);
            eSumCondition.evaluateConditionStoreResult();
            reqObjResult = eSumCondition.condLastResult();

            cond1Comb = (eSumCondition.getCombinationsInCond());
            cndObjTypeVec[1] = (corrEnergySum->objectType())[0];

            if (m_verbosity) {
                std::ostringstream myCout;
                eSumCondition.print(myCout);

                LogTrace("L1GlobalTrigger") << myCout.str() << std::endl;
            }
*/        }
            break;
        default: {
            // should not arrive here, there are no correlation conditions defined for this object
            return false;
        }
            break;
    }

    // return if second sub-condition is false
    if (!reqObjResult) {
        return false;
    } else {
        LogDebug("L1TGlobal") << "\n"
                << "    Both sub-conditions true for object requirements."
                << "    Evaluate correlation requirements.\n" << std::endl;

    }

    // since we have two good legs get the correlation parameters
    CorrelationTemplate::CorrelationParameter corrPar =
        *(m_gtCorrelationTemplate->correlationParameter());


    // vector to store the indices of the calorimeter objects
    // from the combination evaluated in the condition
    SingleCombInCond objectsInComb;
    objectsInComb.reserve(nObjInCond);

    // clear the m_combinationsInCond vector
    (combinationsInCond()).clear();

    // pointers to objects
//    const std::vector<const L1MuGMTCand*>* candMuVec = 0;
    const BXVector<const l1t::Muon*>* candMuVec = 0;

    // make the conversions of the indices, depending on the combination of objects involved
    // (via pair index)

    int phiIndex0 = 0;
    int phiIndex1 = 0;

    int etaIndex0 = 0;
    int etaIndex1 = 0;

    int etIndex0 = 0;
    int etIndex1 = 0;

    LogTrace("L1TGlobal")
            << "  Sub-condition 0: std::vector<SingleCombInCond> size: "
            << (cond0Comb.size()) << std::endl;
    LogTrace("L1TGlobal")
            << "  Sub-condition 1: std::vector<SingleCombInCond> size: "
            << (cond1Comb.size()) << std::endl;


    // loop over all combinations which produced individually "true" as Type1s
    //  
    // BLW: Optimization issue: potentially making the same comparison twice  
    //                          if both legs are the same object type.
    for (std::vector<SingleCombInCond>::const_iterator it0Comb =
            cond0Comb.begin(); it0Comb != cond0Comb.end(); it0Comb++) {

        // Type1s: there is 1 object only, no need for a loop, index 0 should be OK in (*it0Comb)[0]
        // ... but add protection to not crash
        int obj0Index = -1;

        if ((*it0Comb).size() > 0) {
            obj0Index = (*it0Comb)[0];
        } else {
            LogTrace("L1TGlobal")
                    << "\n  SingleCombInCond (*it0Comb).size() "
                    << ((*it0Comb).size()) << std::endl;
            return false;
        }

// Collect the information on the first leg of the correlation
        switch (cond0Categ) {
            case CondMuon: {
                candMuVec = m_uGtB->getCandL1Mu();
                phiIndex0 =  (candMuVec->at(bxEval,obj0Index))->hwPhi(); //(*candMuVec)[obj0Index]->phiIndex();
                etaIndex0 =  (candMuVec->at(bxEval,obj0Index))->hwEta();
		etIndex0  =  (candMuVec->at(bxEval,obj0Index))->hwPt();
            }
                break;
            case CondCalo: {
 
 
            }
                break;
            case CondEnergySum: {

            }
                break;
            default: {
                // should not arrive here, there are no correlation conditions defined for this object
                return false;
            }
                break;
        } //end switch on first leg type

// Now loop over the second leg to get its information
        for (std::vector<SingleCombInCond>::const_iterator it1Comb =
                cond1Comb.begin(); it1Comb != cond1Comb.end(); it1Comb++) {

            // Type1s: there is 1 object only, no need for a loop (*it1Comb)[0]
            // ... but add protection to not crash
            int obj1Index = -1;

            if ((*it1Comb).size() > 0) {
                obj1Index = (*it1Comb)[0];
            } else {
                LogTrace("L1TGlobal")
                        << "\n  SingleCombInCond (*it1Comb).size() "
                        << ((*it1Comb).size()) << std::endl;
                return false;
            }
	    
	    //If we are dealing with the same object type avoid the two legs
	    // either being the same object 
	    if( cndObjTypeVec[0] == cndObjTypeVec[1] &&
	               obj0Index == obj1Index ) continue;

            switch (cond1Categ) {
                case CondMuon: {
                   candMuVec = m_uGtB->getCandL1Mu();
                   phiIndex1 =  (candMuVec->at(bxEval,obj1Index))->hwPhi(); //(*candMuVec)[obj0Index]->phiIndex();
                   etaIndex1 =  (candMuVec->at(bxEval,obj1Index))->hwEta();
		   etIndex1  =  (candMuVec->at(bxEval,obj1Index))->hwPt();
                }
                    break;
                case CondCalo: {


                }
                    break;
                case CondEnergySum: {

                }
                    break;
                default: {
                    // should not arrive here, there are no correlation conditions defined for this object
                    return false;
                }
                    break;
            } //end switch on second leg

            if (m_verbosity) {
                LogDebug("L1TGlobal") << "    Correlation pair ["
                        << l1GtObjectEnumToString(cndObjTypeVec[0]) << ", "
                        << l1GtObjectEnumToString(cndObjTypeVec[1])
                        << "] with collection indices [" << obj0Index << ", "
                        << obj1Index << "] " << " has: \n"
			<< "     Et  value   = ["<< etIndex0  << ", " << etIndex1 <<  "]\n"
			<< "     phi indices = ["<< phiIndex0 << ", " << phiIndex1 << "]\n"
			<< "     eta indices = ["<< etaIndex0 << ", " << etaIndex1 << "]\n" << std::endl;
            }


// Now perform the desired correlation on these two objects
            bool reqResult = true;
	    
            // clear the indices in the combination
            objectsInComb.clear();

            objectsInComb.push_back(obj0Index);
            objectsInComb.push_back(obj1Index);

            // if we get here all checks were successful for this combination
            // set the general result for evaluateCondition to "true"

            if (reqResult) {

                condResult = true;
                (combinationsInCond()).push_back(objectsInComb);

            } 

        } //end loop over second leg

    } //end loop over first leg



    if (m_verbosity  && condResult) {
        LogDebug("L1TGlobal") << " pass(es) the correlation condition.\n"
                << std::endl;
    }    
    
      
    return condResult;

}

// load calo candidates
const l1t::L1Candidate* l1t::CorrCondition::getCandidate(const int bx, const int indexCand) const {

    // objectType() gives the type for nrObjects() only,
    // but in a CondCalo all objects have the same type
    // take type from the type of the first object
    switch ((m_gtCorrelationTemplate->objectType())[0]) {
        case NoIsoEG:
            return (m_uGtB->getCandL1EG())->at(bx,indexCand);
            break;

        case CenJet:
            return (m_uGtB->getCandL1Jet())->at(bx,indexCand);
            break;

       case TauJet:
            return (m_uGtB->getCandL1Tau())->at(bx,indexCand);
            break;
        default:
            return 0;
            break;
    }

    return 0;
}

/**
 * checkObjectParameter - Compare a single particle with a numbered condition.
 *
 * @param iCondition The number of the condition.
 * @param cand The candidate to compare.
 *
 * @return The result of the comparison (false if a condition does not exist).
 */

const bool l1t::CorrCondition::checkObjectParameter(const int iCondition, const l1t::L1Candidate& cand) const {


    return true;
}

void l1t::CorrCondition::print(std::ostream& myCout) const {

    myCout << "Dummy Print for CorrCondition" << std::endl;
    m_gtCorrelationTemplate->print(myCout);
   

    ConditionEvaluation::print(myCout);

}


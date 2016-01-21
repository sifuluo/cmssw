///
/// \class l1t::Stage2Layer2JetAlgorithmFirmwareImp1
///
/// \author:
///
/// Description:

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2DemuxJetAlgoFirmware.h"

#include "L1Trigger/L1TCalorimeter/interface/CaloParamsHelper.h"
#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"


#include <vector>
#include <algorithm>

inline bool operator> ( l1t::Jet& a, l1t::Jet& b )
{
  if ( a.hwPt() > b.hwPt() ){
    return true;
  } else {
    return false;
  }
}

#include "L1Trigger/L1TCalorimeter/interface/BitonicSort.h"

l1t::Stage2Layer2DemuxJetAlgoFirmwareImp1::Stage2Layer2DemuxJetAlgoFirmwareImp1(CaloParamsHelper* params) :
  params_(params)
{


}


l1t::Stage2Layer2DemuxJetAlgoFirmwareImp1::~Stage2Layer2DemuxJetAlgoFirmwareImp1() {


}


void l1t::Stage2Layer2DemuxJetAlgoFirmwareImp1::processEvent(const std::vector<l1t::Jet> & inputJets,
                                                             std::vector<l1t::Jet> & outputJets) {

  // Set the output jets to the input jets
  outputJets = inputJets;

  // Sort the jets by pT
  std::vector<l1t::Jet>::iterator start(outputJets.begin());
  std::vector<l1t::Jet>::iterator end(outputJets.end());

  BitonicSort< l1t::Jet >(down,start,end);


  // convert eta to GT coordinates
  vector<pair<int,double> > etaGT;
  for(int i=0;i<115;i++)
    etaGT.push_back( make_pair(i,i*(0.087/2.)) );

  vector<pair<int,double> > phiGT;
  for(int i=0;i<145;i++)
    phiGT.push_back( make_pair(i,i*(M_PI/72.)) );
  phiGT[144] = make_pair(0,2*M_PI); //2pi = 0


  for(auto& jet : outputJets){

    double eta = CaloTools::towerEta(jet.hwEta());
    double phi = CaloTools::towerEta(jet.hwPhi());
 
    double minDistance = 99999.;
    pair<int, double> closestPoint = make_pair(0,0.);
    
    for(const auto& p : etaGT){
      double distance = abs(abs(eta) - p.second);
      if(distance < minDistance){
	closestPoint = p;
	minDistance = distance;
      }
    }

    int hwEta_GT = (eta>0) ? closestPoint.first : - closestPoint.first;

    minDistance = 99999.;
    closestPoint = make_pair(0,0.);
    
    for(const auto& p : phiGT){
      double distance = abs(phi - p.second);
      if(distance < minDistance){
	closestPoint = p;
	minDistance = distance;
      }
    }

    int hwPhi_GT = closestPoint.first;

    jet.setHwEta(hwEta_GT);
    jet.setHwPhi(hwPhi_GT);

  }

}

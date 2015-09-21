///
/// \class l1t::Stage2Layer2TauAlgorithmFirmwareImp1
///
/// \author: Jim Brooke
///
/// Description: first iteration of stage 2 jet algo

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2TauAlgorithmFirmware.h"

#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"
#include "L1Trigger/L1TCalorimeter/interface/CaloStage2Nav.h"



l1t::Stage2Layer2TauAlgorithmFirmwareImp1::Stage2Layer2TauAlgorithmFirmwareImp1(CaloParams* params) :
  params_(params)
{

  loadCalibrationLuts();
  GetisoLutCompression();
}


l1t::Stage2Layer2TauAlgorithmFirmwareImp1::~Stage2Layer2TauAlgorithmFirmwareImp1() {


}


void l1t::Stage2Layer2TauAlgorithmFirmwareImp1::processEvent(const std::vector<l1t::CaloCluster> & clusters,
															 const std::vector<l1t::CaloTower>& towers,
							      							 std::vector<l1t::Tau> & taus) {

  merging(clusters, towers, taus);
  
}


// FIXME: to be organized better
void l1t::Stage2Layer2TauAlgorithmFirmwareImp1::merging(const std::vector<l1t::CaloCluster>& clusters, const std::vector<l1t::CaloTower>& towers, std::vector<l1t::Tau>& taus){
  //std::cout<<"---------------   NEW EVENT -----------------------------\n";
  //std::cout<<"---------------------------------------------------------\n";
  // navigator
  l1t::CaloStage2Nav caloNav; 

  // Temp copy of clusters (needed to set merging flags)
  std::vector<l1t::CaloCluster> tmpClusters(clusters);
  // First loop: setting merging flags
  for ( auto itr = tmpClusters.begin(); itr != tmpClusters.end(); ++itr ) {
    if( itr->isValid() ){
      l1t::CaloCluster& mainCluster = *itr;
      int iEta = mainCluster.hwEta();
      int iPhi = mainCluster.hwPhi();
      int iEtaP = caloNav.offsetIEta(iEta, 1);
      int iEtaM = caloNav.offsetIEta(iEta, -1);
      int iPhiP2 = caloNav.offsetIPhi(iPhi, 2);
      int iPhiP3 = caloNav.offsetIPhi(iPhi, 3);
      int iPhiM2 = caloNav.offsetIPhi(iPhi, -2);
      int iPhiM3 = caloNav.offsetIPhi(iPhi, -3);


      const l1t::CaloCluster& clusterN2  = l1t::CaloTools::getCluster(tmpClusters, iEta, iPhiM2);
      const l1t::CaloCluster& clusterN3  = l1t::CaloTools::getCluster(tmpClusters, iEta, iPhiM3);
      const l1t::CaloCluster& clusterN2W = l1t::CaloTools::getCluster(tmpClusters, iEtaM, iPhiM2);
      const l1t::CaloCluster& clusterN2E = l1t::CaloTools::getCluster(tmpClusters, iEtaP, iPhiM2);
      const l1t::CaloCluster& clusterS2  = l1t::CaloTools::getCluster(tmpClusters, iEta, iPhiP2);
      const l1t::CaloCluster& clusterS3  = l1t::CaloTools::getCluster(tmpClusters, iEta, iPhiP3);
      const l1t::CaloCluster& clusterS2W = l1t::CaloTools::getCluster(tmpClusters, iEtaM, iPhiP2);
      const l1t::CaloCluster& clusterS2E = l1t::CaloTools::getCluster(tmpClusters, iEtaP, iPhiP2);

      std::list<l1t::CaloCluster> satellites;
      if(clusterN2 .isValid()) satellites.push_back(clusterN2);
      if(clusterN3 .isValid()) satellites.push_back(clusterN3);
      if(clusterN2W.isValid()) satellites.push_back(clusterN2W);
      if(clusterN2E.isValid()) satellites.push_back(clusterN2E);
      if(clusterS2 .isValid()) satellites.push_back(clusterS2);
      if(clusterS3 .isValid()) satellites.push_back(clusterS3);
      if(clusterS2W.isValid()) satellites.push_back(clusterS2W);
      if(clusterS2E.isValid()) satellites.push_back(clusterS2E);

      if(satellites.size()>0) {
        satellites.sort();
        const l1t::CaloCluster& secondaryCluster = satellites.back();

        if(secondaryCluster>mainCluster) {
          // is secondary
          mainCluster.setClusterFlag(CaloCluster::IS_SECONDARY, true);
          // to be merged up or down?
          if(secondaryCluster.hwPhi()==iPhiP2 || secondaryCluster.hwPhi()==iPhiP3) {
            mainCluster.setClusterFlag(CaloCluster::MERGE_UPDOWN, true); // 1 (down)
          }
          else if(secondaryCluster.hwPhi()==iPhiM2 || secondaryCluster.hwPhi()==iPhiM3) {
            mainCluster.setClusterFlag(CaloCluster::MERGE_UPDOWN, false); // 0 (up)
          }
          // to be merged left or right?
          if(secondaryCluster.hwEta()==iEtaP) {
            mainCluster.setClusterFlag(CaloCluster::MERGE_LEFTRIGHT, true); // 1 (right)
          }
          else if(secondaryCluster.hwEta()==iEta || secondaryCluster.hwEta()==iEtaM) {
            mainCluster.setClusterFlag(CaloCluster::MERGE_LEFTRIGHT, false); // 0 (left)
          }
        }
        else {
          // is main cluster
          mainCluster.setClusterFlag(CaloCluster::IS_SECONDARY, false);
          // to be merged up or down?
          if(secondaryCluster.hwPhi()==iPhiP2 || secondaryCluster.hwPhi()==iPhiP3) {
            mainCluster.setClusterFlag(CaloCluster::MERGE_UPDOWN, true); // 1 (down)
          }
          else if(secondaryCluster.hwPhi()==iPhiM2 || secondaryCluster.hwPhi()==iPhiM3) {
            mainCluster.setClusterFlag(CaloCluster::MERGE_UPDOWN, false); // 0 (up)
          }
          // to be merged left or right?
          if(secondaryCluster.hwEta()==iEtaP) {
            mainCluster.setClusterFlag(CaloCluster::MERGE_LEFTRIGHT, true); // 1 (right)
          }
          else if(secondaryCluster.hwEta()==iEta || secondaryCluster.hwEta()==iEtaM) {
            mainCluster.setClusterFlag(CaloCluster::MERGE_LEFTRIGHT, false); // 0 (left)
          }
        }
      }
    }
  }

  // Second loop: do the actual merging based on merging flags
  for ( auto itr = tmpClusters.begin(); itr != tmpClusters.end(); ++itr ) {
    if( itr->isValid() ){
      l1t::CaloCluster& mainCluster = *itr;
      int iEta = mainCluster.hwEta();
      int iPhi = mainCluster.hwPhi();
	  
      // physical eta/phi
      double eta = 0.;
      double phi = 0.;
      double seedEta     = CaloTools::towerEta(mainCluster.hwEta());
      double seedEtaSize = CaloTools::towerEtaSize(mainCluster.hwEta());
      double seedPhi     = CaloTools::towerPhi(mainCluster.hwEta(), mainCluster.hwPhi());
      double seedPhiSize = CaloTools::towerPhiSize(mainCluster.hwEta());
      if     (mainCluster.fgEta()==0) eta = seedEta; // center
      else if(mainCluster.fgEta()==2) eta = seedEta + seedEtaSize*0.25; // center + 1/4
      else if(mainCluster.fgEta()==1) eta = seedEta - seedEtaSize*0.25; // center - 1/4
      if     (mainCluster.fgPhi()==0) phi = seedPhi; // center
      else if(mainCluster.fgPhi()==2) phi = seedPhi + seedPhiSize*0.25; // center + 1/4
      else if(mainCluster.fgPhi()==1) phi = seedPhi - seedPhiSize*0.25; // center - 1/4


      int iEtaP = caloNav.offsetIEta(iEta, 1);
      int iEtaM = caloNav.offsetIEta(iEta, -1);
      //int iPhiP = caloNav.offsetIPhi(iPhi, 1);
      //int iPhiM = caloNav.offsetIPhi(iPhi, -1);      
      int iPhiP2 = caloNav.offsetIPhi(iPhi, 2);
      int iPhiP3 = caloNav.offsetIPhi(iPhi, 3);
      int iPhiM2 = caloNav.offsetIPhi(iPhi, -2);
      int iPhiM3 = caloNav.offsetIPhi(iPhi, -3);

      const l1t::CaloCluster& clusterN2  = l1t::CaloTools::getCluster(tmpClusters, iEta, iPhiM2);
      const l1t::CaloCluster& clusterN3  = l1t::CaloTools::getCluster(tmpClusters, iEta, iPhiM3);
      const l1t::CaloCluster& clusterN2W = l1t::CaloTools::getCluster(tmpClusters, iEtaM, iPhiM2);
      const l1t::CaloCluster& clusterN2E = l1t::CaloTools::getCluster(tmpClusters, iEtaP, iPhiM2);
      const l1t::CaloCluster& clusterS2  = l1t::CaloTools::getCluster(tmpClusters, iEta, iPhiP2);
      const l1t::CaloCluster& clusterS3  = l1t::CaloTools::getCluster(tmpClusters, iEta, iPhiP3);
      const l1t::CaloCluster& clusterS2W = l1t::CaloTools::getCluster(tmpClusters, iEtaM, iPhiP2);
      const l1t::CaloCluster& clusterS2E = l1t::CaloTools::getCluster(tmpClusters, iEtaP, iPhiP2);

      std::list<l1t::CaloCluster> satellites;
      if(clusterN2 .isValid()) satellites.push_back(clusterN2);
      if(clusterN3 .isValid()) satellites.push_back(clusterN3);
      if(clusterN2W.isValid()) satellites.push_back(clusterN2W);
      if(clusterN2E.isValid()) satellites.push_back(clusterN2E);
      if(clusterS2 .isValid()) satellites.push_back(clusterS2);
      if(clusterS3 .isValid()) satellites.push_back(clusterS3);
      if(clusterS2W.isValid()) satellites.push_back(clusterS2W);
      if(clusterS2E.isValid()) satellites.push_back(clusterS2E);

      // neighbour exists
      if(satellites.size()>0) {
        satellites.sort();
        const l1t::CaloCluster& secondaryCluster = satellites.back();
        // this is the most energetic cluster
        // merge with the secondary cluster if it is not merged to an other one
        if(mainCluster>secondaryCluster) {
          bool canBeMerged = true; // optimization: always merge (following part is difficult for hardware to do)
          
          /*
          bool mergeUp = (secondaryCluster.hwPhi()==iPhiM2 || secondaryCluster.hwPhi()==iPhiM3);
          bool mergeLeft = (secondaryCluster.hwEta()==iEtaM);
          bool mergeRight = (secondaryCluster.hwEta()==iEtaP);

          if(mergeUp && !secondaryCluster.checkClusterFlag(CaloCluster::MERGE_UPDOWN)) canBeMerged = false;
          if(!mergeUp && secondaryCluster.checkClusterFlag(CaloCluster::MERGE_UPDOWN)) canBeMerged = false;
          if(mergeLeft && !secondaryCluster.checkClusterFlag(CaloCluster::MERGE_LEFTRIGHT)) canBeMerged = false;
          if(mergeRight && secondaryCluster.checkClusterFlag(CaloCluster::MERGE_LEFTRIGHT)) canBeMerged = false;
          */
          
          if(canBeMerged) {
            double calibPt = calibratedPt(mainCluster.hwPtEm()+secondaryCluster.hwPtEm(), mainCluster.hwPtHad()+secondaryCluster.hwPtHad(), mainCluster.hwEta());
            math::PtEtaPhiMLorentzVector p4(calibPt, eta, phi, 0.);
            l1t::Tau tau( p4, mainCluster.hwPt()+secondaryCluster.hwPt(), mainCluster.hwEta(), mainCluster.hwPhi(), 0);
            taus.push_back(tau);
            //std::cout << "===================== IS MERGED ========================" << std::endl;


			int hwFootPrint=0;
            int hwEtSum=0;

            //Evaluation of the tau footprint as the energy of the merged cluster 
			//hwFootPrint = ((mainCluster.hwPtEm()+secondaryCluster.hwPtEm()) + (mainCluster.hwPtHad()+secondaryCluster.hwPtHad())); 
			hwFootPrint = mainCluster.hwPt()+secondaryCluster.hwPt();

            /*
			if(mainCluster.checkClusterFlag(CaloCluster::MERGE_UPDOWN) && !secondaryCluster.checkClusterFlag(CaloCluster::MERGE_UPDOWN)){    
				// SumEt in (ieta,iphi) = 5x9 centered in the cluster.Eta, cluster.Phi+1
				hwEtSum = CaloTools::calHwEtSum(iEta,iPhiP,towers,-1*params_->tauIsoAreaNrTowersEta(),params_->tauIsoAreaNrTowersEta(),
              									-1*params_->tauIsoAreaNrTowersPhi(),params_->tauIsoAreaNrTowersPhi(),params_->tauPUSParam(2),CaloTools::CALO); 
   			}
			if(!mainCluster.checkClusterFlag(CaloCluster::MERGE_UPDOWN) && secondaryCluster.checkClusterFlag(CaloCluster::MERGE_UPDOWN)){   
			    // SumEt in (ieta,iphi) = 5x9 centered in the cluster.Eta, cluster.Phi-1
			    hwEtSum = CaloTools::calHwEtSum(iEta,iPhiM,towers,-1*params_->tauIsoAreaNrTowersEta(),params_->tauIsoAreaNrTowersEta(),
                								-1*params_->tauIsoAreaNrTowersPhi(),params_->tauIsoAreaNrTowersPhi(),params_->tauPUSParam(2),CaloTools::CALO); 
            }
            */

		    // SumEt in (ieta,iphi) = 5x9 centered always around the main cluster.Eta,  and Phi
		    hwEtSum = CaloTools::calHwEtSum(iEta,iPhi,towers,-1*params_->tauIsoAreaNrTowersEta(),params_->tauIsoAreaNrTowersEta(),
               								-1*params_->tauIsoAreaNrTowersPhi(),params_->tauIsoAreaNrTowersPhi(),params_->tauPUSParam(2),CaloTools::CALO); 


            int isolBit = 0;
        	
			int nrTowers = CaloTools::calNrTowers(-1*params_->tauPUSParam(1),params_->tauPUSParam(1),1,72,towers,1,999,CaloTools::CALO);
        	unsigned int lutAddress = isoLutIndex(hwFootPrint, mainCluster.hwEta(), nrTowers);
        	
        	//cout << hwFootPrint << " " << mainCluster.hwEta() << " " << nrTowers << " --> " << lutAddress << " --> " << (params_->tauIsolationLUT()->data(lutAddress)) << endl;
        	
        	isolBit = hwEtSum-hwFootPrint <= (params_->tauIsolationLUT()->data(lutAddress));
			//isolBit+=1;
        	taus.back().setHwIso(isolBit);
			//taus.back().setHwIso(hwEtSum-hwFootPrint);
			//taus.back().setHwQual(nrTowers);
            taus.back().setHwQual(BitwiseClusterFlags(&mainCluster, 0, &secondaryCluster));
		  }
          /*
          else {
            double calibPt = calibratedPt(mainCluster.hwPtEm(), mainCluster.hwPtHad(), mainCluster.hwEta());
            math::PtEtaPhiMLorentzVector p4(calibPt, eta, phi, 0.);
            l1t::Tau tau( p4, mainCluster.hwPt(), mainCluster.hwEta(), mainCluster.hwPhi(), 0);
            taus.push_back(tau);
            //std::cout<<"   Make tau, No merging\n";
            
            // Isolation part
            int hwEtSum = CaloTools::calHwEtSum(mainCluster.hwEta(),mainCluster.hwPhi(),towers,-1*params_->tauIsoAreaNrTowersEta(),params_->tauIsoAreaNrTowersEta(),
                                    -1*params_->tauIsoAreaNrTowersPhi(),params_->tauIsoAreaNrTowersPhi(),params_->tauPUSParam(2),CaloTools::CALO);
                                    
            //int hwFootPrint = (mainCluster.hwPtEm()+ mainCluster.hwPtHad()); 
			int hwFootPrint = 0;
			hwFootPrint = mainCluster.hwPt();
			
			int isolBit = 0;
			int nrTowers = CaloTools::calNrTowers(-1*params_->tauPUSParam(1),params_->tauPUSParam(1),1,72,towers,1,999,CaloTools::CALO);
        	unsigned int lutAddress = isoLutIndex(calibPt, nrTowers);
        	
        	isolBit = hwEtSum-hwFootPrint <= params_->tauIsolationLUT()->data(lutAddress);
			isolBit+=1;
			//taus.back().setHwIso(isolBit);
			taus.back().setHwIso(hwEtSum-hwFootPrint);
			//taus.back().setHwQual(nrTowers);
            taus.back().setHwQual(BitwiseClusterFlags(&mainCluster, 1, 0));
		  }
		  */
        }
        /*
        else {
          bool canBeKept = false;
          bool mergeUp = (secondaryCluster.hwPhi()==iPhiM2 || secondaryCluster.hwPhi()==iPhiM3);
          bool mergeLeft = (secondaryCluster.hwEta()==iEtaM);
          bool mergeRight = (secondaryCluster.hwEta()==iEtaP);

          if(mergeUp && !secondaryCluster.checkClusterFlag(CaloCluster::MERGE_UPDOWN)) canBeKept = true;
          if(!mergeUp && secondaryCluster.checkClusterFlag(CaloCluster::MERGE_UPDOWN)) canBeKept = true;
          if(mergeLeft && !secondaryCluster.checkClusterFlag(CaloCluster::MERGE_LEFTRIGHT)) canBeKept = true;
          if(mergeRight && secondaryCluster.checkClusterFlag(CaloCluster::MERGE_LEFTRIGHT)) canBeKept = true;
          if(canBeKept) {
            double calibPt = calibratedPt(mainCluster.hwPtEm(), mainCluster.hwPtHad(), mainCluster.hwEta());
            math::PtEtaPhiMLorentzVector p4(calibPt, eta, phi, 0.);
            l1t::Tau tau( p4, mainCluster.hwPt(), mainCluster.hwEta(), mainCluster.hwPhi(), 0);
            taus.push_back(tau);
            //std::cout<<"   Make tau, No merging\n";
            
            // Isolation part
            int hwEtSum = CaloTools::calHwEtSum(mainCluster.hwEta(),mainCluster.hwPhi(),towers,-1*params_->tauIsoAreaNrTowersEta(),params_->tauIsoAreaNrTowersEta(),
                                    -1*params_->tauIsoAreaNrTowersPhi(),params_->tauIsoAreaNrTowersPhi(),params_->tauPUSParam(2),CaloTools::CALO);
                                    
            //int hwFootPrint = (mainCluster.hwPtEm()+ mainCluster.hwPtHad()); 
            int hwFootPrint = 0;
            hwFootPrint = mainCluster.hwPt();
			
            int isolBit = 0;
			int nrTowers = CaloTools::calNrTowers(-1*params_->tauPUSParam(1),params_->tauPUSParam(1),1,72,towers,1,999,CaloTools::CALO);
        	unsigned int lutAddress = isoLutIndex(calibPt, nrTowers);
        	
        	isolBit = hwEtSum-hwFootPrint <= params_->tauIsolationLUT()->data(lutAddress);
			isolBit+=1;
            //taus.back().setHwIso(isolBit);
			taus.back().setHwIso(hwEtSum-hwFootPrint);
			//taus.back().setHwQual(nrTowers);
			taus.back().setHwQual(BitwiseClusterFlags(&mainCluster, 2, 0));
          }
        }
        */
      }
      else {
        double calibPt = calibratedPt(mainCluster.hwPtEm(), mainCluster.hwPtHad(), mainCluster.hwEta());
        math::PtEtaPhiMLorentzVector p4(calibPt, eta, phi, 0.);
        l1t::Tau tau( p4, mainCluster.hwPt(), mainCluster.hwEta(), mainCluster.hwPhi(), 0);
        taus.push_back(tau);
        //std::cout<<"   Make tau, No merging\n";
        
        // Isolation part
        int hwEtSum = CaloTools::calHwEtSum(mainCluster.hwEta(),mainCluster.hwPhi(),towers,-1*params_->tauIsoAreaNrTowersEta(),params_->tauIsoAreaNrTowersEta(),
                                -1*params_->tauIsoAreaNrTowersPhi(),params_->tauIsoAreaNrTowersPhi(),params_->tauPUSParam(2),CaloTools::CALO);
                                
        //int hwFootPrint = (mainCluster.hwPtEm()+ mainCluster.hwPtHad()); 
        int hwFootPrint = 0;
        hwFootPrint = mainCluster.hwPt();
   
        int isolBit = 0;
		int nrTowers = CaloTools::calNrTowers(-1*params_->tauPUSParam(1),params_->tauPUSParam(1),1,72,towers,1,999,CaloTools::CALO);
        unsigned int lutAddress = isoLutIndex(mainCluster.hwEta(), hwFootPrint, nrTowers);
        
        isolBit = hwEtSum-hwFootPrint <= params_->tauIsolationLUT()->data(lutAddress);
		//isolBit+=1;
      	taus.back().setHwIso(isolBit);
		//taus.back().setHwIso(hwEtSum-hwFootPrint);
		//taus.back().setHwQual(nrTowers);
		taus.back().setHwQual(BitwiseClusterFlags(&mainCluster, 3, 0));
      }
    }
  }
}


//calculates the footprint of the tau in hardware values
int l1t::Stage2Layer2TauAlgorithmFirmwareImp1::isoCalTauHwFootPrint(const l1t::CaloCluster& clus,const std::vector<l1t::CaloTower>& towers) 
{
  int iEta=clus.hwEta();
  int iPhi=clus.hwPhi();
  int totHwFootPrint = CaloTools::calHwEtSum(iEta,iPhi,towers,-1,1,-1*params_->tauIsoVetoNrTowersPhi(),params_->tauIsoVetoNrTowersPhi(),params_->tauPUSParam(2),CaloTools::CALO);
  return totHwFootPrint;
}




void l1t::Stage2Layer2TauAlgorithmFirmwareImp1::loadCalibrationLuts()
{
  float minScale    = 0.;
  float maxScale    = 2.;
  float minScaleEta = 0.5;
  float maxScaleEta = 1.5;
  offsetBarrelEH_   = 0.5;
  offsetBarrelH_    = 1.5;
  offsetEndcapsEH_  = 0.;
  offsetEndcapsH_   = 1.5;

  // In the combined calibration LUT, upper 3-bits are used as LUT index:
  // (0=BarrelA, 1=BarrelB, 2=BarrelC, 3=EndCapA, 4=EndCapA, 5=EndCapA, 6=Eta)
  enum {LUT_UPPER = 3};
  enum {LUT_OFFSET = 0x80};
  l1t::LUT* lut = params_->tauCalibrationLUT();
  unsigned int size = (1 << lut->nrBitsData());
  unsigned int nBins = (1 << (lut->nrBitsAddress() - LUT_UPPER));


  std::vector<float> emptyCoeff;
  emptyCoeff.resize(nBins,0.);
  float binSize = (maxScale-minScale)/(float)size;
  for(unsigned iLut=0; iLut < 6; ++iLut ) {
    coefficients_.push_back(emptyCoeff);
    for(unsigned addr=0;addr<nBins;addr++) {
      float y = (float)lut->data(iLut*LUT_OFFSET + addr);
      coefficients_[iLut][addr] = minScale + binSize*y;
    }
  }

  size = (1 << lut->nrBitsData());
  nBins = (1 << 6); // can't auto-extract this now due to combined LUT.
  
  emptyCoeff.resize(nBins,0.);
  binSize = (maxScaleEta-minScaleEta)/(float)size;
  coefficients_.push_back(emptyCoeff);
  for(unsigned addr=0;addr<nBins;addr++) {
    float y = (float)lut->data(6*LUT_OFFSET + addr);
    coefficients_.back()[addr] = minScaleEta + binSize*y;
  }

}


double l1t::Stage2Layer2TauAlgorithmFirmwareImp1::calibratedPt(int hwPtEm, int hwPtHad, int ieta)
{
  // ET calibration
  bool barrel = (abs(ieta)<=17);
  unsigned int nBins = coefficients_[0].size();
  double e = (double)hwPtEm*params_->tauLsb();
  double h = (double)hwPtHad*params_->tauLsb();
  double calibPt = 0.;
  int ilutOffset = (barrel) ? 0: 3;
  unsigned int ibin=(unsigned int)(floor(e+h));
  if (ibin >= nBins -1) ibin = nBins-1;
  if(e>0.) {
    double offset = (barrel) ? offsetBarrelEH_ : offsetEndcapsEH_;
    calibPt = e*coefficients_[ilutOffset][ibin] + h*coefficients_[1+ilutOffset][ibin] + offset;
  }
  else {
    double offset = (barrel) ? offsetBarrelH_ : offsetEndcapsH_;
    calibPt = h*coefficients_[2+ilutOffset][ibin]+offset;
  } 

  // eta calibration
  if(ieta<-28) ieta=-28;
  if(ieta>28) ieta=28;
  ibin = (ieta>0 ? ieta+27 : ieta+28);
  calibPt *= coefficients_.back()[ibin];

  //std::cout << "  ieta: " << ieta << " | calibPt: " << calibPt << " | hwPtEm: " << hwPtEm << " | hwPtHad: " << hwPtHad << std::endl;

  return calibPt;
}

// retrieve compressed num bits for each variable
// as the first part of the LUT is the compression block (indexing),
// read the last entry of each variable block and round to the closest
// larger power of two to get the number of bits used for that variable
// an analytic formula allows this rounding
void  l1t::Stage2Layer2TauAlgorithmFirmwareImp1::GetisoLutCompression()
{
    // non compressed bits used - it is a firmware property
    const int bit_eta = 5;
    const int bit_Et = 9;
    const int bit_nTT = 10;
    int index = 0;
    
    // LUT first entry is indexed with 0
    index = pow(2,bit_eta)-1;
    iso_etaBit_ = floor(log2(1.*params_->tauIsolationLUT()->data( index )) ) + 1;

    index = pow(2,bit_Et)-1 + pow (2, bit_eta);
    iso_EtBit_ = floor(log2(1.*params_->tauIsolationLUT()->data( index )) ) + 1;

    index = pow(2,bit_nTT)-1 + pow (2, bit_eta) + pow (2, bit_Et);
    iso_nTTBit_ = floor(log2(1.*params_->tauIsolationLUT()->data( index )) ) + 1;

    // other number useful in getting LUT address - compute just once
    iso_offset_ = pow(2, bit_eta) + pow(2, bit_Et) + pow (2, bit_nTT);
    iso_Etblocksize_ = pow (2, iso_nTTBit_);
    iso_etablocksize_ = pow (2, iso_nTTBit_+iso_EtBit_);

    return;
}

// LUT FORMAT: N=1024 (10 bit) blocks for each value of nrTowers
// Each one of this blocks has a substructure of N=256 (8 bit) for the energy value
unsigned int l1t::Stage2Layer2TauAlgorithmFirmwareImp1::isoLutIndex(int ieta, int Et, unsigned int nTT)
{
   //const unsigned int kTowerGranularity=params_->tauPUSParam(0);
   //unsigned int nrTowersNormed = nrTowers/kTowerGranularity;
   
   int abseta = abs(ieta);
   
   // set to available bit precision
   if (abseta >= 32) abseta = 31; // 5 bits for eta 
   if (Et >= 512) Et = 511; // 9 bit for Et 
   if (nTT >= 1024) nTT  = 1023; // 10 bits for towers
   
   int etaInd  = params_->tauIsolationLUT()->data(abseta);
   int EtInd   = params_->tauIsolationLUT()->data(Et + 32); // offset on 5 eta bits (32 rows)
   int nTTInd  = params_->tauIsolationLUT()->data(nTT + 544); // offset on 5 eta bits and 9 Et bits (32+ 512 rows)
  
   int addr = iso_offset_ + etaInd*iso_etablocksize_ + EtInd*iso_Etblocksize_ + nTTInd;   
   return addr;
}




// this has been added for PU-sub debug purposes only on 14 Jan 2015.
// is used to produce an int value that contains all the flags of the cluster(s) corrsponding to that particular tau
// to be removed in official emulator
//
// int is composed by 4 bytes (checked with std::cout << "Size of int: " << sizeof(int) << std::endl; )
//
// bit 0 is the least significant bit; bit numbering convention:
/*
        // related to main cluster

        INCLUDE_SEED        = 0,
        INCLUDE_NW          = 1,
        INCLUDE_N           = 2,
        INCLUDE_NE          = 3,
        INCLUDE_E           = 4,
        INCLUDE_SE          = 5,
        INCLUDE_S           = 6,
        INCLUDE_SW          = 7,
        INCLUDE_W           = 8,
        INCLUDE_NN          = 9,
        INCLUDE_SS          = 10,
        TRIM_LEFT           = 11,
        IS_SECONDARY        = 12,
        MERGE_UPDOWN        = 13, // 0=up, 1=down
        MERGE_LEFTRIGHT     = 14 // 0=left, 1=right

        // related to secondary cluster (if not existing, set all to 0)

        INCLUDE_SEED        = 15,
        INCLUDE_NW          = 16,
        INCLUDE_N           = 17,
        INCLUDE_NE          = 18,
        INCLUDE_E           = 19,
        INCLUDE_SE          = 20,
        INCLUDE_S           = 21,
        INCLUDE_SW          = 22,
        INCLUDE_W           = 23,
        INCLUDE_NN          = 24,
        INCLUDE_SS          = 25,
        TRIM_LEFT           = 26,
        IS_SECONDARY        = 27,
        MERGE_UPDOWN        = 28, // 0=up, 1=down
        MERGE_LEFTRIGHT     = 29 // 0=left, 1=right

		// related to the way tau was computed (i.e., in which part of the code was filled; this means that you can find if it was merged, kept alone, etc...)
		// this information is passed to the function as a number (second parameter) with this convention:
		0:	if (satellite.size() > 0)     AND     mainCluster > secondaryCluster    AND canBeMerged == true
		1:	if (satellite.size() > 0)     AND     mainCluster > secondaryCluster    AND canBeMerged == false
		2:	if (satellite.size() > 0)     AND     mainCluster < secondaryCluster    AND canBeKept   == true    // NOTE: if canBeKept == false, the tau is not saved, so there is no flag defined for this case
		3:  if (satellite.size() == 0)
		// these two numbers are converted to a binary and then saved to bits 30 and 31, i.e. the bin sequence (31, 30)  will be: 0=(0,0), 1=(0,1), 2=(1,0), 3=(1,1)
*/

int l1t::Stage2Layer2TauAlgorithmFirmwareImp1::BitwiseClusterFlags (const l1t::CaloCluster* mainCluster, int HowComputed, const l1t::CaloCluster* secondaryCluster)
{
   // first of all, be sure that the machine supports int with 32 bits, otherwise abort the execution
   assert ( sizeof(int) == sizeof(int32_t) );
   
   // check HowComputed range
   if (HowComputed < 0 || HowComputed > 3)
   {
      std::cout << "WARNING: value of HowComputed not in range -  returning 0" << std::endl;
	  return 0;
   }
   
   int flags = 0; // initialize all flags to 0
   
   // set flags according to numbering scheme
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_SEED))      flags |= (1 << 0);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_NW))        flags |= (1 << 1);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_N))         flags |= (1 << 2);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_NE))        flags |= (1 << 3);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_E))         flags |= (1 << 4);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_SE))        flags |= (1 << 5);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_S))         flags |= (1 << 6);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_SW))        flags |= (1 << 7);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_W))         flags |= (1 << 8);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_NN))        flags |= (1 << 9);
   if (mainCluster->checkClusterFlag(CaloCluster::INCLUDE_SS))        flags |= (1 << 10);
   if (mainCluster->checkClusterFlag(CaloCluster::TRIM_LEFT))         flags |= (1 << 11);
   if (mainCluster->checkClusterFlag(CaloCluster::IS_SECONDARY))      flags |= (1 << 12);
   if (mainCluster->checkClusterFlag(CaloCluster::MERGE_UPDOWN))      flags |= (1 << 13);
   if (mainCluster->checkClusterFlag(CaloCluster::MERGE_LEFTRIGHT))   flags |= (1 << 14);
   
   if (secondaryCluster != 0)
   {
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_SEED))      flags |= (1 << 15);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_NW))        flags |= (1 << 16);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_N))         flags |= (1 << 17);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_NE))        flags |= (1 << 18);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_E))         flags |= (1 << 19);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_SE))        flags |= (1 << 20);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_S))         flags |= (1 << 21);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_SW))        flags |= (1 << 22);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_W))         flags |= (1 << 23);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_NN))        flags |= (1 << 24);
      if (secondaryCluster->checkClusterFlag(CaloCluster::INCLUDE_SS))        flags |= (1 << 25);
      if (secondaryCluster->checkClusterFlag(CaloCluster::TRIM_LEFT))         flags |= (1 << 26);
      if (secondaryCluster->checkClusterFlag(CaloCluster::IS_SECONDARY))      flags |= (1 << 27);
      if (secondaryCluster->checkClusterFlag(CaloCluster::MERGE_UPDOWN))      flags |= (1 << 28);
      if (secondaryCluster->checkClusterFlag(CaloCluster::MERGE_LEFTRIGHT))   flags |= (1 << 29);
   }

   switch (HowComputed)
   {
   	  // for 0, nothing to be done, already set properly by initialization
      case (1):
         flags |= (1 << 30);
         break;
      case (2):
	     flags |= (1 << 31);
         break;
	  case (3):
         flags |= (1 << 30);
         flags |= (1 << 31);
         break; 
   }
   
   return flags;   
}


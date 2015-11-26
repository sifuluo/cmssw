#include <iomanip>
#include <iostream>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1TCaloParamsRcd.h"
#include "CondFormats/L1TObjects/interface/CaloParams.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

class L1TCaloParamsDumper : public edm::EDAnalyzer {
public:
    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    explicit L1TCaloParamsDumper(const edm::ParameterSet&) : edm::EDAnalyzer(){}
    virtual ~L1TCaloParamsDumper(void){}
};

void L1TCaloParamsDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup){

    edm::ESHandle<l1t::CaloParams> handle1;
    evSetup.get<L1TCaloParamsRcd>().get( handle1 ) ;
    boost::shared_ptr<l1t::CaloParams> ptr1(new l1t::CaloParams(*(handle1.product ())));

    edm::Service<cond::service::PoolDBOutputService> poolDb;
    if( poolDb.isAvailable() ){
        cond::Time_t firstSinceTime = poolDb->beginOfTime();
        poolDb->writeOne(ptr1.get(),firstSinceTime,"L1TCaloParamsRcd");
    }

}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"

DEFINE_FWK_MODULE(L1TCaloParamsDumper);


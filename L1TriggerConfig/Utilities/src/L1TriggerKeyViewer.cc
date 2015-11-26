#include <iomanip>
#include <iostream>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1TriggerKeyRcd.h"
#include "CondFormats/L1TObjects/interface/L1TriggerKey.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondCore/CondDB/interface/Session.h"

#include <iostream>
using namespace std;

class L1TriggerKeyViewer : public edm::EDAnalyzer {
public:
    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    explicit L1TriggerKeyViewer(const edm::ParameterSet&) : edm::EDAnalyzer(){}
    virtual ~L1TriggerKeyViewer(void){}
};

void L1TriggerKeyViewer::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup){

    edm::ESHandle<L1TriggerKey> handle1;
    evSetup.get<L1TriggerKeyRcd>().get( handle1 ) ;
    boost::shared_ptr<L1TriggerKey> ptr1(new L1TriggerKey(*(handle1.product ())));

    cout<<"L1TriggerKey: "<<endl;
    cout<<" TSC: "<<ptr1->tscKey()<<endl;

    const map<string,string> &r2k = ptr1->recordToKeyMap();
    for(auto i : r2k){
        cout<<i.first<<" -> "<<i.second<<endl;
    }
}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
DEFINE_FWK_MODULE(L1TriggerKeyViewer);

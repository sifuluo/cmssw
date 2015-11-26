#include <iomanip>
#include <iostream>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1TriggerKeyExtRcd.h"
#include "CondFormats/L1TObjects/interface/L1TriggerKeyExt.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondCore/CondDB/interface/Session.h"

#include <iostream>
using namespace std;

class L1TriggerKeyExtViewer : public edm::EDAnalyzer {
public:
    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    explicit L1TriggerKeyExtViewer(const edm::ParameterSet&) : edm::EDAnalyzer(){}
    virtual ~L1TriggerKeyExtViewer(void){}
};

void L1TriggerKeyExtViewer::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup){

    edm::ESHandle<L1TriggerKeyExt> handle1;
    evSetup.get<L1TriggerKeyExtRcd>().get( handle1 ) ;
    boost::shared_ptr<L1TriggerKeyExt> ptr1(new L1TriggerKeyExt(*(handle1.product ())));

    cout<<"L1TriggerKey: "<<endl;
    cout<<" TSC: "<<ptr1->tscKey()<<endl;

    const map<string,string> &r2k = ptr1->recordToKeyMap();
    for(auto i : r2k){
        cout<<i.first<<" -> "<<i.second<<endl;
    }
    cout<<" S1CALOL2 key:"<<ptr1->subsystemKey( L1TriggerKeyExt::kS1CALOL2 )<<endl;
}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
DEFINE_FWK_MODULE(L1TriggerKeyExtViewer);

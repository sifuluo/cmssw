#include <iomanip>
#include <iostream>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1TriggerKeyListExtRcd.h"
#include "CondFormats/L1TObjects/interface/L1TriggerKeyListExt.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondCore/CondDB/interface/Session.h"

#include <iostream>
using namespace std;

class L1TriggerKeyExtListViewer : public edm::EDAnalyzer {
private:
    std::string tscKey;
public:
    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    explicit L1TriggerKeyExtListViewer(const edm::ParameterSet& iConfig) : edm::EDAnalyzer(){
        tscKey = iConfig.getParameter<std::string>("TSC_KEY");
    }
    virtual ~L1TriggerKeyExtListViewer(void){}
};

void L1TriggerKeyExtListViewer::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup){

    edm::ESHandle<L1TriggerKeyListExt> handle1;
    evSetup.get<L1TriggerKeyListExtRcd>().get( handle1 ) ;
    boost::shared_ptr<L1TriggerKeyListExt> ptr1(new L1TriggerKeyListExt(*(handle1.product ())));

    cout<<"L1TriggerKeyListExt: "<<ptr1->token( tscKey )<<endl;
    map<string,string> qwe = ptr1->tscKeyToTokenMap();

    for( auto &i : qwe)
        cout<<i.first<<" -> "<<i.second<<endl;
}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
DEFINE_FWK_MODULE(L1TriggerKeyExtListViewer);

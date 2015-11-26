#include <iomanip>
#include <iostream>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondCore/CondDB/interface/Session.h"

#include <iostream>
using namespace std;

class L1MenuViewer : public edm::EDAnalyzer {
public:
    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    explicit L1MenuViewer(const edm::ParameterSet&) : edm::EDAnalyzer(){}
    virtual ~L1MenuViewer(void){}
};

void L1MenuViewer::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup){

    edm::ESHandle<L1GtTriggerMenu> handle1;
    evSetup.get<L1GtTriggerMenuRcd>().get( handle1 ) ;
    boost::shared_ptr<L1GtTriggerMenu> ptr1(new L1GtTriggerMenu(*(handle1.product ())));

    cout<<"L1GtTriggerMenu: "<<endl;
    cout<<" name: "<<ptr1->gtTriggerMenuName()<<endl;
    cout<<" iface: "<<ptr1->gtTriggerMenuInterface()<<endl;
    cout<<" implem: "<<ptr1->gtTriggerMenuImplementation()<<endl;
    cout<<" db_key: "<<ptr1->gtScaleDbKey()<<endl;

    cout<<" L1GtTriggerMenu: "<<endl;

    cout<<"L1GtMuonTemplate: "<<endl;
    const std::vector<std::vector<L1GtMuonTemplate> >& muons = ptr1->vecMuonTemplate();
    int i=0, j=0;
    for(auto vec : muons){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtCaloTemplate: "<<endl;
    const std::vector<std::vector<L1GtCaloTemplate> >& calos = ptr1->vecCaloTemplate();
    i=0; j=0;
    for(auto vec : calos){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtEnergySumTemplate: "<<endl;
    const std::vector<std::vector<L1GtEnergySumTemplate> >& es = ptr1->vecEnergySumTemplate();
    i=0; j=0;
    for(auto vec : es){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtJetCountsTemplate: "<<endl;
    const std::vector<std::vector<L1GtJetCountsTemplate> >& jc = ptr1->vecJetCountsTemplate();
    i=0; j=0;
    for(auto vec : jc){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtCastorTemplate: "<<endl;
    const std::vector<std::vector<L1GtCastorTemplate> >& cast = ptr1->vecCastorTemplate();
    i=0; j=0;
    for(auto vec : cast){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtHfBitCountsTemplate: "<<endl;
    const std::vector<std::vector<L1GtHfBitCountsTemplate> >& hfb = ptr1->vecHfBitCountsTemplate();
    i=0; j=0;
    for(auto vec : hfb){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtHfRingEtSumsTemplate: "<<endl;
    const std::vector<std::vector<L1GtHfRingEtSumsTemplate> >& hfr = ptr1->vecHfRingEtSumsTemplate();
    i=0; j=0;
    for(auto vec : hfr){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtBptxTemplate: "<<endl;
    const std::vector<std::vector<L1GtBptxTemplate> >& bptx = ptr1->vecBptxTemplate();
    i=0; j=0;
    for(auto vec : bptx){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtExternalTemplate: "<<endl;
    const std::vector<std::vector<L1GtExternalTemplate> >& gtx = ptr1->vecExternalTemplate();
    i=0; j=0;
    for(auto vec : gtx){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtCorrelationTemplate: "<<endl;
    const std::vector<std::vector<L1GtCorrelationTemplate> >& gtcorr = ptr1->vecCorrelationTemplate();
    i=0; j=0;
    for(auto vec : gtcorr){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtMuonTemplate: "<<endl;
    const std::vector<std::vector<L1GtMuonTemplate> >& gtm = ptr1->corMuonTemplate();
    i=0; j=0;
    for(auto vec : gtm){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtCaloTemplate: "<<endl;
    const std::vector<std::vector<L1GtCaloTemplate> >& gtc = ptr1->corCaloTemplate();
    i=0; j=0;
    for(auto vec : gtc){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

    cout<<"L1GtEnergySumTemplate: "<<endl;
    const std::vector<std::vector<L1GtEnergySumTemplate> >& ges = ptr1->corEnergySumTemplate();
    i=0; j=0;
    for(auto vec : ges){
        cout<<"  ["<<i<<"]"<<endl;
        i++;
        for(auto temp : vec){
            cout<<"   ["<<j<<"]";
            temp.print(cout);
            j++;
        }
    }

//gtAlgorithmMap
//gtAlgorithmAliasMap
//gtTechnicalTriggerMap
   int verb = 2;
   ptr1->print(cout, verb);

}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
DEFINE_FWK_MODULE(L1MenuViewer);

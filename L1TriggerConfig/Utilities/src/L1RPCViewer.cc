#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1RPCConfigRcd.h"
#include "CondFormats/L1TObjects/interface/L1RPCConfig.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondCore/CondDB/interface/Session.h"

class L1RPCReader : public edm::EDAnalyzer {
public:
    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    explicit L1RPCReader(const edm::ParameterSet&) : edm::EDAnalyzer(){}
    virtual ~L1RPCReader(void){}
};

#include <iostream>
using namespace std;

void L1RPCReader::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup){

    edm::ESHandle<L1RPCConfig> handle1;
    evSetup.get<L1RPCConfigRcd>().get( handle1 ) ;
    boost::shared_ptr<L1RPCConfig> ptr1(new L1RPCConfig(*(handle1.product ())));

    cout<<"L1RPCConfig: "<<endl;
//    cout<<" in binary: "<<hex<<endl;
//    char *p = (char*)(ptr1.get());
//    for(size_t pos=0; pos<sizeof(L1RPCConfig); pos++){
//        cout<<short(*(p+pos))<<endl;
//    }
//    cout<<dec<<endl;

    cout<<" ppt:"<<ptr1->getPPT()<<endl<<" patterns: "<<endl;
    for(RPCPattern::RPCPatVec::const_iterator pat = ptr1->m_pats.begin(); pat != ptr1->m_pats.end(); pat++)
        cout<<short(pat->getTower())<<" "<<short(pat->getLogSector())<<" "<<short(pat->getCode())<<" "<<short(pat->getSign())<<" "<<short(pat->getNumber())<<" "<<short(pat->getPatternType())<<" "<<short(pat->getRefGroup())<<" "<<short(pat->getQualityTabNumber())<<" "<<short(pat->getStripFrom(0))<<" "<<short(pat->getStripFrom(1))<<" "<<short(pat->getStripFrom(2))<<" "<<short(pat->getStripFrom(3))<<" "<<short(pat->getStripFrom(4))<<" "<<short(pat->getStripFrom(5))<<" "<<short(pat->getStripTo(0))<<" "<<short(pat->getStripTo(1))<<" "<<short(pat->getStripTo(2))<<" "<<short(pat->getStripTo(3))<<" "<<short(pat->getStripTo(4))<<" "<<short(pat->getStripTo(5))<<endl;

    cout<<" qualities: "<<endl;
    for(RPCPattern::TQualityVec::const_iterator qual = ptr1->m_quals.begin(); qual != ptr1->m_quals.end(); qual++){
        cout<<short(qual->m_FiredPlanes)<<" "<<short(qual->m_QualityTabNumber)<<" "<<short(qual->m_QualityValue)<<" "<<short(qual->m_logsector)<<" "<<short(qual->m_logsegment)<<" "<<short(qual->m_tower)<<endl;
    }

}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"

DEFINE_FWK_MODULE(L1RPCReader);


// L1TBXVectorTester:  unit test for BXVector

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/Tau.h"

using namespace std;
using namespace l1t;
using namespace edm;

//
// class declaration
//

class L1TBXVectorTester : public edm::EDProducer {
public:
  explicit L1TBXVectorTester(const edm::ParameterSet&);
  ~L1TBXVectorTester();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  //virtual void beginJob() override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  //virtual void endJob() override;
      
  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;


  // ----------member data ---------------------------

  edm::EDGetTokenT<MuonBxCollection> muonToken_;

};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
L1TBXVectorTester::L1TBXVectorTester(const edm::ParameterSet& iConfig) {
  produces<EtSumRef>();
  produces<EtSumRefVector>();
  produces<EGammaRef>();
  produces<EGammaRefVector>();
  produces<JetRef>();
  produces<JetRefVector>();
  produces<MuonRef>();
  produces<MuonRefVector>();
  produces<TauRef>();
  produces<TauRefVector>();
  muonToken_ = consumes<MuonBxCollection>   (iConfig.getParameter<InputTag>("muonToken"));
}

L1TBXVectorTester::~L1TBXVectorTester() {
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void
L1TBXVectorTester::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  std::auto_ptr<EGammaRef> egref (new EGammaRef);
  std::auto_ptr<EGammaRefVector> egrefv (new EGammaRefVector);

  std::auto_ptr<EtSumRef> etref (new EtSumRef);
  std::auto_ptr<EtSumRefVector> etrefv (new EtSumRefVector);

  std::auto_ptr<JetRef> jref (new JetRef);
  std::auto_ptr<JetRefVector> jrefv (new JetRefVector);

  std::auto_ptr<MuonRef> mref (new MuonRef);
  std::auto_ptr<MuonRefVector> mrefv (new MuonRefVector);

  std::auto_ptr<TauRef> tref (new TauRef);
  std::auto_ptr<TauRefVector> trefv (new TauRefVector);

  Handle<MuonBxCollection> XTMP; 
  iEvent.getByToken(muonToken_, XTMP);
  if (XTMP.isValid()){ 
    cout << "INFO:  L1T found muon collection.\n";
    for (int ibx = XTMP->getFirstBX(); ibx <= XTMP->getLastBX(); ++ibx) {
      for (auto it=XTMP->begin(ibx); it!=XTMP->end(ibx); it++){      
	if (it->et() > 0){ 
	  MuonRef x(XTMP,it - XTMP->begin());
	  (*mref) = x;
	  mrefv->push_back(x);
	  cout << "bx:  " << ibx << "  et:  "  << it->et() << "  eta:  "  << it->eta() << "  phi:  "  << it->phi() << "\n";
	}
      }
    }
  } else {
    LogWarning("MissingProduct") << "L1T upgrade muons not found." << std::endl;
  }

  iEvent.put(egref);
  iEvent.put(egrefv);
  iEvent.put(etref);
  iEvent.put(etrefv);
  iEvent.put(jref);
  iEvent.put(jrefv);
  iEvent.put(mref);
  iEvent.put(mrefv);
  iEvent.put(tref);
  iEvent.put(trefv);
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TBXVectorTester::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TBXVectorTester);

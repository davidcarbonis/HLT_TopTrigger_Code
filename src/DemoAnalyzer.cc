//class DemoAnalyzer DemoAnalyzer.cc Demo/DemoAnalyzer/src/DemoAnalyzer.cc
//
// Description: [one line class summary]
//
// Implementation:
//     [Notes on implementation]
//
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Ptr.h"
//#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"

#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/Common/interface/Handle.h"

#include "TH1.h"

#include <stdlib.h>
#include <time.h>


//
// class decleration
//

class DemoAnalyzer : public edm::EDAnalyzer {
public:
  explicit DemoAnalyzer(const edm::ParameterSet&);
  ~DemoAnalyzer();


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  unsigned int minTracks_;
  TH1D *demohisto;

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

DemoAnalyzer::DemoAnalyzer(const edm::ParameterSet& iConfig) :
  minTracks_(iConfig.getUntrackedParameter<unsigned int>("minTracks",0))
{

  //now do what ever initialization is needed

  // I want to make a histogram of number of tracks in an event
     
  edm::Service<TFileService> fs;
  demohisto = fs->make<TH1D>("tracks" , "Tracks" , 100 , 0 , 5000 );

}


DemoAnalyzer::~DemoAnalyzer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
DemoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel("generalTracks", tracks); 
  //     LogInfo("Demo") << "number of tracks "<<tracks->size();
  demohisto->Fill(tracks->size());


  if( minTracks_ <= tracks->size() ) {
    LogInfo("Demo") << "number of tracks "<<tracks->size();
  }


  //Get b tag information
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("trackCountingHighEffBJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());

  // Loop over jets and study b tag info.
  for (Int_t i = 0; i != Int_t(bTags.size()); ++i) {
    std::cout <<" Jet "<< i << " has b tag discriminator = " << bTags[i].second << " and jet Pt = " << bTags[i].first->pt() << std::endl;
  }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
DemoAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DemoAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(DemoAnalyzer);

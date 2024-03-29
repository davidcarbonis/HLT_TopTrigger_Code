// -*- C++ -*-
//
// Package:    Demo/DemoAnalyser
// Class:      BTaggingReaderMiniAOD
// 
/**\class BTaggingReaderMiniAOD BTaggingReaderMiniAOD.cc Demo/DemoAnalyser/src/BTaggingReaderMiniAOD.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/


// system include files
#include <memory>
#include <math.h> 

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

// ROOT include files
#include "TTree.h"

//
// class declaration
//

class BTaggingReaderMiniAOD: public edm::EDAnalyzer 
{
public:
  explicit BTaggingReaderMiniAOD(const edm::ParameterSet&);
  ~BTaggingReaderMiniAOD();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  
  // ----------member data ---------------------------
  const edm::InputTag jets_;
  const std::vector<std::string> bDiscriminators_;
  
  bool triggerfired(const edm::Event& ev, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname);
  bool triggerfound(const edm::Event& ev, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname);
  unsigned int triggerIndex(const edm::Event& ev, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname);
  
  //  HLTConfigProvider hltConfig_; // to get configuration for L1s/Pre
  edm::InputTag hlTriggerResults_;    // Input tag for TriggerResults
  edm::InputTag hlTriggerEvent_;      // Input tag for TriggerEvent
  
  std::string triggerName_;
  edm::InputTag collectionName_;

  Float_t pT;
  Float_t eta;
  Int_t flavour;
  Float_t CSV;
  Int_t triggerFlag;

  bool debug;
  int counter;

  edm::Service<TFileService> fs;

  TTree *mytree_;
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
BTaggingReaderMiniAOD::BTaggingReaderMiniAOD(const edm::ParameterSet& iConfig) :

  jets_(iConfig.getParameter<edm::InputTag>("jets")),
  bDiscriminators_(iConfig.getParameter<std::vector<std::string> >("bDiscriminators"))
{
  std::string bDiscr_flav = "";
  triggerName_ = iConfig.getUntrackedParameter<std::string>("PathName","HLT_Dimuon0_Jpsi_v");
  hlTriggerResults_  = iConfig.getUntrackedParameter<edm::InputTag>("TriggerResultsTag", edm::InputTag("TriggerResults", "", "HLT"));
  hlTriggerEvent_    = iConfig.getUntrackedParameter<edm::InputTag>("TriggerEventTag", edm::InputTag("hltTriggerSummaryAOD", "", "HLT"));

  debug = iConfig.getUntrackedParameter<bool>("debug",true);

  mytree_ = fs->make<TTree>( "tree", "tree" );

  mytree_->Branch("pT",&pT,"pT/F");
  mytree_->Branch("eta",&eta,"eta/F");
  mytree_->Branch("flavour",&flavour,"flavour/I");
  mytree_->Branch("CSV",&CSV,"CSV/F");
  mytree_->Branch("triggerFlag",&triggerFlag,"triggerFlag/I");

}


BTaggingReaderMiniAOD::~BTaggingReaderMiniAOD()
{
	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}

bool BTaggingReaderMiniAOD::triggerfired(const edm::Event& iEvent, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname){
  const edm::TriggerNames TrigNames_ = iEvent.triggerNames(*triggerResultsHandle_);
  const unsigned int ntrigs = triggerResultsHandle_->size();
  for (unsigned int itr=0; itr<ntrigs; itr++){
    TString trigName=TrigNames_.triggerName(itr);
    if (!triggerResultsHandle_->accept(itr)) continue;
    if(trigName.Contains(trigname))      return true;
  }
  return false;
}

bool BTaggingReaderMiniAOD::triggerfound(const edm::Event& iEvent, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname){
  const edm::TriggerNames TrigNames_ = iEvent.triggerNames(*triggerResultsHandle_);
  const unsigned int ntrigs = triggerResultsHandle_->size();
  for (unsigned int itr=0; itr<ntrigs; itr++){
    TString trigName=TrigNames_.triggerName(itr);
    if(trigName.Contains(trigname))      return true;
  }
  return false;
}

unsigned int BTaggingReaderMiniAOD::triggerIndex(const edm::Event& iEvent, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname){
  const edm::TriggerNames TrigNames_ = iEvent.triggerNames(*triggerResultsHandle_);
  const unsigned int ntrigs = triggerResultsHandle_->size();
  unsigned int itr;
  for (itr=0; itr<ntrigs; itr++){
    TString trigName=TrigNames_.triggerName(itr);
    if(trigName.Contains(trigname))      return itr;
  }
  return itr;
}



//
// member functions
//

// ------------ method called for each event  ------------
void
BTaggingReaderMiniAOD::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  counter++;

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// *** get Handle to the TriggerResults
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
  edm::Handle<edm::TriggerResults> HLTR; 
  iEvent.getByLabel(hlTriggerResults_, HLTR);
  if (!HLTR.isValid()) 
  {
      if (debug) std::cout << "HLT TriggerResults with label " << hlTriggerResults_ << " not found!";
      return;
  }

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// define a Jet handle
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

  edm::Handle<std::vector<pat::Jet> > jets;
  // Get Jets from the event
  iEvent.getByLabel(jets_, jets);

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// Only events in which the path has actually fired has stored the filter results and products
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

  bool triggerFound = triggerfound(iEvent,HLTR,triggerName_);
  //if (triggerFound) countInAccepted++;
  bool triggerFired = triggerfired(iEvent,HLTR,triggerName_);
  
  const unsigned int numberOfHltPaths = HLTR->size();
  //const unsigned int numberOfHltFilters = triggerEventHandle_->sizeFilters();
  
  unsigned int pathIndex = triggerIndex(iEvent,HLTR,triggerName_);

  if (pathIndex>=numberOfHltPaths) 
  {
    std::cout << " WARNING: path " << triggerName_ << " out of range in TriggerResults" << std::endl;
    return;
  }

  if (HLTR->wasrun(pathIndex)) 
  {
    if (!triggerFound) std::cout << " WARNING: path exists in HLTR but not found in TriggerResults" << std::endl;
  }
  
  else 	
  {
    if (triggerFound) std::cout << " WARNING: path found in TriggerResults but it does not exist in HLTR" << std::endl;
  }

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// Start analysing appropriate jets
// --------------------------------------------------------------------------------	
// --------------------------------------------------------------------------------	

  std::string bDiscr_flav = "";
  
  for( auto jet = jets->begin(); jet != jets->end(); ++jet )
  {

    for( const std::string &bDiscr : bDiscriminators_ )
    {
     
      if( jet->pt()<30. || std::abs(jet->eta())>3.0 ) 
      {
	continue; // skip jets with low pT or outside the tracker acceptance
      }

      pT = jet->pt();
      eta = jet->eta();
      flavour= jet->partonFlavour();
      CSV = jet->bDiscriminator(bDiscr);

    }
    
   
    // Fill trigger fired Flag
    if (triggerFired)  
    {
      triggerFlag = 1;	
    }
    else
    {
      triggerFlag = 0;
    }

    mytree_->Fill();

  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
BTaggingReaderMiniAOD::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
BTaggingReaderMiniAOD::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
BTaggingReaderMiniAOD::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
BTaggingReaderMiniAOD::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
BTaggingReaderMiniAOD::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
BTaggingReaderMiniAOD::std::endluminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BTaggingReaderMiniAOD::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTaggingReaderMiniAOD);

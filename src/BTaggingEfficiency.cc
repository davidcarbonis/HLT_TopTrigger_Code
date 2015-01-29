// -*- C++ -*-
//
// Package:    Demo/DemoAnalyser
// Class:      BTaggingEfficiency
// 
/**\class BTaggingEfficiency BTaggingEfficiency.cc Demo/DemoAnalyser/src/BTaggingEfficiency.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alexander Morton
//         Created:  Tue, 16 Dec 2014 00:09:47 GMT
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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "TH2F.h"

//
// class declaration
//

class BTaggingEfficiency : public edm::EDAnalyzer 
{
public:
  explicit BTaggingEfficiency(const edm::ParameterSet&);
  ~BTaggingEfficiency();

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

  bool debug;
  int counter;
  int lTotalNumJets;

  double lFracJetsCsv[8];
  double lFracBjetsCsv[8];

  edm::Service<TFileService> fs;

  // declare a map of b-tag discriminator histograms
  std::map<std::string, TH2F *> bDiscriminatorsMap;
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
BTaggingEfficiency::BTaggingEfficiency(const edm::ParameterSet& iConfig) :

  jets_(iConfig.getParameter<edm::InputTag>("jets")),
  bDiscriminators_(iConfig.getParameter<std::vector<std::string> >("bDiscriminators"))
  {
 	counter = 0;
	lTotalNumJets = 0;
	for (int i = 0; i != 8; ++i) {
	lFracJetsCsv[i] = {0};
	lFracBjetsCsv[i] = {0};}


   	std::string bDiscr_flav = "";
   	triggerName_ = iConfig.getUntrackedParameter<std::string>("PathName","HLT_Dimuon0_Jpsi_v");
  	hlTriggerResults_  = iConfig.getUntrackedParameter<edm::InputTag>("TriggerResultsTag", edm::InputTag("TriggerResults", "", "HLT"));
  	hlTriggerEvent_    = iConfig.getUntrackedParameter<edm::InputTag>("TriggerEventTag", edm::InputTag("hltTriggerSummaryAOD", "", "HLT"));

  	debug = iConfig.getUntrackedParameter<bool>("debug",true);

   	// initialize b-tag discriminator histograms
   	for( const std::string &bDiscr : bDiscriminators_ )
   	{
    		for( const std::string &flav : {"b","c","udsg"} )
     		{
       			bDiscr_flav = bDiscr + "_" + flav;
       			if( bDiscr.find("Counting") != std::string::npos ) // track counting discriminator can be both positive and negative and covers a wider range then other discriminators
         			bDiscriminatorsMap[bDiscr_flav] = fs->make<TH2F>(bDiscr_flav.c_str(), (bDiscr_flav + ";Jet p_{T} [GeV];b-tag discriminator").c_str(), 20, 0, 200, 11000, -15, 40);
       			else
         			bDiscriminatorsMap[bDiscr_flav] = fs->make<TH2F>(bDiscr_flav.c_str(), (bDiscr_flav + ";Jet p_{T} [GeV];b-tag discriminator").c_str(), 20, 0, 200, 4400, -11, 11);
     }
   }
}


BTaggingEfficiency::~BTaggingEfficiency()
{
 
	std::cout << "lTotalNumJets: " << lTotalNumJets << std::endl;

	std::cout << "lFracJetsCsv[0]: " << lFracJetsCsv[0] << std::endl;
	std::cout << "lFracBjetsCsv[0]: " << lFracBjetsCsv[0] << std::endl;

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}

bool BTaggingEfficiency::triggerfired(const edm::Event& iEvent, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname){
  const edm::TriggerNames TrigNames_ = iEvent.triggerNames(*triggerResultsHandle_);
  const unsigned int ntrigs = triggerResultsHandle_->size();
  for (unsigned int itr=0; itr<ntrigs; itr++){
    TString trigName=TrigNames_.triggerName(itr);
    if (!triggerResultsHandle_->accept(itr)) continue;
    if(trigName.Contains(trigname))      return true;
  }
  return false;
}

bool BTaggingEfficiency::triggerfound(const edm::Event& iEvent, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname){
  const edm::TriggerNames TrigNames_ = iEvent.triggerNames(*triggerResultsHandle_);
  const unsigned int ntrigs = triggerResultsHandle_->size();
  for (unsigned int itr=0; itr<ntrigs; itr++){
    TString trigName=TrigNames_.triggerName(itr);
    if(trigName.Contains(trigname))      return true;
  }
  return false;
}

unsigned int BTaggingEfficiency::triggerIndex(const edm::Event& iEvent, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname){
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
BTaggingEfficiency::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  	counter++;
	/*if (debug)  std::cout << "==== Event "<<counter<<" ====" << std::endl;

  	// *** get Handle to the TriggerEvent
  	edm::Handle<trigger::TriggerEvent> triggerEventHandle_;
  	iEvent.getByLabel(hlTriggerEvent_,triggerEventHandle_);
  	if (!triggerEventHandle_.isValid()) 
	{
    		std::cout << "Error in getting TriggerEvent product from Event!" << std::endl;
    		return;
  	} */

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
//  	const unsigned int numberOfHltFilters = triggerEventHandle_->sizeFilters();

/*	if (debug) 
	{
		std::cout << "Number of filters present: " << numberOfHltFilters << std::endl;
	}
*/
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

	lTotalNumJets = lTotalNumJets + jets->size();

	std::string bDiscr_flav = "";
	uint32_t i = 1;
	// loop over leading 4 jets
	for( auto jet = jets->begin(); /*jet != jets->end()*/ i !=5 ; ++jet, ++i )
	{
		if (triggerFired)  continue;	// Only continue if trigger has been fired!
		//std::cout << "Trigger Fired!" << std::endl;
		int flavour = std::abs( jet->partonFlavour() );
		//std::std::cout << "i: " << i << std::std::endl;

		// fill discriminator histograms
		for( const std::string &bDiscr : bDiscriminators_ )
		{
			if( flavour==0 ) continue; // skip jets with undefined flavour
			if( jet->pt()<30. || std::abs(jet->eta())>3.0 ) continue; // skip jets with low pT or outside the tracker acceptance
			
			int lArrayIt = 0;
			for (double lBdiscrIt = 0.20; lBdiscrIt != 1.00; lBdiscrIt += 0.10, ++lArrayIt )
			{
				if (jet->bDiscriminator(bDiscr) >= lBdiscrIt)
				{
					++lFracJetsCsv[lArrayIt];
					if (flavour == 5)
					{
						++lFracBjetsCsv[lArrayIt];
					}
				}

			}

			if( flavour==5 ) // b jet
				bDiscr_flav = bDiscr + "_b";
			else if( flavour==4 ) // c jets
				bDiscr_flav = bDiscr + "_c";
			else // light-flavour jet
				bDiscr_flav = bDiscr + "_udsg";

			bDiscriminatorsMap[bDiscr_flav]->Fill( jet->pt(), jet->bDiscriminator(bDiscr) );
		}
	}
}


// ------------ method called once each job just before starting event loop  ------------
void 
BTaggingEfficiency::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
BTaggingEfficiency::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
BTaggingEfficiency::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
BTaggingEfficiency::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
BTaggingEfficiency::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
BTaggingEfficiency::std::endluminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BTaggingEfficiency::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTaggingEfficiency);

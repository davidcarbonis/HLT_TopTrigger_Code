#include <string>
#include <cstring>
#include <stdlib.h>
#include <math.h>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TLatex.h"

#include "TColor.h"
#include "TStyle.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Config.h"
#include "TMVA/Event.h"
#endif

#include "AnalysisFileEvent.h"


using namespace TMVA;


int main( int argc, char *argv[] )
{

  Int_t lNumberOfFiles = argc;
  TChain *lTchain = new TChain( "bTaggingReader/tree" ); // bTaggingReader - subdir, tree - tree
  TFile *lRootFile = new TFile ( "Output.root", "RECREATE" ) ;
  
  int lTotalNumJets;
  int lTotalNumTriggeredJets;
  int lTotalNumPassedJets;
  int lTotalNumBjets;
  int lTotalNumBjetsCsv[22];
  int lTotalNumRejJets;
  int lTotalNumPassedBjets;

  double lPassedJetsCsv[22];
  double lRejJetsCsv[22];
  double lPassedTriggeredBjetsCsv[22];
  double lPassedJetFraction[22];
  double lRejJetFraction[22];
  double lPassedBjetFraction[22];
  double lPassedBjetFractionTotalBjets[22];

  lTotalNumJets = 0;
  lTotalNumTriggeredJets = 0;
  lTotalNumPassedJets = 0;
  lTotalNumBjets = 0;
  lTotalNumRejJets = 0;
  lTotalNumPassedBjets = 0;

  double lCsvValueArraryFiller = 0.00;

  for (int i = 0; i != 22; ++i, lCsvValueArraryFiller += 0.05) 
    {
      lPassedJetsCsv[i] = {0.0};
      lPassedTriggeredBjetsCsv[i] = {0.0};
      lPassedJetFraction[i] = {0.0};
      lTotalNumBjetsCsv[i] = {0};
      lRejJetFraction[i] = {0.0};
      lPassedBjetFraction[i] = {0.0};
      lPassedBjetFractionTotalBjets[i] = {0.0};
    }


  gStyle->SetOptStat(0); 

  const Int_t NRGBs = 5;
  const Int_t NCont = 999;  

  // RGBFT povray definitions of colour RED/GREEN?BLUE/FILTER/TRANSMIT
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  TH2F * h_OldBjetTurnOnCurve = new TH2F ("h_OldBjetTurnOnCurve", "Old Turn-on Curve (b jets)", 200, 0.0, 2.0, 100, 0.0, 1.0);
  TH2F * h_BjetTurnOnCurve = new TH2F ("h_BjetTurnOnCurve", "Turn-on Curve (b jets)", 100, 0.0, 1.0, 100, 0.0, 1.0);
  TH2F * h_NewBjetTurnOnCurve = new TH2F ("h_NewBjetTurnOnCurve", "Turn-on Curve (b jets)", 100, 0.0, 1.0, 100, 0.0, 1.0);

  TH2F * h_JetPassEfficiency = new TH2F ("h_JetPassEfficiency", "Jet Pass Efficiency", 100, 0.0, 1.0, 100, 0.0, 1.0);
  TH2F * h_BjetPassEfficiency = new TH2F ("h_BjetPassEfficiency", "b-jet Pass Effifiency", 100, 0.0, 1.0, 100, 0.0, 1.0);
  TH2F * h_NewBjetPassEfficiency = new TH2F ("h_NewBjetPassEfficiency", "b-jet Pass Effifiency", 100, 0.0, 1.0, 100, 0.0, 1.0);

  h_OldBjetTurnOnCurve->SetTitle("Turn-on Curves for b-jets");
  h_OldBjetTurnOnCurve->GetXaxis()->SetTitle("-log_{10}(1-CSV)");
  h_OldBjetTurnOnCurve->GetYaxis()->SetTitle("Trigger Efficiency wrt. offline Cut");
  h_BjetTurnOnCurve->SetTitle("Turn-on Curves for b-jets");
  h_BjetTurnOnCurve->GetXaxis()->SetTitle("Offline Cut Rejection Efficiency");
  h_BjetTurnOnCurve->GetYaxis()->SetTitle("Trigger Efficiency wrt. offline Cut");
  h_NewBjetTurnOnCurve->SetTitle("Turn-on Curves for b-jets");
  h_NewBjetTurnOnCurve->GetXaxis()->SetTitle("Offline Cut Rejection Efficiency");
  h_NewBjetTurnOnCurve->GetYaxis()->SetTitle("Trigger Efficiency wrt. offline Cut");

  h_JetPassEfficiency->SetTitle("Efficiency of b-tag trigger");
  h_JetPassEfficiency->GetXaxis()->SetTitle("CSV Cut");
  h_JetPassEfficiency->GetYaxis()->SetTitle("Efficiency");
  h_BjetPassEfficiency->SetTitle("Efficiency of b-tag trigger - b-jets");
  h_BjetPassEfficiency->GetXaxis()->SetTitle("CSV Cut");
  h_BjetPassEfficiency->GetYaxis()->SetTitle("Efficiency");

  // Set up TChain

  for ( Int_t i = 1; i < (lNumberOfFiles) ; i++ )
    {
      std::stringstream lSStr;
      lSStr << argv[i];
      lTchain->Add((lSStr.str()).c_str());
    }

  AnalysisFileEvent* lEvent = new AnalysisFileEvent(lTchain);
  Int_t lNumEntries = lTchain->GetEntries(); // Get number of of entries/events in TChain

  // Start Analysis

  for ( Int_t i = 0; i < lNumEntries; i++ )
    {
      lTchain->GetEvent(i);
      //      std::cout << "Processing jet: " << i << std::endl;
    
      Float_t lPt (lEvent->pT);
      Float_t lEta (lEvent->eta);
      Int_t lFlavour (lEvent->flavour);
      Float_t lCSV (lEvent->CSV);
      Int_t lTriggerFlag (lEvent->triggerFlag);
    
      if ( lPt < 30.0 || std::abs( lEta > 3.0 ))
	{
	  continue;
	}     
      
      int lArrayItr1 = 0;
      
      if (lFlavour == 5 )
	{
	  ++lTotalNumBjets;
	}
      
      for (float lBdiscrIt = 0.0; lBdiscrIt <= 1.00; lBdiscrIt += 0.05, ++lArrayItr1 )
	{
	  if ( lCSV >= lArrayItr1 )
	    {
	      ++lTotalNumBjetsCsv[lArrayItr1];
	    }
	}

      if ( lTriggerFlag == 1 ) continue;
      ++lTotalNumTriggeredJets;

      ++lTotalNumPassedJets;

      int lArrayItr2 = 0;
      for (float lBdiscrIt = 0.0; lBdiscrIt <= 1.00; lBdiscrIt += 0.05, ++lArrayItr2 )
	{

	  //std::cout << "lBdiscrIt: " << lBdiscrIt << std::endl;;
	  if ( lCSV <= lBdiscrIt )
	    {
	      ++lRejJetsCsv[lArrayItr2];
	    }


	  if ( lCSV >= lBdiscrIt )
	    {
	      //std::cout << "lBdiscrIt: " << lBdiscrIt << std::endl;;
	      //std::cout << "lArrayItr2: " << lArrayItr2 << std::endl;

	      ++lPassedJetsCsv[lArrayItr2];
	      if (lFlavour == 5)
		{
		  ++lPassedTriggeredBjetsCsv[lArrayItr2];
		}
	    }
				
	}


      if( lFlavour==5 ) // b jet
	{
	  ++lTotalNumPassedBjets;
	}


    }
  

  double lTempCsvDiscr = 0.0;

  for (int i = 0; i != 22; ++i, lTempCsvDiscr += 0.05) 
    {
      lPassedJetFraction[i] = lPassedJetsCsv[i]/lTotalNumPassedJets;
      lPassedBjetFraction[i] = lPassedTriggeredBjetsCsv[i]/lTotalNumPassedJets;
      lPassedBjetFractionTotalBjets[i] = lPassedTriggeredBjetsCsv[i]/lTotalNumBjetsCsv[0];	// Using [0] for denominator so that the total number of b-Jets (ie. those which passed the lowest cut) are used ...
      lRejJetFraction[i] = lRejJetsCsv[i]/lTotalNumPassedJets;
      //		std::cout << "1 - lPassedJetFraction[" << i << "]: " << (1-lPassedJetFraction[i]) << std::endl;
      //		std::cout << "lRejJetFraction[" << i << "]: " << lRejJetFraction[i] << std::endl;
      //		std::cout << "lPassedBjetFraction[" << i << "]: " << lPassedBjetFraction[i] << std::endl;

      double Old_f_x = -log10(1-lTempCsvDiscr);// lPassedJetsCsv[i]/lTotalNumJets;
      double f_x = lPassedJetFraction[i];
      double New_f_x = lPassedBjetFractionTotalBjets[i];
      double eps_x = lPassedTriggeredBjetsCsv[i]/lPassedJetsCsv[i];

      //		std::cout << "Old_f_x: " << Old_f_x << std::endl;
      //		std::cout << "f_x: " << f_x << std::endl;
      //		std::cout << "eps_x: " << eps_x << std::endl;

      if (lPassedJetFraction [i] > 1.0) 
	{
	  std::cout << " WARNING: lPassedJetFraction [" << i << "]: " << lPassedJetFraction[i] << " is greater than 1.0!" << std::endl;
	  return 0;
	}

      if (lPassedBjetFraction [i] > 1.0) 
	{
	  std::cout << " WARNING: lPassedBjetFraction [" << i << "]: " << lPassedBjetFraction[i] << " is greater than 1.0!" << std::endl;
	  return 0;
	}

      if (eps_x > 1.0)
	{
	  std::cout << " WARNING: eps_x = lPassedTriggeredBjetsCsv[" << i << "]/lPassedJetsCsv[" << i << "]: " << eps_x << " is greater than 1.0!" << std::endl;
	}

      h_JetPassEfficiency->Fill( lTempCsvDiscr, lPassedJetFraction[i] );
      h_BjetPassEfficiency->Fill (lTempCsvDiscr, lPassedBjetFractionTotalBjets[i] );
      h_NewBjetPassEfficiency->Fill (lTempCsvDiscr, lPassedTriggeredBjetsCsv[i]/lTotalNumBjets);

      h_OldBjetTurnOnCurve->Fill( Old_f_x, eps_x );
      h_BjetTurnOnCurve->Fill( 1-f_x, eps_x );
      h_NewBjetTurnOnCurve->Fill( 1-New_f_x, eps_x );

    }

  
  h_JetPassEfficiency->Write();
  h_BjetPassEfficiency->Write();
  h_NewBjetPassEfficiency->Write();
  
  h_OldBjetTurnOnCurve->Write();
  h_BjetTurnOnCurve->Write();
  h_NewBjetTurnOnCurve->Write();
  
  lRootFile->Close();

}

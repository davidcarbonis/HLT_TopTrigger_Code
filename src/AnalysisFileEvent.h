
#ifndef AnalysisFileEvent_h
#define AnalysisFileEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <string>

// Header file for the classes stored in the TTree if any.

class AnalysisFileEvent {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         pT;
   Float_t         eta;
   Int_t           flavour;
   Float_t         CSV;
   Int_t           triggerFlag;

   // List of branches
   TBranch        *b_pT;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_flavour;   //!
   TBranch        *b_CSV;   //!
   TBranch        *b_triggerFlag;   //!

   AnalysisFileEvent(TTree *tree=0);
   virtual ~AnalysisFileEvent();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef AnalysisFileEvent_cc
AnalysisFileEvent::AnalysisFileEvent(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) 
     {
       TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("./File0.root");
       if (!f || !f->IsOpen()) 
	 {
	   f = new TFile("./File0.root");
	 }
      f->GetObject("tree",tree);

      }
   Init(tree);
}

AnalysisFileEvent::~AnalysisFileEvent()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t AnalysisFileEvent::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t AnalysisFileEvent::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void AnalysisFileEvent::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("pT", &pT, &b_pT);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("flavour", &flavour, &b_flavour);
   fChain->SetBranchAddress("CSV", &CSV, &b_CSV);
   fChain->SetBranchAddress("triggerFlag", &triggerFlag, &b_triggerFlag);

   Notify();
}

Bool_t AnalysisFileEvent::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void AnalysisFileEvent::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t AnalysisFileEvent::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

#endif // #ifdef SkimFileEvent_cxx


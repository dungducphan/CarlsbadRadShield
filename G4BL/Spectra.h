//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jul 24 09:46:07 2025 by ROOT version 6.30/06
// from TTree Spec1/VirtualDetector/Spec1
// found on file: ./GammaSpectrum.root
//////////////////////////////////////////////////////////

#ifndef Spectra_h
#define Spectra_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class Spectra {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         x;
   Float_t         y;
   Float_t         z;
   Float_t         Px;
   Float_t         Py;
   Float_t         Pz;
   Float_t         t;
   Float_t         PDGid;
   Float_t         EventID;
   Float_t         TrackID;
   Float_t         ParentID;
   Float_t         Weight;

   // List of branches
   TBranch        *b_x;   //!
   TBranch        *b_y;   //!
   TBranch        *b_z;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_t;   //!
   TBranch        *b_PDGid;   //!
   TBranch        *b_EventID;   //!
   TBranch        *b_TrackID;   //!
   TBranch        *b_ParentID;   //!
   TBranch        *b_Weight;   //!

   Spectra(TTree *tree=0);
   virtual ~Spectra();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Spectra_cxx
Spectra::Spectra(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("./GammaSpectrum.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("./GammaSpectrum.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("./GammaSpectrum.root:/VirtualDetector");
      dir->GetObject("Spec1",tree);

   }
   Init(tree);
}

Spectra::~Spectra()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Spectra::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Spectra::LoadTree(Long64_t entry)
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

void Spectra::Init(TTree *tree)
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

   fChain->SetBranchAddress("x", &x, &b_x);
   fChain->SetBranchAddress("y", &y, &b_y);
   fChain->SetBranchAddress("z", &z, &b_z);
   fChain->SetBranchAddress("Px", &Px, &b_Px);
   fChain->SetBranchAddress("Py", &Py, &b_Py);
   fChain->SetBranchAddress("Pz", &Pz, &b_Pz);
   fChain->SetBranchAddress("t", &t, &b_t);
   fChain->SetBranchAddress("PDGid", &PDGid, &b_PDGid);
   fChain->SetBranchAddress("EventID", &EventID, &b_EventID);
   fChain->SetBranchAddress("TrackID", &TrackID, &b_TrackID);
   fChain->SetBranchAddress("ParentID", &ParentID, &b_ParentID);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   Notify();
}

Bool_t Spectra::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Spectra::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Spectra::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Spectra_cxx

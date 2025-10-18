//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Sep 25 13:12:12 2025 by ROOT version 6.30/06
// from TTree Det/VirtualDetector/Det
// found on file: ./E1000000.root
//////////////////////////////////////////////////////////

#ifndef ana_h
#define ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class ana {
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
   Float_t         Bx;
   Float_t         By;
   Float_t         Bz;
   Float_t         Ex;
   Float_t         Ey;
   Float_t         Ez;
   Float_t         ProperTime;
   Float_t         PathLength;
   Float_t         PolX;
   Float_t         PolY;
   Float_t         PolZ;
   Float_t         InitX;
   Float_t         InitY;
   Float_t         InitZ;
   Float_t         InitT;
   Float_t         InitKE;

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
   TBranch        *b_Bx;   //!
   TBranch        *b_By;   //!
   TBranch        *b_Bz;   //!
   TBranch        *b_Ex;   //!
   TBranch        *b_Ey;   //!
   TBranch        *b_Ez;   //!
   TBranch        *b_ProperTime;   //!
   TBranch        *b_PathLength;   //!
   TBranch        *b_PolX;   //!
   TBranch        *b_PolY;   //!
   TBranch        *b_PolZ;   //!
   TBranch        *b_InitX;   //!
   TBranch        *b_InitY;   //!
   TBranch        *b_InitZ;   //!
   TBranch        *b_InitT;   //!
   TBranch        *b_InitKE;   //!

   ana(TTree *tree=0);
   virtual ~ana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ana_cxx
ana::ana(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("./E1000000.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("./E1000000.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("./E1000000.root:/VirtualDetector");
      dir->GetObject("Det",tree);

   }
   Init(tree);
}

ana::~ana()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ana::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ana::LoadTree(Long64_t entry)
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

void ana::Init(TTree *tree)
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
   fChain->SetBranchAddress("Bx", &Bx, &b_Bx);
   fChain->SetBranchAddress("By", &By, &b_By);
   fChain->SetBranchAddress("Bz", &Bz, &b_Bz);
   fChain->SetBranchAddress("Ex", &Ex, &b_Ex);
   fChain->SetBranchAddress("Ey", &Ey, &b_Ey);
   fChain->SetBranchAddress("Ez", &Ez, &b_Ez);
   fChain->SetBranchAddress("ProperTime", &ProperTime, &b_ProperTime);
   fChain->SetBranchAddress("PathLength", &PathLength, &b_PathLength);
   fChain->SetBranchAddress("PolX", &PolX, &b_PolX);
   fChain->SetBranchAddress("PolY", &PolY, &b_PolY);
   fChain->SetBranchAddress("PolZ", &PolZ, &b_PolZ);
   fChain->SetBranchAddress("InitX", &InitX, &b_InitX);
   fChain->SetBranchAddress("InitY", &InitY, &b_InitY);
   fChain->SetBranchAddress("InitZ", &InitZ, &b_InitZ);
   fChain->SetBranchAddress("InitT", &InitT, &b_InitT);
   fChain->SetBranchAddress("InitKE", &InitKE, &b_InitKE);
   Notify();
}

Bool_t ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ana::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ana::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ana_cxx

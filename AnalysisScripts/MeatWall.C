#include <TH2.h>
#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>

#include <iostream>

const std::string DataPath = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/";
const std::string DataFile = "EDep_002_88000000_Iso_1MeV_HDC18.root";
const std::string PlotPath = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/Plots/";
const double massOfPhantom = 1.03; // kg
const double scaleFactor = 7102.3; // scale factor to the targeted charge
const double laserReprate = 100; // Hz

class MeatWall {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        EDep;
   Int_t           Phantom_ID;

   // List of branches
   TBranch        *b_EDep;   //!
   TBranch        *b_Phantom_ID;   //!

   MeatWall(TTree *tree=0);
   virtual ~MeatWall();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

MeatWall::MeatWall(TTree *tree) : fChain(0) {
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject((DataPath + DataFile).c_str());
      if (!f || !f->IsOpen()) {
         f = new TFile((DataPath + DataFile).c_str());
      }
      f->GetObject("EDep",tree);

   }
   Init(tree);
}

MeatWall::~MeatWall() {
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MeatWall::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MeatWall::LoadTree(Long64_t entry)
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

void MeatWall::Init(TTree *tree) {
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EDep", &EDep, &b_EDep);
   fChain->SetBranchAddress("HumanPhantom_ID", &Phantom_ID, &b_Phantom_ID);
   Notify();
}

Bool_t MeatWall::Notify() {
   return kTRUE;
}

void MeatWall::Show(Long64_t entry) {
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t MeatWall::Cut(Long64_t entry) {
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void MeatWall::Loop() {
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TH2D* meatWall = new TH2D("meatWall", "Meat Wall", 31, -0.5, 30.5, 31, -0.5, 30.5);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      int x = Phantom_ID / 100;
      int y = Phantom_ID % 100;
      meatWall->Fill(x, y, EDep);
   }

   meatWall->Scale(scaleFactor * laserReprate / massOfPhantom);
   meatWall->Draw("colz");
   gStyle->SetOptStat(0);
}

#if !defined(__CINT__) && !defined(__CLING__) && !defined(__ACLIC__)

int main() {
   MeatWall *ana = new MeatWall();
   ana->Loop();

   return 0;
}
#endif

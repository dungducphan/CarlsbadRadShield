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
const std::string DataFile = "EDep_000_440000_Beam.root";
const std::string PlotPath = "/home/dphan/Documents/GitHub/CarlsbadRadShield/AnalysisScripts/Plots/";
const double massOfHumanPhantom = 252.175; // kg

class BeamAna {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        EDep;
   Double_t        PDGID;
   Double_t        HumanPhantom_ID;

   // List of branches
   TBranch        *b_EDep;   //!
   TBranch        *b_PDGID;   //!
   TBranch        *b_HumanPhantom_ID;   //!

   BeamAna(TTree *tree=0);
   virtual ~BeamAna();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

BeamAna::BeamAna(TTree *tree) : fChain(0) {
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

BeamAna::~BeamAna() {
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t BeamAna::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t BeamAna::LoadTree(Long64_t entry)
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

void BeamAna::Init(TTree *tree) {
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EDep", &EDep, &b_EDep);
   fChain->SetBranchAddress("PDGID", &PDGID, &b_PDGID);
   fChain->SetBranchAddress("HumanPhantom_ID", &HumanPhantom_ID, &b_HumanPhantom_ID);
   Notify();
}

Bool_t BeamAna::Notify() {
   return kTRUE;
}

void BeamAna::Show(Long64_t entry) {
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t BeamAna::Cut(Long64_t entry) {
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void BeamAna::Loop() {
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   double absorbedEnergy_Human1 = 0; // Joule
   double absorbedEnergy_Human2 = 0; // Joule
   double absorbedEnergy_Human3 = 0; // Joule
   double absorbedEnergy_Human4 = 0; // Joule

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if (HumanPhantom_ID == 1) {
         absorbedEnergy_Human1 += EDep;
      } else if (HumanPhantom_ID == 2) {
         absorbedEnergy_Human2 += EDep;
      } else if (HumanPhantom_ID == 3) {
         absorbedEnergy_Human3 += EDep;
      } else if (HumanPhantom_ID == 4) {
         absorbedEnergy_Human4 += EDep;
      }
   }

   absorbedEnergy_Human1 = absorbedEnergy_Human1 * 710.2 * 100 / massOfHumanPhantom; // Gy/s
   absorbedEnergy_Human2 = absorbedEnergy_Human2 * 710.2 * 100 / massOfHumanPhantom; // Gy/s
   absorbedEnergy_Human3 = absorbedEnergy_Human3 * 710.2 * 100 / massOfHumanPhantom; // Gy/s
   absorbedEnergy_Human4 = absorbedEnergy_Human4 * 710.2 * 100 / massOfHumanPhantom; // Gy/s

   std::cout << "Beam Charge: 50pC. Rep-rate 100Hz. Beam Energy 200MeV." << std::endl;
   std::cout << "Absorbed Dose Rate in Human Phantom 1: " << absorbedEnergy_Human1 << " Gy/s" << std::endl;
   std::cout << "Absorbed Dose Rate in Human Phantom 2: " << absorbedEnergy_Human2 << " Gy/s" << std::endl;
   std::cout << "Absorbed Dose Rate in Human Phantom 3: " << absorbedEnergy_Human3 << " Gy/s" << std::endl;
   std::cout << "Absorbed Dose Rate in Human Phantom 4: " << absorbedEnergy_Human4 << " Gy/s" << std::endl;
}

#if !defined(__CINT__) && !defined(__CLING__) && !defined(__ACLIC__)

int main() {
   BeamAna *ana = new BeamAna();
   ana->Loop();

   return 0;
}
#endif

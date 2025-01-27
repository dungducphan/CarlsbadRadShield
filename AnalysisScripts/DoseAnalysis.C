#include <TH2.h>
#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TMath.h>
#include <iostream>

double* logspace(double Emin, double Emax, int N) {
   double* result = new double[N];
   double logEmin = TMath::Log10(Emin);
   double logEmax = TMath::Log10(Emax);
   double step = (logEmax - logEmin) / (N - 1);

   for (int i = 0; i < N; ++i) {
      result[i] = TMath::Power(logEmin + i * step, 10);
   }

   return result;
}

double numberOfElectronsSimulated = 44000;
double targetCharge = 75E-12; // Coulombs
double chargeScaleFactor = targetCharge / (numberOfElectronsSimulated * 1.6E-19); // scale factor to the targeted charge
double phantomMass = 1.03; // kg
double laserReprate = 100; // Hz
double Sievert_to_rem = 100; // Sievert to Rem conversion factor
double rem_to_mrem = 1000;
std::string filePath = "/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/Sandwich/Sandwich_44000.100MeV.Phantom9m.root";
std::string outputFile = "PhantomWall_Sandwich_AccidentalDirectHit_100MeV_75pC.Phantom9m.png";

class HDC18 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        EDep;
   Double_t        Weight;
   Int_t           HumanPhantom_ID;
   Double_t        Energy;
   Double_t        Px;
   Double_t        Py;
   Double_t        Pz;
   Double_t        X;
   Double_t        Y;
   Double_t        Z;
   Int_t           PID;

   // List of branches
   TBranch        *b_EDep;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_HumanPhantom_ID;   //!
   TBranch        *b_Energy;
   TBranch        *b_Px;
   TBranch        *b_Py;
   TBranch        *b_Pz;
   TBranch        *b_X;
   TBranch        *b_Y;
   TBranch        *b_Z;
   TBranch        *b_PID;


   HDC18(TTree *tree=0);
   virtual ~HDC18();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

HDC18::HDC18(TTree *tree) : fChain(0) {
   std::string filename = filePath;
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename.c_str());
      if (!f || !f->IsOpen()) {
         f = new TFile(filename.c_str());
      }
      f->GetObject("EDep",tree);

   }
   Init(tree);
}

HDC18::~HDC18() {
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t HDC18::GetEntry(Long64_t entry) {
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t HDC18::LoadTree(Long64_t entry) {
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

void HDC18::Init(TTree *tree) {
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EDep", &EDep, &b_EDep);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("HumanPhantom_ID", &HumanPhantom_ID, &b_HumanPhantom_ID);
   fChain->SetBranchAddress("Energy", &Energy, &b_Energy);
   fChain->SetBranchAddress("Px", &Px, &b_Px);
   fChain->SetBranchAddress("Py", &Py, &b_Py);
   fChain->SetBranchAddress("Pz", &Pz, &b_Pz);
   fChain->SetBranchAddress("X", &X, &b_X);
   fChain->SetBranchAddress("Y", &Y, &b_Y);
   fChain->SetBranchAddress("Z", &Z, &b_Z);
   fChain->SetBranchAddress("PID", &PID, &b_PID);
   Notify();
}

Bool_t HDC18::Notify() {
   return kTRUE;
}

void HDC18::Show(Long64_t entry) {
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t HDC18::Cut(Long64_t entry) {
   return 1;
}

void HDC18::Loop() {
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TH2D* PhantomWall;

   double Emin = 1.0E0; // Example minimum value
   double Emax = 1.0E6; // Example maximum value
   int N = 1001; // Example number of elements

   double* logScaleArray = logspace(Emin, Emax, N);
   TH1D* HistEnergy;

   if (gSystem->AccessPathName("PhantomWall.root")) {
      PhantomWall = new TH2D("PhantomWall", "PhantomWall", 31, -0.5, 30.5, 31, -0.5, 30.5);
      HistEnergy = new TH1D("Energy", "Energy", N - 1, logScaleArray);
      Long64_t nbytes = 0, nb = 0;
      for (Long64_t jentry=0; jentry<nentries;jentry++) {
         Long64_t ientry = LoadTree(jentry);
         if (ientry < 0) break;
         nb = fChain->GetEntry(jentry);   nbytes += nb;
         const int x = HumanPhantom_ID % 31;
         const int y = (HumanPhantom_ID - x) / 31;
         PhantomWall->Fill(y, x, EDep * Weight);
         std::cout << "PID: " << PID << std::endl;
         if (PID == 22) HistEnergy->Fill(Energy, Weight);
      }
      PhantomWall->Scale(chargeScaleFactor * laserReprate / phantomMass);
      HistEnergy->Scale(chargeScaleFactor);
      auto outfile = new TFile("PhantomWall.root", "RECREATE");
      PhantomWall->Write();
      HistEnergy->Write();
      outfile->Close();
   } else {
      auto infile = new TFile("PhantomWall.root");
      PhantomWall = (TH2D*)infile->Get("PhantomWall");
      HistEnergy = (TH1D*)infile->Get("Energy");
   }

   gStyle->SetOptStat(0);
   PhantomWall->Scale(Sievert_to_rem * rem_to_mrem);
   PhantomWall->SetTitle("Phantom Wall Dose Rate Map (mrem/s)");
   PhantomWall->GetXaxis()->SetTitle("Cell Number (x10 cm)");
   PhantomWall->GetYaxis()->SetTitle("Cell Number (x10 cm)");
   PhantomWall->GetXaxis()->CenterTitle();
   PhantomWall->GetYaxis()->CenterTitle();
   // PhantomWall->GetZaxis()->SetRangeUser(0, 25);
   auto c1 = new TCanvas("c1", "c1", 800, 800);
   c1->SetMargin(0.15, 0.15, 0.15, 0.15);
   PhantomWall->Draw("colz");
   // c1->SaveAs("PhantomWall_HDC20_Occupational_2MeV_10nC.png");
   // c1->SaveAs("PhantomWall_HDC20_Accidental_200MeV_50pC_DirectHit.png");
   c1->SaveAs(outputFile.c_str());

   auto c2 = new TCanvas("c2", "c2", 800, 800);
   c2->SetMargin(0.15, 0.15, 0.15, 0.15);
   c2->SetLogx();
   c2->SetLogy();
   HistEnergy->SetTitle("Gamma Spectrum");
   HistEnergy->GetXaxis()->SetTitle("Energy (keV)");
   HistEnergy->GetYaxis()->SetTitle("Intensity (A.U.)");
   HistEnergy->GetXaxis()->SetRangeUser(Emin, Emax);
   HistEnergy->GetXaxis()->CenterTitle();
   HistEnergy->GetYaxis()->CenterTitle();
   HistEnergy->GetXaxis()->SetTitleOffset(1.4);
   HistEnergy->GetYaxis()->SetTitleOffset(1.4);
   HistEnergy->Draw();
   c2->SaveAs("EnergySpectrum.png");

   std::cout << "Total yield: " << HistEnergy->Integral() << std::endl;
   std::cout << "Percentage of gamma with energy more than 1MeV: " << HistEnergy->Integral(HistEnergy->FindBin(1000.0), HistEnergy->GetNbinsX()) / HistEnergy->Integral() * 100 << "%" << std::endl;
}

#if !defined(__CINT__) && !defined(__CLING__) && !defined(__ACLIC__)

int main() {
   HDC18 *ana = new HDC18();
   ana->Loop();

   return 0;
}
#endif
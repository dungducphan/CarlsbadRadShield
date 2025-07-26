#define Dosimeter_cxx
#include "Dosimeter.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Dosimeter::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Dosimeter.C
//      root> Dosimeter t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   auto hDoseMap = new TH2F("hDoseMap", "Dose Map", 40, -2000, 2000, 40, -2000, 2000); // in mm
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      // int binX = hDoseMap->GetXaxis()->FindBin(x);
      // int binY = hDoseMap->GetYaxis()->FindBin(y);
      hDoseMap->Fill(x, y, Edep);
   }

   double numSimulations = 1E6;
   double targetCharge = 1E-9;
   double eCharge = 1.6E-19;
   double scaleTo1nC = (targetCharge / (numSimulations * eCharge));
   double scaleTo100Hz = 100;
   double MeVToJoule = 1.60218e-13; 
   double mass = 1.0;
   double GyToMilliRem = 100000;
   double scaleFactor = scaleTo1nC * scaleTo100Hz * (MeVToJoule / mass) * GyToMilliRem;
   hDoseMap->Scale(scaleFactor);

   TCanvas *c1 = new TCanvas("c1", "Dose Rate Map", 800, 800);
   c1->SetMargin(0.15, 0.15, 0.15, 0.15);
   gStyle->SetOptStat(0);
   hDoseMap->Draw("COLZ");
   hDoseMap->SetTitle("Dose Rate Map (mrem/s)");
   hDoseMap->GetXaxis()->SetTitle("X (mm)");
   hDoseMap->GetYaxis()->SetTitle("Y (mm)");
   c1->SaveAs("DoseMap_10M.pdf");
}

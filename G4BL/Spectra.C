#define Spectra_cxx
#include "Spectra.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Spectra::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Spectra.C
//      root> Spectra t
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
   auto gamma_spectrum = new TH1D("gamma_spectrum", "Gamma Spectrum", 500, 0, 5);
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      gamma_spectrum->Fill(TMath::Sqrt(Px*Px + Py*Py + Pz*Pz)); // Fill histogram with momentum magnitude
   }
   gStyle->SetOptStat(0); // Show statistics on the histogram
   TCanvas *c1 = new TCanvas("c1", "Gamma Spectrum", 800, 600);
   gamma_spectrum->Draw();
   c1->SaveAs("gamma_spectrum.png");
}

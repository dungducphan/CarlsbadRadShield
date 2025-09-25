#define ana_cxx
#include "ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void ana::Loop()
{
//   In a ROOT session, you can do:
//      root> .L ana.C
//      root> ana t
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

   auto electronProfile = new TH2F("electronProfile","Electron Profile After Pinhole",4000,-200,200,4000,-200,200);
   auto electronProfile_Zoomed = new TH2F("electronProfile_Zoomed","Electron Profile After Pinhole (Zoomed)",200,-2,2,200,-2,2);
   auto electronSpectrum = new TH1F("electronSpectrum","Electron Spectrum After Pinhole",500,0,100);
   auto electronSpectrum_Pinhole = new TH1F("electronSpectrum_Pinhole","Electron Spectrum After Pinhole",500,0,100);
   auto electronSpectrum_Bremss = new TH1F("electronSpectrum_Bremss","Electron Spectrum After Pinhole",500,0,100);

   Long64_t nbytes = 0, nb = 0;
   int count = 0;
   int count_all_e = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if (PDGid == 11) {
         electronProfile_Zoomed->Fill(x,y,Weight);
         electronProfile->Fill(x,y,Weight);
         electronSpectrum->Fill(TMath::Sqrt(Px*Px+Py*Py+Pz*Pz)-0.511,Weight);
         count_all_e++;
         if (TMath::Sqrt(x*x+y*y) < 1) {
            electronSpectrum_Pinhole->Fill(TMath::Sqrt(Px*Px+Py*Py+Pz*Pz)-0.511,Weight);
            count++;
         } else {
            electronSpectrum_Bremss->Fill(TMath::Sqrt(Px*Px+Py*Py+Pz*Pz)-0.511,Weight);
         }
      }
   }

   double distance = 637; //mm
   double half_angle = 6.37E-3; //radian
   double stddev_sqrt2 = TMath::Sin(half_angle)*distance*TMath::Sqrt(2);
   double pinhole_radius = 0.5; //mm
   double erfL = 0.5*(1+TMath::Erf(-pinhole_radius/stddev_sqrt2));
   double erfR = 0.5*(1+TMath::Erf(pinhole_radius/stddev_sqrt2));
   double fraction = erfR - erfL;
   double N_simulation = 1000000;
   double N_expected = N_simulation*fraction;
   std::cout << "Total number of electrons simulated: " << N_simulation << std::endl;
   std::cout << "Total number of electrons in the dataset: " << count_all_e << std::endl;
   std::cout << "Expected fraction of electrons passing through pinhole: " << fraction << std::endl;
   std::cout << "Expected number of electrons passing through pinhole: " << N_expected << std::endl;
   std::cout << "Simulated number of electrons passing through pinhole: " << count << std::endl;

   TCanvas *c1 = new TCanvas("c1","c1",800,800);
   c1->cd();
   gStyle->SetOptStat(0);
   electronProfile->GetXaxis()->SetTitle("X (mm)");
   electronProfile->GetYaxis()->SetTitle("Y (mm)");
   electronProfile->SetTitle("Electron Profile After Pinhole");
   electronProfile->SetStats(0);
   electronProfile->Draw("COLZ");
   c1->SaveAs("electronProfileAfterPinhole.png");

   TCanvas *c3 = new TCanvas("c3","c3",800,800);
   c3->cd();
   gStyle->SetOptStat(0);
   electronProfile_Zoomed->GetXaxis()->SetTitle("X (mm)");
   electronProfile_Zoomed->GetYaxis()->SetTitle("Y (mm)");
   electronProfile_Zoomed->SetTitle("Electron Profile After Pinhole (Zoomed)");
   electronProfile_Zoomed->SetStats(0);
   electronProfile_Zoomed->Draw("COLZ");
   c3->SaveAs("electronProfileAfterPinhole_Zoomed.png");

   TCanvas *c2 = new TCanvas("c2","c2",800,800);
   c2->cd();
   electronSpectrum->GetXaxis()->SetTitle("Kinetic Energy (MeV)");
   electronSpectrum->GetYaxis()->SetTitle("Counts");
   electronSpectrum->SetTitle("Electron Spectrum After Pinhole");
   electronSpectrum->SetStats(0);
   electronSpectrum->SetLineColor(kBlack);
   electronSpectrum_Pinhole->SetLineColor(kRed);
   electronSpectrum_Bremss->SetLineColor(kBlue);
   electronSpectrum->Draw();
   electronSpectrum_Pinhole->Draw("SAME");
   electronSpectrum_Bremss->Draw("SAME");
   TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
   leg->AddEntry(electronSpectrum,"All Electrons","l");
   leg->AddEntry(electronSpectrum_Pinhole,"Through Pinhole","l");
   leg->AddEntry(electronSpectrum_Bremss,"From Bremss.","l");
   leg->SetBorderSize(0);
   leg->Draw();
   c2->SaveAs("electronSpectrumAfterPinhole.png");
}

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
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

#if !defined(__CINT__) && !defined(__CLING__) && !defined(__ACLIC__)

int main() {
    gStyle->SetOptStat(0);

    auto infile = new TFile("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_07222025/DoseMap_NoRoof_Beam50MeV_62500000.root");
    auto h2_ControlRoom = (TH2D*)infile->Get("DoseMap_ControlRoom");
    auto h2_ParkingArea = (TH2D*)infile->Get("DoseMap_ParkingArea");
    auto h2_LunchArea   = (TH2D*)infile->Get("DoseMap_LunchArea");
    auto h2_OfficeArea  = (TH2D*)infile->Get("DoseMap_OfficeArea");
    auto h2_LaserRoom   = (TH2D*)infile->Get("DoseMap_LaserRoom");

    // Scaling
    double massOfScoringBox = 8.24; // kg
    double scaleTo1nC = 10; // For 625000000 events
    double scaleTo15pC = 1.5; // For 62500000 events
    double scaleTo100Hz = 100;
    double eVtoJoules = 1.60218E-19; // eV to Joules
    double GyToMillirem = 100000; // Gy to millirem
    double scalingFactor = scaleTo15pC * scaleTo100Hz * (eVtoJoules / massOfScoringBox) * GyToMillirem;
    h2_ControlRoom->Scale(scalingFactor);
    h2_ParkingArea->Scale(scalingFactor);
    h2_LunchArea->Scale(scalingFactor);
    h2_OfficeArea->Scale(scalingFactor);
    h2_LaserRoom->Scale(scalingFactor);

    auto c_ControlRoom = new TCanvas("c_ControlRoom", "c_ControlRoom", 1800, 800);
    c_ControlRoom->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_ControlRoom->Draw("colz");
    h2_ControlRoom->SetTitle("Dose Rate Map - Control Room");
    h2_ControlRoom->GetXaxis()->SetRangeUser(0, 75);
    h2_ControlRoom->GetZaxis()->SetTitle("Dose rate (mrem/s)");
    h2_ControlRoom->GetXaxis()->CenterTitle();
    h2_ControlRoom->GetYaxis()->CenterTitle();
    h2_ControlRoom->GetXaxis()->SetTitle("Cell Number");
    h2_ControlRoom->GetYaxis()->SetTitle("Cell Number");
    c_ControlRoom->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_07222025/Beam50MeV/DoseMap_ControlRoom.png");

    auto c_ParkingArea = new TCanvas("c_ParkingArea", "c_ParkingArea", 1800, 800);
    c_ParkingArea->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_ParkingArea->Draw("colz");
    h2_ParkingArea->SetTitle("Dose Rate Map - Parking Area");
    // h2_ParkingArea->GetXaxis()->SetRangeUser(0, 75);
    h2_ParkingArea->GetZaxis()->SetTitle("Dose rate (mrem/s)");
    h2_ParkingArea->GetXaxis()->CenterTitle();
    h2_ParkingArea->GetYaxis()->CenterTitle();
    h2_ParkingArea->GetXaxis()->SetTitle("Cell Number");
    h2_ParkingArea->GetYaxis()->SetTitle("Cell Number");
    c_ParkingArea->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_07222025/Beam50MeV/DoseMap_ParkingArea.png");

    auto c_LunchArea = new TCanvas("c_LunchArea", "c_LunchArea", 1800, 800);
    c_LunchArea->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_LunchArea->Draw("colz");
    h2_LunchArea->SetTitle("Dose Rate Map - Lunch Area");
    h2_LunchArea->GetXaxis()->SetRangeUser(0, 40);
    h2_LunchArea->GetZaxis()->SetTitle("Dose rate (mrem/s)");
    h2_LunchArea->GetXaxis()->CenterTitle();
    h2_LunchArea->GetYaxis()->CenterTitle();
    h2_LunchArea->GetXaxis()->SetTitle("Cell Number");
    h2_LunchArea->GetYaxis()->SetTitle("Cell Number");
    c_LunchArea->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_07222025/Beam50MeV/DoseMap_LunchArea.png");

    auto c_OfficeArea = new TCanvas("c_OfficeArea", "c_OfficeArea", 1800, 800);
    c_OfficeArea->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_OfficeArea->Draw("colz");
    h2_OfficeArea->SetTitle("Dose Rate Map - Office Area");
    h2_OfficeArea->GetXaxis()->SetRangeUser(0, 40);
    h2_OfficeArea->GetZaxis()->SetTitle("Dose rate (mrem/s)");
    h2_OfficeArea->GetXaxis()->CenterTitle();
    h2_OfficeArea->GetYaxis()->CenterTitle();
    h2_OfficeArea->GetXaxis()->SetTitle("Cell Number");
    h2_OfficeArea->GetYaxis()->SetTitle("Cell Number");
    c_OfficeArea->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_07222025/Beam50MeV/DoseMap_OfficeArea.png");

    auto c_LaserRoom = new TCanvas("c_LaserRoom", "c_LaserRoom", 1800, 800);
    c_LaserRoom->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_LaserRoom->Draw("colz");
    h2_LaserRoom->SetTitle("Dose Rate Map - Laser Room");
    h2_LaserRoom->GetXaxis()->SetRangeUser(0, 40);
    h2_LaserRoom->GetZaxis()->SetTitle("Dose rate (mrem/s)");
    h2_LaserRoom->GetXaxis()->CenterTitle();
    h2_LaserRoom->GetYaxis()->CenterTitle();
    h2_LaserRoom->GetXaxis()->SetTitle("Cell Number");
    h2_LaserRoom->GetYaxis()->SetTitle("Cell Number");
    c_LaserRoom->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_07222025/Beam50MeV/DoseMap_LaserRoom.png");

    return 0;
}
#endif
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

    auto infile = new TFile("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_08222025/DoseMap_NoRoof_Beam50MeV.root");
    auto h2_ControlRoom = (TH2D*)infile->Get("DoseMap_ControlRoom");
    auto h2_ParkingArea = (TH2D*)infile->Get("DoseMap_ParkingArea");
    auto h2_LunchArea   = (TH2D*)infile->Get("DoseMap_LunchArea");
    auto h2_OfficeArea  = (TH2D*)infile->Get("DoseMap_OfficeArea");
    auto h2_LaserRoom   = (TH2D*)infile->Get("DoseMap_LaserRoom");

    // Scaling

    /* Change these 2 values */
    /* ============================================================================== */
    double targetCharge = 15E-12; // C
    double numberOfElectronsSimulated = 100000000;
    bool scaleToFullWorkingYear = false;
    /* ============================================================================== */

    double massOfScoringBox = 8.24; // kg
    double eCharge = 1.6E-19; // C
    double scaleToTargetCharge = targetCharge / (eCharge * numberOfElectronsSimulated);
    std::cout << "Scale Factor: " << scaleToTargetCharge << std::endl;
    double scaleTo100Hz = 100;
    double eVtoJoules = 1.6E-19; // eV to Joules
    double GyToMillirem = 100000; // Gy to millirem

    double numberOfSecondsInAYear = 3600 * 8 * 250;

    double scalingFactor = scaleToTargetCharge * scaleTo100Hz * (eVtoJoules / massOfScoringBox) * GyToMillirem;
    if (scaleToFullWorkingYear) {
        scalingFactor *= numberOfSecondsInAYear;
    }
    h2_ControlRoom->Scale(scalingFactor);
    h2_ParkingArea->Scale(scalingFactor);
    h2_LunchArea->Scale(scalingFactor);
    h2_OfficeArea->Scale(scalingFactor);
    h2_LaserRoom->Scale(scalingFactor);

    auto c_ControlRoom = new TCanvas("c_ControlRoom", "c_ControlRoom", 1800, 800);
    c_ControlRoom->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_ControlRoom->Draw("colz");
    h2_ControlRoom->SetTitle(scaleToFullWorkingYear ? "Dose Map - Control Room" : "Dose Rate Map - Control Room");
    h2_ControlRoom->GetXaxis()->SetRangeUser(0, 75);
    h2_ControlRoom->GetZaxis()->SetTitle(scaleToFullWorkingYear ? "Yearly Dose (mrem)" : "Dose rate (mrem/s)");
    h2_ControlRoom->GetXaxis()->CenterTitle();
    h2_ControlRoom->GetYaxis()->CenterTitle();
    h2_ControlRoom->GetXaxis()->SetTitle("Cell Number");
    h2_ControlRoom->GetYaxis()->SetTitle("Cell Number");
    c_ControlRoom->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_08222025/DoseMap_ControlRoom.png");

    auto c_ParkingArea = new TCanvas("c_ParkingArea", "c_ParkingArea", 1800, 800);
    c_ParkingArea->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_ParkingArea->Draw("colz");
    h2_ParkingArea->SetTitle(scaleToFullWorkingYear ? "Dose Map - Parking Area" : "Dose Rate Map - Parking Area");
    // h2_ParkingArea->GetXaxis()->SetRangeUser(0, 75);
    h2_ParkingArea->GetZaxis()->SetTitle(scaleToFullWorkingYear ? "Yearly Dose (mrem)" : "Dose rate (mrem/s)");
    h2_ParkingArea->GetXaxis()->CenterTitle();
    h2_ParkingArea->GetYaxis()->CenterTitle();
    h2_ParkingArea->GetXaxis()->SetTitle("Cell Number");
    h2_ParkingArea->GetYaxis()->SetTitle("Cell Number");
    c_ParkingArea->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_08222025/DoseMap_ParkingArea.png");

    auto c_LunchArea = new TCanvas("c_LunchArea", "c_LunchArea", 1800, 800);
    c_LunchArea->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_LunchArea->Draw("colz");
    h2_LunchArea->SetTitle(scaleToFullWorkingYear ? "Dose Rate - Lunch Area" : "Dose Rate Map - Lunch Area");
    h2_LunchArea->GetXaxis()->SetRangeUser(0, 40);
    h2_LunchArea->GetZaxis()->SetTitle(scaleToFullWorkingYear ? "Yearly Dose (mrem)" : "Dose rate (mrem/s)");
    h2_LunchArea->GetXaxis()->CenterTitle();
    h2_LunchArea->GetYaxis()->CenterTitle();
    h2_LunchArea->GetXaxis()->SetTitle("Cell Number");
    h2_LunchArea->GetYaxis()->SetTitle("Cell Number");
    c_LunchArea->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_08222025/DoseMap_LunchArea.png");

    auto c_OfficeArea = new TCanvas("c_OfficeArea", "c_OfficeArea", 1800, 800);
    c_OfficeArea->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_OfficeArea->Draw("colz");
    h2_OfficeArea->SetTitle(scaleToFullWorkingYear ? "Dose Map - Office Area" : "Dose Rate Map - Office Area");
    h2_OfficeArea->GetXaxis()->SetRangeUser(0, 40);
    h2_OfficeArea->GetZaxis()->SetTitle(scaleToFullWorkingYear ? "Yearly Dose (mrem)" : "Dose rate (mrem/s)");
    h2_OfficeArea->GetXaxis()->CenterTitle();
    h2_OfficeArea->GetYaxis()->CenterTitle();
    h2_OfficeArea->GetXaxis()->SetTitle("Cell Number");
    h2_OfficeArea->GetYaxis()->SetTitle("Cell Number");
    c_OfficeArea->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_08222025/DoseMap_OfficeArea.png");

    auto c_LaserRoom = new TCanvas("c_LaserRoom", "c_LaserRoom", 1800, 800);
    c_LaserRoom->SetMargin(0.1, 0.15, 0.15, 0.15);
    h2_LaserRoom->Draw("colz");
    h2_LaserRoom->SetTitle(scaleToFullWorkingYear ? "Dose Rate - Laser Room" : "Dose Rate Map - Laser Room");
    h2_LaserRoom->GetXaxis()->SetRangeUser(0, 40);
    h2_LaserRoom->GetZaxis()->SetTitle(scaleToFullWorkingYear ? "Yearly Dose (mrem)" : "Dose rate (mrem/s)");
    h2_LaserRoom->GetXaxis()->CenterTitle();
    h2_LaserRoom->GetYaxis()->CenterTitle();
    h2_LaserRoom->GetXaxis()->SetTitle("Cell Number");
    h2_LaserRoom->GetYaxis()->SetTitle("Cell Number");
    c_LaserRoom->SaveAs("/home/dphan/Documents/GitHub/CarlsbadRadShield/Results/NoRoof_08222025/DoseMap_LaserRoom.png");

    return 0;
}
#endif
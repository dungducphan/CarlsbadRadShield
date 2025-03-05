double numberOfSimulatedElectron = 388800000;
double numberOfElectronsIn100pC = 625000000;
double scaleTo100pC = numberOfElectronsIn100pC / numberOfSimulatedElectron;
double eVtoJoule = 1.60218e-19;
double repRate = 100; // Hz
double secondsInWorkingYear = 3600 * 8 * 250;
double massOfPhantom = 8.24; // kg
double scaleToMilliGray = 1000;

void fullFloorPlanDose() {
    auto file = TFile::Open("Dose_388800000.root");
    auto DoseMap_ParkingArea = (TH2D*)file->Get("DoseMap_ParkingArea");
    auto DoseMap_ControlRoom = (TH2D*)file->Get("DoseMap_ControlRoom");
    auto DoseMap_LunchArea   = (TH2D*)file->Get("DoseMap_LunchArea"  );

    gStyle->SetOptStat(0);

    DoseMap_ParkingArea->Scale(scaleTo100pC * eVtoJoule * repRate * secondsInWorkingYear * scaleToMilliGray / massOfPhantom);
    DoseMap_ControlRoom->Scale(scaleTo100pC * eVtoJoule * repRate * secondsInWorkingYear * scaleToMilliGray / massOfPhantom);
    DoseMap_LunchArea->Scale(scaleTo100pC * eVtoJoule * repRate * secondsInWorkingYear * scaleToMilliGray / massOfPhantom);

    // auto c1 = new TCanvas("c1", "c1", 3600, 400); 
    // c1->SetLeftMargin(0.04);
    // c1->SetRightMargin(0.08);
    // c1->SetBottomMargin(0.2);
    // c1->SetTopMargin(0.2);
    // DoseMap_ParkingArea->Draw("colz");
    // DoseMap_ParkingArea->SetTitle("Parking Area Dose Map (in mGy)");
    // DoseMap_ParkingArea->GetXaxis()->SetTitle("X [#times 20cm]");
    // DoseMap_ParkingArea->GetYaxis()->SetTitle("Y [#times 20cm]");
    // DoseMap_ParkingArea->GetXaxis()->CenterTitle();
    // DoseMap_ParkingArea->GetYaxis()->CenterTitle();
    // DoseMap_ParkingArea->GetXaxis()->SetNdivisions(505);
    // DoseMap_ParkingArea->GetYaxis()->SetNdivisions(505);
    // DoseMap_ParkingArea->GetZaxis()->SetNdivisions(505);
    // DoseMap_ParkingArea->GetZaxis()->SetRangeUser(0, 10);
    // DoseMap_ParkingArea->GetXaxis()->SetLabelSize(0.08);
    // DoseMap_ParkingArea->GetYaxis()->SetLabelSize(0.08);
    // DoseMap_ParkingArea->GetZaxis()->SetLabelSize(0.08);
    // DoseMap_ParkingArea->GetXaxis()->SetTitleSize(0.08);
    // DoseMap_ParkingArea->GetYaxis()->SetTitleSize(0.08);
    // DoseMap_ParkingArea->GetXaxis()->SetTitleOffset(0.9);
    // DoseMap_ParkingArea->GetYaxis()->SetTitleOffset(0.2);
    // c1->SaveAs("DoseMap_ParkingArea.pdf");

    // auto c2 = new TCanvas("c2", "c2", 1800, 400);
    // c2->SetLeftMargin(0.04);
    // c2->SetRightMargin(0.08);
    // c2->SetBottomMargin(0.2);
    // c2->SetTopMargin(0.2);
    // DoseMap_ControlRoom->Draw("colz");
    // DoseMap_ControlRoom->SetTitle("Control Room Dose Map (in mGy)");
    // DoseMap_ControlRoom->GetXaxis()->SetTitle("X [#times 20cm]");
    // DoseMap_ControlRoom->GetYaxis()->SetTitle("Y [#times 20cm]");
    // DoseMap_ControlRoom->GetXaxis()->CenterTitle();
    // DoseMap_ControlRoom->GetYaxis()->CenterTitle();
    // DoseMap_ControlRoom->GetXaxis()->SetNdivisions(505);
    // DoseMap_ControlRoom->GetYaxis()->SetNdivisions(505);
    // DoseMap_ControlRoom->GetZaxis()->SetNdivisions(505);
    // DoseMap_ControlRoom->GetZaxis()->SetRangeUser(0, 2);
    // DoseMap_ControlRoom->GetXaxis()->SetLabelSize(0.08);
    // DoseMap_ControlRoom->GetYaxis()->SetLabelSize(0.08);
    // DoseMap_ControlRoom->GetZaxis()->SetLabelSize(0.08);
    // DoseMap_ControlRoom->GetXaxis()->SetTitleSize(0.08);
    // DoseMap_ControlRoom->GetYaxis()->SetTitleSize(0.08);
    // DoseMap_ControlRoom->GetXaxis()->SetTitleOffset(0.9);
    // DoseMap_ControlRoom->GetYaxis()->SetTitleOffset(0.2);
    // c2->SaveAs("DoseMap_ControlRoom.pdf");

    auto c3 = new TCanvas("c3", "c3", 2000, 400);
    c3->SetLeftMargin(0.1);
    c3->SetRightMargin(0.1);
    c3->SetBottomMargin(0.2);
    c3->SetTopMargin(0.2);
    DoseMap_LunchArea->Draw("colz");
    DoseMap_LunchArea->SetTitle("Lunch Area Dose Map (in mGy)");
    DoseMap_LunchArea->GetXaxis()->SetTitle("X [#times 20cm]");
    DoseMap_LunchArea->GetYaxis()->SetTitle("Y [#times 20cm]");
    DoseMap_LunchArea->GetXaxis()->CenterTitle();
    DoseMap_LunchArea->GetXaxis()->SetRangeUser(0, 40);
    DoseMap_LunchArea->GetYaxis()->CenterTitle();
    DoseMap_LunchArea->GetXaxis()->SetNdivisions(505);
    DoseMap_LunchArea->GetYaxis()->SetNdivisions(505);
    DoseMap_LunchArea->GetZaxis()->SetNdivisions(505);
    DoseMap_LunchArea->GetZaxis()->SetRangeUser(0, 10);
    DoseMap_LunchArea->GetXaxis()->SetLabelSize(0.08);
    DoseMap_LunchArea->GetYaxis()->SetLabelSize(0.08);
    DoseMap_LunchArea->GetZaxis()->SetLabelSize(0.08);
    DoseMap_LunchArea->GetXaxis()->SetTitleSize(0.08);
    DoseMap_LunchArea->GetYaxis()->SetTitleSize(0.09);
    DoseMap_LunchArea->GetXaxis()->SetTitleOffset(1.0);
    DoseMap_LunchArea->GetYaxis()->SetTitleOffset(0.4);
    c3->SaveAs("DoseMap_LunchArea.pdf");
}
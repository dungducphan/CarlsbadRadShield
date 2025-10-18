#include "runaction.hh"

runAction::runAction() : G4UserRunAction() {
    man = G4AnalysisManager::Instance();

    // Control room
    man->CreateH2("DoseMap_ControlRoom", "Dose Map in Control Room",
                  100, 0., 100., // 100*200mmm
                  10, 0., 10.  // 10x200mm
    );

    // Parking Area
    man->CreateH2("DoseMap_ParkingArea", "Dose Map in Parking Area",
                  150, 0., 150., // 150*200mm
                  10, 0., 10.  // 10x200mm
    );

    // Lunch Area
    man->CreateH2("DoseMap_LunchArea", "Dose Map in Lunch Area",
                  100, 0., 100., // x-axis: 100 bins from 0 to 100
                  10, 0., 10.  // y-axis: 100 bins from 0 to 100
    );

    // Office 2nd Floor
    man->CreateH2("DoseMap_OfficeArea", "Dose Map in Office 2nd Floor",
                  100, 0., 100., // x-axis: 100 bins from 0 to 100
                  10, 0., 10.  // y-axis: 100 bins from 0 to 100
    );

    // Laser Room
    man->CreateH2("DoseMap_LaserRoom", "Dose Map in Laser Lab",
                  100, 0., 100., // x-axis: 100 bins from 0 to 100
                  10, 0., 10.  // y-axis: 100 bins from 0 to 100
    );
}

runAction::~runAction() = default;

void runAction::BeginOfRunAction(const G4Run *run) {
    man = G4AnalysisManager::Instance();
    man->OpenFile(Form("Dose_%03i.root", (int) run->GetRunID()));
}

void runAction::EndOfRunAction(const G4Run *) {
    man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}


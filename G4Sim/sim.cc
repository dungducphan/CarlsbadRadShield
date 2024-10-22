#include "detcon.hh"
#include "actioninit.hh"

#ifdef G4MULTITHREADED

#include "G4MTRunManager.hh"

#else
#include "G4RunManager.hh"
#endif

#include "Shielding.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4GeometrySampler.hh"
#include "G4ImportanceBiasing.hh"
#include "G4GeometryManager.hh"

#include "TH1D.h"
#include "TFile.h"

int main(int argc, char **argv) {
    G4UIExecutive *ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc, argv);

    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

#ifdef G4MULTITHREADED
    G4MTRunManager *runManager = new G4MTRunManager();
#else
    G4RunManager* runManager = new G4RunManager();
#endif

    auto detector = new detcon();
    runManager->SetUserInitialization(detector);
    G4GeometrySampler mgs(detector->GetWorldVolume(), "gamma");
    auto physics = new Shielding();
    physics->RegisterPhysics(new G4ImportanceBiasing(&mgs));
    runManager->SetUserInitialization(physics);
    runManager->SetUserInitialization(new actioninit());
    //runManager->SetNumberOfThreads(44);
    runManager->Initialize();

    detector->CreateImportanceStore();

    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (!ui) {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // open geometry for clean biasing stores clean-up
    //
    G4GeometryManager::GetInstance()->OpenGeometry();

    delete visManager;
    delete runManager;
}

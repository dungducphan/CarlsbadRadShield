#include <detcon.hh>
#include <ParticleSD.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>
#include <cstdlib>

detcon::detcon(const G4GDMLParser &parser) : G4VUserDetectorConstruction() {
    fWorldVolume = parser.GetWorldVolume();
//    logicalDetector    = parser.GetVolume("LV_Detector");
    logicalArc         = parser.GetVolume("LV_Arc");
    logicalChamber     = parser.GetVolume("LV_VacuumChamber");
    logicalFusedSilica = parser.GetVolume("LV_Window");
    logicalPbBlock1    = parser.GetVolume("LV_Pb_");
    logicalPbBlock2    = parser.GetVolume("LV_Arc");
    logicalHDPEBlock1  = parser.GetVolume("LV_Arc");
    logicalHDPEBlock2  = parser.GetVolume("LV_Arc");
    logicalWBlock      = parser.GetVolume("LV_Arc");
}

detcon::~detcon() {}

G4VPhysicalVolume *detcon::Construct() {

    std::cout << "________________________________________________________________________" << std::endl;
    std::cout << "Mass of Human Phantom: " << logic_HumanPhantom1->GetMass() / kg << " kg" << std::endl;

    return physWorld;
}

void detcon::ConstructSDandField() {
    if (logicalSD == nullptr) {
        G4cout << "Logical volume for sensitive detector is not set." << G4endl;
        return;
    }

    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    G4String SDname;

    // Create sensitive detectors
    auto particleSD = new ParticleSD("ParticleSD");
    SDman->AddNewDetector(particleSD);
    logicalSD->SetSensitiveDetector(particleSD);
}
#include <detcon.hh>
#include <nTOF_SD.hh>

#include <CADMesh.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>
#include <cstdlib>

detcon::detcon() : G4VUserDetectorConstruction() {}

detcon::~detcon() {}

G4VPhysicalVolume *detcon::Construct() {
    G4NistManager *nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    // Getting the ${STL_DIR} env variable
    auto stlPath = getenv("STL_DIR");

    // Define material for EJ-200 plastic scintillator
    G4Element* elH  = nist->FindOrBuildElement("H");
    G4Element* elC  = nist->FindOrBuildElement("C");
    G4Material* matEJ200 = new G4Material("matEJ200", 1.0221 * g/cm3, 2);
    matEJ200->AddElement(elH, 0.084838648);
    matEJ200->AddElement(elC, 0.915161352);

    // Define material for bubble detector polymer hydrogel matrix
    // define a material from elements and/or others materials (mixture of mixtures)
    G4double density = 01.200*g/cm3;
    G4int ncomponents = 2;
    G4double fractionmass = 0;
    G4Material* matGelPolymer = new G4Material("matGelPolymer", density, ncomponents);
    matGelPolymer->AddMaterial(nist->FindOrBuildMaterial("G4_POLYVINYL_ALCOHOL"), fractionmass = 20*perCent);
    matGelPolymer->AddMaterial(nist->FindOrBuildMaterial("G4_WATER"), fractionmass = 80*perCent);

    // World
    G4double worldSize = 20 * m;
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    auto mesh_Dome = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-Dome.stl", stlPath));
    G4VSolid *solid_Dome = mesh_Dome->GetSolid();
    logic_Dome = new G4LogicalVolume(solid_Dome, nist->FindOrBuildMaterial("G4_CONCRETE"), "logic_Dome");
    G4VPhysicalVolume *phys_Dome = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_Dome, "phys_Dome", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes* va_Dome = new G4VisAttributes();
    va_Dome->SetVisibility();
    va_Dome->SetForceSolid();
    va_Dome->SetColor(0, 1, 1, 0.5);
    logic_Dome->SetVisAttributes(va_Dome);

    auto mesh_DoorF = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-Door-F.stl", stlPath));
    G4VSolid *solid_DoorF = mesh_DoorF->GetSolid();
    logic_DoorF = new G4LogicalVolume(solid_DoorF, nist->FindOrBuildMaterial("G4_CONCRETE"), "logic_DoorF");
    G4VPhysicalVolume *phys_DoorF = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_DoorF, "phys_DoorF", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes* va_DoorF = new G4VisAttributes();
    va_DoorF->SetVisibility();
    va_DoorF->SetForceSolid();
    va_DoorF->SetColor(0, 1, 1, 0.5);
    logic_DoorF->SetVisAttributes(va_DoorF);

    auto mesh_DoorB = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-Door-B.stl", stlPath));
    G4VSolid *solid_DoorB = mesh_DoorB->GetSolid();
    logic_DoorB = new G4LogicalVolume(solid_DoorB, nist->FindOrBuildMaterial("G4_CONCRETE"), "logic_DoorB");
    G4VPhysicalVolume *phys_DoorB = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_DoorB, "phys_DoorB", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes* va_DoorB = new G4VisAttributes();
    va_DoorB->SetVisibility();
    va_DoorB->SetForceSolid();
    va_DoorB->SetColor(0, 1, 1, 0.5);
    logic_DoorB->SetVisAttributes(va_DoorB);

    return physWorld;
}

void detcon::ConstructSDandField() {
    nTOF_SD *aTOFSD = new nTOF_SD("neutronTOFSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(aTOFSD);
    for (auto &elem: logic_nTOFs) SetSensitiveDetector(elem, aTOFSD);
    for (auto &elem: logic_nBDs) SetSensitiveDetector(elem, aTOFSD);

    // FIXME: When I run a test, I make the radShield volume to become the nTOF sensitive detector to get a lot of neutrons quickly. There was std::bad_aloc error when running the test simulation
    //  with a lot of primary neutron (n = 1000). That problem does not show up when the number of neutron hits recorded is small. So I will look over this problem right now, but keep this in mind
    //  if seeing this bug in the future.

}
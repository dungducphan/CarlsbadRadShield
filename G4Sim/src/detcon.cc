#include <detcon.hh>
#include <ParticleSD.hh>

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
    // auto stlPath = getenv("STL_DIR");
    auto stlPath = ".";

    // Define High-Density Concrete
    auto HDConcrete = new G4Material("HDConcrete", 3.7 * g / cm3, 10);
    HDConcrete->AddElement(nist->FindOrBuildElement("H"),  0.01);
    HDConcrete->AddElement(nist->FindOrBuildElement("C"),  0.001);
    HDConcrete->AddElement(nist->FindOrBuildElement("O"),  0.529107);
    HDConcrete->AddElement(nist->FindOrBuildElement("Na"), 0.016);
    HDConcrete->AddElement(nist->FindOrBuildElement("Mg"), 0.002);
    HDConcrete->AddElement(nist->FindOrBuildElement("Al"), 0.033872);
    HDConcrete->AddElement(nist->FindOrBuildElement("Si"), 0.337021);
    HDConcrete->AddElement(nist->FindOrBuildElement("K"),  0.013);
    HDConcrete->AddElement(nist->FindOrBuildElement("Ca"), 0.044);
    HDConcrete->AddElement(nist->FindOrBuildElement("Fe"), 0.014);

    // World
    G4double worldSize_X = 15 * m;
    G4double worldSize_Y = 15 * m;
    G4double worldSize_Z = 5 * m;
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * worldSize_X, 0.5 * worldSize_Y, 0.5 * worldSize_Z);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    auto mesh_VacuumChamber = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-VacuumChamber.stl", stlPath));
    G4VSolid *solid_VacuumChamber = mesh_VacuumChamber->GetSolid();
    logic_VacuumChamber = new G4LogicalVolume(solid_VacuumChamber, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logic_VacuumChamber");
    G4VPhysicalVolume *phys_VacuumChamber = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_VacuumChamber, "phys_VacuumChamber", logicWorld, false, 0, checkOverlaps);
    auto va_VacuumChamber = new G4VisAttributes();
    va_VacuumChamber->SetVisibility();
    va_VacuumChamber->SetForceSolid();
    va_VacuumChamber->SetColor(1, 0, 0, 0.5);
    logic_VacuumChamber->SetVisAttributes(va_VacuumChamber);

    auto mesh_HDConcrete3in = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HDC3in.stl", stlPath));
    G4VSolid *solid_HDConcrete3in = mesh_HDConcrete3in->GetSolid();
    logic_HDConcrete3in = new G4LogicalVolume(solid_HDConcrete3in, HDConcrete, "logic_HDConcrete3in");
    G4VPhysicalVolume *phys_HDConcrete3in = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_HDConcrete3in, "phys_HDConcrete3in", logicWorld, false, 0, checkOverlaps);
    auto va_HDConcrete3in = new G4VisAttributes();
    va_HDConcrete3in->SetVisibility();
    va_HDConcrete3in->SetForceSolid();
    va_HDConcrete3in->SetColor(0, 1, 1, 0.5);
    logic_HDConcrete3in->SetVisAttributes(va_HDConcrete3in);

    auto mesh_HDConcrete12in = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HDC12in.stl", stlPath));
    G4VSolid *solid_HDConcrete12in = mesh_HDConcrete12in->GetSolid();
    logic_HDConcrete12in = new G4LogicalVolume(solid_HDConcrete12in, HDConcrete, "logic_HDConcrete12in");
    G4VPhysicalVolume *phys_HDConcrete12in = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_HDConcrete12in, "phys_HDConcrete12in", logicWorld, false, 0, checkOverlaps);
    auto va_HDConcrete12in = new G4VisAttributes();
    va_HDConcrete12in->SetVisibility();
    va_HDConcrete12in->SetForceSolid();
    va_HDConcrete12in->SetColor(0, 1, 1, 0.5);
    logic_HDConcrete12in->SetVisAttributes(va_HDConcrete12in);

    auto mesh_Fe1in = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-Fe1in.stl", stlPath));
    G4VSolid *solid_Fe1in = mesh_Fe1in->GetSolid();
    logic_Fe1in = new G4LogicalVolume(solid_Fe1in, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logic_Fe1in");
    G4VPhysicalVolume *phys_Fe1in = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_Fe1in, "phys_Fe1in", logicWorld, false, 0, checkOverlaps);
    auto va_Fe1in = new G4VisAttributes();
    va_Fe1in->SetVisibility();
    va_Fe1in->SetForceSolid();
    va_Fe1in->SetColor(0, 1, 1, 0.5);
    logic_Fe1in->SetVisAttributes(va_Fe1in);

     auto mesh_HumanPhantom1 = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HP1.stl", stlPath));
    G4VSolid *solid_HumanPhantom1 = mesh_HumanPhantom1->GetSolid();
    logic_HumanPhantom1 = new G4LogicalVolume(solid_HumanPhantom1, nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"), "logic_HumanPhantom1");
    G4VPhysicalVolume *phys_HumanPhantom1 = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_HumanPhantom1, "phys_HumanPhantom1", logicWorld, false, 0, checkOverlaps);
    auto va_HumanPhantom1 = new G4VisAttributes();
    va_HumanPhantom1->SetVisibility();
    va_HumanPhantom1->SetForceSolid();
    va_HumanPhantom1->SetColor(0, 1, 0, 0.5);
    logic_HumanPhantom1->SetVisAttributes(va_HumanPhantom1);

    auto mesh_HumanPhantom2 = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HP2.stl", stlPath));
    G4VSolid *solid_HumanPhantom2 = mesh_HumanPhantom2->GetSolid();
    logic_HumanPhantom2 = new G4LogicalVolume(solid_HumanPhantom2, nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"), "logic_HumanPhantom2");
    G4VPhysicalVolume *phys_HumanPhantom2 = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_HumanPhantom2, "phys_HumanPhantom2", logicWorld, false, 0, checkOverlaps);
    auto va_HumanPhantom2 = new G4VisAttributes();
    va_HumanPhantom2->SetVisibility();
    va_HumanPhantom2->SetForceSolid();
    va_HumanPhantom2->SetColor(0, 1, 0, 0.5);
    logic_HumanPhantom2->SetVisAttributes(va_HumanPhantom2);

    std::cout << "________________________________________________________________________" << std::endl;
    std::cout << "Mass of Human Phantom: " << logic_HumanPhantom1->GetMass() / kg << " kg" << std::endl;

    return physWorld;
}

void detcon::ConstructSDandField() {
    auto *aDoseSD = new ParticleSD("RadiationDoseSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(aDoseSD);
    SetSensitiveDetector(logic_HumanPhantom1, aDoseSD);
    SetSensitiveDetector(logic_HumanPhantom2, aDoseSD);
    SetSensitiveDetector(logic_HumanPhantom3, aDoseSD);
    SetSensitiveDetector(logic_HumanPhantom4, aDoseSD);
}
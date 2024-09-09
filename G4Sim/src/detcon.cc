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

    auto mesh_HDConcrete = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HDConcrete.stl", stlPath));
    G4VSolid *solid_HDConcrete = mesh_HDConcrete->GetSolid();
    logic_HDConcrete = new G4LogicalVolume(solid_HDConcrete, HDConcrete, "logic_HDConcrete");
    G4VPhysicalVolume *phys_HDConcrete = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_HDConcrete, "phys_HDConcrete", logicWorld, false, 0, checkOverlaps);
    auto va_HDConcrete = new G4VisAttributes();
    va_HDConcrete->SetVisibility();
    va_HDConcrete->SetForceSolid();
    va_HDConcrete->SetColor(0, 1, 1, 0.5);
    logic_HDConcrete->SetVisAttributes(va_HDConcrete);

    auto mesh_VacuumChamber = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-VacuumChamber.stl", stlPath));
    G4VSolid *solid_VacuumChamber = mesh_VacuumChamber->GetSolid();
    logic_VacuumChamber = new G4LogicalVolume(solid_VacuumChamber, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logic_VacuumChamber");
    G4VPhysicalVolume *phys_VacuumChamber = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_VacuumChamber, "phys_VacuumChamber", logicWorld, false, 0, checkOverlaps);
    auto va_VacuumChamber = new G4VisAttributes();
    va_VacuumChamber->SetVisibility();
    va_VacuumChamber->SetForceSolid();
    va_VacuumChamber->SetColor(1, 0, 0, 0.5);
    logic_VacuumChamber->SetVisAttributes(va_VacuumChamber);

    auto mesh_Ground = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-Ground.stl", stlPath));
    G4VSolid *solid_Ground = mesh_Ground->GetSolid();
    logic_Ground = new G4LogicalVolume(solid_Ground, nist->FindOrBuildMaterial("G4_CONCRETE"), "logic_Ground");
    G4VPhysicalVolume *phys_Ground = new G4PVPlacement(0, G4ThreeVector(7000, 7000, 1650), logic_Ground, "phys_Ground", logicWorld, false, 0, checkOverlaps);
    auto va_Ground = new G4VisAttributes();
    va_Ground->SetVisibility();
    va_Ground->SetForceSolid();
    va_Ground->SetColor(0, 0, 1, 0.5);
    logic_Ground->SetVisAttributes(va_Ground);

    auto mesh_SteelShield = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-IronShield.stl", stlPath));
    G4VSolid *solid_SteelShield = mesh_SteelShield->GetSolid();
    logic_SteelShield = new G4LogicalVolume(solid_SteelShield, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logic_SteelShield");
    G4VPhysicalVolume *phys_SteelShield = new G4PVPlacement(0, G4ThreeVector(0, 0, 1450 * mm), logic_SteelShield, "phys_SteelShield", logicWorld, false, 0, checkOverlaps);
    auto va_SteelShield = new G4VisAttributes();
    va_SteelShield->SetVisibility();
    va_SteelShield->SetForceSolid();
    va_SteelShield->SetColor(1, 0, 1, 0.5);
    logic_SteelShield->SetVisAttributes(va_SteelShield);

    auto mesh_HumanPhantom1 = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HP-1.stl", stlPath));
    G4VSolid *solid_HumanPhantom1 = mesh_HumanPhantom1->GetSolid();
    logic_HumanPhantom1 = new G4LogicalVolume(solid_HumanPhantom1, nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"), "logic_HumanPhantom1");
    G4VPhysicalVolume *phys_HumanPhantom1 = new G4PVPlacement(0, G4ThreeVector(0, 2000 * mm, 0), logic_HumanPhantom1, "phys_HumanPhantom1", logicWorld, false, 0, checkOverlaps);
    auto va_HumanPhantom1 = new G4VisAttributes();
    va_HumanPhantom1->SetVisibility();
    va_HumanPhantom1->SetForceSolid();
    va_HumanPhantom1->SetColor(0, 1, 0, 0.5);
    logic_HumanPhantom1->SetVisAttributes(va_HumanPhantom1);

    auto mesh_HumanPhantom2 = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HP-2.stl", stlPath));
    G4VSolid *solid_HumanPhantom2 = mesh_HumanPhantom2->GetSolid();
    logic_HumanPhantom2 = new G4LogicalVolume(solid_HumanPhantom2, nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"), "logic_HumanPhantom2");
    G4VPhysicalVolume *phys_HumanPhantom2 = new G4PVPlacement(0, G4ThreeVector(-1000 * mm, 2000 * mm, 0), logic_HumanPhantom2, "phys_HumanPhantom2", logicWorld, false, 0, checkOverlaps);
    auto va_HumanPhantom2 = new G4VisAttributes();
    va_HumanPhantom2->SetVisibility();
    va_HumanPhantom2->SetForceSolid();
    va_HumanPhantom2->SetColor(0, 1, 0, 0.5);
    logic_HumanPhantom2->SetVisAttributes(va_HumanPhantom2);

    auto mesh_HumanPhantom3 = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HP-3.stl", stlPath));
    G4VSolid *solid_HumanPhantom3 = mesh_HumanPhantom3->GetSolid();
    logic_HumanPhantom3 = new G4LogicalVolume(solid_HumanPhantom3, nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"), "logic_HumanPhantom3");
    G4VPhysicalVolume *phys_HumanPhantom3 = new G4PVPlacement(0, G4ThreeVector(-3000 * mm, 2000 * mm, 0), logic_HumanPhantom3, "phys_HumanPhantom3", logicWorld, false, 0, checkOverlaps);
    auto va_HumanPhantom3 = new G4VisAttributes();
    va_HumanPhantom3->SetVisibility();
    va_HumanPhantom3->SetForceSolid();
    va_HumanPhantom3->SetColor(0, 1, 0, 0.5);
    logic_HumanPhantom3->SetVisAttributes(va_HumanPhantom3);

    auto mesh_HumanPhantom4 = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HP-4.stl", stlPath));
    G4VSolid *solid_HumanPhantom4 = mesh_HumanPhantom4->GetSolid();
    logic_HumanPhantom4 = new G4LogicalVolume(solid_HumanPhantom4, nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"), "logic_HumanPhantom4");
    auto HP4_Rot = new G4RotationMatrix();
    HP4_Rot->rotateX(0*deg);
    HP4_Rot->rotateY(0*deg);
    HP4_Rot->rotateZ(90*deg);
    G4VPhysicalVolume *phys_HumanPhantom4 = new G4PVPlacement(HP4_Rot, G4ThreeVector(10300, 5300, 0), logic_HumanPhantom4, "phys_HumanPhantom4", logicWorld, false, 0, checkOverlaps);
    auto va_HumanPhantom4 = new G4VisAttributes();
    va_HumanPhantom4->SetVisibility();
    va_HumanPhantom4->SetForceSolid();
    va_HumanPhantom4->SetColor(0, 1, 0, 0.5);
    logic_HumanPhantom4->SetVisAttributes(va_HumanPhantom4);

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
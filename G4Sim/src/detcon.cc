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
    G4double worldSize_X = 5 * m;
    G4double worldSize_Y = 5 * m;
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
    va_VacuumChamber->SetColor(1, 0, 0, 0.7);
    logic_VacuumChamber->SetVisAttributes(va_VacuumChamber);

    auto mesh_HDConcrete18in = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HDC18.stl", stlPath));
    G4VSolid *solid_HDConcrete18in = mesh_HDConcrete18in->GetSolid();
    logic_HDConcrete18in = new G4LogicalVolume(solid_HDConcrete18in, HDConcrete, "logic_HDConcrete18in");
    G4VPhysicalVolume *phys_HDConcrete18in = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_HDConcrete18in, "phys_HDConcrete18in", logicWorld, false, 0, checkOverlaps);
    auto va_HDConcrete18in = new G4VisAttributes();
    va_HDConcrete18in->SetVisibility();
    va_HDConcrete18in->SetForceSolid();
    va_HDConcrete18in->SetColor(0, 1, 1, 0.3);
    logic_HDConcrete18in->SetVisAttributes(va_HDConcrete18in);


    auto elemBox = new G4Box("elemBox", 5 * cm, 5 * cm, 5 * cm);
    auto logicBox = new G4LogicalVolume(elemBox, nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"), "logicBox");
    // Build a grid of 31x31 wall of boxes
    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 31; j++) {
            new G4PVPlacement(0, G4ThreeVector((i - 15) * 10. * cm, (j - 15) * 10. * cm, 2 * m), Form("HPWall_1_%02i_%02i", i, j),
                logicBox, physWorld, false, 0, checkOverlaps);
        }
    }

    return physWorld;
}

void detcon::ConstructSDandField() {
    auto *aDoseSD = new ParticleSD("RadiationDoseSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(aDoseSD);
}
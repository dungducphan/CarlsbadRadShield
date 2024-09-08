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
    G4double worldSize = 11 * m;
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    // Radiation Shield
    auto mesh_radShield = CADMesh::TessellatedMesh::FromSTL(Form("%s/radShield.stl", stlPath));
    G4VSolid *solid_radShield = mesh_radShield->GetSolid();
    logic_radShield = new G4LogicalVolume(solid_radShield, nist->FindOrBuildMaterial("G4_CONCRETE"), "logic_radShield");
    G4VPhysicalVolume *phys_radShield = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_radShield, "phys_radShield", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes* va_radShield = new G4VisAttributes();
    va_radShield->SetVisibility();
    va_radShield->SetForceSolid();
    va_radShield->SetColor(0, 1, 1, 0.5);
    logic_radShield->SetVisAttributes(va_radShield);

    // Target Chamber
    auto mesh_chamber = CADMesh::TessellatedMesh::FromSTL(Form("%s/chamberAndBreadboard.stl", stlPath));
    G4VSolid *solid_chamber = mesh_chamber->GetSolid();
    G4LogicalVolume *logic_chamber = new G4LogicalVolume(solid_chamber, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logic_chamber");
    G4VPhysicalVolume *phys_chamber = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_chamber, "phys_chamber", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes* va_chamber = new G4VisAttributes();
    va_chamber->SetVisibility();
    va_chamber->SetForceSolid();
    va_chamber->SetColor(1, 0, 0, 0.5);
    logic_chamber->SetVisAttributes(va_chamber);

    // TOFs
    std::vector<G4VSolid *> solid_nTOFs;
    for (unsigned int i = 0; i < 3; i++) {
        auto mesh_nTOF = CADMesh::TessellatedMesh::FromSTL(Form("%s/nTOF_%i.stl", stlPath, i));
        solid_nTOFs.push_back(mesh_nTOF->GetSolid());
        logic_nTOFs.push_back(new G4LogicalVolume(solid_nTOFs.at(i), matEJ200, Form("logic_nTOF_%i", i)));
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_nTOFs.at(i), Form("phys_nTOF_%i", i), logicWorld, false, i,
                          checkOverlaps);
    }
    G4VisAttributes* va_nTOF = new G4VisAttributes();
    va_nTOF->SetVisibility();
    va_nTOF->SetForceSolid();
    va_nTOF->SetColor(0, 1, 0, 0.5);
    for (auto& elem : logic_nTOFs) elem->SetVisAttributes(va_nTOF);
    G4cout << "Mass of TOF: " << logic_nTOFs.at(0)->GetMass() / kilogram << " kg." << G4endl;
    // FIXME: my understand of the GetSolids() function in this case is wrong, only one physical volume is
    //  created even though the CAD model might contains more than one, disconnected solid volumes.

    G4cout << solid_nTOFs.size() << G4endl;
    // FIXME: display detector ID next to the model in visualization

    // Bubble Detectors
    std::vector<G4VSolid*> solid_nBDs;
    for (unsigned int i = 0; i < 10; i++) {
        auto mesh_nBD = CADMesh::TessellatedMesh::FromSTL(Form("%s/nBD_0_%i.stl", stlPath, i));
        solid_nBDs.push_back(mesh_nBD->GetSolid());
        logic_nBDs.push_back(new G4LogicalVolume(solid_nBDs.at(i), matGelPolymer, Form("logic_nBD_%i", i)));
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_nBDs.at(i), Form("phys_nBD_%i", i), logicWorld, false,
                          100+i, checkOverlaps);
    }
    G4VisAttributes* va_nBD = new G4VisAttributes();
    va_nBD->SetVisibility();
    va_nBD->SetForceSolid();
    va_nBD->SetColor(1, 0, 1, 0.5);
    for (auto& elem : logic_nBDs) elem->SetVisAttributes(va_nBD);
    G4cout << "Mass of BD: " << logic_nBDs.at(0)->GetMass() / kilogram << " kg." << G4endl;
    // FIXME: display detector ID next to the model in visualization

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
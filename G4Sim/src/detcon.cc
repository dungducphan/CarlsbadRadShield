#include <detcon.hh>
#include <ParticleSD.hh>

#include <G4Colour.hh>
#include <G4IStore.hh>
#include <G4VisAttributes.hh>

#include <TString.h>
#include <cstdlib>

detcon::detcon() : G4VUserDetectorConstruction() {}

detcon::~detcon() = default;

G4VSolid* detcon::GenerateShell(const double& innerWidth, const double& outerWidth, const G4String& name) {
    if (innerWidth == 0) {
        return new G4Box(name, 0.5 * outerWidth, 0.5 * outerWidth, 0.5 * outerWidth);
    }
    const auto innerSolid = new G4Box(Form("innerSolid_%s", name.c_str()), 0.5 * innerWidth, 0.5 * innerWidth, 0.5 * innerWidth);
    const auto outerSolid = new G4Box(Form("outerSolid_%s", name.c_str()), 0.5 * outerWidth, 0.5 * outerWidth, 0.5 * outerWidth);
    return new G4SubtractionSolid(name, outerSolid, innerSolid);
}

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
    auto *solidWorld = new G4Box("solidWorld", 0.5 * worldSize_X, 0.5 * worldSize_Y, 0.5 * worldSize_Z);
    auto *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlaps);
    vec_logical_volumes.push_back(logicWorld);
    vec_physical_volumes.push_back(physWorld);

    // auto mesh_VacuumChamber = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-VacuumChamber.stl", stlPath));
    // G4VSolid *solid_VacuumChamber = mesh_VacuumChamber->GetSolid();
    // logic_VacuumChamber = new G4LogicalVolume(solid_VacuumChamber, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logic_VacuumChamber");
    // G4VPhysicalVolume *phys_VacuumChamber = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_VacuumChamber, "phys_VacuumChamber", logicWorld, false, 0, checkOverlaps);
    // auto va_VacuumChamber = new G4VisAttributes();
    // va_VacuumChamber->SetVisibility();
    // va_VacuumChamber->SetForceSolid();
    // va_VacuumChamber->SetColor(1, 0, 0, 0.7);
    // logic_VacuumChamber->SetVisAttributes(va_VacuumChamber);
    //
    // auto mesh_HDConcrete18in = CADMesh::TessellatedMesh::FromSTL(Form("%s/CarlsbadRadShield-HDC18.stl", stlPath));
    // G4VSolid *solid_HDConcrete18in = mesh_HDConcrete18in->GetSolid();
    // logic_HDConcrete18in = new G4LogicalVolume(solid_HDConcrete18in, HDConcrete, "logic_HDConcrete18in");
    // G4VPhysicalVolume *phys_HDConcrete18in = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic_HDConcrete18in, "phys_HDConcrete18in", logicWorld, false, 0, checkOverlaps);
    // auto va_HDConcrete18in = new G4VisAttributes();
    // va_HDConcrete18in->SetVisibility();
    // va_HDConcrete18in->SetForceSolid();
    // va_HDConcrete18in->SetColor(0, 1, 1, 0.3);
    // logic_HDConcrete18in->SetVisAttributes(va_HDConcrete18in);

    // Dimensions
    NumberOfShieldingLayers = 8;
    double vacuumChamberSize = 0.5 * m;
    double distanceToShieldingWall = 1 * m;
    double innerVacuumWidth = vacuumChamberSize - 2.54 * cm;
    double thicknessOfShieldingLayer = 2.54 * cm;
    double innerShieldingWidth = innerVacuumWidth + 2 * distanceToShieldingWall;

    // // Vacuum
    // auto solidVacuum = G4Box("solidVacuum", 0.5 * innerVacuumWidth, 0.5 * innerVacuumWidth, 0.5 * innerVacuumWidth);
    // auto logicVacuum = new G4LogicalVolume(&solidVacuum, nist->FindOrBuildMaterial("G4_Galactic"), "logicVacuum");
    // auto physVacuum = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicVacuum, "physVacuum", logicWorld, false, 0, checkOverlaps);
    // vec_logical_volumes.push_back(logicVacuum);
    // vec_physical_volumes.push_back(physVacuum);

    // Vacuum Chamber
    auto solidVacuumChamber = GenerateShell(innerVacuumWidth, vacuumChamberSize, "solidVacuumChamber");
    auto logicVacuumChamber = new G4LogicalVolume(solidVacuumChamber, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logicVacuumChamber");
    auto physVacuumChamber = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicVacuumChamber, "physVacuumChamber", logicWorld, false, 0, checkOverlaps);
    auto vaVacuumChamber = new G4VisAttributes();
    vaVacuumChamber->SetVisibility();
    vaVacuumChamber->SetForceSolid();
    vaVacuumChamber->SetColor(1, 0, 0, 0.7);
    logicVacuumChamber->SetVisAttributes(vaVacuumChamber);
    vec_logical_volumes.push_back(logicVacuumChamber);
    vec_physical_volumes.push_back(physVacuumChamber);

    // Air
    auto solidAir = GenerateShell(vacuumChamberSize, innerShieldingWidth, "solidAir");
    auto logicAir = new G4LogicalVolume(solidAir, nist->FindOrBuildMaterial("G4_AIR"), "logicAir");
    auto physAir = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicAir, "physAir", logicWorld, false, 0, checkOverlaps);
    vec_logical_volumes.push_back(logicAir);
    vec_physical_volumes.push_back(physAir);
    auto vaAir = new G4VisAttributes();
    vaAir->SetVisibility();
    vaAir->SetForceSolid();
    vaAir->SetColor(0, 1, 0, 0.1);
    logicAir->SetVisAttributes(vaAir);

    // Shielding Layers
    for (unsigned int i = 0; i < NumberOfShieldingLayers; i++) {
        auto solidShieldingLayer = GenerateShell(innerShieldingWidth + i * thicknessOfShieldingLayer, innerShieldingWidth + (i + 1) * thicknessOfShieldingLayer, Form("solidShieldingLayer_%02i", i + 1));
        auto logicShieldingLayer = new G4LogicalVolume(solidShieldingLayer, HDConcrete, Form("logicShieldingLayer_%02i", i + 1));
        auto physShieldingLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicShieldingLayer, Form("physShieldingLayer_%02i", i + 1), logicWorld, false, 0, checkOverlaps);
        vec_logical_volumes.push_back(logicShieldingLayer);
        vec_physical_volumes.push_back(physShieldingLayer);
        auto vaShieldingLayer = new G4VisAttributes();
        vaShieldingLayer->SetVisibility();
        vaShieldingLayer->SetForceSolid();
        vaShieldingLayer->SetColor(0, 0, 1, 0.05);
        logicShieldingLayer->SetVisAttributes(vaShieldingLayer);
    }

    // Rest
    auto solidRest = GenerateShell(innerShieldingWidth + NumberOfShieldingLayers * thicknessOfShieldingLayer, worldSize_X, "solidRest");
    auto logicRest = new G4LogicalVolume(solidRest, nist->FindOrBuildMaterial("G4_AIR"), "logicRest");
    auto physRest = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicRest, "physRest", logicWorld, false, 0, checkOverlaps);
    vec_logical_volumes.push_back(logicRest);
    vec_physical_volumes.push_back(physRest);
    auto vaRest = new G4VisAttributes();
    vaRest->SetVisibility();
    vaRest->SetForceSolid();
    vaRest->SetColor(1, 1, 1, 0.1);
    logicRest->SetVisAttributes(vaRest);

    // Element Phantoms
    auto elem_Phantom = new G4Box("elem_Phantom", 5 * cm, 5 * cm, 5 * cm);
    logic_Phantom = new G4LogicalVolume(elem_Phantom, nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"), "logic_Phantom");
    // Build a grid of 31x31 wall of boxes
    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 31; j++) {
            auto solidPhantom = new G4PVPlacement(0, G4ThreeVector((i - 15) * 10. * cm, (j - 15) * 10. * cm, 2 * m), Form("Phantom_%02i_%02i", i, j),
                logic_Phantom, physRest, false, vec_physical_volumes_phantom.size(), checkOverlaps);
            vec_physical_volumes_phantom.push_back(solidPhantom);
        }
    }

    std::cout << "Mass of Element Phantom: " << logic_Phantom->GetMass() / kg << " kg" << std::endl;

    return physWorld;
}

void detcon::ConstructSDandField() {
    auto *aDoseSD = new ParticleSD("RadiationDoseSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(aDoseSD);
    SetSensitiveDetector(logic_Phantom, aDoseSD);
}

G4VPhysicalVolume* detcon::GetWorldVolume() {
    return fWorldVolume;
}

G4VIStore* detcon::CreateImportanceStore() {
    G4cout << " detcon:: Creating Importance Store " << G4endl;
    if (!vec_physical_volumes.size()) {
        G4Exception("detcon::CreateImportanceStore"
                   ,"Carlsbad Radiation Shielding",RunMustBeAborted
                   ,"no physical volumes created yet!");
    }

    fWorldVolume = vec_physical_volumes[0];

    // creating and filling the importance store
    G4IStore *istore = G4IStore::GetInstance();
    istore->AddImportanceGeometryCell(1, *vec_physical_volumes[0]); // Vacuum World
    G4cout << "Going to assign importance: " << 1 << ", to volume: " << (vec_physical_volumes[0])->GetName() << G4endl;
    istore->AddImportanceGeometryCell(2, *vec_physical_volumes[1]); // Vacuum Chamber
    G4cout << "Going to assign importance: " << 2 << ", to volume: " << (vec_physical_volumes[1])->GetName() << G4endl;
    istore->AddImportanceGeometryCell(2, *vec_physical_volumes[2]); // Air
    G4cout << "Going to assign importance: " << 2 << ", to volume: " << (vec_physical_volumes[2])->GetName() << G4endl;
    G4double imp = 1;
    for (int i = 0; i < NumberOfShieldingLayers; i++) {
        imp = TMath::Power(2., 2 + i);
        istore->AddImportanceGeometryCell(imp, *vec_physical_volumes[3 + i]); // Shielding Layers
        G4cout << "Going to assign importance: " << imp << ", to volume: " << (vec_physical_volumes[3 + i])->GetName() << G4endl;
    }
    imp = TMath::Power(2., 1 + NumberOfShieldingLayers);
    istore->AddImportanceGeometryCell(imp, *vec_physical_volumes[3 + NumberOfShieldingLayers]); // Air Rest
    G4cout << "Going to assign importance: " << imp << ", to volume: " << (vec_physical_volumes[3 + NumberOfShieldingLayers])->GetName() << G4endl;

    for (int j = 0; j < vec_physical_volumes_phantom.size(); j++) {
        istore->AddImportanceGeometryCell(imp, *vec_physical_volumes_phantom[j], j); // Element Phantoms
        G4cout << "Going to assign importance: " << imp << ", to volume: " << vec_physical_volumes_phantom[j]->GetName() << G4endl;
    }
    return istore;
}

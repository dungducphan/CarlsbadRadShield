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
    G4double worldSize_X = 7 * m;
    G4double worldSize_Y = 7 * m;
    G4double worldSize_Z = 7 * m;
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    auto *solidWorld = new G4Box("solidWorld", 0.5 * worldSize_X, 0.5 * worldSize_Y, 0.5 * worldSize_Z);
    auto *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlaps);
    vec_physical_volumes.clear();
    vec_physical_volumes.push_back(physWorld);

    // Dimensions
    double vacuumChamberSize = 0.5 * m;
    double innerVacuumWidth = vacuumChamberSize - 2.54 * cm;

    // Vacuum Chamber
    auto solidVacuumChamber = GenerateShell(innerVacuumWidth, vacuumChamberSize, "solidVacuumChamber");
    auto logicVacuumChamber = new G4LogicalVolume(solidVacuumChamber, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logicVacuumChamber");
    auto physVacuumChamber = new G4PVPlacement(0, G4ThreeVector(0, 0, -150 * cm), logicVacuumChamber, "physVacuumChamber", logicWorld, false, 0, checkOverlaps);
    auto vaVacuumChamber = new G4VisAttributes();
    vaVacuumChamber->SetVisibility();
    vaVacuumChamber->SetForceSolid();
    vaVacuumChamber->SetColor(1, 0, 0, 0.7);
    logicVacuumChamber->SetVisAttributes(vaVacuumChamber);
    vec_physical_volumes.push_back(physVacuumChamber);

    auto solid_nippleTube = new G4Tubs("solid_nippleTube", (2-0.1) * 2.54 * cm, 2 * 2.54 * cm, 0.5 * m, 0, 2 * M_PI);
    auto logic_nippleTube = new G4LogicalVolume(solid_nippleTube, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logic_nippleTube");
    logic_nippleTube->SetVisAttributes(vaVacuumChamber);
    auto phys_nippleTube = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.75 * m - 150 * cm), logic_nippleTube, "phys_nippleTube", logicWorld, false, 0, false);
    vec_physical_volumes.push_back(phys_nippleTube);

    // Sandwich of steel and HDC
    double slabXY = worldSize_X;
    double slabZ = 4.75 * 2.54 * cm;
    double ss_slabZ = 0.25 * 2.54 * cm;
    double hdc_slabZ = 4.5 * 2.54 * cm;
    G4Box* solidSlab = new G4Box("slab", slabXY/2, slabXY/2, slabZ/2);
    G4Box* solidSSSlab = new G4Box("ss_slab", slabXY/2, slabXY/2, ss_slabZ/2);
    G4Box* solidHDCSlab = new G4Box("hdc_slab", slabXY/2, slabXY/2, hdc_slabZ/2);
    G4LogicalVolume* logicSlab = new G4LogicalVolume(solidSlab, worldMat, "logicSlab");
    G4LogicalVolume* logicSSSlab = new G4LogicalVolume(solidSSSlab, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logicSSSlab");
    G4LogicalVolume* logicHDCSlab = new G4LogicalVolume(solidHDCSlab, HDConcrete, "logicHDCSlab");
    auto vaSSSlab = new G4VisAttributes();
    vaSSSlab->SetVisibility();
    vaSSSlab->SetForceSolid();
    vaSSSlab->SetColor(0, 0, 1, 0.5);
    logicSSSlab->SetVisAttributes(vaSSSlab);
    auto vaHDCSlab = new G4VisAttributes();
    vaHDCSlab->SetVisibility();
    vaHDCSlab->SetForceSolid();
    vaHDCSlab->SetColor(1, 0, 0, 0.5);
    logicHDCSlab->SetVisAttributes(vaHDCSlab);

    auto firstHDCLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, -hdc_slabZ/2), logicHDCSlab, "physHDCSlab_00", logicWorld, false, 0);
    vec_physical_volumes.push_back(firstHDCLayer);
    // create consecutive layers of slabs
    for (int i = 0; i < 8; i++) {
        auto physSandwiches = new G4PVPlacement(0, G4ThreeVector(0, 0, slabZ/2 + i * slabZ), logicSlab, Form("physSlab_%02i", i+1), logicWorld, false, 0);
        auto phys_ssslab = new G4PVPlacement(0, G4ThreeVector(0, 0, -slabZ/2 + ss_slabZ/2), logicSSSlab, Form("physSSSlab_%02i", i+1), physSandwiches->GetLogicalVolume(), false, 0);
        auto phys_hdc_slab = new G4PVPlacement(0, G4ThreeVector(0, 0, slabZ/2 - hdc_slabZ/2), logicHDCSlab, Form("physHDCSlab_%02i", i+1), physSandwiches->GetLogicalVolume(), false, 0);
        vec_physical_volumes.push_back(phys_ssslab);
        vec_physical_volumes.push_back(phys_hdc_slab);
    }

    // Rest
    double rest_Z = worldSize_Z / 2 - 8 * 4.75 * 2.54 * cm; // Half of worldZ and 8 layers of SS-HDC sandwiches.
    double rest_ZY = worldSize_X;
    auto solidRest = new G4Box("solidRest", rest_ZY / 2, rest_ZY / 2, rest_Z / 2);
    auto logicRest = new G4LogicalVolume(solidRest, nist->FindOrBuildMaterial("G4_AIR"), "logicRest");
    auto physRest = new G4PVPlacement(0, G4ThreeVector(0, 0, worldSize_Z/2 - rest_Z/2), logicRest, "physRest", logicWorld, false, 0, checkOverlaps);
    vec_physical_volumes.push_back(physRest);
    auto vaRest = new G4VisAttributes();
    vaRest->SetVisibility(false);
    vaRest->SetForceSolid();
    vaRest->SetColor(1, 1, 1, 0.8);
    logicRest->SetVisAttributes(vaRest);

    // Element Phantoms
    auto elem_Phantom = new G4Box("elem_Phantom", 5 * cm, 5 * cm, 5 * cm);
    logic_Phantom = new G4LogicalVolume(elem_Phantom, nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"), "logic_Phantom");
    // Build a grid of 31x31 wall of boxes
    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 31; j++) {
            auto solidPhantom = new G4PVPlacement(0, G4ThreeVector((i - 15) * 10. * cm, (j - 15) * 10. * cm, -rest_Z/2 + 30*cm), Form("Phantom_%02i_%02i", i, j),
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
    istore->AddImportanceGeometryCell(1, *vec_physical_volumes[1]); // Vacuum Chamber
    G4cout << "Going to assign importance: " << 1 << ", to volume: " << (vec_physical_volumes[1])->GetName() << G4endl;
    istore->AddImportanceGeometryCell(1, *vec_physical_volumes[2]); // Vacuum pipe
    G4cout << "Going to assign importance: " << 1 << ", to volume: " << (vec_physical_volumes[2])->GetName() << G4endl;

    double baseImportance = 4;
    istore->AddImportanceGeometryCell(baseImportance, *vec_physical_volumes[3]); // First HDC layer
    G4cout << "Going to assign importance: " << baseImportance << ", to volume: " << (vec_physical_volumes[3])->GetName() << G4endl;
    for (int i = 0; i < 8; i++) {
        double imp = baseImportance * TMath::Power(2, i + 1);
        istore->AddImportanceGeometryCell(imp, *vec_physical_volumes[4 + 2 * i + 0]); // Sandwich Layer
        istore->AddImportanceGeometryCell(imp, *vec_physical_volumes[4 + 2 * i + 1]); // Sandwich Layer
        G4cout << "Going to assign importance: " << imp << ", to volume: " << (vec_physical_volumes[4 + 2 * i + 0])->GetName() << G4endl;
        G4cout << "Going to assign importance: " << imp << ", to volume: " << (vec_physical_volumes[4 + 2 * i + 1])->GetName() << G4endl;
    }
    double lastImportance = baseImportance * TMath::Power(2, 8);
    istore->AddImportanceGeometryCell(lastImportance, *vec_physical_volumes[20]); // Rest of the world
    G4cout << "Going to assign importance: " << lastImportance << ", to volume: " << (vec_physical_volumes[20])->GetName() << G4endl;

    for (int j = 0; j < vec_physical_volumes_phantom.size(); j++) {
        istore->AddImportanceGeometryCell(lastImportance, *vec_physical_volumes_phantom[j], j); // Element Phantoms
        G4cout << "Going to assign importance: " << lastImportance << ", to volume: " << vec_physical_volumes_phantom[j]->GetName() << G4endl;
    }

    return istore;
}

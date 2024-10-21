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
    G4double worldSize_X = 5 * m;
    G4double worldSize_Y = 5 * m;
    G4double worldSize_Z = 5 * m;
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    auto *solidWorld = new G4Box("solidWorld", 0.5 * worldSize_X, 0.5 * worldSize_Y, 0.5 * worldSize_Z);
    auto *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlaps);
    vec_logical_volumes.push_back(logicWorld);
    vec_physical_volumes.push_back(physWorld);

    // Dimensions
    NumberOf3inHDCShieldingLayersIn9inHDC = 3;
    double vacuumChamberSize = 0.5 * m;
    double innerVacuumWidth = vacuumChamberSize - 2.54 * cm;
    double distance_FromChamber_To3inHDCShielding = 1 * m;
    double innerShieldingWidth_3inHDC = innerVacuumWidth + 2 * distance_FromChamber_To3inHDCShielding;
    double outerShieldingWidth_3inHDC = innerShieldingWidth_3inHDC + 3 * 2.54 * cm;
    double innerShieldingWidth_2inSS = outerShieldingWidth_3inHDC;
    double outerShieldingWidth_2inSS = innerShieldingWidth_2inSS + 2 * 2.54 * cm;
    double innerShieldingWidth_9inHDC = outerShieldingWidth_2inSS;
    double thicknessOfShieldingLayer_3inHDC = 3 * 2.54 * cm;
    double outerShieldingWidth_9inHDC = innerShieldingWidth_9inHDC + NumberOf3inHDCShieldingLayersIn9inHDC * thicknessOfShieldingLayer_3inHDC;

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

    // Air between Chamber and first layer of HDC
    auto solidAir_01 = GenerateShell(vacuumChamberSize, innerShieldingWidth_3inHDC, "solidAir_01");
    auto logicAir_01 = new G4LogicalVolume(solidAir_01, nist->FindOrBuildMaterial("G4_AIR"), "logicAir_01");
    auto physAir_01 = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicAir_01, "physAir_01", logicWorld, false, 0, checkOverlaps);
    vec_logical_volumes.push_back(logicAir_01);
    vec_physical_volumes.push_back(physAir_01);
    auto vaAir = new G4VisAttributes();
    vaAir->SetVisibility();
    vaAir->SetForceSolid();
    vaAir->SetColor(0, 1, 0, 0.1);
    logicAir_01->SetVisAttributes(vaAir);

    // 3inHDC Shielding Layer
    auto solid3inHDCShieldingLayer = GenerateShell(innerShieldingWidth_3inHDC, outerShieldingWidth_3inHDC, "solid3inHDCShieldingLayer");
    auto logic3inHDCShieldingLayer = new G4LogicalVolume(solid3inHDCShieldingLayer, HDConcrete, "logic3inHDCShieldingLayer");
    auto phys3inHDCShieldingLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic3inHDCShieldingLayer, "phys3inHDCShieldingLayer", logicWorld, false, 0, checkOverlaps);
    vec_logical_volumes.push_back(logic3inHDCShieldingLayer);
    vec_physical_volumes.push_back(phys3inHDCShieldingLayer);
    auto vaHDCShieldingLayer = new G4VisAttributes();
    vaHDCShieldingLayer->SetVisibility();
    vaHDCShieldingLayer->SetForceSolid();
    vaHDCShieldingLayer->SetColor(0, 0, 1, 0.1);
    logic3inHDCShieldingLayer->SetVisAttributes(vaHDCShieldingLayer);

    // Stainless steel Shielding Layers
    auto solid2inSSShieldingLayer = GenerateShell(innerShieldingWidth_2inSS, outerShieldingWidth_2inSS, "solid2inSSShieldingLayer");
    auto logic2inSSShieldingLayer = new G4LogicalVolume(solid2inSSShieldingLayer, nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"), "logic2inSSShieldingLayer");
    auto phys2inSSShieldingLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logic2inSSShieldingLayer, "phys2inSSShieldingLayer", logicWorld, false, 0, checkOverlaps);
    vec_logical_volumes.push_back(logic2inSSShieldingLayer);
    vec_physical_volumes.push_back(phys2inSSShieldingLayer);
    auto vaSSShieldingLayer = new G4VisAttributes();
    vaSSShieldingLayer->SetVisibility();
    vaSSShieldingLayer->SetForceSolid();
    vaSSShieldingLayer->SetColor(0, 1, 1, 0.5);
    logic2inSSShieldingLayer->SetVisAttributes(vaSSShieldingLayer);

    // 9inHDC in 3 layers of 3inHDC
    for (unsigned int i = 0; i < NumberOf3inHDCShieldingLayersIn9inHDC; i++) {
        auto solidShieldingLayer_dummy = GenerateShell(innerShieldingWidth_9inHDC + i * thicknessOfShieldingLayer_3inHDC, innerShieldingWidth_9inHDC + (i + 1) * thicknessOfShieldingLayer_3inHDC, Form("solid3inHDCShieldingSublayer_%02i", i));
        auto logicShieldingLayer_dummy = new G4LogicalVolume(solidShieldingLayer_dummy, HDConcrete, Form("logic3inHDCShieldingSublayer_%02i", i));
        auto physShieldingLayer_dummy = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicShieldingLayer_dummy, Form("phys3inHDCShieldingSublayer_%02i", i), logicWorld, false, 0, checkOverlaps);
        vec_logical_volumes.push_back(logicShieldingLayer_dummy);
        vec_physical_volumes.push_back(physShieldingLayer_dummy);
        logicShieldingLayer_dummy->SetVisAttributes(vaHDCShieldingLayer);
    }

    // Rest
    auto solidRest = GenerateShell(outerShieldingWidth_9inHDC, worldSize_X, "solidRest");
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
    istore->AddImportanceGeometryCell(4, *vec_physical_volumes[3]); // First 3inHDC layer
    G4cout << "Going to assign importance: " << 4 << ", to volume: " << (vec_physical_volumes[3])->GetName() << G4endl;
    istore->AddImportanceGeometryCell(8, *vec_physical_volumes[4]); // 2inSS layer
    G4cout << "Going to assign importance: " << 8 << ", to volume: " << (vec_physical_volumes[4])->GetName() << G4endl;
    istore->AddImportanceGeometryCell(16, *vec_physical_volumes[5]); // Second HDC layer - first 3inHDC sublayer
    G4cout << "Going to assign importance: " << 16 << ", to volume: " << (vec_physical_volumes[5])->GetName() << G4endl;
    istore->AddImportanceGeometryCell(32, *vec_physical_volumes[6]); // Second HDC layer - second 3inHDC sublayer
    G4cout << "Going to assign importance: " << 32 << ", to volume: " << (vec_physical_volumes[6])->GetName() << G4endl;
    istore->AddImportanceGeometryCell(64, *vec_physical_volumes[7]); // Second HDC layer - third 3inHDC sublayer
    G4cout << "Going to assign importance: " << 64 << ", to volume: " << (vec_physical_volumes[7])->GetName() << G4endl;
    istore->AddImportanceGeometryCell(64, *vec_physical_volumes[8]); // Rest of the world
    G4cout << "Going to assign importance: " << 64 << ", to volume: " << (vec_physical_volumes[8])->GetName() << G4endl;

    for (int j = 0; j < vec_physical_volumes_phantom.size(); j++) {
        istore->AddImportanceGeometryCell(64, *vec_physical_volumes_phantom[j], j); // Element Phantoms
        G4cout << "Going to assign importance: " << 64 << ", to volume: " << vec_physical_volumes_phantom[j]->GetName() << G4endl;
    }

    // Testing
    // G4IStore *istore = G4IStore::GetInstance();
    // istore->AddImportanceGeometryCell(1,  *vec_physical_volumes[0]); // Vacuum World
    // istore->AddImportanceGeometryCell(1,  *vec_physical_volumes[1]); // Vacuum Chamber
    // istore->AddImportanceGeometryCell(1,  *vec_physical_volumes[2]); // Air
    // istore->AddImportanceGeometryCell(1,  *vec_physical_volumes[3]); // First 3inHDC layer
    // istore->AddImportanceGeometryCell(1,  *vec_physical_volumes[4]); // 2inSS layer
    // istore->AddImportanceGeometryCell(1, *vec_physical_volumes[5]); // Second HDC layer - first 3inHDC sublayer
    // istore->AddImportanceGeometryCell(1, *vec_physical_volumes[6]); // Second HDC layer - second 3inHDC sublayer
    // istore->AddImportanceGeometryCell(1, *vec_physical_volumes[7]); // Second HDC layer - third 3inHDC sublayer
    // istore->AddImportanceGeometryCell(1, *vec_physical_volumes[8]); // Second HDC layer - third 3inHDC sublayer
    //
    // for (int j = 0; j < vec_physical_volumes_phantom.size(); j++) {
    //     istore->AddImportanceGeometryCell(1, *vec_physical_volumes_phantom[j], j); // Element Phantoms
    // }

    return istore;
}

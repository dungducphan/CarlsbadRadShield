#pragma once

#include <utility>
#include <tuple>
#include <fstream>

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4VIStore.hh"

#include "TMath.h"

class G4VPhysicalVolume;

class G4LogicalVolume;

class detcon : public G4VUserDetectorConstruction {
public:
    detcon();

    ~detcon() override;

    G4VSolid* GenerateShell(const double& innerWidth, const double& outerWidth, const G4String& name);

    G4VPhysicalVolume *Construct() override;
    void ConstructSDandField() override;
    G4VPhysicalVolume* GetWorldVolume();
    G4VIStore* CreateImportanceStore();

    std::vector<G4LogicalVolume*> vec_logical_volumes;
    G4LogicalVolume* logic_Phantom;
    std::vector<G4VPhysicalVolume*> vec_physical_volumes;
    std::vector<G4VPhysicalVolume*> vec_physical_volumes_phantom;

private:
    G4VPhysicalVolume* fWorldVolume;
    G4int NumberOf3inHDCShieldingLayersIn15inHDC;
};
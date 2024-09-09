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

#include "TMath.h"

class G4VPhysicalVolume;

class G4LogicalVolume;

class detcon : public G4VUserDetectorConstruction {
public:
    detcon();

    virtual ~detcon();

    virtual G4VPhysicalVolume *Construct();
    virtual void ConstructSDandField();

    std::vector<G4LogicalVolume*> logic_nTOFs;
    std::vector<G4LogicalVolume*> logic_nBDs;
    G4LogicalVolume *logic_HDConcrete;
    G4LogicalVolume *logic_VacuumChamber;
    G4LogicalVolume *logic_Ground;
    G4LogicalVolume *logic_HumanPhantom1;
    G4LogicalVolume *logic_HumanPhantom2;
    G4LogicalVolume *logic_HumanPhantom3;
    G4LogicalVolume *logic_HumanPhantom4;
    G4LogicalVolume *logic_SteelShield;
};
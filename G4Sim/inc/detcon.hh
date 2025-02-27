#pragma once

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
#include "G4GDMLParser.hh"

#include "TMath.h"

#include "ParticleSD.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class detcon : public G4VUserDetectorConstruction {
public:
    detcon(const G4GDMLParser& parser);
    ~detcon() override;
    G4VPhysicalVolume *Construct() override;
    void ConstructSDandField() override;
    G4VPhysicalVolume* GetWorldVolume() { return fWorldVolume; }

private:
    void SetVisualAttributes();
    void SetMaterials();

    G4Material*        mat_Air                          = nullptr;
    G4Material*        mat_Concrete                     = nullptr;
    G4Material*        mat_Glass                        = nullptr;
    G4Material*        mat_Lead                         = nullptr;
    G4Material*        mat_HDPE                         = nullptr;
    G4Material*        mat_Tungsten                     = nullptr;
    G4Material*        mat_Vacuum                       = nullptr;
    G4Material*        mat_StainlessSteel               = nullptr;
    G4Material*        mat_HumanTissue                  = nullptr;

    G4VPhysicalVolume* fWorldVolume                     = nullptr;

    G4LogicalVolume*   logical_Phantom_ControlRoom      = nullptr;
    G4LogicalVolume*   logical_Phantom_LunchArea        = nullptr;
    G4LogicalVolume*   logical_Phantom_ParkingArea      = nullptr;
    G4LogicalVolume*   logical_Phantom_Office_1F        = nullptr;
    G4LogicalVolume*   logical_Phantom_Office_2F        = nullptr;
    G4LogicalVolume*   logical_Phantom_OpenLab          = nullptr;
    G4LogicalVolume*   logical_FirstFloor               = nullptr;
    G4LogicalVolume*   logical_SecondFloor              = nullptr;
    G4LogicalVolume*   logical_OuterWalls               = nullptr;
    G4LogicalVolume*   logical_InnerWalls               = nullptr;
    G4LogicalVolume*   logical_GlassWindow_01           = nullptr;
    G4LogicalVolume*   logical_GlassWindow_02           = nullptr;
    G4LogicalVolume*   logical_GlassWindow_03           = nullptr;
    G4LogicalVolume*   logical_GlassWindow_04           = nullptr;
    G4LogicalVolume*   logical_GlassWindow_05           = nullptr;
    G4LogicalVolume*   logical_GlassWindow_06           = nullptr;
    G4LogicalVolume*   logical_ArcVault                 = nullptr;
    G4LogicalVolume*   logical_VacuumChamber            = nullptr;
    G4LogicalVolume*   logical_VacuumWindow             = nullptr;
    G4LogicalVolume*   logical_BeamDump_LeadBlock       = nullptr;
    G4LogicalVolume*   logical_BeamDump_HDPEBlock_Inner = nullptr;
    G4LogicalVolume*   logical_BeamDump_HDPEBlock_Outer = nullptr;
    G4LogicalVolume*   logical_BeamDump_TungstenBlock   = nullptr;

    G4VisAttributes*   visAttr_ArcVault                 = nullptr;
    G4VisAttributes*   visAttr_Floor                    = nullptr;
    G4VisAttributes*   visAttr_Wall                     = nullptr;
    G4VisAttributes*   visAttr_GlassWindow              = nullptr;
    G4VisAttributes*   visAttr_VacuumChamber            = nullptr;
    G4VisAttributes*   visAttr_VacuumWindow             = nullptr;
    G4VisAttributes*   visAttr_BeamDump_LeadBlock       = nullptr;
    G4VisAttributes*   visAttr_BeamDump_HDPEBlock       = nullptr;
    G4VisAttributes*   visAttr_BeamDump_TungstenBlock   = nullptr;
    G4VisAttributes*   visAttr_Phantom                  = nullptr;
};
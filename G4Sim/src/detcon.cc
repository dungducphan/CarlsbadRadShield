#include <detcon.hh>
#include <ParticleSD.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>

detcon::detcon(const G4GDMLParser &parser) : G4VUserDetectorConstruction() {
    fWorldVolume = parser.GetWorldVolume();

    logical_Phantom_ControlRoom      = parser.GetVolume( "V-ControlRoom-2");
    logical_Phantom_LunchArea        = parser.GetVolume( "V-LunchArea-3");
    logical_Phantom_ParkingArea      = parser.GetVolume( "V-ParkingArea-4");
    logical_Phantom_Office_1F        = parser.GetVolume( "V-Office_1F-5");
    logical_Phantom_Office_2F        = parser.GetVolume( "V-Office_2F-6");
    logical_Phantom_OpenLab          = parser.GetVolume( "V-OpenLab-7");
    logical_FirstFloor               = parser.GetVolume( "V-FirstFloor-12");
    logical_SecondFloor              = parser.GetVolume( "V-SecondFloor-13");
    logical_OuterWalls               = parser.GetVolume( "V-FirstFloorWalls-14");
    logical_InnerWalls               = parser.GetVolume( "V-SecondFloorWalls-15");
    logical_GlassWindow_01           = parser.GetVolume( "V-Glass001-17");
    logical_GlassWindow_02           = parser.GetVolume( "V-Glass002-18");
    logical_GlassWindow_03           = parser.GetVolume( "V-Glass003-19");
    logical_GlassWindow_04           = parser.GetVolume( "V-Glass004-20");
    logical_GlassWindow_05           = parser.GetVolume( "V-Glass005-21");
    logical_GlassWindow_06           = parser.GetVolume( "V-Glass006-22");
    logical_ArcVault                 = parser.GetVolume( "V-Arc-24");
    logical_VacuumChamber            = parser.GetVolume( "V-VacuumChamber-9");
    logical_VacuumWindow             = parser.GetVolume( "V-Window-10");
    logical_BeamDump_LeadBlock       = parser.GetVolume( "V-Pb_Block-28");
    logical_BeamDump_HDPEBlock_Inner = parser.GetVolume( "V-HDPE_Block_02-27");
    logical_BeamDump_HDPEBlock_Outer = parser.GetVolume( "V-HDPE_Block_01-29");
    logical_BeamDump_TungstenBlock   = parser.GetVolume( "V-W_Plate-26");

    SetVisualAttributes();
    SetMaterials();
}

detcon::~detcon() {
    delete logical_Phantom_ControlRoom;
    delete logical_Phantom_LunchArea;
    delete logical_Phantom_ParkingArea;
    delete logical_Phantom_Office_1F;
    delete logical_Phantom_Office_2F;
    delete logical_Phantom_OpenLab;
    delete logical_FirstFloor;
    delete logical_SecondFloor;
    delete logical_OuterWalls;
    delete logical_InnerWalls;
    delete logical_GlassWindow_01;
    delete logical_GlassWindow_02;
    delete logical_GlassWindow_03;
    delete logical_GlassWindow_04;
    delete logical_GlassWindow_05;
    delete logical_GlassWindow_06;
    delete logical_ArcVault;
    delete logical_VacuumChamber;
    delete logical_VacuumWindow;
    delete logical_BeamDump_LeadBlock;
    delete logical_BeamDump_HDPEBlock_Inner;
    delete logical_BeamDump_HDPEBlock_Outer;
    delete logical_BeamDump_TungstenBlock;

    delete visAttr_Phantom;
    delete visAttr_Floor;
    delete visAttr_Wall;
    delete visAttr_GlassWindow;
    delete visAttr_ArcVault;
    delete visAttr_VacuumChamber;
    delete visAttr_VacuumWindow;
    delete visAttr_BeamDump_LeadBlock;
    delete visAttr_BeamDump_HDPEBlock;
    delete visAttr_BeamDump_TungstenBlock;

    delete fWorldVolume;
}

void detcon::SetMaterials() {
    const auto nist = G4NistManager::Instance();

    mat_Air            = nist->FindOrBuildMaterial( "G4_AIR");
    mat_Concrete       = nist->FindOrBuildMaterial( "G4_CONCRETE");
    mat_Lead           = nist->FindOrBuildMaterial( "G4_Pb");
    mat_HDPE           = nist->FindOrBuildMaterial( "G4_POLYETHYLENE");
    mat_Tungsten       = nist->FindOrBuildMaterial( "G4_W");
    mat_StainlessSteel = nist->FindOrBuildMaterial( "G4_STAINLESS-STEEL");
    mat_Glass          = nist->FindOrBuildMaterial( "G4_SILICON_DIOXIDE");
    mat_Vacuum         = nist->FindOrBuildMaterial( "G4_Galactic");
    mat_HumanTissue    = nist->FindOrBuildMaterial( "G4_TISSUE_SOFT_ICRP");

    fWorldVolume->GetLogicalVolume()->SetMaterial(mat_Vacuum);

    logical_Phantom_ControlRoom      ->SetMaterial(mat_Vacuum);
    logical_Phantom_LunchArea        ->SetMaterial(mat_Vacuum);
    logical_Phantom_ParkingArea      ->SetMaterial(mat_Vacuum);
    logical_Phantom_Office_1F        ->SetMaterial(mat_Vacuum);
    logical_Phantom_Office_2F        ->SetMaterial(mat_Vacuum);
    logical_Phantom_OpenLab          ->SetMaterial(mat_Vacuum);
    logical_FirstFloor               ->SetMaterial(mat_Vacuum);
    logical_SecondFloor              ->SetMaterial(mat_Vacuum);
    logical_OuterWalls               ->SetMaterial(mat_Vacuum);
    logical_InnerWalls               ->SetMaterial(mat_Vacuum);
    logical_GlassWindow_01           ->SetMaterial(mat_Vacuum);
    logical_GlassWindow_02           ->SetMaterial(mat_Vacuum);
    logical_GlassWindow_03           ->SetMaterial(mat_Vacuum);
    logical_GlassWindow_04           ->SetMaterial(mat_Vacuum);
    logical_GlassWindow_05           ->SetMaterial(mat_Vacuum);
    logical_GlassWindow_06           ->SetMaterial(mat_Vacuum);
    logical_ArcVault                 ->SetMaterial(mat_Vacuum);
    logical_VacuumChamber            ->SetMaterial(mat_Vacuum);
    logical_VacuumWindow             ->SetMaterial(mat_Vacuum);
    logical_BeamDump_LeadBlock       ->SetMaterial(mat_Vacuum);
    logical_BeamDump_HDPEBlock_Inner ->SetMaterial(mat_Vacuum);
    logical_BeamDump_HDPEBlock_Outer ->SetMaterial(mat_Vacuum);
    logical_BeamDump_TungstenBlock   ->SetMaterial(mat_Vacuum);

    // logical_Phantom_ControlRoom      ->SetMaterial(mat_HumanTissue);
    // logical_Phantom_LunchArea        ->SetMaterial(mat_HumanTissue);
    // logical_Phantom_ParkingArea      ->SetMaterial(mat_HumanTissue);
    // logical_Phantom_Office_1F        ->SetMaterial(mat_HumanTissue);
    // logical_Phantom_Office_2F        ->SetMaterial(mat_HumanTissue);
    // logical_Phantom_OpenLab          ->SetMaterial(mat_HumanTissue);
    // logical_FirstFloor               ->SetMaterial(mat_Concrete);
    // logical_SecondFloor              ->SetMaterial(mat_Concrete);
    // logical_OuterWalls               ->SetMaterial(mat_Air);
    // logical_InnerWalls               ->SetMaterial(mat_Air);
    // logical_GlassWindow_01           ->SetMaterial(mat_Glass);
    // logical_GlassWindow_02           ->SetMaterial(mat_Glass);
    // logical_GlassWindow_03           ->SetMaterial(mat_Glass);
    // logical_GlassWindow_04           ->SetMaterial(mat_Glass);
    // logical_GlassWindow_05           ->SetMaterial(mat_Glass);
    // logical_GlassWindow_06           ->SetMaterial(mat_Glass);
    // logical_ArcVault                 ->SetMaterial(mat_Concrete);
    // logical_VacuumChamber            ->SetMaterial(mat_StainlessSteel);
    // logical_VacuumWindow             ->SetMaterial(mat_Glass);
    // logical_BeamDump_LeadBlock       ->SetMaterial(mat_Lead);
    // logical_BeamDump_HDPEBlock_Inner ->SetMaterial(mat_HDPE);
    // logical_BeamDump_HDPEBlock_Outer ->SetMaterial(mat_HDPE);
    // logical_BeamDump_TungstenBlock   ->SetMaterial(mat_Tungsten);
}


void detcon::SetVisualAttributes() {
    visAttr_Phantom = new G4VisAttributes();
    visAttr_Phantom->SetVisibility(true);
    visAttr_Phantom->SetForceSolid(true);
    visAttr_Phantom->SetColour(1, 1, 1, 0.3);
    logical_Phantom_ControlRoom->SetVisAttributes(visAttr_Phantom);
    logical_Phantom_LunchArea->SetVisAttributes(visAttr_Phantom);
    logical_Phantom_ParkingArea->SetVisAttributes(visAttr_Phantom);
    logical_Phantom_Office_1F->SetVisAttributes(visAttr_Phantom);
    logical_Phantom_Office_2F->SetVisAttributes(visAttr_Phantom);
    logical_Phantom_OpenLab->SetVisAttributes(visAttr_Phantom);

    visAttr_Floor = new G4VisAttributes();
    visAttr_Floor->SetVisibility(true);
    visAttr_Floor->SetForceSolid(true);
    visAttr_Floor->SetColour(0.2, 0.8, 0.1, 0.7);
    logical_FirstFloor->SetVisAttributes(visAttr_Floor);
    logical_SecondFloor->SetVisAttributes(visAttr_Floor);

    visAttr_Wall = new G4VisAttributes();
    visAttr_Wall->SetVisibility(true);
    visAttr_Wall->SetForceSolid(true);
    visAttr_Wall->SetColour(0.2, 0.1, 0.8, 0.7);
    logical_OuterWalls->SetVisAttributes(visAttr_Wall);
    logical_InnerWalls->SetVisAttributes(visAttr_Wall);

    visAttr_GlassWindow = new G4VisAttributes();
    visAttr_GlassWindow->SetVisibility(true);
    visAttr_GlassWindow->SetForceSolid(true);
    visAttr_GlassWindow->SetColour(0.8, 0.1, 0.2, 0.1);
    logical_GlassWindow_01->SetVisAttributes(visAttr_GlassWindow);
    logical_GlassWindow_02->SetVisAttributes(visAttr_GlassWindow);
    logical_GlassWindow_03->SetVisAttributes(visAttr_GlassWindow);
    logical_GlassWindow_04->SetVisAttributes(visAttr_GlassWindow);
    logical_GlassWindow_05->SetVisAttributes(visAttr_GlassWindow);
    logical_GlassWindow_06->SetVisAttributes(visAttr_GlassWindow);

    visAttr_ArcVault = new G4VisAttributes();
    visAttr_ArcVault->SetVisibility(true);
    visAttr_ArcVault->SetForceSolid(true);
    visAttr_ArcVault->SetColour(0.8, 0.8, 0.8, 0.2);
    logical_ArcVault->SetVisAttributes(visAttr_ArcVault);

    visAttr_VacuumChamber = new G4VisAttributes();
    visAttr_VacuumChamber->SetVisibility(true);
    visAttr_VacuumChamber->SetForceSolid(true);
    visAttr_VacuumChamber->SetColour(0.0, 0.8, 0.0, 0.6);
    logical_VacuumChamber->SetVisAttributes(visAttr_VacuumChamber);

    visAttr_VacuumWindow = new G4VisAttributes();
    visAttr_VacuumWindow->SetVisibility(true);
    visAttr_VacuumWindow->SetForceSolid(true);
    visAttr_VacuumWindow->SetColour(0.8, 0.1, 0.2, 0.1);
    logical_VacuumWindow->SetVisAttributes(visAttr_VacuumWindow);

    visAttr_BeamDump_LeadBlock = new G4VisAttributes();
    visAttr_BeamDump_LeadBlock->SetVisibility(true);
    visAttr_BeamDump_LeadBlock->SetForceSolid(true);
    visAttr_BeamDump_LeadBlock->SetColour(0.1, 0.1, 0.1, 0.6);
    logical_BeamDump_LeadBlock->SetVisAttributes(visAttr_BeamDump_LeadBlock);

    visAttr_BeamDump_HDPEBlock = new G4VisAttributes();
    visAttr_BeamDump_HDPEBlock->SetVisibility(true);
    visAttr_BeamDump_HDPEBlock->SetForceSolid(true);
    visAttr_BeamDump_HDPEBlock->SetColour(0., 0., 0., 0.9);
    logical_BeamDump_HDPEBlock_Inner->SetVisAttributes(visAttr_BeamDump_HDPEBlock);
    logical_BeamDump_HDPEBlock_Outer->SetVisAttributes(visAttr_BeamDump_HDPEBlock);

    visAttr_BeamDump_TungstenBlock = new G4VisAttributes();
    visAttr_BeamDump_TungstenBlock->SetVisibility(true);
    visAttr_BeamDump_TungstenBlock->SetForceSolid(true);
    visAttr_BeamDump_TungstenBlock->SetColour(1., 0., 0., 0.9);
    logical_BeamDump_TungstenBlock->SetVisAttributes(visAttr_BeamDump_TungstenBlock);
}

G4VPhysicalVolume *detcon::Construct() {

    std::cout << "________________________________________________________________________" << std::endl;
    std::cout << "Mass of Arc " << logical_ArcVault->GetMass() / kg << " kg" << std::endl;
    std::cout << "Mass of Beam Dump: " << logical_BeamDump_TungstenBlock->GetMass() / kg + logical_BeamDump_HDPEBlock_Outer->GetMass() / kg + logical_BeamDump_HDPEBlock_Inner->GetMass() / kg + logical_BeamDump_LeadBlock->GetMass() / kg << " kg" << std::endl;
    std::cout << "________________________________________________________________________" << std::endl;

    return fWorldVolume;
}

void detcon::ConstructSDandField() {
}
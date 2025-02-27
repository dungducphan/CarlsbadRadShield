#include <detcon.hh>
#include <ParticleSD.hh>

#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <TString.h>

void detcon::BuildPhantomRegion(const G4double Lx,      const G4double Ly,      const G4double Lz,
                                const G4double x_start, const G4double y_start, const G4double z_start,
                                const std::string& areaName) const {
    for (int i_x = 0; i_x < Lx / ScoringBoxSize; i_x++) {
        const double x = ScoringBoxSize * i_x + x_start + ScoringBoxSize / 2. + GDML_X_OFFSET;
        for (int i_y = 0; i_y < Ly / ScoringBoxSize; i_y++) {
            const double y = ScoringBoxSize * i_y + y_start + ScoringBoxSize / 2. + GDML_Y_OFFSET;
            for (int i_z = 0; i_z < Lz / ScoringBoxSize; i_z++) {
                const double z = ScoringBoxSize * i_z + z_start + ScoringBoxSize / 2. + GDML_Z_OFFSET;
                const G4String name = Form("pv_%s_%02i_%02i_%02i", areaName, i_x, i_y, i_z);
                new G4PVPlacement(nullptr, G4ThreeVector(x, y, z), logical_PhantomBox, name, fWorldVolume->GetLogicalVolume(), false, 0);
            }
        }
    }
}

detcon::detcon(const G4GDMLParser &parser) : G4VUserDetectorConstruction() {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Materials */
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Get volumes from GDML */
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fWorldVolume                     = parser.GetWorldVolume();
    logical_FirstFloor               = parser.GetVolume( "V-FirstFloor-9");
    logical_SecondFloor              = parser.GetVolume( "V-SecondFloor-10");
    logical_OuterWalls               = parser.GetVolume( "V-FirstFloorWalls-11");
    logical_InnerWalls               = parser.GetVolume( "V-SecondFloorWalls-12");
    logical_GlassWindow_01           = parser.GetVolume( "V-Glass001-14");
    logical_GlassWindow_02           = parser.GetVolume( "V-Glass002-15");
    logical_GlassWindow_03           = parser.GetVolume( "V-Glass003-16");
    logical_GlassWindow_04           = parser.GetVolume( "V-Glass004-17");
    logical_GlassWindow_05           = parser.GetVolume( "V-Glass005-18");
    logical_GlassWindow_06           = parser.GetVolume( "V-Glass006-19");
    logical_ArcVault                 = parser.GetVolume( "V-Arc-7");
    logical_VacuumChamber            = parser.GetVolume( "V-VacuumChamber-21");
    logical_VacuumWindow             = parser.GetVolume( "V-Window-22");
    logical_BeamDump_LeadBlock       = parser.GetVolume( "V-Pb_Block-4");
    logical_BeamDump_HDPEBlock_Inner = parser.GetVolume( "V-HDPE_Block_02-3");
    logical_BeamDump_HDPEBlock_Outer = parser.GetVolume( "V-HDPE_Block_01-5");
    logical_BeamDump_TungstenBlock   = parser.GetVolume( "V-W_Plate-2");

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Sensitive/Dosimetry Region */
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    auto SolidScoringBox = new G4Box("SolidScoringBox", ScoringBoxSize / 2., ScoringBoxSize / 2., ScoringBoxSize / 2.);
    logical_PhantomBox = new G4LogicalVolume(SolidScoringBox, mat_HumanTissue, "PhantomBox");
    G4double Lz = 2000 * mm;

    /* Lunch Area */
    G4double Lx = 8000 * mm;
    G4double Ly = 200 * mm;
    G4double x_start = 5000 * mm;
    G4double y_start = 29000 * mm;
    G4double z_start = 0 * mm;
    std::string areaName = "LunchArea";
    BuildPhantomRegion(Lx, Ly, Lz, x_start, y_start, z_start, areaName);

    /* Parking Area */
    Lx = 200 * mm;
    Ly = 30000 * mm;
    x_start = 15000 * mm;
    y_start = 0 * mm;
    z_start = 0 * mm;
    areaName = "ParkingArea";
    BuildPhantomRegion(Lx, Ly, Lz, x_start, y_start, z_start, areaName);

    /* Control Room */
    Lx = 200 * mm;
    Ly = 15000 * mm;
    x_start = -10000 * mm;
    y_start = -4000 * mm;
    z_start = 4200 * mm;
    areaName = "ControlRoom";
    BuildPhantomRegion(Lx, Ly, Lz, x_start, y_start, z_start, areaName);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Visual Props */
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetVisualAttributes();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Assigns Materials */
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fWorldVolume->GetLogicalVolume() ->SetMaterial(mat_Air);
    logical_FirstFloor               ->SetMaterial(mat_Concrete);
    logical_SecondFloor              ->SetMaterial(mat_Concrete);
    logical_OuterWalls               ->SetMaterial(mat_Air);
    logical_InnerWalls               ->SetMaterial(mat_Air);
    logical_GlassWindow_01           ->SetMaterial(mat_Glass);
    logical_GlassWindow_02           ->SetMaterial(mat_Glass);
    logical_GlassWindow_03           ->SetMaterial(mat_Glass);
    logical_GlassWindow_04           ->SetMaterial(mat_Glass);
    logical_GlassWindow_05           ->SetMaterial(mat_Glass);
    logical_GlassWindow_06           ->SetMaterial(mat_Glass);
    logical_ArcVault                 ->SetMaterial(mat_Concrete);
    logical_VacuumChamber            ->SetMaterial(mat_StainlessSteel);
    logical_VacuumWindow             ->SetMaterial(mat_Glass);
    logical_BeamDump_LeadBlock       ->SetMaterial(mat_Lead);
    logical_BeamDump_HDPEBlock_Inner ->SetMaterial(mat_HDPE);
    logical_BeamDump_HDPEBlock_Outer ->SetMaterial(mat_HDPE);
    logical_BeamDump_TungstenBlock   ->SetMaterial(mat_Tungsten);
}

detcon::~detcon() {
    delete SolidScoringBox;
    delete logical_PhantomBox;
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

void detcon::SetVisualAttributes() {
    visAttr_Phantom = new G4VisAttributes();
    visAttr_Phantom->SetVisibility(true);
    visAttr_Phantom->SetForceSolid(true);
    visAttr_Phantom->SetColour(1, 0, 0, 0.9);
    logical_PhantomBox->SetVisAttributes(visAttr_Phantom);

    visAttr_Floor = new G4VisAttributes();
    visAttr_Floor->SetVisibility(true);
    visAttr_Floor->SetForceSolid(true);
    visAttr_Floor->SetColour(0.2, 0.8, 0.1, 0.2);
    logical_FirstFloor->SetVisAttributes(visAttr_Floor);
    logical_SecondFloor->SetVisAttributes(visAttr_Floor);

    visAttr_Wall = new G4VisAttributes();
    visAttr_Wall->SetVisibility(true);
    visAttr_Wall->SetForceSolid(true);
    visAttr_Wall->SetColour(0.2, 0.1, 0.8, 0.2);
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
    visAttr_ArcVault->SetColour(0.8, 0.8, 0.8, 0.1);
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
    visAttr_BeamDump_LeadBlock->SetColour(0.8, 0.8, 0.1, 0.7);
    logical_BeamDump_LeadBlock->SetVisAttributes(visAttr_BeamDump_LeadBlock);

    visAttr_BeamDump_HDPEBlock = new G4VisAttributes();
    visAttr_BeamDump_HDPEBlock->SetVisibility(true);
    visAttr_BeamDump_HDPEBlock->SetForceSolid(true);
    visAttr_BeamDump_HDPEBlock->SetColour(1., 0., 1., 0.9);
    logical_BeamDump_HDPEBlock_Inner->SetVisAttributes(visAttr_BeamDump_HDPEBlock);
    logical_BeamDump_HDPEBlock_Outer->SetVisAttributes(visAttr_BeamDump_HDPEBlock);

    visAttr_BeamDump_TungstenBlock = new G4VisAttributes();
    visAttr_BeamDump_TungstenBlock->SetVisibility(true);
    visAttr_BeamDump_TungstenBlock->SetForceSolid(true);
    visAttr_BeamDump_TungstenBlock->SetColour(1., 0., 0., 1);
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
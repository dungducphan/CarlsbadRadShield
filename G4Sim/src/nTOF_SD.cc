//
// Created by dphan on 2/22/23.
//

#include <nTOF_SD.hh>
#include <G4SystemOfUnits.hh>
#include <TString.h>

nTOF_SD::nTOF_SD(const G4String &name) : G4VSensitiveDetector(name) {}

nTOF_SD::~nTOF_SD() noexcept {}

G4bool nTOF_SD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

    G4double edep = aStep->GetTotalEnergyDeposit();
    G4double tof = aStep->GetPreStepPoint()->GetGlobalTime();
    G4int det_id = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber();

    // FIXME
    //  1. Bubble detector are not sensitive to gamma rays.
    //  2. Need additional logic to separate the energy deposition of neutrons from those of other particles.

#ifdef DEBUG
        G4cout << "Registered a hit: " << det_id << G4endl;
#endif

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, edep * 1E9 / joule);
    man->FillNtupleDColumn(1, tof * 1E9 / second);
    man->FillNtupleDColumn(2, det_id);
    man->AddNtupleRow(0);

    return true;
}
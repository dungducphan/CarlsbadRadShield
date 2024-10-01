//
// Created by dphan on 2/22/23.
//

#include <ParticleSD.hh>
#include <G4SystemOfUnits.hh>
#include <TString.h>

ParticleSD::ParticleSD(const G4String &name) : G4VSensitiveDetector(name) {}

ParticleSD::~ParticleSD() noexcept {}

G4bool ParticleSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

    // Get track and track weight
    auto track = aStep->GetTrack();
    auto weight = track->GetWeight();

    // Get the energy deposited by the particle
    G4double edep = aStep->GetTotalEnergyDeposit();

    // Get the ID of the HumanPhantom that got hit
    auto det_id = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();

#ifdef DEBUG
        G4cout << "Registered a hit: " << det_id << G4endl;
#endif

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, edep / joule);
    man->FillNtupleDColumn(1, weight);
    man->FillNtupleIColumn(2, det_id);
    man->AddNtupleRow(0);

    return true;
}
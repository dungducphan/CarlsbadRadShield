//
// Created by dphan on 2/22/23.
//

#include <ParticleSD.hh>
#include <G4SystemOfUnits.hh>

ParticleSD::ParticleSD(const G4String &name) : G4VSensitiveDetector(name) {}

ParticleSD::~ParticleSD() noexcept {}

G4bool ParticleSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

    // Get track and track weight
    auto track = aStep->GetTrack();
    auto weight = track->GetWeight();

    // Get PID
    auto PID = track->GetParticleDefinition()->GetPDGEncoding();

    // Get the energy deposited by the particle
    G4double edep = aStep->GetTotalEnergyDeposit();

    // Get the ID of the HumanPhantom that got hit
    auto det_id = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();

    // Get energy/momentum/position of the particle
    auto energy = track->GetTotalEnergy();
    auto momentum = track->GetMomentum();
    auto position = track->GetPosition();

#ifdef DEBUG
        G4cout << "Registered a hit: " << det_id << G4endl;
#endif

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, edep / joule);
    man->FillNtupleDColumn(1, weight);
    man->FillNtupleIColumn(2, det_id);
    man->FillNtupleDColumn(3, energy/keV);
    man->FillNtupleDColumn(4, momentum.x()/keV);
    man->FillNtupleDColumn(5, momentum.y()/keV);
    man->FillNtupleDColumn(6, momentum.z()/keV);
    man->FillNtupleDColumn(7, position.x()/mm);
    man->FillNtupleDColumn(8, position.y()/mm);
    man->FillNtupleDColumn(9, position.z()/mm);
    man->FillNtupleIColumn(10,PID);
    man->AddNtupleRow(0);

    return true;
}
//
// Created by dphan on 2/22/23.
//

#include <ParticleSD.hh>
#include <G4SystemOfUnits.hh>
#include <TString.h>

ParticleSD::ParticleSD(const G4String &name) : G4VSensitiveDetector(name) {}

ParticleSD::~ParticleSD() noexcept {}

G4bool ParticleSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {
    // Get the energy deposited by the particle
    G4double edep = aStep->GetTotalEnergyDeposit();

    // Get the PDG ID of the particle
    G4int PDGID = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();

    // Get the ID of the HumanPhantom that got hit
    auto det_name = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    G4int det_id;
    if      (det_name == "phys_HumanPhantom1") det_id = 1;
    else if (det_name == "phys_HumanPhantom2") det_id = 2;
    else if (det_name == "phys_HumanPhantom3") det_id = 3;
    else if (det_name == "phys_HumanPhantom4") det_id = 4;
    else                                       det_id = 0;

#ifdef DEBUG
        G4cout << "Registered a hit: " << det_id << G4endl;
#endif

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, edep / joule);
    man->FillNtupleDColumn(1, PDGID);
    man->FillNtupleDColumn(2, det_id);
    man->AddNtupleRow(0);

    return true;
}
//
// Created by dphan on 2/22/23.
//

#pragma once

#include "G4VSensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "globals.hh"

class G4Step;

class ParticleSD : public G4VSensitiveDetector {
public:
    ParticleSD(const G4String& name);
    virtual ~ParticleSD();
    virtual G4bool  ProcessHits(G4Step*, G4TouchableHistory*);

private:
    G4AnalysisManager* man = nullptr;

    tools::histo::h2d* h2_controlRoom = nullptr;
    tools::histo::h2d* h2_parkingArea = nullptr;
    tools::histo::h2d* h2_lunchArea = nullptr;
};
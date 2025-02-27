
#include <ParticleSD.hh>

#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4OpticalPhoton.hh"

ParticleSD::ParticleSD(const G4String &name) : G4VSensitiveDetector(name) {
    man = G4AnalysisManager::Instance();
    h2_controlRoom = man->GetH2(man->GetH2Id("DoseMap_ControlRoom"));
    h2_parkingArea = man->GetH2(man->GetH2Id("DoseMap_ParkingArea"));
    h2_lunchArea = man->GetH2(man->GetH2Id("DoseMap_LunchArea"));
}

ParticleSD::~ParticleSD() noexcept {
}

void ParseNameToXYZ(std::string name, int &x, int &y, int &z) {
    std::string delimiter = "_";
    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = name.find(delimiter)) != std::string::npos) {
        token = name.substr(0, pos);
        if (i == 1) {
            x = std::stoi(token);
        } else if (i == 2) {
            y = std::stoi(token);
        } else if (i == 3) {
            z = std::stoi(token);
        }
        name.erase(0, pos + delimiter.length());
        i++;
    }
}

G4bool ParticleSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {
    auto eDep = aStep->GetTotalEnergyDeposit() / eV;
    const auto volName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    int x, y, z;
    ParseNameToXYZ(volName, x, y, z);

    if (volName.find("ControlRoom") != std::string::npos) {
        h2_controlRoom->fill(y, z, eDep);
    } else if (volName.find("ParkingArea") != std::string::npos) {
        h2_parkingArea->fill(y, z, eDep);
    } else if (volName.find("LunchArea") != std::string::npos) {
        h2_lunchArea->fill(x, z, eDep);
    } else {
        G4cout << "Volume name not found: " << volName << G4endl;
    }

    return true;
}
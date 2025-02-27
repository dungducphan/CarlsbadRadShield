#include "generator.hh"

generator::generator() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    fGeneralParticleSource = new G4GeneralParticleSource();

    G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

    sps->SetParticleDefinition(particle);
    sps->SetNumberOfParticles(1);

    auto sourcePosition_FreeCAD = G4ThreeVector(9430.00*mm, 9400.00*mm, 1250.00*mm);
    auto u_FreeCAD_GDML = G4ThreeVector(0.00*mm, 0.00*mm, 4373.00*mm);
    sps->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    sps->GetPosDist()->SetCentreCoords(sourcePosition_FreeCAD - u_FreeCAD_GDML);
    sps->GetPosDist()->ConfineSourceToVolume("NULL");

    sps->GetEneDist()->SetEnergyDisType("Mono"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
    sps->GetEneDist()->SetMonoEnergy(200 * MeV);

    fRandom = new TRandom3();
}

generator::~generator() {
    delete fGeneralParticleSource;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
    fGeneralParticleSource->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(RandomizedDirection());
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}

G4ThreeVector generator::RandomizedDirection() {
    double openingAngle = 10; // mrad
    double rdm1 = fRandom->Gaus(0, openingAngle);
    double y_mom = TMath::Cos(rdm1 * mrad);
    double tmp = TMath::Sin(rdm1 * mrad);

    double rdm2 = fRandom->Uniform(0, TMath::TwoPi());
    double x_mom = TMath::Sin(rdm2 * rad) * tmp;
    double z_mom = TMath::Cos(rdm2 * rad) * tmp;

    return {x_mom, y_mom, z_mom};
}

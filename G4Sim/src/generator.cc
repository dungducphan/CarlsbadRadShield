#include "generator.hh"

generator::generator() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    fGeneralParticleSource = new G4GeneralParticleSource();

    G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

    sps->SetParticleDefinition(particle);
    sps->SetNumberOfParticles(10);

    sps->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    sps->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0, 0.));
    sps->GetPosDist()->ConfineSourceToVolume("NULL");

    sps->GetEneDist()->SetEnergyDisType("Gauss"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
    sps->GetEneDist()->SetMonoEnergy(200 * MeV);
    sps->GetEneDist()->SetBeamSigmaInE(10 * MeV);

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
    double rdm1 = fRandom->Gaus(0, 10);
    double x_mom = TMath::Cos(rdm1 * mrad);
    double tmp = TMath::Sin(rdm1 * mrad);
    double rdm2 = static_cast<double>(rand()) / RAND_MAX;
    rdm2 = rdm2 * TMath::TwoPi();
    double y_mom = TMath::Sin(rdm2 * rad) * tmp;
    double z_mom = TMath::Cos(rdm2 * rad) * tmp;

    return {x_mom, y_mom, z_mom};
}

#include "generator.hh"

generator::generator() {
    fGeneralParticleSource = new G4GeneralParticleSource();

    G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");

    sps->SetParticleDefinition(particle);
    sps->SetNumberOfParticles(1);

    sps->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    sps->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0.3 * m, 0.));
    sps->GetPosDist()->ConfineSourceToVolume("NULL");

    sps->GetAngDist()->SetAngDistType("iso"); // Isotropic, Cosine-law, Beam, User-defined

    sps->GetAngDist()->SetMinTheta(0. * rad);
    sps->GetAngDist()->SetMaxTheta(TMath::Pi() * rad);
    sps->GetAngDist()->SetMinPhi(0 * rad);
    sps->GetAngDist()->SetMaxPhi(TMath::Pi() * 2. * rad);

    sps->GetEneDist()->SetEnergyDisType("Gauss"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
    sps->GetEneDist()->SetMonoEnergy(2.5 * MeV);
    sps->GetEneDist()->SetBeamSigmaInE(0.08 * MeV);
}

generator::~generator() {
    delete fGeneralParticleSource;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}

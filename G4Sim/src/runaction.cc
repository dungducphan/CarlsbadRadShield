#include "runaction.hh"

runAction::runAction() : G4UserRunAction() {
    man = G4AnalysisManager::Instance();
    man->CreateNtuple("EDep", "EDep");
    man->CreateNtupleDColumn("EDep");
    man->CreateNtupleDColumn("PDGID");
    man->CreateNtupleDColumn("HumanPhantom_ID");
    man->FinishNtuple(0);
}

runAction::~runAction() = default;

void runAction::BeginOfRunAction(const G4Run *run) {
    man = G4AnalysisManager::Instance();
    man->OpenFile(Form("EDep_%03i.root", (int) run->GetRunID()));
}

void runAction::EndOfRunAction(const G4Run *) {
    man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}


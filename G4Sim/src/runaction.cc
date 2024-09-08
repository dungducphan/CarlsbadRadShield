#include "runaction.hh"

runAction::runAction() : G4UserRunAction() {
    man = G4AnalysisManager::Instance();
    man->CreateNtuple("Ana", "Ana");
    man->CreateNtupleDColumn("Edep");
    man->CreateNtupleDColumn("T");
    man->CreateNtupleDColumn("DET_ID");
    man->FinishNtuple(0);
}

runAction::~runAction() {}

void runAction::BeginOfRunAction(const G4Run *run) {
    man = G4AnalysisManager::Instance();
    man->OpenFile(Form("Ana.root", (int) run->GetRunID()));
}

void runAction::EndOfRunAction(const G4Run *) {
    man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}


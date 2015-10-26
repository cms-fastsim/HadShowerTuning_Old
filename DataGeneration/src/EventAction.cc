#include "EventAction.h"
#include "Data.h"
#include "TTree.h"

using namespace hadshowertuning;

EventAction::EventAction(TTree * tree,Data * data)
    : G4UserEventAction()
    , fTree(tree)
    , fData(data)
{}

void EventAction::BeginOfEventAction(const G4Event*){
  fData->clear();
}

void EventAction::EndOfEventAction(const G4Event*){
  fTree->Fill();
}

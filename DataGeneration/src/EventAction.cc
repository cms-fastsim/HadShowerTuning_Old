#include "EventAction.h"
#include "G4Event.hh"
#include "Data.h"
#include "TTree.h"
#include "TFile.h"
#include "ShowerHistMaker.h"

using namespace hadshowertuning;

EventAction::EventAction(TFile* file,TTree * tree,Data * data)
    : G4UserEventAction()
    , fFile(file)
    , fTree(tree)
    , fData(data)
{}

void EventAction::BeginOfEventAction(const G4Event *){
  fData->clear();
}

void EventAction::EndOfEventAction(const G4Event * event){
    if(fTree)
	fTree->Fill();
    else{
	fFile->cd();
	char dirName[200];
	sprintf(dirName,"shower_%i",event->GetEventID());
	TDirectory * dir = fFile->mkdir(dirName);
	ShowerHistMaker histMaker;
	histMaker.makeHist(dir,fData);
    }
}

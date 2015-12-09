#ifndef HADSHOWERTUNING_EventAction_h
#define HADSHOWERTUNING_EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"


class TTree;
class TFile;

namespace hadshowertuning{

    class Data;

    class EventAction : public G4UserEventAction
    {
	public:
	EventAction(TFile * f,TTree * tree,Data * data);
	virtual ~EventAction(){;}
	virtual void  BeginOfEventAction(const G4Event*);
	virtual void  EndOfEventAction(const G4Event*);
	private:
	TFile * fFile;
	TTree * fTree;
	Data * fData;
    };
}

#endif

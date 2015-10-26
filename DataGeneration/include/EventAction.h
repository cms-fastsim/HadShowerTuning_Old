#ifndef HADSHOWERTUNING_EventAction_h
#define HADSHOWERTUNING_EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"


class TTree;

namespace hadshowertuning{

    class Data;

    class EventAction : public G4UserEventAction
    {
	public:
	EventAction(TTree * tree,Data * data);
	virtual ~EventAction(){;}
	virtual void  BeginOfEventAction(const G4Event*);
	virtual void  EndOfEventAction(const G4Event*);
	private:
	TTree * fTree;
	Data * fData;
    };
}

#endif

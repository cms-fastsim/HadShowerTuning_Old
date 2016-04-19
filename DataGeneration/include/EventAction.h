/*
 *   class responsible for storing information in output file
 */

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
	EventAction(TFile * f,Data * data,bool MakeTree);
	virtual ~EventAction();
	// clear the data before the event is generated
	virtual void  BeginOfEventAction(const G4Event*);
	// store the data after the event generation
	virtual void  EndOfEventAction(const G4Event*);
	private:
	void writeExtraParticles();
	void findPi0();
	void writeHistograms();
	TFile * fFile;
	Data * fData;
	TTree * fTree;
	int index_pi0_1;   // index of hardest pi0 in shower
	int index_pi0_2;   // index of second hardest pi0 in shower
    };
}

#endif

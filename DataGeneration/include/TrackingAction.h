#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"

namespace hadshowertuning 
{
    class Data;
    class TrackInformation;


    class TrackingAction : public G4UserTrackingAction 
    {
    public:
	TrackingAction(hadshowertuning::Data * data);
	virtual void PreUserTrackingAction(const G4Track*);
	virtual void PostUserTrackingAction(const G4Track*);
    private:
	void storeParticle(G4Track * aTrack,int parentIndex);
	hadshowertuning::Data * fData;
	bool fFoundShowerStart;
    };
}

#endif

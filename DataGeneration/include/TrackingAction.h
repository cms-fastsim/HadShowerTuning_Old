/*!
 *   user action class to gather simTrack level data
 */

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
	// gather simTrack level data at the begin vertex of the simTrack
	virtual void PreUserTrackingAction(const G4Track*);
	// gather simTrack level data at the end vertex of the simTrack
	virtual void PostUserTrackingAction(const G4Track*);
    private:
	void storeParticle(G4Track * aTrack);
	hadshowertuning::Data * fData;
    };
}

#endif

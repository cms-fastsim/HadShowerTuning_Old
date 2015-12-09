#ifndef HADSHOWERTUNING_SteppingAction_h
#define HADSHOWERTUNING_SteppingAction_h

#include "G4UserSteppingAction.hh"
#include "G4ThreeVector.hh"

class G4Step;
class G4VPhysicalVolume;
class G4Track;

#include <map>

namespace hadshowertuning
{

    class Data;
    class SteppingAction : public G4UserSteppingAction
    {
	
    public:
	
	SteppingAction(Data * data,const std::map<const G4VPhysicalVolume*,int> * volumeIdMap);
	~SteppingAction(){};
	virtual void UserSteppingAction(const G4Step*);
	
    private:
	
	int storeParticle(const G4Track * aTrack,int parentIndex);
	Data * fData;
	bool fFoundShowerStart;
	const std::map<const G4VPhysicalVolume * , int > * fVolumeIdMap;
	int fCurrentG4TrackID;
	G4VPhysicalVolume * fCurrentVolume;
	G4ThreeVector fFirstStepInHitPos;
    };
}

#endif

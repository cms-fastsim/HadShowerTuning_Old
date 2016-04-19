/*!
 *   user action class to 
 *    - gather simHit level data
 *    - store state of primary at particular positions
 *       - entry and exit ecal
 *       - entry hcal
 */

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
        // each G4Step is considered as a hit
        // and stored in the fData->hit_* vectors
        // in addition, store a couple of extra particles
	virtual void UserSteppingAction(const G4Step*);
	
    private:
	
	Data * fData;
	const std::map<const G4VPhysicalVolume * , int > * fVolumeIdMap;
    };
}

#endif

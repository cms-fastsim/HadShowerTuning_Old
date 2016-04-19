/*
 *  class used to pass through information from mother simTrack to daughter simTrack
 */

#ifndef HADSHOWERTUNGING_TRACKINFORMATION
#define HADSHOWERTUNGING_TRACKINFORMATION

#include "G4VUserTrackInformation.hh"

namespace hadshowertuning
{
    class TrackInformation : public G4VUserTrackInformation 
    {
	
    public:
	
	// constructors
	TrackInformation()
	    : particleIndex_(-1)
	    , parentIndex_(-1)
	    , inHadronicShower_(false)
	    {;}
	
	TrackInformation(const TrackInformation * info) 
	    : particleIndex_(info->particleIndex()) 
	    , parentIndex_(info->parentIndex()) 
	    , inHadronicShower_(info->inHadronicShower())
	    {;}

	// getters
	void setParticleIndex(int _particleIndex){particleIndex_ = _particleIndex;}
	void setParentIndex(int _parentIndex){parentIndex_ = _parentIndex;}
	void setInHadronicShower(bool _inHadronicShower = true){inHadronicShower_ = _inHadronicShower;}

	// setters
	int particleIndex() const {return particleIndex_;}
	int parentIndex() const {return parentIndex_;}
	bool inHadronicShower() const {return inHadronicShower_;}
	

    private:

	// data
	int particleIndex_;        // index in Data.particle_*             (-1 if simTrack is not stored in Data.particle_*)
	int parentIndex_;          // index of parent in Data.particle_*   (-1 if parent is not stored in Data.particle_*)
	bool inHadronicShower_;    // true if SimTrack is part of hadronic sub-shower
  };
}

#endif

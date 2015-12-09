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
	    : firstInBranchIndex_(-1)
	    , particleIndex_(-1)
	    , parentIndex_(-1)
	    , inEMShower_(false)
	    , initiatesEMShower_(false)
	    , inHadronicShower_(false)
	    {;}
	
	TrackInformation(const TrackInformation * info) 
	    : firstInBranchIndex_(info->firstInBranchIndex())
	    , particleIndex_(info->particleIndex()) 
	    , parentIndex_(info->parentIndex()) 
	    , inEMShower_(info->inEMShower())
	    , initiatesEMShower_(info->initiatesEMShower())
	    , inHadronicShower_(info->inHadronicShower())
	    {;}

	// getters
	void setFirstInBranchIndex(int _firstInBranchIndex){firstInBranchIndex_ = _firstInBranchIndex;}
	void setParticleIndex(int _particleIndex){particleIndex_ = _particleIndex;}
	void setParentIndex(int _parentIndex){parentIndex_ = _parentIndex;}
	void setInEMShower(bool _inEMShower = true){inEMShower_ = _inEMShower;}
	void setInitiatesEMShower(bool _initiatesEMShower = true){initiatesEMShower_ = _initiatesEMShower;}
	void setInHadronicShower(bool _inHadronicShower = true){inHadronicShower_ = _inHadronicShower;}

	// setters
	int firstInBranchIndex() const {return firstInBranchIndex_;}
	int particleIndex() const {return particleIndex_;}
	int parentIndex() const {return parentIndex_;}
	bool inEMShower() const {return inEMShower_;}
	bool initiatesEMShower() const {return initiatesEMShower_;}
	bool inHadronicShower() const {return inHadronicShower_;}
	

    private:

	// data
	int firstInBranchIndex_;
	int particleIndex_;
	int parentIndex_;
	bool inEMShower_;
	bool initiatesEMShower_;
	bool inHadronicShower_;
  };
}

#endif

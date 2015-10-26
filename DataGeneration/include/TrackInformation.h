#ifndef HADSHOWERTUNGING_TRACKINFORMATION
#define HADSHOWERTUNGING_TRACKINFORMATION

#include "G4VUserTrackInformation.hh"

namespace hadshowertuning{
  class TrackInformation : public G4VUserTrackInformation {

  public:
    
  TrackInformation(int _pi0_index) : pi0_index(_pi0_index) {;}
  TrackInformation(const TrackInformation * info) : pi0_index(info->pi0_index){;}
    
  int pi0_index;
  };
}

#endif

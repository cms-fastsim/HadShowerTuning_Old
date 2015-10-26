#ifndef HADSHOWERTUNING_SteppingAction_h
#define HADSHOWERTUNING_SteppingAction_h

#include "G4UserSteppingAction.hh"

class G4Step;
class G4VPhysicalVolume;

#include <map>

namespace hadshowertuning{

  class Data;
  class SteppingAction : public G4UserSteppingAction{
      
  public:
      
    SteppingAction(Data * data,const std::map<const G4VPhysicalVolume*,int> * volumeIdMap);
    ~SteppingAction(){};
    virtual void UserSteppingAction(const G4Step*);

  private:

    Data * fData;
    const std::map<const G4VPhysicalVolume * , int > * fVolumeIdMap;
  };
}

#endif

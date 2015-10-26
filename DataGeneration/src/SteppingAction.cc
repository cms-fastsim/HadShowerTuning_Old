#include "SteppingAction.h"
#include "Data.h"
#include "TrackInformation.h"

#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"

using namespace hadshowertuning;

SteppingAction::SteppingAction(Data * data,const std::map<const G4VPhysicalVolume*,int> * volumeIdMap) 
    : G4UserSteppingAction()
    , fData(data)
    , fVolumeIdMap(volumeIdMap)
{}

void SteppingAction::UserSteppingAction(const G4Step * g4Step){
    
    fData->hit_energy.push_back(g4Step->GetTotalEnergyDeposit()/GeV);
    const G4ThreeVector & startPos = g4Step->GetPreStepPoint()->GetPosition()/cm;
    const G4ThreeVector & stopPos = g4Step->GetPostStepPoint()->GetPosition()/cm;
    fData->hit_x.push_back(startPos.getX() + stopPos.getX() / 2 /cm);
    fData->hit_y.push_back(startPos.getY() + stopPos.getY() / 2 /cm);
    fData->hit_z.push_back(startPos.getZ() + stopPos.getZ() / 2 /cm);
    auto volumeIdItr = fVolumeIdMap->find(g4Step->GetTrack()->GetVolume());
    if(volumeIdItr == fVolumeIdMap->end())
      fData->hit_volume_id.push_back(-1);
    else
      fData->hit_volume_id.push_back(volumeIdItr->second);
    TrackInformation * info = (TrackInformation*)g4Step->GetTrack()->GetUserInformation();
    if(info){
      fData->hit_pi0_index.push_back(info->pi0_index);
    }
    else{
      fData->hit_pi0_index.push_back(-1);
    }
}

#include "SteppingAction.h"
#include "Data.h"
#include "TrackInformation.h"
#include "TVector3.h"

#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4HadronInelasticProcess.hh"
#include "G4SystemOfUnits.hh"

using namespace hadshowertuning;

SteppingAction::SteppingAction(Data * data,const std::map<const G4VPhysicalVolume*,int> * volumeIdMap) 
    : G4UserSteppingAction()
    , fData(data)
    , fFoundShowerStart(false)
    , fVolumeIdMap(volumeIdMap)
    , fCurrentG4TrackID(-1)
    , fCurrentVolume(0)
    , fFirstStepInHitPos(-999*cm,-999*cm,-999*cm)
{}

void SteppingAction::UserSteppingAction(const G4Step * g4Step){

    // volume
    auto volumeIdItr = fVolumeIdMap->find(g4Step->GetTrack()->GetVolume());
    if(volumeIdItr == fVolumeIdMap->end())
    {
	std::cout << "ERROR: found unknown volume. This should never happen, something is wrong with the code ..." << std::endl;
	std::exit(1);
    }
    else
    {
	fData->hit_volumeId.push_back(volumeIdItr->second);
    }

    // particle
    TrackInformation * info = (TrackInformation*)g4Step->GetTrack()->GetUserInformation();
    if(!info)
    {
	std::cout << "ERROR: found G4Track w/o user information. This should never happen, something is wrong with the code ..." << std::endl;
	std::exit(1);
    }
    else
    {
	fData->hit_particleIndex.push_back(info->firstInBranchIndex());
    }

    // hit position
    const G4ThreeVector & startPos = g4Step->GetPreStepPoint()->GetPosition();
    const G4ThreeVector & stopPos = g4Step->GetPostStepPoint()->GetPosition();
    const G4ThreeVector pos = (startPos + stopPos)/2;
    fData->hit_x.push_back(pos.getX()/cm);
    fData->hit_y.push_back(pos.getY()/cm);
    fData->hit_z.push_back(pos.getZ()/cm);

    // hit energy
    fData->hit_e.push_back(g4Step->GetTotalEnergyDeposit()/GeV);
}


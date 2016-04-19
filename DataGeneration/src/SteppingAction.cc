#include "SteppingAction.h"
#include "TrackInformation.h"
#include "TVector3.h"

#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4HadronInelasticProcess.hh"
#include "G4SystemOfUnits.hh"

#include "Data.h"

using namespace hadshowertuning;

SteppingAction::SteppingAction(Data * data,const std::map<const G4VPhysicalVolume*,int> * volumeIdMap) 
    : G4UserSteppingAction()
    , fData(data)
    , fVolumeIdMap(volumeIdMap)
{}

void SteppingAction::UserSteppingAction(const G4Step * g4Step){

    // store the volume id
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
    
    // store particle index
    TrackInformation * info = (TrackInformation*)g4Step->GetTrack()->GetUserInformation();
    if(!info)
    {
	std::cout << "ERROR: found G4Track w/o user information. This should never happen, something is wrong with the code ..." << std::endl;
	std::exit(1);
    }
    else
    {
	fData->hit_particleIndex.push_back(info->particleIndex()>=0 ? info->particleIndex() : info->parentIndex());
    }

    // store hit position
    const G4ThreeVector & startPos = g4Step->GetPreStepPoint()->GetPosition();
    const G4ThreeVector & stopPos = g4Step->GetPostStepPoint()->GetPosition();
    const G4ThreeVector pos = (startPos + stopPos)/2;
    fData->hit_x.push_back(pos.getX()/cm);
    fData->hit_y.push_back(pos.getY()/cm);
    fData->hit_z.push_back(pos.getZ()/cm);

    // store hit energy
    fData->hit_e.push_back(g4Step->GetTotalEnergyDeposit()/GeV);

    // store primary state at special spots in detector
    if(g4Step->GetTrack()->GetTrackID()==1 && g4Step->IsLastStepInVolume())
    {
	std::string vName = g4Step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
	std::string vNameNext = g4Step->GetPostStepPoint()->GetPhysicalVolume()->GetName();

	std::string extraParticleName = "";
	if(vName == "world" && vNameNext == "eb")
	{
	    extraParticleName = "ecalEntrance";
	}
	else if(vName == "eb" && vNameNext == "gap")
	{
	    extraParticleName = "ecalExit";
	}
	else if(vName == "gap" && vNameNext == "hb_sci_0")
	{
	    extraParticleName = "hcalEntrance";
	}

	if(extraParticleName != "")
	{
	    G4ThreeVector position = g4Step->GetPostStepPoint()->GetPosition();
	    G4ThreeVector momentum = g4Step->GetPostStepPoint()->GetMomentum();
	    G4double kinE = g4Step->GetPostStepPoint()->GetKineticEnergy();
	    int pdgId = g4Step->GetTrack()->GetDynamicParticle()->GetPDGcode();
	    fData->addExtraParticle(position,momentum,kinE,pdgId,extraParticleName);
	}
    }

}

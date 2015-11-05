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
{}

/*
int SteppingAction::storeParticle(const G4Track * aTrack,int parentIndex)
{
    const G4ThreeVector & position = aTrack->GetPosition();
    fData->particle_x.push_back(position.x()/cm);
    fData->particle_y.push_back(position.y()/cm);
    fData->particle_z.push_back(position.z()/cm);
    // calculate momentum at production
    double restMass,phi,theta,p,kinE,energy;
    phi = aTrack->GetVertexMomentumDirection().phi();
    theta = aTrack->GetVertexMomentumDirection().theta();
    restMass = aTrack->GetDynamicParticle()->GetDefinition()->GetPDGMass();
    kinE = aTrack->GetVertexKineticEnergy();
    energy = kinE + restMass;
    p = sqrt(energy*energy - restMass*restMass);
    TVector3 momentum;
    momentum.SetMagThetaPhi(p,theta,phi);
    fData->particle_px.push_back(momentum.x()/GeV);
    fData->particle_py.push_back(momentum.y()/GeV);
    fData->particle_pz.push_back(momentum.z()/GeV);
    fData->particle_pdgId.push_back(aTrack->GetDynamicParticle()->GetPDGcode());
    fData->particle_kinE.push_back(kinE/GeV);
    fData->particle_parentIndex.push_back(parentIndex);
    return fData->particle_x.size();
}
*/

void SteppingAction::UserSteppingAction(const G4Step * g4Step){

    //-----------------------
    // store the hits
    //-----------------------
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
    if(!info)
    {
	// hits outside shower
	fData->hit_particleIndex.push_back(-1);
    }
    else{
	fData->hit_particleIndex.push_back(info->firstInBranchIndex());
    }

    //------------------------
    // store particles
    //------------------------
    /*
    G4Track * aTrack = g4Step->GetTrack();

    // when a new event starts, reset, and store primary
    if(aTrack->GetTrackID()==1)
    {
	fFoundShowerStart = false;
    }

    // find secondaries
    G4TrackVector * secondaries = g4Step->GetfSecondary();
    if(!secondaries || secondaries->size()==0)
    {
	return;
    }

    TrackInformation * parentInfo = aTrack->GetUserInformation() ? (TrackInformation*)(aTrack->GetUserInformation()) : 0;
    
    // pass through the track's history to it's secondaries
    for(auto & secondary : *secondaries)
    {
	if(aTrack->GetUserInformation())
	{
	    secondary->SetUserInformation(new TrackInformation(parentInfo));
	    ((TrackInformation*)(secondary->GetUserInformation()))->setInitiatesEMShower(false);
	}
    }

    // detect the shower start
    if(!fFoundShowerStart)
    {
	for(auto secondary : *secondaries)
	{
	    const G4VProcess * process = secondary->GetCreatorProcess();
	    if(process
	       && process->GetProcessType() == fHadronic
	       && dynamic_cast<const G4HadronInelasticProcess *>(process))
	    {
		if(!fFoundShowerStart)
		{
		    fFoundShowerStart = true;
		    // store the primary
		    storeParticle(aTrack,-1);
		    // store the shower start
		    const G4ThreeVector & position = secondary->GetPosition();
		    fData->showerStart_x = position.x()/cm;
		    fData->showerStart_y = position.y()/cm;
		    fData->showerStart_z = position.z()/cm;
		}
		// history starts at the shower start
		// the secondaries are not necessarily stored => no valid particle index
		secondary->SetUserInformation(new TrackInformation());
	    }
	}
    }

    // now, select particles to be stored
    // detect particles initiating EM showers
    for(auto secondary : *secondaries)
    {
	// ignore anything that's outside the shower
	if(!secondary->GetUserInformation())
	{
	    continue;
	}

	// treat photons
	if(secondary->GetDynamicParticle()->GetPDGcode() == 22) 
	{
	    // store photon if
	    if (!parentInfo                           // parent track is primary
		|| !parentInfo->isPartOfEMShower())   // parent track is not part of an EM sub-shower
	    {
		// find parent index
		int parentIndex = -999;
		if(!parentInfo)     // parent is primary
		{
		    parentIndex = 0;
		}
		else                // parent is not primary
		{     
		    parentIndex = parentInfo->particleIndex();
		    // store parent if not yet done 
		    if(parentIndex == -1)
		    {
			parentIndex = storeParticle(aTrack,parentInfo->parentIndex());
			partentInfo->setParticleIndex(parentIndex);
		    }
		}
		// store photon
		int photonIndex = storeParticle(secondary,parentIndex);
		// and update history of g4track
		TrackInformation * info = (TrackInformation*)(secondary->GetUserInformation());
		info->setParticleIndex(photonIndex); 
		info->setParentIndex(parentIndex);
		info->setIsPartOfEMShower();
		info->setInitiatesEMShower();
	    }
	}
	// treat electrons
	else if(abs(secondary->GetDynamicParticle()->GetPDGcode()) == 11)
	{
	    // store electron if 
	    if (!parentInfo                          // parent track is primary
		|| !parentInfo->isPartOfEMShower()   // parent track is not part of an EM sub-shower
		|| parentInfo->initiatesEMShower())  // parent track initiates the EM sub-shower
	    {
		// find parent index
		int parentIndex = -999;
		if(!parentInfo)     // parent is primary
		{
		    parentIndex = 0;
		}
		else{               // parent is not primary
		    parentIndex = parentInfo->particleIndex();
		    // store parent if not yet done 
		    if(parentIndex == -1)
		    {
			parentIndex = storeParticle(aTrack,parentInfo->parentIndex());
			partentInfo->setParticleIndex(parentIndex);
		    }
		}
		int electronIndex = storeParticle(secondary,parentInfo->particleIndex());
		TrackInformation * info = (TrackInformation*)(secondary->GetUserInformation());
		info->setParticleIndex(electronIndex);
		info->setParentIndex(parentIndex);
		info->setIsPartOfEMShower();
		if(!parentInfo                         // parent is primary
		   || !parentInfo->isPartOfEMShower()) // parent is not part of EM sub-shower 
		{
		    info->setInitiatesEMShower();
		}
	    }
	}
	// skip anything else
	else
	{
	    continue;
	}
    }
    */

}


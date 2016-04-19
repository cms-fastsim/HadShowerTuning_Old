#include "TrackingAction.h"
#include "TrackInformation.h"
#include "TVector3.h"

#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4HadronInelasticProcess.hh"
#include "G4ThreeVector.hh"
#include "G4TrackingManager.hh"
#include "G4SystemOfUnits.hh"

#include "Data.h"

using namespace hadshowertuning;

TrackingAction::TrackingAction(Data * data)
  : fData(data)
{}

void TrackingAction::storeParticle(G4Track * aTrack)
{
    // get user information
    TrackInformation * info = (TrackInformation*)aTrack->GetUserInformation();

    // return w/o action if particle already stored i.e. particle index >= 0
    if(info->particleIndex() >= 0)
    {
	return;
    }

    // get and set index of the particle
    int particleIndex = fData->particle_px.size();
    info->setParticleIndex(particleIndex);
    // add particle to fData->particle_*
    const G4ThreeVector & position = aTrack->GetPosition();
    fData->particle_x.push_back(position.x()/cm);
    fData->particle_y.push_back(position.y()/cm);
    fData->particle_z.push_back(position.z()/cm);
    const G4ThreeVector momentum = aTrack->GetMomentum();
    fData->particle_px.push_back(momentum.x()/GeV);
    fData->particle_py.push_back(momentum.y()/GeV);
    fData->particle_pz.push_back(momentum.z()/GeV);
    fData->particle_pdgId.push_back(aTrack->GetDynamicParticle()->GetPDGcode());
    fData->particle_kinE.push_back(aTrack->GetKineticEnergy()/GeV);
    fData->particle_parentIndex.push_back(info->parentIndex());
}


void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    // cheat away the constness of aTrack
    G4Track * myTrack = fpTrackingManager->GetTrack();

    // when a new event starts, reset, and store primary
    if(myTrack->GetTrackID()==1)
    {
	TrackInformation * info = new TrackInformation();
	myTrack->SetUserInformation(info);
	storeParticle(myTrack);
	// store state of primary at begin vertex
	fData->addExtraParticle(myTrack->GetPosition(),
				  myTrack->GetMomentum(),
				  myTrack->GetKineticEnergy(),
				  myTrack->GetDynamicParticle()->GetPDGcode(),
				  "primary");
    }
    
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    // cheat away the constness of aTrack
    G4Track * myTrack = fpTrackingManager->GetTrack();
    
    // store state of primary at end vertex
    if(myTrack->GetTrackID()==1)
    {
	fData->addExtraParticle(myTrack->GetPosition(),
				myTrack->GetStep()->GetPreStepPoint()->GetMomentum(),
				myTrack->GetStep()->GetPreStepPoint()->GetKineticEnergy(),
				myTrack->GetDynamicParticle()->GetPDGcode(),
				"primary_endVertex");
    }

    // get the track info
    TrackInformation * myTrackInfo = (TrackInformation*)myTrack->GetUserInformation();
    
    // find secondaries
    G4TrackVector * secondaries = fpTrackingManager->GimmeSecondaries();
    if(!secondaries || secondaries->size()==0)
    {
	return;
    }

    // add track info to secondaries
    for(unsigned secIndex = 0;secIndex < secondaries->size();secIndex++)
    {
	TrackInformation * secondaryInfo = new TrackInformation();
	secondaryInfo->setInHadronicShower(myTrackInfo->inHadronicShower());
	secondaryInfo->setParentIndex( myTrackInfo->particleIndex() >= 0 ? myTrackInfo->particleIndex() : myTrackInfo->parentIndex() );
	(*secondaries)[secIndex]->SetUserInformation(secondaryInfo);
    }

    // decide what to store and what not
    std::vector<bool> storeSecondary(secondaries->size(),false);
    bool foundShowerStart = false;
    for(unsigned secIndex = 0;secIndex < secondaries->size();secIndex++)
    {
	
	G4Track * secondary = (*secondaries)[secIndex];
	TrackInformation * secondaryInfo = (TrackInformation*)secondary->GetUserInformation();

        // store particles from the first inelastic hadronic interaction of the primary
	const G4VProcess * process = secondary->GetCreatorProcess();
	if( myTrack->GetTrackID() == 1
	    && process
	    && process->GetProcessType() == fHadronic
	    && dynamic_cast<const G4HadronInelasticProcess *>(process))
	{
	    storeSecondary[secIndex] = true;
	    secondaryInfo->setInHadronicShower(true);
	    // note: if it exists, showerStart is same as primary_endVertex
	    if(!foundShowerStart)
	    {
		fData->addExtraParticle(myTrack->GetPosition(),
					myTrack->GetStep()->GetPreStepPoint()->GetMomentum(),
					myTrack->GetStep()->GetPreStepPoint()->GetKineticEnergy(),
					myTrack->GetDynamicParticle()->GetPDGcode(),
					"primary_showerStart");
	    }
	    foundShowerStart = true;
	    
	}

	// don't bother with anything that is not in the hadronic shower
	if(!secondaryInfo->inHadronicShower())
	    continue;
	
	// store photon and parent, if it initiates EM shower (i.e. parent is not electron)
	if(secondary->GetDynamicParticle()->GetPDGcode() == 22)
	{
	    if(abs(myTrack->GetDynamicParticle()->GetPDGcode()) != 11)
	    {
		storeSecondary[secIndex] = true;
	    }
	}

	// store electron if it initiates EM shower (i.e. parent is not photon)
	// or parent is photon and initiates shower (i.e. parent is photon and is stored)
	if(abs(secondary->GetDynamicParticle()->GetPDGcode()) == 11)
	{
	    if(abs(myTrack->GetDynamicParticle()->GetPDGcode()) != 22
	       || myTrackInfo->particleIndex() >= 0)
	    {
		storeSecondary[secIndex] = true;
	    }
	}

	// store all pi0 and eta mesons
	if(secondary->GetDynamicParticle()->GetPDGcode() == 221 // eta
	   || secondary->GetDynamicParticle()->GetPDGcode() == 111) // pi0
	{
	    storeSecondary[secIndex] = true;
	}
	    
	// store all dauthers of eta and pi0 mesons
	if(myTrack->GetDynamicParticle()->GetPDGcode() == 221
	   || myTrack->GetDynamicParticle()->GetPDGcode() == 221)
	{
	    if(secondaryInfo->particleIndex() < 0)
	    {
		storeSecondary[secIndex] = true;
	    }
	}
    }

    // ... and store
    for(unsigned secIndex = 0;secIndex < secondaries->size();secIndex++)
    {
	if(storeSecondary[secIndex])
	{
	    storeParticle((*secondaries)[secIndex]);
	}
    }
}


    
